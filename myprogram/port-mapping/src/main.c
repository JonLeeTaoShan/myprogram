
#include "product.h"
#include "vhash.h"
#include "pktdump.h"
#include "format.h"
#include "routerinfo.h"
#include "rule.h"
#include "cap.h"
#include "redis_op.h"
#include "mac_record_file.h"
#include "crc_url.h"

#include "udpclient.h"

#include "tcpfragment_queue.h"
#include "dev_vhash.h"

#define OPT_MAX  256

#define MAX_DEVCOUNT 4

#define UDP_SERVER_PORT 11518

typedef struct _tagCONFIG
{
	char opt_config[OPT_MAX];
	char opt_pid[OPT_MAX];
	char opt_devSN[32];
	char apmac[32];
	u_int ap_ip;
	int  opt_daemon;
	int  opt_syslog;
	int  opt_debug;
	int  opt_savepcap;
	int  opt_runAudit;//�Ƿ�����ƹ���
	int   devcount;
    u_int audit_server;
	char devname[MAX_DEVCOUNT][64];
}CONFIG;

CONFIG g_config;

int fork_daemon()
{
	/* Our process ID and Session ID */
        pid_t pid, sid;

        /* Fork off the parent process */
        pid = fork();
        if (pid < 0) 
		{
        	printf( "ghbeacon::fork_daemon, fork failed.\n");
            return -1;
        }
        /* If we got a good PID, then
           we can exit the parent process. */
        if (pid > 0) 
		{
            exit(EXIT_SUCCESS);
        }

        /* Change the file mode mask */
        umask(0);

        /* Open any logs here */

        /* Create a new SID for the child process */
        sid = setsid();
        if (sid < 0) 
		{
            /* Log the failure */
			printf("ghbeacon::fork_daemon, setsid failed.\n");
			return -1;
        }


        /* Change the current working directory */
        chdir("/");

        /* Close out the standard file descriptors */
        close(STDIN_FILENO);
        close(STDOUT_FILENO);
        close(STDERR_FILENO);
        return 0;
}

void write_pid(CONFIG stConfig)
{
	if( stConfig.opt_daemon && stConfig.opt_pid[0] != '\0' )
	{
		FILE * fp = fopen( stConfig.opt_pid, "w" );
		if( fp != NULL )
		{
			fprintf( fp , "%d\n", getpid());
			fclose(fp);
		}
	}
}

void sig_handler( int signum )
{
	char szpidname[64]={0};
	sprintf(szpidname,"%s",OPT_PID_FILE);
	FILE *fp = fopen(szpidname,"rb");
	if(NULL != fp)
	{
		fclose(fp);
		unlink(szpidname);
	}
	
	exit(1);
}

char * getUCIItem( char * buf, char * item ){
	char * p1 = buf;
	char * p2;
	char delim;
	while( *p1 == '\t' || *p1 == ' ' )
	{
		p1++;
	}
	if( *p1 == '\'' || *p1 == '"' )
	{
		delim = *p1++;
		p2 = strchr( p1, delim );
	}
	else
	{
		p2 = strchr( p1, ' ' );
		if( p2 == NULL )
		{
			p2 = strchr( p1, '\t');
			if(NULL == p2)
			{
				p2 = strstr( p1, "\r\n");
			}
		}
	}
	if( p2 != NULL )
	{
		*p2 = '\0';
		strncpy( item, p1, OPT_MAX );
		return p2+1;
	}else{
		return NULL;
	}
}

int getUCIConf( char * buf, char * option, char * value ){

	char * p = strstr( buf, "option" );

	if( p != NULL )
	{
		p += 6;
		if( (p = getUCIItem( p, option )) != NULL )
		{
			if( getUCIItem( p, value ) != NULL ){
				return 0;
			}
		}
	}
	return -1;
}

int get_config(CONFIG *pConfig)
{
	char szfile[64]={0};
	sprintf(szfile,"%s",CONFIG_FILE);

	FILE * fp = fopen(szfile, "r");
	char sline[1024];

	char tmp[256]={0};

	if(fp == NULL)
	{
		return -1;
	}
	memset(sline, 0, sizeof(sline));
	while(fgets(sline, sizeof(sline), fp) != NULL)
	{
		char option[OPT_MAX] = {0};
		char value[OPT_MAX] = {0};
		if(sline[0] == '#' || sline[0] == '\0')
		{
			continue;
		}
		if(getUCIConf(sline, option, value) == 0)
		{
			if(strcmp(option, "target") == 0)
			{
				if(strcmp(value, "air") == 0)
				{
                    pConfig->opt_runAudit = 1;
                }
				else if(strcmp(value, "file") == 0)
				{
                    pConfig->opt_runAudit = 0;
                }
			}
			else if(strcmp(option, "source_ports") == 0)
			{
				char *token = strtok(value, ",");
				while(token != NULL)
				{	
					memset(tmp,0,256);
					DelSpace(tmp,token);
					if(tmp[0] != 0 && pConfig->devcount < MAX_DEVCOUNT)
					{
						strncpy(pConfig->devname[pConfig->devcount],tmp,64);
						pConfig->devcount ++;
					}
					token = strtok(NULL, ",");
				}
				if(pConfig->devcount == 0)
				{
					printf("please config devname!\n");
					return -1;
				}
			}
			else if(strcmp(option, "save_log") == 0)
			{
				pConfig->opt_syslog = atoi(value);
            }
			else if(strcmp( option, "save_pcap") == 0)
			{
                pConfig->opt_savepcap = atoi(value);
            }
            else if(strcmp(option, "auditserverip") == 0)
            {
                pConfig->audit_server = inet_addr(value);
            }
		}
		memset(sline, 0, sizeof(sline));
	}
	
	fclose(fp);
	
	return 0;
}


int  CheckMyself()
{
	int nRet = 0;
	int fd = 0;
	
	fd = open("/tmp/.port-mirroring", O_WRONLY|O_CREAT,0777);
	if (fd < 0)
	{
		return -1;
	}
	
	nRet = flock(fd, LOCK_EX | LOCK_NB);
	if (nRet != 0)
	{
		close(fd);
		return -1;
	}
	
	//close(fd);
	return 0;
	
}

void *send_heartbeat(void *arg)
{
    char *apmac = (char *)arg;
    while(1)
    {
        write_heartbeat(apmac);
        sleep(60);
    }
    //return ;
}

int main(int argc,char *argv[])
{
	char c;
	int option_index = 0;
	if(argc > 1 )
	{
		if(!strcmp(argv[1], "-v"))
		{
			char szdate[] = "2016-03-30 11:00";
#if defined(MIPSA1)
			char szVersion[] = " MIPS A1";
#else
			char szVersion[] = "NORMAL";
#endif
			printf("port-mirroring_v2.0  %s %s %d bit\n",szdate,szVersion,sizeof(unsigned long)== 4? 32:64);
			return 0;
		}
	}

	memset(&g_config,0,sizeof(CONFIG));
	
	static struct option long_options[] = {
        	{"config", required_argument, 0, 'c'},
        	{"pid", required_argument, 0, 'p'},
        	{"daemon", no_argument, 0, 'b'},
        	{"debug", no_argument, 0, 'd' },
        	{"syslog", no_argument, 0, 's'},
        	{NULL, 0, NULL, 0}
    	};

		while ((c = getopt_long( argc, argv, "c:p:bds",
                 long_options, &option_index)) != -1) 
		{
        	int this_option_optind = optind ? optind : 1;
        	switch (c) {
        		case 'c':
        			if( optarg ){
        				strncpy( g_config.opt_config, optarg, sizeof(g_config.opt_config));
        			}
        			break;
        		case 'p':
        			if( optarg ){
        				strncpy( g_config.opt_pid, optarg, sizeof(g_config.opt_pid));
        			}
        			break;
        		case 'b':
        			g_config.opt_daemon = 1;
        			break;
        		case 'd':
        			g_config.opt_debug = 1;
        			break;
        		case 's':
        			g_config.opt_syslog = 1;
        			break;
        		default:
        			break;
        	}
	}

	if (-1 == CheckMyself())
    {
		printf("\nport-mirroring: There is a port-mirroring already! exit port-mirroring-_-\n");
		return -1;
    }

#if 0
	if( g_config.opt_daemon && fork_daemon() == -1 )
	{
		//fork_daemon failed.
		return -1;
	}
#endif
    if (1 == g_config.opt_daemon)
    {
        daemon(1,0);
    }

	sprintf(g_config.opt_pid,"%s",OPT_PID_FILE);

	write_pid(g_config);

	signal( SIGINT, sig_handler );
    signal( SIGTERM, sig_handler );

	
	u_char routerMac[6]= {0};
	if( getInterfaceMac("br-lan", routerMac) == 0 )
	{
        sprintf(g_config.apmac,"%02X-%02X-%02X-%02X-%02X-%02X",
		routerMac[0], routerMac[1], routerMac[2], 
		routerMac[3], routerMac[4], routerMac[5]);
    }
	printf("this router mac %s\n",g_config.apmac);

	char waname[32]={0};
	get_wan_name(waname);
	getInterfaceIp(waname,&g_config.ap_ip);
	printf("wan ip %s\n",numToIP(g_config.ap_ip));

	if( 0 != get_config(&g_config))
	{
		printf("get global configure error!\n");
		return -1;
	}
#if 0
    if(0 != redis_op_init())
    {
        printf("connect redis db error\n");
        return 0;
    }

#endif

//   initsyslog();
    
    if(udp_client_init(g_config.audit_server,UDP_SERVER_PORT) < 0)
    {
        printf("udp client init error\n");
    }
    
    VHash_Init(en_VHASH_MOUDLE_MAX);

	protocol_audit_reg();

	DEV_VHash_init();

    TcpFragment_Queue_Init(0);
#ifndef MIPSA1
	file_record_init();
	crc_url_init();
#endif
    
    THREAD_HANDLE handle_heart;
    int hret=0;
    __CreateThread(handle_heart, send_heartbeat, (void *)g_config.apmac,hret);
    
    if(hret)
    {
        printf("__CreateThread heart_thread error\n");
        return 0;
    }

	sleep(10);

	int i = 0;
	for(i=0; i< g_config.devcount;i++)
	{
		PKTDUMP_PARAM stPKTParam;
		memset(&stPKTParam,0,sizeof(PKTDUMP_PARAM));
		
		PKTDUMP_INFO *pPKTDumpInfo = NULL;

		stPKTParam.savelog = g_config.opt_syslog;
		stPKTParam.savepcap = g_config.opt_savepcap;
		stPKTParam.runflag = g_config.opt_runAudit;
		stPKTParam.ap_ip = g_config.ap_ip;
		stPKTParam.debugCmd = g_config.opt_debug;
		stPKTParam.dump_mode = DUMP_MODE_PCAP;
		strcpy(stPKTParam.apmac,g_config.apmac);
		strcpy(stPKTParam.devInName,g_config.devname[i]);

		urlpattern_init(&stPKTParam.acsm);

		printf("log %d pcap %d runflag %d ap_ip %s apmac %s devname %s\r\n",\
			stPKTParam.savelog,stPKTParam.savepcap,stPKTParam.runflag,\
			numToIP(stPKTParam.ap_ip),stPKTParam.apmac,stPKTParam.devInName);
		
		if(0 != pktdump_register((void *)&stPKTParam, (void **)&pPKTDumpInfo))
		{
			printf("register param error\n");
			continue;
		}
		
		if(NULL != pPKTDumpInfo)
		{ 
		
			THREAD_HANDLE handle;
			int iret=0;
			__CreateThread(handle, pktdump_start, (void *)pPKTDumpInfo,iret);

			if(iret)
			{
				printf("__CreateThread devname %s error\n",pPKTDumpInfo->devName);
				continue;
			}
		}
		
	}

	while(1)
	{
		sleep(5);
	}
	return 0;
}





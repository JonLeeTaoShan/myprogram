
#include "product.h"
#include "public.h"
#include "format.h"
#include "spinlock.h"
#include "mac_vhash.h"
#include "udpclient.h"



typedef struct _tagFILEOPT
{
    int parse_count;
    char filename[64];
    char szbuf[3072];
    char szdst[10240];
    char szdst1[10240];
    SPINLOCK_S spinlock;
}FILEOPT;

FILEOPT g_FileOpt;

/*
*����ļ����Ƿ����
*/
int  CheckFolderExist(const char *szPath)
{
    int  rValue = 0;
	DIR *pDir = opendir(szPath);
	if(NULL != pDir)
	{
		rValue = 1;  
		closedir(pDir);
	}
    return rValue;
}

/*
	�����ļ���Ŀ¼
*/

void CreateDir(const char *p)
{
	char spec = '/';

	char *p1,*p2;
	if(p1 = strrchr(p,spec))
	{ 
		p2 = strchr(p,spec);
		if(p1==p2)
			return;

		*p1 = '\0';
		if (!CheckFolderExist(p))
		{
			CreateDir(p);
			mkdir(p,0750);
		}
		*p1 = spec;
	}
}


int initsyslog()
{
	 openlog("syslog", LOG_NDELAY, LOG_LOCAL6);
    
    InitSpinLock(&g_FileOpt.spinlock);
    memset(g_FileOpt.filename, 0, 64);
    memset(g_FileOpt.szbuf,0, 3072);
    memset(g_FileOpt.szdst, 0, 10240);
    memset(g_FileOpt.szdst1, 0, 10240);
    
	return 0;
}

int closesyslog()
{
	  closelog();
	  return 0;
}

void writeLogInfo(void *pstMacItem)
{
    MAC_ITEM *macitem = (MAC_ITEM *)pstMacItem;
	char sz[128]={0};
	char dst[512]={0};
    char dst1[256];
	sprintf(sz,"^_^%s^_^%s^_^%u^_^%u^_^%u",macitem->apmac,macitem->key,macitem->fir_time,\
		macitem->las_time,macitem->lognum);
	base64_encode1(sz,dst);
//	syslog(LOG_INFO, "[UNGOL]%s\r\n",dst);
    
    int len = sprintf(dst1,"[UNGOL]%s\r\n", dst );
    if (udp_client_sent_recv_buff(dst1, len, NULL, NULL) < 0)
    {
        printf("[UNGOL]udp client send data error\n");
    }
    
	return;
}


void write_heartbeat(char *mac)
{
	unsigned int uiCur = time(NULL);
    char sz[64]={0};
    char dst[64]={0};
    char dst1[64]={0};
	sprintf(sz,"^_^%u^_^%s",uiCur,mac);
	base64_encode1(sz,dst);
//	 syslog(LOG_INFO, "[TRAEH]%s\r\n",dst);
    int len = sprintf(dst1,"[TRAEH]%s\r\n", dst );
    if (udp_client_sent_recv_buff(dst1, len, NULL, NULL) < 0)
    {
        printf("[TRAEH]udp client send data error\n");
    }
}


void writeFile(int flag,PROTOCOL_PARSE *parse,char *buf_raw,char *buf_res_pre,char *buf_res_back)
{
	 /* ��һ���ж������Ƿ���ȷ  */
	 #if 0
	    if(protocol_parse->optype == 1)
	    {
	        if(!strlen(protocol_parse->username))
	            return;
	    }
	
		if(parse->optype == 3 || parse->optype == 4)
		{
			if(!strlen(parse->sendname) && !strlen(parse->toname))
				return;
		}
	#endif
//	    if(parse->optype == 5 || parse->optype == 6)
//	    {
//	        if(!strlen(parse->sendname) && !strlen(parse->toname) && !strlen(parse->subject))
//	            return;
//	    }
		//���п��ֶ���Ϊ"-"
		{
				if(0 == parse->apid[0])parse->apid[0]='-';
				if(0 == parse->longitude[0])parse->longitude[0]='-';
				if(0 == parse->latitude[0])parse->latitude[0]='-';
				if(0 == parse->username[0])parse->username[0]='-';
				if(0 == parse->sendname[0])parse->sendname[0]='-';
				if(0 == parse->toname[0])parse->toname[0]='-';
				if(0 == parse->subject[0])parse->subject[0]='-';
				if(0 == parse->content[0])parse->content[0]='-';
				if(0 == parse->url[0])parse->url[0]='-';
				if(0 == parse->keyword[0])parse->keyword[0]='-';
				if(0 == parse->devname[0])parse->devname[0]='-';
				if(0 == parse->imei[0])parse->imei[0]='-';
		}
	
	if(flag == 1)
	{
		sprintf(buf_raw,"^_^%s^_^%s^_^%s^_^%s^_^%s^_^%s^_^%s^_^%s^_^%s^_^%s\
^_^%s^_^%s^_^%s^_^%s^_^%s^_^%s^_^%s^_^%d^_^%u^_^%u^_^%u^_^%u^_^%u^_^%u^_^%u^_^%u\
^_^%u^_^%u^_^%u^_^%s^_^%s^_^%s",parse->id,parse->wacode,parse->watype,parse->apmac,parse->apid,\
parse->srcMac,parse->srcIPV6,parse->dstIPV6,parse->longitude,parse->latitude,\
parse->proto_name,parse->username,parse->sendname,parse->toname,parse->subject,\
parse->content,parse->keyword,parse->optype,parse->ipaddr,parse->srcip,parse->dstip,\
parse->port,parse->src_p_s,parse->src_p_e,parse->src_p_s_v6,parse->src_p_e_v6,\
parse->dst_port,parse->dst_port_v6,parse->captime,parse->devname,parse->imei,parse->url);
        printf("%s\n",buf_raw);
		base64_encode1(buf_raw, buf_res_pre);
//		syslog(LOG_INFO, "[PLMGN]%s\r\n",g_FileOpt.szdst);
        int len = sprintf(buf_res_back,"[PLMGN]%s\r\n",buf_res_pre);
        if (udp_client_sent_recv_buff(buf_res_back, len, NULL, NULL) < 0)
        {
            printf("[PLMGN]udp client send data error\n");
        }
//		int sssss = 0;
//		char *sss = base64Decode1(buf_res_pre,&sssss);
//		printf("%d %s\n\n",sssss,sss);
//		free(sss);
	}
	else if(flag == 0)
	{
        SpinLock_Lock(&g_FileOpt.spinlock);
		if(g_FileOpt.parse_count == PRO_OUTPUT_COUNT)
		{
			memset(g_FileOpt.filename, 0, 64);
			g_FileOpt.parse_count = 0;
		}
		if(g_FileOpt.parse_count == 0)
		{
			long now;
		
		    time(&now);

			sprintf(g_FileOpt.filename,"%s_%ld.as", OUTPUT_PATH, now);
		}

		FILE *fp;
		if((fp = fopen(g_FileOpt.filename, "ab+")) == NULL)
		{
			printf("open file failed !\n");
            SpinLock_unLock(&g_FileOpt.spinlock);
			return;
		}
		fprintf(fp, "id:%s,apip:%s,apmac:%s,aptype:%s,captime:%u,srcmac:%s,srcip:%s,dstip:%s,srcipv6:%s,dstipv6:%s,port:%u,dstport:%u,\
		ipv6dstport:%u,ip_s:%u,ip_e:%u,ipv6_s:%u,ipv6_e:%u,apptype:%u,protoname:%s,logitud:%s,latitude:%s\n",parse->id,\
		numToIP(parse->srcip),parse->apmac,parse->watype,parse->captime,parse->srcMac,numToIP(parse->srcip),numToIP(parse->dstip),\
		parse->srcIPV6,parse->dstIPV6,parse->port,parse->dst_port,parse->dst_port_v6,parse->src_p_s,\
		parse->src_p_e,parse->src_p_s_v6,parse->src_p_e_v6,parse->optype,parse->proto_name,parse->longitude,parse->latitude);
		g_FileOpt.parse_count++;
		fclose(fp);
        SpinLock_unLock(&g_FileOpt.spinlock);
	}

	return;	
}




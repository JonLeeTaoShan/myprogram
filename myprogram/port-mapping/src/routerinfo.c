
#include "product.h"
#include "routerinfo.h"


int get_wan_name(char *wanname)//获取wan口名称
{
	FILE *fp = NULL;
	fp = popen("route -n |awk '$4 ==\"UG\" {print $8}'","r");
	char szbuf[32]={0};
	if(NULL != fp)
	{
		fread( szbuf, 1, sizeof(szbuf),  fp);
	}
	if(NULL != fp )
	{
		pclose(fp);
	}
	int i = 0;
	for(i;i < strlen(szbuf);i++)
	{
		if(szbuf[i] == '\r' || szbuf[i] == '\n')
		{
			szbuf[i]='\0';
		}
	}
	strcpy(wanname,szbuf);
	return 0;
}

int get_wan_IP(char * device, u_int *ip)
{
	FILE *fp = NULL;
	char cmd[64]={0};
	sprintf(cmd,"ifconfig %s",device);
	fp = popen(cmd,"r");
	char szbuf[1024]={0};
	if(NULL != fp)
	{
		fread( szbuf, 1, sizeof(szbuf),  fp);
	}
	if(NULL != fp )
	{
		pclose(fp);
	}
	char szip[32]={0};
	char *p = strstr(szbuf,"inet addr:");
	char *p1 = NULL;
	if(NULL != p)
	{
		p += strlen("inet addr:");
		p1 = strchr(p,' ');
		if(NULL != p1)
		{
			 u_int len = (u_int)p1 - (u_int)p;
			if(len < 32)
			{
				memcpy(szip,p,len);
			}
		}
	}

	if(0 != szip[0])
	{
		*ip = ntohl(inet_addr(szip));
	}
	
	return 0;
}

int getInterfaceMac(char * device, u_char * mac )
{
	int s;
	struct ifreq buffer;

	if( (s = socket( PF_INET, SOCK_DGRAM, 0)) < 0 ){
		return -1;
	}
	memset(&buffer, 0x00, sizeof(buffer));
    	strncpy( buffer.ifr_name, device, sizeof(buffer.ifr_name));

	if( ioctl(s, SIOCGIFHWADDR, &buffer) < 0 ){
		close( s );
		return -1;
	}

    close(s);

	memcpy( mac, buffer.ifr_hwaddr.sa_data, 6 );

	return 0;
}

int getInterfaceIp(char * device, u_int * ip)
{
	int s;
	struct ifreq buffer;

	if( (s = socket( AF_INET, SOCK_DGRAM, 0)) < 0 )
	{
        printf("create socket error.");
        return -1;
	}
	memset(&buffer, 0x00, sizeof(buffer));
	buffer.ifr_addr.sa_family = AF_INET;
	strncpy( buffer.ifr_name, device, sizeof(buffer.ifr_name));
	int iret =0;
	iret = ioctl(s, SIOCGIFADDR, &buffer);
	if( iret < 0 )
	{
        printf("unable to query mac address of [%s].\n", device );
        close( s );
        return -1;
	}

	close(s);

	*ip = htonl(((struct sockaddr_in *)&buffer.ifr_addr)->sin_addr.s_addr);
	return 0;
}






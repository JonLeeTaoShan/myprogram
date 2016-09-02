

/*
pktdump.h
*/
#ifndef __PKTDUMP_H_
#define __PKTDUMP_H_

#define BUFFTMPLEN  2048
#define BUF_RAW_LEN 3072
#define BUF_RES_LEN 10240
typedef struct _tagPKTDUMP_INFO
{
	int savepcap;//0:no write file 1:save match cap 2:save all cap
	int savelog;//0:stream send 1:write file
	u_int ap_ip;
	char apmac[32];
	char devName[64];
	int idebug;
	int dump_mode;
	int runflag;
	u_int uisec; /*snap cap time second*/
	u_int uimsec;/*snap cap time micro second*/
	void *handle;
	void *acsm;
    void *redisctx;//redis db connect
    u_char bufftmp[BUFFTMPLEN];
    int bufftmp_len;
	char szBufRaw[BUF_RAW_LEN];
	char szBufResPre[BUF_RES_LEN];
	char szBufResBack[BUF_RES_LEN];
	int (*recv)(void *intf,u_char *rbuf,int rlen);
}PKTDUMP_INFO;

int pktdump_register(void *pktdump_param,void  **ppPktdump_info);
void *pktdump_start(void *arg);

#endif//__PKTDUMP_INFO_


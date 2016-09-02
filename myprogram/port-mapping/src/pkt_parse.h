
/*
pkt_parse.h
*/
#ifndef __PKT_PARSE_H_
#define __PKT_PARSE_H_

typedef struct _tagDATA_DESC
{
	u_char smac[6];			//souce mac
	u_char dmac[6];			//destination mac
	u_int sIp;			//source ip address
	u_int dIp;	//destination ip address
	u_short sPort;		//source port
	u_short dPort; //destination port
    u_int acknowledgeNumber;	// Number of aknowledge
	u_char protocol;		//protocol used
	u_int uiflag;
	int uidatalen;
	u_char *dataptr;
}DATA_DESC;

int pkt_recv(void *PktdumpInfo,u_char *rbuf,int len);

#endif


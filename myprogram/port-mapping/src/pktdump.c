
#include "product.h"
#include "pktdump.h"
#include "pcap.h"
#include "pkt_parse.h"

int pktdump_register(void *pktdump_param,void  **ppPktdump_info)
{
	PKTDUMP_PARAM *param = (PKTDUMP_PARAM *)pktdump_param;
	PKTDUMP_INFO *pInfo = NULL;
	
	pInfo = (PKTDUMP_INFO *)malloc(sizeof(PKTDUMP_INFO));
	memset(pInfo,0,sizeof(PKTDUMP_INFO));
	if(NULL == pInfo)
	{
		printf("malloc PKTDUMP error!\n");
		return -1;
	}
	pInfo->idebug = param->debugCmd;
	pInfo->runflag = param->runflag;
	pInfo->savelog = param->savelog;
	pInfo->savepcap = param->savepcap;
	pInfo->dump_mode = param->dump_mode;
	pInfo->ap_ip = param->ap_ip;

	pInfo->acsm = param ->acsm;
	strcpy(pInfo->apmac,param->apmac);
	strcpy(pInfo->devName, param->devInName);

	pInfo->recv = pkt_recv;
	int sock;
	if(DUMP_MODE_PCAP == pInfo->dump_mode)
	{
		pcap_t *pt = NULL;
		char errbuf[PCAP_ERRBUF_SIZE];
		if ((pt = pcap_open_live(pInfo->devName,	// name of the device
					65536,							// portion of the packet to capture. 
													// 65536 grants that the whole packet will be captured on all the MACs.
					1,								// promiscuous mode (nonzero means promiscuous)
					1000,							// read timeout
					errbuf							// error buffer
					)) == NULL)
				{
					printf("Error opening adapter %s\n",pInfo->devName);
					free(pInfo);
					exit(1);
				}
		pInfo->handle = (void *)pt;
	}
	
	*ppPktdump_info = (void *)pInfo;
	
	return 0;
}


void * pktdump_start(void *arg)
{
	PKTDUMP_INFO *pktDumpInfo = (PKTDUMP_INFO *)arg;
	int res;
    void *redisCtx = NULL;
    redis_op_init(&redisCtx);
    
    if (NULL != redisCtx)
    {
        pktDumpInfo->redisctx = redisCtx;
    }
    
	if(DUMP_MODE_PCAP == pktDumpInfo->dump_mode)
	{
		/*pcap sniffer*/
		struct pcap_pkthdr *header;
		const u_char *pkt_data;
		pcap_t *ft = (pcap_t *)pktDumpInfo->handle;

		/* Read the packets */
		while((res = pcap_next_ex( ft, &header, &pkt_data)) >= 0)
		{
			if(res = 0)/* Timeout elapsed */	
			{
				continue;
			}
			if(header->len < 43)//
			{
				continue;
			}
			
			pktDumpInfo->uisec = header->ts.tv_sec;
			pktDumpInfo->uimsec = header->ts.tv_usec/1000;			
			pktDumpInfo->recv((void *)pktDumpInfo,(u_char *)pkt_data, header->len);
		}
	}
	
}






#include "product.h"
#include "pkt_parse.h"
#include "pktdump.h"
#include "cap.h"
#include "rule.h"
#include "public.h"
#include "redis_op.h"
#include "mac_record_file.h"
#include "crc_url.h"
#include "tcpfragment_vhash.h"
#include "tcpfragment_queue.h"
#include "acsmx.h"
#include "sfksearch.h"
#include "dev_vhash.h"

void pkt_dump(unsigned char *p_buf, int p_len)
{
	register const char *pref = ""; 	
	unsigned char *buf = p_buf;	
	unsigned int i = 0, width = 16;	
	unsigned int len = p_len;	
	for (i=1; i < len+1; i++)
	{
		printf("%2.2x ", (unsigned char)buf[i-1]);
		if(0 == (i % width))
			printf("\n");
	}
	printf("\n\n");
}		

int dhcp_parse(DATA_DESC data,char *mac,char *devinfo)
{
    u_char *pdata = data.dataptr;
    u_char *ptmp = pdata;
    DHCPHDR *dhcphdr = NULL;

    int ioffset = 0;
    ioffset = sizeof(DHCPHDR);
    if(ioffset > data.uidatalen)
    {
        return -1;
    }
    u_char *pdhcp_option = ptmp + ioffset;

    int flag = 0;
    int optype = 0;
    int oplen = 0;

    while(1)
    {
        optype = pdhcp_option[0];
        oplen = pdhcp_option[1];
        if (optype == 0x35)//dhcp message type
        {
            if(pdhcp_option[2] != 0x03)
            {
                flag = 0;
                break;
            }
        }
        else if (optype == 0x0c)
        {
            memcpy(devinfo,pdhcp_option + 2,oplen);
            flag = 1;
            break;
        }

        else if(optype == 0xff)
        {
            break;
        }
        pdhcp_option += 2;
        pdhcp_option += oplen;
    }

    sprintf(mac,"%02x%02x%02x%02x%02x%02x",data.smac[0],data.smac[1],data.smac[2],data.smac[3],data.smac[4],data.smac[5],data.smac[6]);

    if( 1 == flag )
    {
        return 0;
    }

    return -1;
}

int protocol_parse(void *PktdumpInfo,RULEITEM *rule, DATA_DESC dataDesc)
{
    int ret = 0;
    PKTDUMP_INFO *dumpInfo = (PKTDUMP_INFO *)PktdumpInfo;
    dumpInfo->bufftmp_len = sizeof(PROTOCOL_PARSE);
    PROTOCOL_PARSE * pparse = (PROTOCOL_PARSE *)dumpInfo->bufftmp;
    memset(pparse,0, dumpInfo->bufftmp_len);

    DEV_VHASH_ITEM *pstVhahItem = NULL;
    
    redis_op_get_code(dumpInfo->redisctx, pparse->wacode);
    if(0 == pparse->wacode[0])
    {
        return 0;
    }

    //	pparse->ipaddr = g_ap_ip;
    char mac[16]={0};
    strcpy(pparse->apmac,dumpInfo->apmac);
    sprintf(pparse->watype,"%02d",rule->watype);
    pparse->captime = time(NULL);
    
    redis_op_get_devsn(dumpInfo->redisctx,pparse->apid);
    
    redis_op_get_dev_lon_lat(dumpInfo->redisctx,pparse->longitude, pparse->latitude);
    
    u_int online = 0;
    if(rule->s2c == 1)
    {
        sprintf(pparse->srcMac, "%02x-%02x-%02x-%02x-%02x-%02x", dataDesc.dmac[0],dataDesc.dmac[1],dataDesc.dmac[2],dataDesc.dmac[3],
                dataDesc.dmac[4],dataDesc.dmac[5]);
        sprintf(mac,"%02x%02x%02x%02x%02x%02x",dataDesc.dmac[0],dataDesc.dmac[1],dataDesc.dmac[2],dataDesc.dmac[3],\
                dataDesc.dmac[4],dataDesc.dmac[5]);
        redis_op_get_devinfo(dumpInfo->redisctx,pparse->srcMac, &online,&pparse->src_p_s ,&pparse->src_p_e);
        if(0 == online)
        {
            return 0;
        }
        sprintf(pparse->id,"%s%02x%02x%02x%02x%02x%02x%u",pparse->wacode,dataDesc.dmac[0],dataDesc.dmac[1],dataDesc.dmac[2],
                dataDesc.dmac[3],dataDesc.dmac[4],dataDesc.dmac[5],online);
    }
    else
    {
        sprintf(pparse->srcMac, "%02x-%02x-%02x-%02x-%02x-%02x", dataDesc.smac[0],dataDesc.smac[1],dataDesc.smac[2],dataDesc.smac[3],
                dataDesc.smac[4],dataDesc.smac[5]);
        sprintf(mac,"%02x%02x%02x%02x%02x%02x",dataDesc.smac[0],dataDesc.smac[1],dataDesc.smac[2],dataDesc.smac[3],\
                dataDesc.smac[4],dataDesc.smac[5]);
        redis_op_get_devinfo(dumpInfo->redisctx,pparse->srcMac, &online,&(pparse->src_p_s) ,&(pparse->src_p_e));
        if(0 == online)
        {
            return 0;
        }
        sprintf(pparse->id,"%s%02x%02x%02x%02x%02x%02x%u",pparse->wacode,dataDesc.smac[0],dataDesc.smac[1],dataDesc.smac[2],
                dataDesc.smac[3],dataDesc.smac[4],dataDesc.smac[5],online);
    }

    if(mac[0] != 0)
    {
        DEV_VHash_FindStruct(mac,&pstVhahItem);
        if (NULL != pstVhahItem)
        {
            strcpy(pparse->devname,pstVhahItem->devinfo);
        }
    }

    pparse->ipaddr = pkt_swapu32(dataDesc.sIp);
    pparse->srcip = dumpInfo->ap_ip;
    pparse->dstip = pkt_swapu32(dataDesc.dIp);
    pparse->srcIPV6[0] ='-';
    pparse->dstIPV6[0] ='-';
    pparse->port= pkt_swapu16(dataDesc.sPort);
    pparse->dst_port= pkt_swapu16(dataDesc.dPort);
    pparse->dst_port_v6 = 0;
//		pparse->src_p_s = 0;
//		pparse->src_p_e = 0;
    pparse->src_p_s_v6 = 0;
    pparse->src_p_e_v6 = 0;

    
    pparse->optype = rule->optype;
    
    strcpy(pparse->proto_name, rule->protocol_name);
    
    ret = g_proto_audit[rule->proto_id].packet_audit(pparse,dataDesc.dataptr,dataDesc.uidatalen);
    
    if(1 == ret && rule->proto_id == en_PROTO_ID_HTTP)
    {
/*
        ACSM_STRUCT *acsm;
        acsm = (ACSM_STRUCT *)dumpInfo->acsm;
        int id = 0;
        acsmSearch (acsm, pparse->url, strlen (pparse->url), Match,(void *)pparse,&id);
*/
        KTRIE_STRUCT *ts = (KTRIE_STRUCT *)dumpInfo->acsm;
        int id = 0;
        char *url = pparse->url;
        char *purl = strchr(url,'/');

        u_int ulen = 0;
        if(NULL != purl)
        {
            ulen = (unsigned int)purl- (unsigned int)url;
        }
        else
        {
            ulen = 8;
        }
        KTrieSearch( ts, (unsigned char*)pparse->url, ulen, match, (void *)pparse,&id);

        if(id == en_PROTO_ID_NULL)
        {
            return 0;
        }

        /*
        ////π˝¬ÀURLπÿº¸◊÷£∫login.xiaoyun.com wireless.mapbar.com
        if( strstr(pparse->url, "xiaoyun.com") ||
           strstr(pparse->url, "wireless.mapbar.com") ||
           strstr(pparse->url, "captive.apple.com") ||
           strstr(pparse->url, "configuration.apple.com") ||
           strstr(pparse->url,"gclick.cn")||
           strstr(pparse->url,"weixin.qq.com") ||
           strstr(pparse->url,"sug.m.baidu.com") ||
           strstr(pparse->url,"pro.cn") ||
           strstr(pparse->url,".gdt.qq.com") ||
           strstr(pparse->url,"push.apple.com") ||
           strstr(pparse->url,"bdimg.com") ||
           strstr(pparse->url,"uu.qq.com") ||
           strstr(pparse->url,"pingma.qq.com") ||
           strstr(pparse->url,"omgmta1.qq.com")
           )
        {
            return 0;
        }
*/      if(NULL != purl)
        {
            if(strcasestr(purl,".jpg") || strcasestr(purl,".js") ||
               strcasestr(purl,".gif") || strcasestr(purl,".png") ||
               strcasestr(purl,".svg") || strcasestr(purl,".bmp") ||
               strcasestr(purl,"css") || strcasestr(purl,".jpeg") ||
               strcasestr(purl,".swf") || strcasestr(purl,".tiff") ||
               strcasestr(purl,".xml") || strcasestr(purl,".cmd") ||
               strcasestr(purl,"show") || strcasestr(purl,"log.") ||
               strcasestr(purl,"x.") || strcasestr(purl,"img.") ||
               strcasestr(purl,".img") ||strcasestr(purl,".ico") ||
               strcasestr(purl,".js") || strcasestr(purl,"pic.") ||
               strcasestr(purl,".woff") || strcasestr(purl,".ini") ||
               strcasestr(purl,".dll") || strcasestr(purl,".zip") ||
               strcasestr(purl,".txt") ||   strcasestr(purl,".sys") ||
               strcasestr(purl,".m4a") || strcasestr(purl,".mp4")
                    )
            {
                return 0;
            }
        }

#ifndef MIPSA1
        int cret = check_url(pparse);
        if (cret > 0)
        {
            return 0;
        }
        else if(0 == cret )
        {
            crc_url_add(pparse);
        }
#endif
    }
    
    if(ret)
    {
        writeFile(dumpInfo->savelog,pparse,dumpInfo->szBufRaw,dumpInfo->szBufResPre,dumpInfo->szBufResBack);
#ifndef MIPSA1
        MAC_ITEM stMItem;
        memset(&stMItem, 0, sizeof(MAC_ITEM));
        strcpy(stMItem.apmac,pparse->apmac);
        strcpy(stMItem.key,pparse->srcMac);
        file_record_update(&stMItem);
#endif
    }
    
    return ret;
}


int pparse_get_dataptr(u_char *rbuf,int irlen,DATA_DESC *pDataDesc)
{
	u_char *p = rbuf;
	ETHDR *peth;
	IPHDR *ipp;
	TCPHDR *pTcp = NULL;
	UDPHDR *pUdp = NULL;
	u_char EthType = 0;
	int iOffset = 12;
	
	peth = (ETHDR *)p;
	
/* ∆´“∆ETHER Õ∑*/
 	if(0x08 == p[iOffset] && 0x00 == p[iOffset + 1])
	{
		iOffset += 2;
	}
	else if(0x81 == p[iOffset] && 0x00 == p[iOffset + 1])//vlan
	{
		iOffset += 4;
		if(0x08 == p[iOffset] && 0x00 == p[iOffset + 1])//vlan + ip
		{
			iOffset += 2;
		}
        else if(0x81 == p[iOffset] && 0x00 == p[iOffset + 1])//double vlan + pppoe
        {
            iOffset += 4;
            if(0x08 == p[iOffset] && 0x00 == p[iOffset + 1])
            {
                iOffset += 2;
            }
            else if(0x88 == p[iOffset] && 0x64 == p[iOffset + 1])
            {
                iOffset += 8;
                if(!(0x00 == p[iOffset] && 0x21 == p[iOffset + 1]))
                {
                    return 0;
                }
                iOffset += 2;
            }
        }
		else if(0x88 == p[iOffset] && 0x64 == p[iOffset + 1]) //vlan + pppoe
		{
			iOffset += 8;
			if(!(0x00 == p[iOffset] && 0x21 == p[iOffset + 1]))
			{
				return 0;
			}
			iOffset += 2;
		}
		else
		{
			return 0;
		}
	}
    else if(0x88 == p[iOffset] && 0x64 == p[iOffset + 1]) //pppoe
    {
        iOffset += 8;
        if(!(0x00 == p[iOffset] && 0x21 == p[iOffset + 1]))
        {
            return 0;
        }
        iOffset += 2;
    }
    else if(0x88 == p[iOffset] && 0x47 == p[iOffset + 1])//MPLS
    {
        iOffset += 2;

        u_int label = pkt_swapu32(*((u_int*)&p[iOffset]));

        iOffset += 4;

        while((label & 0x100) != 0x100)
        {
            iOffset += 4;

            label = pkt_swapu32(*((u_int*)&p[iOffset]));
        }
    }
    else
	{
		return 0;
	}

	p = rbuf + iOffset;
	/*ip head offset*/
	ipp=(IPHDR *)p;

	if(4 != ipp->ver)
	{
		return 0;
	}
	
	iOffset += 20;	
	p = rbuf + iOffset;
	EthType = ipp->proto;
	
	
	if (TCP_PACKET != EthType && UDP_PACKET != EthType)
	{
		return 0;
	}

	if(TCP_PACKET == EthType)
	{
		pTcp = (TCPHDR *)p;	
		/*tcp head offset*/
		//		uiOffset += (pTcp->dataoffset >> 4)*4;
		iOffset += (pTcp->tlen)*4;
		p = rbuf + iOffset;
		
		if(iOffset > irlen)
		{
			return 0;	
		}
#if defined(MIPSA1)
        pDataDesc->uiflag = pkt_swapu32((unsigned int)pTcp->flags);
        pDataDesc->protocol = TCP_PACKET;

        pDataDesc->sIp = pkt_swapu32(ipp->uiSIP);
        pDataDesc->dIp = pkt_swapu32(ipp->uiDIP);
        pDataDesc->acknowledgeNumber = pkt_swapu32(pTcp->acknowledgeNumber);
        pDataDesc->sPort = pkt_swapu16(pTcp->sPort);
        pDataDesc->dPort = pkt_swapu16(pTcp->dPort);
#else
		pDataDesc->uiflag = (unsigned int)pTcp->flags;
		pDataDesc->protocol = TCP_PACKET;

		pDataDesc->sIp = ipp->uiSIP;
		pDataDesc->dIp = ipp->uiDIP;
        pDataDesc->acknowledgeNumber = pTcp->acknowledgeNumber;
		pDataDesc->sPort = pTcp->sPort;
		pDataDesc->dPort = pTcp->dPort;
#endif
	}
	else if(UDP_PACKET == EthType)
	{
		pUdp = (UDPHDR *)p;	
		
		/*udp head offset*/
		iOffset += 8;
		p = rbuf + iOffset;
		
		if(iOffset > irlen)
		{
			return 0;	
		}

		pDataDesc->uiflag = 0;
        pDataDesc->acknowledgeNumber = 0;
		pDataDesc->protocol = UDP_PACKET;
#if defined(MIPSA1)
        pDataDesc->sIp = pkt_swapu32(ipp->uiSIP);
        pDataDesc->dIp = pkt_swapu32(ipp->uiDIP);
        pDataDesc->sPort = pkt_swapu16(pUdp->sport);
        pDataDesc->dPort = pkt_swapu16(pUdp->dport);
#else
		pDataDesc->sIp = ipp->uiSIP;
		pDataDesc->dIp = ipp->uiDIP;
		pDataDesc->sPort = pUdp->sport;
		pDataDesc->dPort = pUdp->dport;
#endif
	}
	
	pDataDesc->dataptr = p;
	pDataDesc->uidatalen = irlen - iOffset;


	memcpy(pDataDesc->smac,peth->ucSMac,6);
	memcpy(pDataDesc->dmac,peth->ucDMac,6);

	return 1;	
}



int pkt_recv(void *PktdumpInfo,u_char *rbuf,int len)
{
	PKTDUMP_INFO *dumpInfo = (PKTDUMP_INFO *)PktdumpInfo;
	u_char *pkt_data = rbuf;
	RULEITEM	*pRule = NULL;
	if(0 == dumpInfo->runflag)
	{
		//not run audit
		return 0;
	}

	if(len > 2048)
	{
		return 0;
	}

	DATA_DESC stDataDesc;
    TCPFRAGMENT_ITEM stTcpFItem;
	memset(&stDataDesc,0,sizeof(DATA_DESC));

	if(1 != pparse_get_dataptr(rbuf,len,&stDataDesc))
	{
		return 0;
	}

    int  iret = 0;
    if(stDataDesc.uidatalen > 64)
    {
        iret = get_match_rule(stDataDesc.protocol,stDataDesc.sPort,stDataDesc.dPort,
                              stDataDesc.dataptr,stDataDesc.uidatalen,&pRule);
    }

	 if(0!=iret && NULL != pRule)
	 {
         if (pRule->proto_type == UDP_PACKET)
         {
             if (pRule->proto_id == en_PROTO_ID_DHCP)
             {
                 char mac[16]= {0};
                 char devinfo[64]={0};

                 if(-1 != dhcp_parse(stDataDesc,mac,devinfo))
                 {
                     DEV_VHASH_ITEM *pstDevVhashItem = NULL;
                     DEV_VHash_FindStruct(mac,&pstDevVhashItem);
                     if(NULL == pstDevVhashItem )
                     {
                         DEV_VHASH_ITEM stDevItem;
                         memset(&stDevItem,0, sizeof(DEV_VHASH_ITEM));
                         strcpy(stDevItem.mac,mac);
                         strcpy(stDevItem.devinfo,devinfo);
                         DEV_VHash_Add(&stDevItem,&pstDevVhashItem);
                     }

                 }
             }
             else
             {
                 protocol_parse((void *)dumpInfo,pRule, stDataDesc);
             }
         }
         else if(stDataDesc.dPort == 20480 &&1 == pRule->multicap)
         {
                 memset(&stTcpFItem,0,sizeof(TCPFRAGMENT_ITEM));
                 stTcpFItem.sourceIp        = stDataDesc.sIp;
                 stTcpFItem.destinationIp   = stDataDesc.dIp;
                 stTcpFItem.sourcePort      = stDataDesc.sPort;
                 stTcpFItem.destinationPort = stDataDesc.dPort;
                 stTcpFItem.acknowledgeNumber = stDataDesc.acknowledgeNumber;
                 stTcpFItem.protocol        = stDataDesc.protocol;
                 stTcpFItem.uitcpdatalen    = stDataDesc.uidatalen;
                 stTcpFItem.tcpdataptr      = (char *)stDataDesc.dataptr;
                 stTcpFItem.uitime          = time(NULL);
             
                 TcpFragment_Queue_Add(&stTcpFItem,(void *)pRule);
         }
         else
         {
             protocol_parse((void *)dumpInfo,pRule, stDataDesc);
         }
     }
    else if( 0 != stDataDesc.uiflag)//是否有tcp分片
    {
        TCPFRAGMENT_VHASH_ITEM *pstTcpFVhashItem = NULL;
        
        memset(&stTcpFItem,0,sizeof(TCPFRAGMENT_ITEM));
        stTcpFItem.sourceIp        = stDataDesc.sIp;
        stTcpFItem.destinationIp   = stDataDesc.dIp;
        stTcpFItem.sourcePort      = stDataDesc.sPort;
        stTcpFItem.destinationPort = stDataDesc.dPort;
        stTcpFItem.acknowledgeNumber = stDataDesc.acknowledgeNumber;
        stTcpFItem.protocol        = stDataDesc.protocol;
        stTcpFItem.uitcpdatalen    = stDataDesc.uidatalen;
        stTcpFItem.tcpdataptr      = (char *)stDataDesc.dataptr;
        stTcpFItem.uitime          = time(NULL);
        
        TCPFRAGMENT_VHash_FindStruct(&stTcpFItem,&pstTcpFVhashItem);
        
        if(NULL != pstTcpFVhashItem)//存在tcp分片
        {

            if(stDataDesc.uidatalen > 0 &&  pstTcpFVhashItem->uitcpdatalen + stTcpFItem.uitcpdatalen <= MAX_TCP_FRAGMENT_TOTAL_LENGTH)
            {
                memcpy(pstTcpFVhashItem->tcpdataptr + pstTcpFVhashItem->uitcpdatalen,stTcpFItem.tcpdataptr,stTcpFItem.uitcpdatalen);
                pstTcpFVhashItem->uitcpdatalen += stTcpFItem.uitcpdatalen;
            }
            
            if(pstTcpFVhashItem->acknowledgeNumber != stDataDesc.acknowledgeNumber )/*tcp分片结束*/
            {
                stDataDesc.dataptr = (u_char *)pstTcpFVhashItem->tcpdataptr;
                stDataDesc.uidatalen = pstTcpFVhashItem->uitcpdatalen;
                RULEITEM *pprule = (RULEITEM *)pstTcpFVhashItem->ruleitem;
                protocol_parse((void *)dumpInfo,(RULEITEM *)pstTcpFVhashItem->ruleitem, stDataDesc);
                TcpFragment_Queue_Delete(&stTcpFItem);
            }
        }
    }

    return 1;
}






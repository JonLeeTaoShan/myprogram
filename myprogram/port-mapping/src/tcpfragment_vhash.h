




/* tcpframent_vhash.h*/



#ifndef _TCPFRAGMENT_VHASH_H
#define _TCPFRAGMENT_VHASH_H

#ifdef __cplusplus
extern "C" {
#endif


#define TCPFRAGMENT_VHASH_NUM_HASHSIZE  0x40	// hashtable¥Û–°
#define TCPFRAGMENT_VHASH(num)          ((num) & (TCPFRAGMENT_VHASH_NUM_HASHSIZE - 1))	// hashÀ˜“˝

#define MAX_TCP_FRAGMENT_TOTAL_LENGTH	20480

typedef struct _tagTCPFRAGMENT_ITEM
{
    unsigned int sourceIp;			//source ip address
    unsigned int destinationIp;	    //destination ip address
    unsigned short sourcePort;		//source port
    unsigned short destinationPort; //destination port
    unsigned short protocol;		//protocol used
    
    unsigned int acknowledgeNumber;	// Number of aknowledge
//    unsigned int sqenumber;   //sequence number
    unsigned int uitime;
//    unsigned int uiIsContentLen; //http 是否有表单
    unsigned int uitcpdatalen;
    char *tcpdataptr;
//    void *ruleitem;
}TCPFRAGMENT_ITEM;


typedef struct _tagTCPFRAGMENT_VHASH_ITEM
{
    unsigned int sourceIp;			//source ip address
    unsigned int destinationIp;	    //destination ip address
    unsigned short sourcePort;		//source port
    unsigned short destinationPort; //destination port
    
    unsigned short protocol;		//protocol used
    unsigned int acknowledgeNumber;	// Number of aknowledge
//    unsigned int sqenumber;        //sqeuence number
    unsigned int uitime;
    unsigned int uitcpdatalen;
//    unsigned int uiIsContentLen; //http 是否有表单
    char tcpdataptr[MAX_TCP_FRAGMENT_TOTAL_LENGTH];
    void *ruleitem;
    
    LIST_ENTRY Expired_Entry;       //Expired queue done
    LIST_ITEM *Expired_ListItem;
//    unsigned int uiMemUseCount;
}TCPFRAGMENT_VHASH_ITEM;



void TCPFRAGMENT_VHash_INIT(unsigned int uiFreeQueueLen);
unsigned int TCPFRAGMENT_VHash_Add(TCPFRAGMENT_ITEM *pstTcpFItem,TCPFRAGMENT_VHASH_ITEM **ppstTcpFVhashItem);
unsigned int TCPFRAGMENT_VHash_Update(TCPFRAGMENT_ITEM *pstTcpFItem,TCPFRAGMENT_VHASH_ITEM **ppstTcpFVhashItem);
unsigned int TCPFRAGMENT_VHash_Del(TCPFRAGMENT_ITEM *pstTcpFItem);
unsigned int TCPFRAGMENT_VHash_FindStruct(TCPFRAGMENT_ITEM *pstTcpFItem,TCPFRAGMENT_VHASH_ITEM **ppstTcpFVhashItem);
unsigned int TCPFRAGMENT_VHash_Get_NextNode(TCPFRAGMENT_ITEM *pstTcpFItem,TCPFRAGMENT_VHASH_ITEM *ppstVhashItem_prev, TCPFRAGMENT_VHASH_ITEM **ppstVhashItem);
void TCPFRAGMENT_VHash_Clean();
void TCPFRAGMENT_VHash_EXIT();




#ifdef __cplusplus
}
#endif


#endif/*_TCPFRAGMENT_VHASH_H*/



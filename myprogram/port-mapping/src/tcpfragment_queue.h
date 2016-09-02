
/* tcpframent_queue.h*/

#ifndef _TCPFRAMENT_QUEUE_H
#define _TCPFRAMENT_QUEUE_H

#ifdef __cplusplus
extern "C" {
#endif


#include "tcpfragment_vhash.h"

#define TCPFRAGMENT_QUEUE_INC_MemUseCount(pVHashItem)  \
{\
}

#define TCPFRAGMENT_QUEUE_DEC_MemUseCount(pVHashItem)  \
{\
}

#define TCPFRAGMENT_QUEUE_INSERT_QUEUE_TAIL(pListItem,pVHashItem,entry,ListItem)  \
{\
	if(NULL == pVHashItem->ListItem)\
	{\
		LIST_ENTRY *phead;\
		phead = &pListItem->ListHead;\
		InsertTailList(phead,&pVHashItem->entry);\
		pListItem->ulListLen++;\
		pVHashItem->ListItem = pListItem;\
		TCPFRAGMENT_QUEUE_INC_MemUseCount(pVHashItem);\
	}\
}



#define TCPFRAGMENT_QUEUE_REMOVE_FROM_QUEUE(pVHashItem,entry,ListItem)  \
{\
	LIST_ITEM *pListItem;\
	if(NULL != pVHashItem->ListItem)\
	{\
		RemoveEntryList(&pVHashItem->entry);\
		pListItem = pVHashItem->ListItem;\
		pListItem->ulListLen--;\
		pVHashItem->ListItem = NULL;\
		TCPFRAGMENT_QUEUE_DEC_MemUseCount(pVHashItem)\
	}\
}


#define TCPFRAGMENT_QUEUE_REMOVE_HEAD_FROM_QUEUE(pListItem,entry,ListItem,pVHashItem)  \
{\
	LIST_ENTRY *phead;\
	LIST_ENTRY *remove_entry;\
	phead = &pListItem->ListHead;\
	if(!IsListEmpty(phead))\
	{\
		remove_entry = RemoveHeadList(phead);\
		pVHashItem = CONTAINING_RECORD(remove_entry,TCPFRAGMENT_VHASH_ITEM,entry);\
		pListItem->ulListLen--;\
		pVHashItem->ListItem = NULL;\
		TCPFRAGMENT_QUEUE_DEC_MemUseCount(pVHashItem)\
	}\
}


int TcpFragment_Queue_Delete(TCPFRAGMENT_ITEM *pTcpFragmentItem);

int TcpFragment_Queue_Add(TCPFRAGMENT_ITEM *pTcpFragmentItem,void *ruleitem);

void TcpFragment_Queue_Init(unsigned int uiFreeQueueLen);


#ifdef __cplusplus
}
#endif


#endif/*_TCPFRAMENT_QUEUE_H*/



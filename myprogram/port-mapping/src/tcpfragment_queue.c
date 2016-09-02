


#include "product.h"
#include "vhash.h"
#include "tcpfragment_vhash.h"
#include "tcpfragment_queue.h"



/*TCPFRAGMENT_VHASH_ITEM 为其元素节点*/
typedef struct _tagTCPFRAGMENT_QUEUE
{
	LIST_ITEM ExpiredList;	
	SPINLOCK_S spinlock;
}TCPFRAGMENT_QUEUE;

TCPFRAGMENT_QUEUE g_stExpiredQueue;



int TcpFragment_Queue_Delete(TCPFRAGMENT_ITEM *pTcpFragmentItem)
{
	TCPFRAGMENT_VHASH_ITEM *pVhashItem = NULL;
	TCPFRAGMENT_QUEUE *pQueue = &g_stExpiredQueue;
	
	if(NULL == pTcpFragmentItem)
	{
		return -1;
	}

	SpinLock_Lock(&pQueue->spinlock);
	
	TCPFRAGMENT_VHash_FindStruct(pTcpFragmentItem,&pVhashItem);
	if(NULL != pVhashItem)
	{
		TCPFRAGMENT_QUEUE_REMOVE_FROM_QUEUE(pVhashItem,Expired_Entry,Expired_ListItem);
		
		TCPFRAGMENT_VHash_Del(pTcpFragmentItem);
	}
	
	SpinLock_unLock(&pQueue->spinlock);
	return 0;
}


int TcpFragment_Queue_Add(TCPFRAGMENT_ITEM *pTcpFragmentItem,void *ruleitem)
{
	TCPFRAGMENT_VHASH_ITEM *pVhashItem = NULL;
	LIST_ITEM *pExpiredList = NULL;
	TCPFRAGMENT_QUEUE *pQueue = &g_stExpiredQueue;
	
	if(NULL == pTcpFragmentItem)
	{
		return -1;
	}

	SpinLock_Lock(&pQueue->spinlock);
	
	TCPFRAGMENT_VHash_FindStruct(pTcpFragmentItem,&pVhashItem);
	if(NULL != pVhashItem)
	{
		TCPFRAGMENT_QUEUE_REMOVE_FROM_QUEUE(pVhashItem,Expired_Entry,Expired_ListItem);
		
		TCPFRAGMENT_VHash_Del(pTcpFragmentItem);
	}


	TCPFRAGMENT_VHash_Add(pTcpFragmentItem,&pVhashItem);
	if(NULL != pVhashItem)
	{
		pExpiredList = &pQueue->ExpiredList;
        pVhashItem->ruleitem = ruleitem;
		TCPFRAGMENT_QUEUE_INC_MemUseCount(pVhashItem);
		TCPFRAGMENT_QUEUE_INSERT_QUEUE_TAIL(pExpiredList,pVhashItem,Expired_Entry,Expired_ListItem);
		TCPFRAGMENT_QUEUE_DEC_MemUseCount(pVhashItem);
	}

	SpinLock_unLock(&pQueue->spinlock);
	return 0;
}


void* TcpFragment_Queue_Expired_Proc(void *arg)
{
	TCPFRAGMENT_VHASH_ITEM *pVhashItem = NULL;
	LIST_ENTRY *phead;
	LIST_ENTRY *entry;
	LIST_ITEM *pExpiredList = NULL;
	TCPFRAGMENT_QUEUE *pQueue = &g_stExpiredQueue;
	unsigned int uiCurrentTime = 0;
	unsigned char *pucSip = NULL;
	unsigned char *pucDip = NULL;

	pExpiredList = &pQueue->ExpiredList;
	phead = &pExpiredList->ListHead;
	
	while(1)
	{
		SpinLock_Lock(&pQueue->spinlock);
		if(IsListEmpty(phead))
		{
			SpinLock_unLock(&pQueue->spinlock);
			
			sleep(10);
			continue;
		}

		uiCurrentTime = time(NULL);
		for(entry = phead->Flink; entry != phead;)
		{
			pVhashItem = CONTAINING_RECORD(entry,TCPFRAGMENT_VHASH_ITEM,Expired_Entry);
			entry = entry->Flink;

			/*over 5 second*/
			if(5 < (uiCurrentTime - pVhashItem->uitime))
			{
				/*(1)show some infomation of the item*/
				pucSip = (unsigned char *)&pVhashItem->sourceIp;
				pucDip = (unsigned char *)&pVhashItem->destinationIp;
			
				printf("clean tcpframent sip=%u.%u.%u.%u:%u\t dip=%u.%u.%u.%u:%u\n",
							pucSip[3],pucSip[2],pucSip[1],pucSip[0],
							pVhashItem->sourcePort,
							pucDip[3],pucDip[2],pucDip[1],pucDip[0],
							pVhashItem->destinationPort);
				
				/*(2)delete old item from the Expired Queue*/
				TCPFRAGMENT_QUEUE_REMOVE_FROM_QUEUE(pVhashItem,Expired_Entry,Expired_ListItem);

				/*(3)delete old item from the hash table*/
				TCPFRAGMENT_VHash_Del((TCPFRAGMENT_ITEM *)pVhashItem);/*此处强制转换数据类型,前N个元素一致*/

				pucSip = NULL;
				pucDip = NULL;
			}
			
		}
		
		SpinLock_unLock(&pQueue->spinlock);
		
	}
	//PrintDeThreadLog("[TcpFragment_Queue_Expired_Proc] ----2----\n");
	
	return NULL;
}



void TcpFragment_Queue_Init(unsigned int uiFreeQueueLen)
{
	int iRet = 0;
	THREAD_HANDLE handle;
	LIST_ITEM *pListItem;
	LIST_ENTRY *phead;
	TCPFRAGMENT_QUEUE *pQueue = &g_stExpiredQueue;

	/*hash table init*/
	TCPFRAGMENT_VHash_INIT(uiFreeQueueLen);

	/*Expired queue init*/
	pListItem = &pQueue->ExpiredList;
	phead = &pListItem->ListHead;
	InitializeListHead(phead);
	pListItem->ulListLen = 0;
	InitSpinLock(&pQueue->spinlock);

	/*start a new thread to process expired item*/
	__CreateThread(handle, TcpFragment_Queue_Expired_Proc, (void *)NULL,iRet);
	if(iRet)
	{
		printf("[TcpFragment_Queue_Init] pthread_create fun error!\n");
	}
	
}



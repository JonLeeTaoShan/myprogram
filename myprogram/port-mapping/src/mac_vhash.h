
#ifndef __MAC_VHASH_H_
#define __MAC_VHASH_H_

#define MAC_VHASH_NUM_HASHSIZE		0x200	// hashtable大小
#define MAC_VHASH(num)			((num) & (MAC_VHASH_NUM_HASHSIZE - 1))	// hash索引


typedef struct _tagMAC_ITEM
{
	char key[20];//MAC地址，hash key
	char apmac[32];
	unsigned int fir_time;//第一条记录时间
	unsigned int las_time;//最后一条记录时间
	unsigned int lognum;//记录的日志数目
}MAC_ITEM;


typedef struct _tagMAC_VHASH_ITEM
{
	LIST_ENTRY updateentry;
	LIST_ITEM *upatdeListItem;
	unsigned int uioffset;
	MAC_ITEM stMItem;	
}MAC_VHASH_ITEM;

void MAC_VHash_INIT();
unsigned int MAC_VHash_Add(MAC_ITEM *pstItem,MAC_VHASH_ITEM **ppstVhashItem);
unsigned int MAC_VHash_Update(MAC_ITEM *pstItem,MAC_VHASH_ITEM **ppstVhashItem);
unsigned int MAC_VHash_Del(MAC_ITEM *pstItem);
unsigned int MAC_VHash_FindStruct(MAC_ITEM *pstItem,MAC_VHASH_ITEM **ppstVhashItem);
unsigned int MAC_VHash_Get_NextNode(MAC_ITEM *pstItem,MAC_VHASH_ITEM *ppstVhashItem_prev, MAC_VHASH_ITEM **ppstVhashItem);
void MAC_VHash_EXIT();
void MAC_VHash_Lock();
void MAC_VHash_unLock();
unsigned int MAC_VHash_GetCount(unsigned int *puicount);

void MAC_VHash_Clean();



#define MAC_VHASH_REMOVE_FROM_QUEUE(pVHashItem,entry,ListItem)  \
{\
	LIST_ITEM *pListItem;\
	if(NULL != pVHashItem->ListItem)\
	{\
		RemoveEntryList(&pVHashItem->entry);\
		pListItem = pVHashItem->ListItem;\
		pListItem->ulListLen--;\
		pVHashItem->ListItem = NULL;\
		MAC_VHASH_DEC_MemUseCount(pVHashItem)\
	}\
}

#define MAC_VHASH_INSERT_QUEUE_TAIL(pListItem,pVHashItem,entry,ListItem)  \
{\
	if(NULL == pVHashItem->ListItem)\
	{\
		LIST_ENTRY *phead;\
		unsigned int uiRet = 0;\
		phead = &pListItem->ListHead;\
		InsertTailList(phead,&pVHashItem->entry);\
		pListItem->ulListLen++;\
		pVHashItem->ListItem = pListItem;\
		MAC_VHASH_INC_MemUseCount(pVHashItem,uiRet);\
	}\
}


#define MAC_VHASH_REMOVE_HEAD_FROM_QUEUE(pListItem,entry,ListItem,pVHashItem)  \
{\
	LIST_ENTRY *phead;\
	LIST_ENTRY *remove_entry;\
	phead = &pListItem->ListHead;\
	if(!IsListEmpty(phead))\
	{\
		remove_entry = RemoveHeadList(phead);\
		pVHashItem = CONTAINING_RECORD(remove_entry,MAC_VHASH_ITEM,entry);\
		pListItem->ulListLen--;\
		pVHashItem->ListItem = NULL;\
		MAC_VHASH_DEC_MemUseCount(pVHashItem)\
	}\
}




#define MAC_VHASH_INC_MemUseCount(pVHashItem,Ret)  \
{\
	Ret = __VHASH_OK;\
}


#define MAC_VHASH_DEC_MemUseCount(pVHashItem)  \
{\
}



#endif //




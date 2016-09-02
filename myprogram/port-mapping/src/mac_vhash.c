
#include "product.h"
#include "vhash.h"
#include "mac_vhash.h"


VHASH_UINT4 MAC_VHash_Calc(void *pkey);
VHASH_UINT4 MAC_VHash_Compare(void *pkey,void *pData);
VHASH_UINT4 MAC_VHash_CleanData(void *pData);

VHASH_UINT4 MAC_VHash_Calc(void *pkey)
{
	unsigned int uinum = 0;
	unsigned int *puiNum = (unsigned int *)pkey;
	unsigned int i; 	
	for(i = 0; i < 5; i++)
	{
		uinum ^= puiNum[i];	
	}
	
	return MAC_VHASH(uinum);
}

VHASH_UINT4 MAC_VHash_Compare(void *pkey,void *pData)
{
	char *key = (char *)pkey;
	MAC_VHASH_ITEM *pstVhashItem = (MAC_VHASH_ITEM *)pData;
	if(0 == strcmp(key,pstVhashItem->stMItem.key))
	{
		return __VHASH_OK;
	}
	
	return __VHASH_ERR;
}

VHASH_UINT4 MAC_VHash_IsUpdate(void *pData,void *pNewData)
{
	return __VHASH_OK;
}


VHASH_UINT4 MAC_VHash_clean(void *pData)
{	
	return __VHASH_OK;
}

VHASH_UINT4 MAC_VHash_copy(void *pDestData,void *pSrcData,unsigned int uiLen)
{
	VHASH_UINT4 uiRet = __VHASH_OK;
	MAC_VHASH_ITEM *pstVhashItem = (MAC_VHASH_ITEM *)pDestData;
	MAC_ITEM *pstItem = (MAC_ITEM *)pSrcData;
	
	if (sizeof(MAC_VHASH_ITEM) > uiLen)
	{
		return __VHASH_ERR;
	}
	
	memcpy(&pstVhashItem->stMItem, pstItem,sizeof(MAC_ITEM));

	return uiRet;
}



void MAC_VHash_INIT()
{
	VHASH_MOUDLE stvhm;
	memset(&stvhm,0,sizeof(VHASH_MOUDLE));
	stvhm.uiBucketCount = MAC_VHASH_NUM_HASHSIZE;
	stvhm.uiDataLen = sizeof(MAC_VHASH_ITEM);
	stvhm.pfnVHash_Calc = MAC_VHash_Calc;
	stvhm.pfnVHash_Compare = MAC_VHash_Compare;
	stvhm.pfnVHash_IsUpdate = MAC_VHash_IsUpdate;
	stvhm.pfnVHash_clean = MAC_VHash_clean;
	stvhm.pfnVHash_cpy = MAC_VHash_copy;
	VHash_Create_Moudle(en_MAC,&stvhm);
}

unsigned int MAC_VHash_Add(MAC_ITEM *pstItem,MAC_VHASH_ITEM **ppstVhashItem)
{
	MAC_VHASH_ITEM *pstVhashItem = NULL;
	VHASH_UINT4 vhash_ret;
	vhash_ret = VHash_Add_Node2(en_MAC,(void *)pstItem->key,(void *)pstItem,(void **)&pstVhashItem);
	if((__VHASH_OK == vhash_ret  || __VHASH_UPDATE_OK == vhash_ret ) && NULL != pstVhashItem && NULL != ppstVhashItem)
	{
		*ppstVhashItem = pstVhashItem;
	}
	return vhash_ret;
}


unsigned int MAC_VHash_Update(MAC_ITEM *pstItem,MAC_VHASH_ITEM **ppstVhashItem)
{
	MAC_VHASH_ITEM *pstVhashItem = NULL;
	VHASH_UINT4 vhash_ret;
	vhash_ret = VHash_Add_Node2(en_MAC,(void *)pstItem->key,(void *)pstItem,(void **)&pstVhashItem);
	if(__VHASH_UPDATE_OK == vhash_ret && NULL != pstVhashItem && NULL != ppstVhashItem)
	{
		*ppstVhashItem = pstVhashItem;
	}
	return vhash_ret;
}


unsigned int MAC_VHash_Del(MAC_ITEM *pstItem)
{
	return VHash_Del_Node(en_MAC,(void *)pstItem->key);
}

unsigned int MAC_VHash_FindStruct(MAC_ITEM *pstItem,MAC_VHASH_ITEM **ppstVhashItem)
{
	VHASH_UINT4 ulRet;
	ulRet = VHash_Get_Node(en_MAC,(void *)pstItem->key,(void **)ppstVhashItem);
	return ulRet;
}

unsigned int MAC_VHash_Get_NextNode(MAC_ITEM *pstItem,MAC_VHASH_ITEM *ppstVhashItem_prev, MAC_VHASH_ITEM **ppstVhashItem)
{
	VHASH_UINT4 ulRet;
	ulRet = VHash_Get_NextNode(en_MAC,(void *)pstItem->key,(void *)ppstVhashItem_prev,(void **)ppstVhashItem);
	return ulRet;
}

void MAC_VHash_Clean()
{
	VHash_Clean_Moudle(en_MAC);
}

void MAC_VHash_EXIT()
{
	VHash_Destory_Moudle(en_MAC);
}


void MAC_VHash_Lock()
{
	VHash_Lock(en_MAC);
}


void MAC_VHash_unLock()
{
	VHash_unLock(en_MAC);
}

unsigned int MAC_VHash_GetCount(unsigned int *puicount)
{
	return VHash_Node_Total(en_MAC,puicount);
}




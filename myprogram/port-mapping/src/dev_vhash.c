//
// Created by 韩清华 on 16/2/29.
//
#include "product.h"
#include "vhash.h"
#include "dev_vhash.h"


VHASH_UINT4 DEV_VHash_Calc(void *pkey);
VHASH_UINT4 DEV_VHash_Compare(void *pkey,void *pData);
VHASH_UINT4 DEV_VHash_CleanData(void *pData);

VHASH_UINT4 DEV_VHash_Calc(void *pkey)
{
    unsigned int uinum = 0;
    unsigned int *puiNum = (unsigned int *)pkey;
    unsigned int i;
    for(i = 0; i < 3; i++)
    {
        uinum ^= puiNum[i];
    }

    return DEV_VHASH(uinum);
}

VHASH_UINT4 DEV_VHash_Compare(void *pkey,void *pData)
{
    char *key = (char *)pkey;
    DEV_VHASH_ITEM *pstVhashItem = (DEV_VHASH_ITEM *)pData;
    if(0 == strcmp(key,pstVhashItem->mac))
    {
        return __VHASH_OK;
    }

    return __VHASH_ERR;
}

VHASH_UINT4 DEV_VHash_IsUpdate(void *pData,void *pNewData)
{
    return __VHASH_OK;
}


VHASH_UINT4 DEV_VHash_clean(void *pData)
{
    return __VHASH_OK;
}

VHASH_UINT4 DEV_VHash_copy(void *pDestData,void *pSrcData,unsigned int uiLen)
{
    VHASH_UINT4 uiRet = __VHASH_OK;
    DEV_VHASH_ITEM *pstVhashItem = (DEV_VHASH_ITEM *)pDestData;
    DEV_VHASH_ITEM *pstItem = (DEV_VHASH_ITEM *)pSrcData;

    if (sizeof(DEV_VHASH_ITEM) > uiLen)
    {
        return __VHASH_ERR;
    }

    memcpy(pstVhashItem, pstItem,sizeof(DEV_VHASH_ITEM));

    return uiRet;
}



void DEV_VHash_init()
{
    VHASH_MOUDLE stvhm;
    memset(&stvhm,0,sizeof(VHASH_MOUDLE));
    stvhm.uiBucketCount = DEV_VHASH_NUM_HASHSIZE;
    stvhm.uiDataLen = sizeof(DEV_VHASH_ITEM);
    stvhm.pfnVHash_Calc = DEV_VHash_Calc;
    stvhm.pfnVHash_Compare = DEV_VHash_Compare;
    stvhm.pfnVHash_IsUpdate = DEV_VHash_IsUpdate;
    stvhm.pfnVHash_clean = DEV_VHash_clean;
    stvhm.pfnVHash_cpy = DEV_VHash_copy;
    VHash_Create_Moudle(en_DEV,&stvhm);
}

unsigned int DEV_VHash_Add(DEV_VHASH_ITEM *pstItem,DEV_VHASH_ITEM **ppstVhashItem)
{
    DEV_VHASH_ITEM *pstVhashItem = NULL;
    VHASH_UINT4 vhash_ret;
    vhash_ret = VHash_Add_Node2(en_DEV,(void *)pstItem->mac,(void *)pstItem,(void **)&pstVhashItem);
    if((__VHASH_OK == vhash_ret  || __VHASH_UPDATE_OK == vhash_ret ) && NULL != pstVhashItem && NULL != ppstVhashItem)
    {
        *ppstVhashItem = pstVhashItem;
    }
    return vhash_ret;
}


unsigned int DEV_VHash_Update(DEV_VHASH_ITEM *devItem,DEV_VHASH_ITEM **ppstVhashItem)
{
    DEV_VHASH_ITEM *pstVhashItem = NULL;
    VHASH_UINT4 vhash_ret;
    vhash_ret = VHash_Add_Node2(en_DEV,(void *)devItem->mac,(void *)devItem,(void **)&pstVhashItem);
    if(__VHASH_UPDATE_OK == vhash_ret && NULL != pstVhashItem && NULL != ppstVhashItem)
    {
        *ppstVhashItem = pstVhashItem;
    }
    return vhash_ret;
}



unsigned int DEV_VHash_Del(char *mac)
{
    return VHash_Del_Node(en_DEV,(void *)mac);
}

unsigned int DEV_VHash_FindStruct(char *mac,DEV_VHASH_ITEM **ppstVhashItem)
{
    VHASH_UINT4 ulRet;
    ulRet = VHash_Get_Node(en_DEV,(void *)mac,(void **)ppstVhashItem);
    return ulRet;
}

void DEV_VHash_Clean()
{
    VHash_Clean_Moudle(en_DEV);
}

void DEV_VHash_EXIT()
{
    VHash_Destory_Moudle(en_DEV);
}


void DEV_VHash_Lock()
{
    VHash_Lock(en_DEV);
}


void DEV_VHash_unLock()
{
    VHash_unLock(en_DEV);
}


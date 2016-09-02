


#include "product.h"
#include "vhash.h"
#include "tcpfragment_vhash.h"





VHASH_UINT4 TCPFRAGMENT_VHash_Calc(void *pkey)
{
	unsigned short *puskey = (unsigned short *)pkey;	
	unsigned short sum=0;
	unsigned int i = 0;
	for(i = 0; i < 6; i++)
	{
		sum ^= puskey[i];
	}
	return TCPFRAGMENT_VHASH(sum);
}

VHASH_UINT4 TCPFRAGMENT_VHash_Compare(void *pkey,void *pData)
{
	TCPFRAGMENT_ITEM *pstTcpFItem = (TCPFRAGMENT_ITEM *)pkey;
	TCPFRAGMENT_VHASH_ITEM *pstVhashItem = (TCPFRAGMENT_VHASH_ITEM *)pData;
	if(pstTcpFItem->sourceIp == pstVhashItem->sourceIp
		&& pstTcpFItem->destinationIp == pstVhashItem->destinationIp
		&& pstTcpFItem->sourcePort == pstVhashItem->sourcePort
		&& pstTcpFItem->destinationPort == pstVhashItem->destinationPort)
	{
		return __VHASH_OK;
	}

	return __VHASH_ERR;
}

VHASH_UINT4 TCPFRAGMENT_VHash_IsUpdate(void *pData,void *pNewData)
{
	return __VHASH_OK;
}


VHASH_UINT4 TCPFRAGMENT_VHash_clean(void *pData)
{	
	TCPFRAGMENT_VHASH_ITEM *pstVhashItem = (TCPFRAGMENT_VHASH_ITEM *)pData;
	return __VHASH_OK;
}

VHASH_UINT4 TCPFRAGMENT_VHash_copy(void *pDestData,void *pSrcData,unsigned int uiLen)
{
	TCPFRAGMENT_VHASH_ITEM *pstVhashItem = (TCPFRAGMENT_VHASH_ITEM *)pDestData;
	TCPFRAGMENT_ITEM *pstTcpFItem = (TCPFRAGMENT_ITEM *)pSrcData;
	VHASH_UINT4 uiRet = __VHASH_OK;

	if(0 == pstVhashItem->sourceIp)
	{
		pstVhashItem->sourceIp        = pstTcpFItem->sourceIp;
		pstVhashItem->destinationIp   = pstTcpFItem->destinationIp;
		pstVhashItem->sourcePort      = pstTcpFItem->sourcePort;
		pstVhashItem->destinationPort = pstTcpFItem->destinationPort;
		pstVhashItem->acknowledgeNumber = pstTcpFItem->acknowledgeNumber;
		pstVhashItem->protocol        = pstTcpFItem->protocol;
		pstVhashItem->uitime          = pstTcpFItem->uitime;
		pstVhashItem->uitcpdatalen    = pstTcpFItem->uitcpdatalen;
        
//        pstVhashItem->sqenumber = pstTcpFItem->sqenumber;
//        pstVhashItem->uiIsContentLen = pstTcpFItem->uiIsContentLen;
        
		if(pstTcpFItem->uitcpdatalen > 0 
			&& pstTcpFItem->uitcpdatalen <= MAX_TCP_FRAGMENT_TOTAL_LENGTH
			&& NULL != pstTcpFItem->tcpdataptr)
		{
			memcpy(pstVhashItem->tcpdataptr,pstTcpFItem->tcpdataptr,pstTcpFItem->uitcpdatalen);
		}
	}

	return uiRet;
}



unsigned int TCPFRAGMENT_VHash_Add(TCPFRAGMENT_ITEM *pstTcpFItem,TCPFRAGMENT_VHASH_ITEM **ppstTcpFVhashItem)
{
	TCPFRAGMENT_VHASH_ITEM *pstVhashItem = NULL;
	VHASH_UINT4 vhash_ret;
	vhash_ret = VHash_Add_Node2(en_TCPFRAGMENT,(void *)pstTcpFItem,(void *)pstTcpFItem,(void **)&pstVhashItem);
	if((__VHASH_OK == vhash_ret || __VHASH_UPDATE_OK == vhash_ret) && NULL != pstVhashItem && NULL != ppstTcpFVhashItem)
	{
		*ppstTcpFVhashItem = pstVhashItem;
	}
	return vhash_ret;
}



unsigned int TCPFRAGMENT_VHash_Update(TCPFRAGMENT_ITEM *pstTcpFItem,TCPFRAGMENT_VHASH_ITEM **ppstTcpFVhashItem)
{
	TCPFRAGMENT_VHASH_ITEM *pstVhashItem = NULL;
	VHASH_UINT4 vhash_ret;
	vhash_ret = VHash_Add_Node2(en_TCPFRAGMENT,(void *)pstTcpFItem,(void *)pstTcpFItem,(void **)&pstVhashItem);
	if(__VHASH_UPDATE_OK == vhash_ret && NULL != pstVhashItem && NULL != ppstTcpFVhashItem)
	{
		*ppstTcpFVhashItem = pstVhashItem;
	}
	return vhash_ret;
}


unsigned int TCPFRAGMENT_VHash_Del(TCPFRAGMENT_ITEM *pstTcpFItem)
{
	return VHash_Del_Node(en_TCPFRAGMENT,(void *)pstTcpFItem);
}


unsigned int TCPFRAGMENT_VHash_FindStruct(TCPFRAGMENT_ITEM *pstTcpFItem,TCPFRAGMENT_VHASH_ITEM **ppstTcpFVhashItem)
{
	VHASH_UINT4 ulRet;
	ulRet = VHash_Get_Node(en_TCPFRAGMENT,(void *)pstTcpFItem,(void **)ppstTcpFVhashItem);
	return ulRet;
}

unsigned int TCPFRAGMENT_VHash_Get_NextNode(TCPFRAGMENT_ITEM *pstTcpFItem,TCPFRAGMENT_VHASH_ITEM *ppstVhashItem_prev, TCPFRAGMENT_VHASH_ITEM **ppstVhashItem)
{
	VHASH_UINT4 ulRet;
	ulRet = VHash_Get_NextNode(en_TCPFRAGMENT,(void *)pstTcpFItem,(void *)ppstVhashItem_prev,(void **)ppstVhashItem);
	return ulRet;
}

void TCPFRAGMENT_VHash_Clean()
{
	VHash_Clean_Moudle(en_TCPFRAGMENT);
}

void TCPFRAGMENT_VHash_EXIT()
{
	VHash_Destory_Moudle(en_TCPFRAGMENT);
}

void TCPFRAGMENT_VHash_INIT(unsigned int uiFreeQueueLen)
{
	VHASH_MOUDLE stvhm;

	memset(&stvhm,0,sizeof(VHASH_MOUDLE));
	stvhm.uiModeId = 0;
	stvhm.uiBucketCount = TCPFRAGMENT_VHASH_NUM_HASHSIZE;
	stvhm.uiDataLen = sizeof(TCPFRAGMENT_VHASH_ITEM);
		
	stvhm.pfnVHash_Calc     = TCPFRAGMENT_VHash_Calc;
	stvhm.pfnVHash_Compare  = TCPFRAGMENT_VHash_Compare;
	stvhm.pfnVHash_IsUpdate = TCPFRAGMENT_VHash_IsUpdate;
	stvhm.pfnVHash_clean    = TCPFRAGMENT_VHash_clean;
	stvhm.pfnVHash_cpy      = TCPFRAGMENT_VHash_copy;
	
	VHash_Create_Moudle(en_TCPFRAGMENT,&stvhm);

}



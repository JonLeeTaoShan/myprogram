



#include "product.h"
#include "vhash.h"
#include "mac_vhash.h"
#include "mac_record_file.h"
#include "public.h"

#define FILE_RECORD_TIME_INTERVER (10*60)


typedef struct _tagMAC_QUEUE
{
	LIST_ITEM UpdateListItem;
	SPINLOCK_S spinlock;
}MAC_QUEUE;


MAC_QUEUE g_stMQue;


unsigned int g_uiOffset = 0;


int file_record_update(MAC_ITEM *pstMItem)
{
	MAC_VHASH_ITEM *pVHashItem = NULL;	
	LIST_ITEM *pListItem;
	
	MAC_QUEUE *pQueue = &g_stMQue;

	MAC_VHash_FindStruct(pstMItem,&pVHashItem);

	unsigned int uiCur = time(NULL);

	if(NULL == pVHashItem)
	{
		pstMItem->lognum = 1;
		pstMItem->fir_time = uiCur;
		pstMItem->las_time = uiCur;
		MAC_VHash_Add(pstMItem,&pVHashItem);
		pVHashItem->uioffset = INVALID_OFFSET;
	}
	else
	{
		pVHashItem->stMItem.lognum ++;
		pVHashItem->stMItem.las_time = uiCur;
	}
	
	SpinLock_Lock(&pQueue->spinlock);

	/* ������¶��� */
	pListItem = &pQueue->UpdateListItem;
	MAC_VHASH_INSERT_QUEUE_TAIL(pListItem,pVHashItem,updateentry,upatdeListItem);

	/*
	printf("MAC %s firtime %u lastime %u lognum %u is add updatelist!\n",pVHashItem->stMItem.key,\
		pVHashItem->stMItem.fir_time,pVHashItem->stMItem.las_time,pVHashItem->stMItem.lognum);
	*/
	SpinLock_unLock(&pQueue->spinlock);

	
	return 0;
}




int MAC_Record_WriteFile()
{
	LIST_ENTRY *phead;
	MAC_VHASH_ITEM *pVHashItem = NULL;
	unsigned int i = 0;
	unsigned int uiCount = 0;
	LIST_ITEM *pListItem;
	unsigned int length = 0;
	MAC_QUEUE *pQueue = &g_stMQue;
	MAC_ITEM *pItem = NULL;

	char file[128] = {0};
	sprintf(file,"%s%s",FILE_RECORD_PATH,FILE_RECORD_NAME);

	FILE *fp = fopen(file,"rb+");
	if(NULL == fp)
	{
		fp = fopen(file,"wb");
		if(NULL == fp)
		{
			return -1;
		}
	}

//	openlog("", LOG_NDELAY, LOG_USER);

	unsigned int uiCur = time(NULL);

	fseek(fp,0L,SEEK_SET);
	fwrite(&uiCur,sizeof(unsigned int),1,fp);
	
	SpinLock_Lock(&pQueue->spinlock);


	pListItem = &pQueue->UpdateListItem;
	phead = &pListItem->ListHead;

	while(!IsListEmpty(phead))
	{
		MAC_VHASH_REMOVE_HEAD_FROM_QUEUE(pListItem,updateentry,upatdeListItem,pVHashItem);

		pItem = &pVHashItem->stMItem;

		if(0 == pItem->key[0])
		{
			continue;
		}
		

		if(INVALID_OFFSET == pVHashItem->uioffset)
		{
			fseek(fp,0L,SEEK_END);
			g_uiOffset += sizeof(MAC_ITEM);
		}
		else
		{
			fseek(fp,pVHashItem->uioffset,SEEK_SET);
		}
		
        writeLogInfo((void *)&pVHashItem->stMItem);
		
		
		fwrite(&pVHashItem->stMItem,sizeof(MAC_ITEM),1,fp);
/*
		printf("RECORD MAC %s firtime %u lastime %u lognum %u is add updatelist!\n",pVHashItem->stMItem.key,\
		pVHashItem->stMItem.fir_time,pVHashItem->stMItem.las_time,pVHashItem->stMItem.lognum);
*/
		MAC_VHASH_DEC_MemUseCount(pVHashItem);
	}

	if(NULL !=	fp)
	{
		fclose(fp);
	}
	
	SpinLock_unLock(&pQueue->spinlock);

 //   closelog();
	return 0;
}

int MAC_Queue_List_Clean()
{
	MAC_QUEUE *pQueue = &g_stMQue;
	SpinLock_Lock(&pQueue->spinlock);

	LIST_ENTRY *phead;
	LIST_ITEM *pListItem;
	pListItem = &pQueue->UpdateListItem;
	phead = &pListItem->ListHead;
	MAC_VHASH_ITEM *pVHashItem = NULL;
	while(!IsListEmpty(phead))
	{
		MAC_VHASH_REMOVE_HEAD_FROM_QUEUE(pListItem,updateentry,upatdeListItem,pVHashItem);
	}
	SpinLock_unLock(&pQueue->spinlock);

//	printf("MAC Queue List Clean!\n");

	return 0;
}

#if 0
unsigned int GetNextNMinitePoint(unsigned int interval_minites,unsigned int uiCurTime)
{
	time_t uiNextTime = (time_t)uiCurTime;
	unsigned int uisec = interval_minites * 60;
	unsigned int uiMin = 0;
	struct tm *p;

	p = localtime(&uiNextTime); 

	if(0 == p->tm_sec && 0 == (p->tm_min % interval_minites))
	{
		/*��һ��n���� */
		uiNextTime += uisec; 
	}
	else
	{
		/*+ n������һ�� */
		uiNextTime += (uisec - 1);
		p = localtime(&uiNextTime); 
		p->tm_sec = 0;
		uiMin = p->tm_min % interval_minites;
		p->tm_min -= uiMin;
		uiNextTime = mktime(p);
	}
	
	return uiNextTime;
}


void* Mac_File_Record_Proc(void *arg)
{
	time_t uiCurTime = time(NULL);
	time_t uiNextTime = uiCurTime;
	
	uiNextTime = GetNextNMinitePoint(10,uiNextTime);

	char szbuf[128];
	sprintf(szbuf,"%s%s",FILE_RECORD_PATH,FILE_RECORD_NAME);
	while(1)
	{
		uiCurTime = time(NULL);
		if(uiCurTime > uiNextTime)
		{
			MAC_Record_WriteFile();
			
			uiNextTime = GetNextNMinitePoint(10,uiNextTime);
		}
		struct tm *p;
		p = localtime(&uiCurTime); 
		if((0 == p->tm_hour) && (0 == p->tm_min) && (p->tm_sec >=0) && (p->tm_sec <= 5))
		{
			MAC_Queue_List_Clean();
			MAC_VHash_Clean();
			remove(szbuf);
		}
		sleep(3);
	}
}
#endif
void* Mac_File_Record_Proc(void *arg)
{
	time_t uiCurTime = time(NULL);

	char szbuf[128];
	sprintf(szbuf,"%s%s",FILE_RECORD_PATH,FILE_RECORD_NAME);
	while(1)
	{
		uiCurTime = time(NULL);
		MAC_Record_WriteFile();
		
		struct tm *p;
		p = localtime(&uiCurTime); 
		if((0 == p->tm_hour) && (p->tm_min >= 0) && (p->tm_min <= 20))
		{
			MAC_Queue_List_Clean();
			MAC_VHash_Clean();
			remove(szbuf);
		}
		sleep(600);
	}
}

int Mac_ReadFile()
{
	MAC_VHASH_ITEM *pVHashItem = NULL;
	FILE *pDBFile = NULL;
	unsigned int uisize = 0;
	unsigned int  uiOffset = 0;
	int iFileRet = 0;
	MAC_ITEM stItem;

	unsigned int uiCurRecords = 0;
	
	char file[128];


	sprintf(file,"%s%s",FILE_RECORD_PATH,FILE_RECORD_NAME);

	pDBFile = fopen(file, "rb");
	if(NULL == pDBFile)
	{
		return -1;;
	}

	unsigned int uiCur = time(NULL);
	unsigned int uiFileTime = 0;
	fread(&uiFileTime,sizeof(unsigned int),1,pDBFile);
	if(uiCur - uiFileTime > FILE_RECORD_TIME_INTERVER)
	{
		fclose(pDBFile);
		remove(file);
		return 0;
	}

	printf("file Record Time %d\n",uiFileTime);

	uiOffset += sizeof(unsigned int) ;
	uisize = sizeof(MAC_ITEM);

	while(1)
	{
		if(feof(pDBFile))
		{
			break;
		}

		
		memset(&stItem,0,sizeof(MAC_ITEM));
		iFileRet = fread(&stItem,uisize,1,pDBFile);
		if(iFileRet <= 0)
		{
			break;
		}

		if(feof(pDBFile))
		{
			break;
		}
		if(0 == stItem.key[0])
		{
			
		}
		else
		{
		
			pVHashItem = NULL;
			if (0 == MAC_VHash_Add(&stItem,&pVHashItem) && NULL != pVHashItem)
			{
				pVHashItem->uioffset = uiOffset;
					printf("Read MAC %s firtime %u lastime %u lognum %u is add updatelist!\n",pVHashItem->stMItem.key,\
		pVHashItem->stMItem.fir_time,pVHashItem->stMItem.las_time,pVHashItem->stMItem.lognum);
			}
		}
		
		uiOffset += uisize;
		uiCurRecords++;	
	}

	fclose(pDBFile);

	g_uiOffset = uiOffset;
	
	return 0;
}


int file_record_init()
{
	LIST_ENTRY *phead;
	LIST_ITEM *pListItem;
	int iRet = 0;
	MAC_QUEUE *pQueue = &g_stMQue;
	

	pListItem = &pQueue->UpdateListItem;
	phead = &pListItem->ListHead;
	InitializeListHead(phead);
	pListItem->ulListLen = 0;
	
	InitSpinLock(&pQueue->spinlock);

	MAC_VHash_INIT();
	

	Mac_ReadFile();


	THREAD_HANDLE handle;

	__CreateThread(handle, Mac_File_Record_Proc, (void *)NULL,iRet);
	if(iRet)
	{
		printf("pthread_create fun error!\n");
	}

	return 0;


}








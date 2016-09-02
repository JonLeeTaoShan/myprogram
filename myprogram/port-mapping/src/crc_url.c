
#include "product.h"
#include "crc_url.h"
#include "list.h"

#define CMP_STR_LEN		128
#define LIST_LEN		1000

#define DEAD_TIME	20

typedef struct _tagPARSEENTRY
{
	LIST_ENTRY parentry;
	char cmpstr[CMP_STR_LEN];
	unsigned int stamp;
	PROTOCOL_PARSE stPtcParse;
}PARSEENTRY;

typedef struct _tagURLQUEUE
{
 	LIST_ITEM ListItem;
	SPINLOCK_S spinlock;
}URLQUEUE;

URLQUEUE g_UQue;

int url_parse(char *url,char *dest)
{
	if(NULL == url || NULL == dest || 0 == strlen(url))
	{
		return 0;
	}
	char *p = url;
	char *p3 = strstr(p,"http://");
	if(NULL != p3)
	{
		p += 7;
	}
	else
	{
		p3 = strstr(p,"https://");
		if(NULL != p3)
		{
			p += 8;
		}
	}
	char *p1 = NULL;
	char *p2 = NULL;
	p1 = strchr(p,'/');
	unsigned int len = 0;
	if(NULL != p1)
	{
		p1 += 1;
		p2 = strchr(p1,'/');
		if(NULL != p2)
		{
			len = (unsigned long)p2 - (unsigned long)p;
			len = len < 128?len:127;
			memcpy(dest,p,len);			
		}
		else
		{
			p2 = strchr(p1,'&');
			if(NULL != p2)
			{
				len = (unsigned long)p2 - (unsigned long)p;
				len = len < 128?len:127;
				memcpy(dest,p,len);			
			}
			else
			{
				len = strlen(p);
				len = len < 128?len:127;
				memcpy(dest,p,len);
			}
		}
	}
	else
	{
		len = strlen(p);
		len = len < 128?len:127;
		memcpy(dest,p,len);
	}
	
	return 0;	
}


int check_url(PROTOCOL_PARSE *pPtcParse)
{
	if(NULL == pPtcParse)
	{
		return -1;
	}

	URLQUEUE *pQue = &g_UQue;
	LIST_ITEM *pListItem = &pQue->ListItem;
	LIST_ENTRY *phead = &pListItem->ListHead;
	LIST_ENTRY *pentry;

	PARSEENTRY *pParTmp =NULL;

	int flag = 0;

	char cmpstr[CMP_STR_LEN] = {0};
	
	SpinLock_Lock(&pQue->spinlock);

	url_parse(pPtcParse->url, cmpstr);
	
	for(pentry = phead->Flink; pentry != phead; pentry = pentry->Flink)
	{
		pParTmp = CONTAINING_RECORD(pentry, PARSEENTRY, parentry);
		if( (NULL != pParTmp) && (0 == strcmp(pParTmp->cmpstr,cmpstr)) &&
			0 == strcmp(pParTmp->stPtcParse.srcMac,pPtcParse->srcMac))
		{
			flag = 1;
			break;
		}
		pParTmp =NULL;
	}
	SpinLock_unLock(&pQue->spinlock);

	if(1 == flag)
	{
//		printf("url %s is exist!\n",pPtcParse->url);
		return 1;
	}

//	printf("url %s is NOT exist!\n",pPtcParse->url);
	
	return 0;
	
}

int crc_url_add(PROTOCOL_PARSE *pPtcParse)
{
	if(NULL == pPtcParse)
	{
		return -1;
	}

	URLQUEUE *pQue = &g_UQue;
	LIST_ITEM *pListItem = &pQue->ListItem;
	LIST_ENTRY *phead = &pListItem->ListHead;
	LIST_ENTRY *pentry;
#if 0
	if( 0 != SpinLock_tryLock(&pQue->spinlock))//链表还在使用
	{
		SpinLock_unLock(&pQue->spinlock);//
		return -1;
	}
//	SpinLock_unLock(&pQue->spinlock);//
#endif
	SpinLock_Lock(&pQue->spinlock);
	if(pListItem->ulListLen >= LIST_LEN)
	{
		SpinLock_unLock(&pQue->spinlock);
		return -1;
	}
	
	PARSEENTRY *pPar = (PARSEENTRY *)malloc(sizeof(PARSEENTRY));
	memset(pPar, 0, sizeof(PARSEENTRY));
	memcpy(&pPar->stPtcParse,pPtcParse,sizeof(PROTOCOL_PARSE));
	pPar->stamp = time(NULL);
	url_parse(pPar->stPtcParse.url, pPar->cmpstr);

	InsertTailList(phead, &pPar->parentry);
	pListItem->ulListLen ++;
	
	SpinLock_unLock(&pQue->spinlock);

//	printf("add url %s is to list!\n",pPtcParse->url);
	
	return 0;
}

int  _aging_check()
{
	URLQUEUE *pQue = &g_UQue;
	LIST_ITEM *pListItem = &pQue->ListItem;
	LIST_ENTRY *phead = &pListItem->ListHead;
	LIST_ENTRY *pentry;

	PARSEENTRY *pParTmp =NULL;

	unsigned int uiCur = time(NULL);
		
	SpinLock_Lock(&pQue->spinlock);
	if(IsListEmpty(phead))
	{
		SpinLock_unLock(&pQue->spinlock);
		return -1;
	}
	pentry = phead->Blink;
	while (pentry != phead)
	{
		pParTmp = CONTAINING_RECORD(pentry, PARSEENTRY, parentry);
		pentry = pentry->Blink;
		if(uiCur - pParTmp->stamp > DEAD_TIME)
		{
			RemoveEntryList(&pParTmp->parentry);
			pListItem->ulListLen --;
//			printf("del url %s is from list!\n",pParTmp->stPtcParse.url);
			free(pParTmp);
			pParTmp = NULL;
		}
	}
	
	SpinLock_unLock(&pQue->spinlock);

	return 0;
}
int parse_title(char *szbuf,char **szDest)
{
	if(NULL == szbuf)
	{
		return -1;
	}

	char *data = szbuf;
	
	char *responseOK = strstr(data,"HTTP/1.1 200 OK");
	
	if(NULL == responseOK)
	{
		return -1;
	}
	data += 20;

	char code[16] = {0};
	char *p1 = NULL;
	char *p2 = NULL;
	unsigned int len = 0;
	char tmp[128]={0};

	p1 = strstr(data,"charset=");
	if(NULL != p1)
	{
		p1 += 8;
		p2 = strstr(p1,";");
		if(NULL == p2)
		{
			p2 = strstr(p1,"\r\n");
			if(NULL != p2)
			{
				len = (unsigned long)p2 - (unsigned long)p1;
				if(len<16)
				{
					memcpy(code,p1,len);
				}
			}
			else
			{
				return 0;
			}
		}
	}
	else
	{
		return 0;
	}

	data = p2 + 20;
	p1 =strstr(data,"<title>");
	if(NULL != p1)
	{
		p1 += 7;
		p2 = strstr(p1,"</title>");
		if(NULL != p2)
		{
			len = (unsigned long)p2 - (unsigned long)p1;
			len = len < 128 ? len:127;
			char *title = (char *)malloc(128);
			memset(title,0,128);
			if( 0 == strcmp(code,"utf-8"))
			{
				memcpy(tmp,p1,len);
				utf2gbk(tmp,len,title,128);
			}
			else
			{
				memcpy(title,p1,len);
			}
			*szDest = tmp;
		}
	}
	else
	{
		return 0;
	}
	
	return 0;
}

#if 0
int url_get_title()
{
	URLQUEUE *pQue = &g_UQue;
	LIST_ITEM *pListItem = &pQue->ListItem;
	LIST_ENTRY *phead = &pListItem->ListHead;
	LIST_ENTRY *pentry;

	PARSEENTRY *pParTmp =NULL;

	SpinLock_Lock(&pQue->spinlock);
	if(IsListEmpty(phead))
	{
		SpinLock_unLock(&pQue->spinlock);
		return -1;
	}

	char szbuf[2048] = {0};
	int count = 0;
	pentry = phead->Blink;
	while (pentry != phead)
	{
		pParTmp = CONTAINING_RECORD(pentry, PARSEENTRY, parentry);
		pentry = pentry->Blink;
		if(NULL != pParTmp)
		{
			memset(szbuf,0,2048);
			printf("url = %s \n",pParTmp->stPtcParse.url);
			if(-1 != Http_Get(pParTmp->stPtcParse.url, szbuf, 2048))
			{
				char *tit = NULL;
				if(-1 == parse_title(szbuf,&tit))
				{
					continue;
				}
				if(NULL != tit)
				{
					strcpy(pParTmp->stPtcParse.title,tit);
					free(tit);
					tit = NULL;
				}
			}
			printf("title = %s \n",pParTmp->stPtcParse.title);
			writeFile(OUTPUT_PATH, &pParTmp->stPtcParse);
			
		}
		RemoveEntryList(&pParTmp->parentry);
		pListItem->ulListLen --;
		free(pParTmp);
		pParTmp =NULL;
	}

	SpinLock_unLock(&pQue->spinlock);
	
	return 0;
}

#endif

void *url_queue_proc(void *arg)
{
	while(1)
	{
//		url_get_title();
		_aging_check();
		sleep(3);
	}
}

int crc_url_init()
{
	URLQUEUE *pQue = &g_UQue;
	LIST_ITEM *pListItem = &pQue->ListItem;
	LIST_ENTRY *phead = &pListItem->ListHead;
	pListItem->ulListLen = 0;
	
	InitializeListHead(phead);

	InitSpinLock(&pQue->spinlock);

	THREAD_HANDLE handle;
	int iret = 0;
	__CreateThread(handle, url_queue_proc, (void *)NULL,iret);
	if(iret)
	{
		printf("[INFO]create url_queue_proc error iret %d!\n",iret);
	}

	return 0;
}





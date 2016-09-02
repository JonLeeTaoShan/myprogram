
#include "product.h"
#include "appmodule.h"
#include "format.h"
#include "public.h"

/* telnet user */
static int telnetLogin = 0;		//telnet login flag
static char telnetUser[96] = {0};     //telnet username


/*********************************************************/
int protocol_null(void *protocol_parse, u_char *data, int data_len)
{
	PROTOCOL_PARSE *parse = (PROTOCOL_PARSE *)protocol_parse;
	return 0;
}
int protocol_qq(void *protocol_parse, u_char *data, int data_len)
{
	PROTOCOL_PARSE *parse = (PROTOCOL_PARSE *)protocol_parse;
#if 0
	int n = 0;
	printf("------------------QQ PACKET---------------------\n");
	for(; n < data_len; n++)
	{
		printf("%02x ", data[n]);
		if((n+1)%16 == 0)
		{
			printf("\n");
		}
	}
	printf("\n\n");
#endif

	if(data_len < 70)
		return 0;

	u_char pcLoginFlag[2] = {0x08, 0x25};
	u_char mobileLoginFlag[2] = {0x08, 0x01};
	u_char mobileLoginFlag1[2] = {0x08, 0x02};
	u_char mobileLoginFlag2[2] = {0x09, 0x01};
	u_char mobileLoginFlag3[2] = {0x08, 0x00};
	u_char mobileLoginFlag4[2] = {0x03, 0x01};
	u_char mobileLoginFlag5[2] = {0x0b, 0x01};
	/***********************************
	/*  pcLoginFlag
	/*	u_char flag;
	/*	u_short version;
	/*	u_short commend; 0x0825:login
	/*	u_short seqnum;
	/*	u_int   qq_num;
	/*	u_char	*data;
	************************************/
	if(!memcmp(data + 3, pcLoginFlag, 2))
	{
		if(data[data_len - 1] != 0x03)
			return 0;
		if(*(data + 11) != 0x03)
			return 0;
		u_int nQqNumlogin = 0;
		nQqNumlogin = ntohl(*(u_int *)(data + 7));

		sprintf(parse->username, "%u", nQqNumlogin);
		return 1;

	}
	else if(!memcmp(data + 5, pcLoginFlag, 2))
	{
		if(data[data_len - 1] != 0x03)
			return 0;
		if(*(data + 13) != 0x03)
			return 0;
		u_int nQqNumlogin = 0;
		nQqNumlogin = ntohl(*(u_int *)(data + 9));

		sprintf((char *)parse->username, "%u", nQqNumlogin);
		return 1;
	}
	else if(!memcmp(data + 7, mobileLoginFlag, 2) )
	{
		//data_len = 195 196
		if (data_len < 90)
		{
			return 0;
		}
		if(ntohs(*(u_short *)(data + 2)) != data_len)
			return 0;
		//qq ºÅÂë ±ØÐëÐ¡ÓÚ 16Î»
		if((*(data + 81) < 0x0f)&&(*(data + 81) > 0x08))
		{
			memcpy(parse->username, data + 82, *(data + 81) - 4);
			return 1;
		}
		else if((*(data + 13) < 0x0f)&&(*(data + 13) > 0x08))
		{
			memcpy(parse->username, data + 14, *(data + 13) - 4);
			return 1;
		}
		else
			return 0;
	}
	else if(!memcmp(data + 7, mobileLoginFlag1, 2))
	{
		if (data_len < 64)
		{
			return 0;
		}
		if(ntohs(*(u_short *)(data + 2)) != data_len)
			return 0;

		//qq ºÅÂë ±ØÐëÐ¡ÓÚ 16Î»
		if(*(data + 57) < 0x0f && *(data + 57) > 0x08)
		{
			memcpy(parse->username, data + 58, *(data + 57) - 4);
			return 1;
		}
		else if (*(data + 17) < 0x0f && *(data + 17) > 0x08)
		{
			memcpy(parse->username, data + 18, *(data + 17) - 4);
			return 1;
		}
		else
			return 0;
	}
	else if(!memcmp(data + 7, mobileLoginFlag2, 2) ||
			!memcmp(data + 7, mobileLoginFlag3, 2) ||
			!memcmp(data + 7, mobileLoginFlag5, 2))
	{
		if (data_len < 23)
		{
			return 0;
		}
		if(ntohs(*(u_short *)(data + 2)) != data_len)
			return 0;
		//qq ºÅÂë ±ØÐëÐ¡ÓÚ 16Î»
		if((*(data + 17) < 0x0f)&& (*(data + 17) > 0x08))
		{
			memcpy(parse->username, data + 18, *(data + 17) - 4);
			return 1;
		}
		else if((*(data + 13) < 0x0f)&&(*(data + 13) > 0x08))
		{
			memcpy(parse->username, data + 14, *(data + 13) - 4);
			return 1;
		}
		else
			return 0;
	}
	else if(!memcmp(data + 7, mobileLoginFlag4, 2))
	{
		if (data_len < 20)
		{
			return 0;
		}
		if(ntohs(*(unsigned short *)(data + 2)) != data_len)
			return 0;
		if((*(data + 13) < 0x0f)&&(*(data + 13) > 0x08))
		{
			memcpy(parse->username, data + 14, *(data + 13) - 4);
			return 1;
		}
		else
			return 0;
	}
	return 0;
}

/*********************************************************/

int protocol_miliao(void *protocol_parse, u_char *data, int data_len)
{
	PROTOCOL_PARSE *parse = (PROTOCOL_PARSE *)protocol_parse;
	char *idm = NULL, *idn = NULL;
	if(data_len < 20)
		return 0;

	if(parse->optype == 1)
	{
		if((idm = strstr(data, " to=")) != NULL && (idn = strstr(idm, "@xiaomi.com"))!= NULL)
		{
			idm += 5;
			if(idn - idm > 0)
			{
				memcpy(parse->username, idm, idn - idm > 31? 31 : idn - idm); 	
			}
		}
		idm = NULL, idn = NULL;

		return 1;
	}
	if((idm = strstr(data, " from=")) != NULL)
	{
		if((idn = strstr(idm, "@xiaomi.com"))!= NULL)
		{
			idm += 7;
			if(idn - idm > 0)
			{
				memcpy(parse->sendname, idm, idn - idm > 31 ? 31 : idn - idm); 	
			}
		}
		idm = NULL, idn = NULL;
	}
	if((idm = strstr(data, " to=")) != NULL)
	{
		if((idn = strstr(idm, "@xiaomi.com")) != NULL)
		{
			idm += 5;
			if(idn - idm > 0)
				memcpy(parse->toname, idm, idn - idm > 63 ? 63: idn - idm);		
		}
		idm = NULL, idn = NULL;
	}
	if(parse->optype == 3)
	{
		if(!strstr(data, "<s") || !strstr(data, "</s>"))
			return 0;
		memcpy(parse->username, parse->sendname, 31);
	}
	else if(parse->optype == 4)
	{
		if(!strstr(data, "<s") || !strstr(data, "</s>"))
			return 0;
		memcpy(parse->username, parse->toname, strlen(parse->toname) > 31 ? 31 :strlen(parse->toname));
	}
	return 1;
}


/*********************************************************/

int protocol_qqduizhan(void *protocol_parse, u_char *data, int data_len)
{	
	PROTOCOL_PARSE *parse = (PROTOCOL_PARSE *)protocol_parse;
	if(data_len < 110)
		return 0;
	
	/***********************************
	/*	u_char flag;
	/*	u_short version; 
	/*	u_short commend; 0x0825:login
	/*	u_short seqnum;
	/*	u_int   qq_num;
	/*	u_char	*data;
	************************************/

	if(*(data + 11) != 0x0f)
		return 0;

	u_int nQqNumlogin = 0;
	nQqNumlogin = ntohl(*(u_int *)(data + 7));
	sprintf(parse->username, "%u", nQqNumlogin);
	
	return 1;
}


/***********************************************************/

int protocol_qqyouxi(void *protocol_parse, u_char *data, int data_len)
{	
	PROTOCOL_PARSE *parse = (PROTOCOL_PARSE *)protocol_parse;
	if(data_len < 110)
		return 0;

	do {
		char *p = strstr(data, "uin=");
		if (!p)
			break;
		char *p1 = strstr(p, "&");
		if (!p1 || p1 - p > 20)
			break;
		else
			memcpy(parse->username, p + 4, p1 - p - 4);
	}while(0);

	u_char pcLoginFlag1[2] = {0x00, 0x01};
	u_char pcLoginFlag2[2] = {0x00, 0x30};
	
	/***********************************
	/*	u_char flag;
	/*	u_short version; 
	/*	u_short commend;
	/*	u_short seqnum;
	/*	u_int   qq_num;
	/*	u_char	*data;
	************************************/

	if(!memcmp(data + 11, pcLoginFlag1, 2) || !memcmp(data + 11, pcLoginFlag2, 2))
	{
		u_int nQqNumlogin = 0;
		nQqNumlogin = ntohl(*(u_int *)(data + 7));
		sprintf(parse->username, "%u", nQqNumlogin);

		return 1;
	}
	
	return 0;
}


/***********************************************************/

int protocol_webmail(void *protocol_parse, u_char *data, int data_len)
{
	PROTOCOL_PARSE *parse = (PROTOCOL_PARSE *)protocol_parse;
	if(data == NULL)
		return 0;

	char token[] ="userMailList=";
	char *psrc =(char *)data;
	char *out = NULL;
	u_int len =0;
	char tmp[128]={0};
	if (1== get_tab_contant(psrc,token,"&",NULL,&out,&len) && NULL != out && len > 0)
	{
		len = (len > 127) ? 127:len;
		memcpy(tmp,out,len);
		urlDecode(tmp);

		strncpy(parse->username,tmp,32);
	}

	char *ides = NULL, *iden = NULL, *idex = NULL;
	//163 sendmail
	if(!memcmp(parse->proto_name, "163webmail", 10))
	{
		if((ides = strstr(data, "%3Cstring%20name%3D%22account%22%3E")) != NULL)
		{
			ides += 35;
			if(ides && (iden = strstr(ides, "%26lt%3B")) != NULL)
			{
				iden += 8;
				
				if(iden && (idex = strstr(iden, "%26gt%3B%3C%2Fstring%3E")) != NULL)
				{
					memcpy(parse->username, iden, (idex - iden) > 31 ? 31 : (idex - iden));
					urlDecode(parse->username);
					if(parse->username == NULL)
						return 0;
					//ȷ��Ϊ163����
					if(!strstr(parse->username, "163.com"))
						return 0;
					//sendmail username = sendname
					memcpy(parse->sendname, parse->username, strlen(parse->username));
				}
				
			}
		}
		if((ides = strstr(data, "%3Carray%20name%3D%22to%22%3E")) != NULL)
		{
			ides += 29;
			if(ides && (iden = strstr(ides, "%3Cstring%3E")) != NULL)
			{
				iden += 12;
				
				if(iden && (/*(idex = strstr(iden, "%26gt%3B%3C%2Fstring%3E")) != NULL ||*/ (idex = strstr(iden, "%3C%2Fstring%3E")) != NULL))
				{
					memcpy(parse->toname, iden, (idex - iden) > 63 ? 63 : (idex - iden));
					//�����ǳ����
					if(parse->toname && (ides = strstr(parse->toname, "%26lt%3B")) != NULL)
					{	
						int len = 0;
						ides += 8;
						
						if(ides == NULL)
							return 0;
						len = strlen(ides);
						
						if((idex = strstr(ides, "%26gt%3B")) != NULL)
							len = idex - ides;

						memcpy(parse->toname, ides, len);
						parse->toname[len] = '\0';
					}
					urlDecode(parse->toname);
				}
			}
		}
		if((ides = strstr(data, "%3Cstring%20name%3D%22subject%22%3E")) != NULL)
		{
			ides += 35;
			if(ides && (iden = strstr(ides, "%3C%2Fstring%3E")) != NULL)
			{
				memcpy(parse->subject, ides, (iden - ides) > 63 ? 63 : (iden - ides));
				urlDecode(parse->subject);
				//gbk decode
			}
		}
		if((ides = strstr(data, "%3Cstring%20name%3D%22content%22%3E")) != NULL)
		{
			iden = NULL;
			ides += 35;
			if(ides == NULL)
				return 0;
			if((iden = strstr(ides, "%26gt%3B%26lt%3Bp%26gt%3B")) != NULL)
				iden += 25;
			else if((iden = strstr(ides, "%26gt%3B")) != NULL)
				iden += 8;

			if(iden == NULL)
				return 0;
			if((idex = strstr(iden, "%26lt%3B")) != NULL)
			{
				memcpy(parse->content, iden, (idex - iden) > 511 ? 511 : (idex - iden));
				urlDecode(parse->content);
			}
			else
			{
				memcpy(parse->content, iden, strlen(iden) > 511 ? 511 : strlen(iden));
				urlDecode(parse->content);
			}
		}	
	}
	#if 0
	else if(!memcmp(parse->proto_name, "tomwebmail", 10))
	{
		if((ides = strstr(data, "from=")) != NULL)
		{
			ides += 5;
			if(ides && (iden = strstr(ides, "%22%3C")) != NULL)
			{
				iden += 6;
				
				if(iden && (idex = strstr(iden, "%3E")) != NULL)
				{
					memcpy(parse->username, iden, (idex - iden) > 99 ? 99 : (idex - iden));
					urlDecode(parse->username);
					if(strlen(parse->username) == 0)
						return 0;
					//ȷ��Ϊtom����
					if(!strstr(parse->username, "tom.com"))
						return 0;
					//sendmail username = sendname
					memcpy(parse->sendname, parse->username, strlen(parse->username));
				}
				
			}
		}
		if((ides = strstr(data, "&to=")) != NULL)
		{
			ides += 4;
			if(ides && (iden = strstr(ides, "%3C")) != NULL)
			{
				iden += 3;
				
				if(iden && ((idex = strstr(iden, "%3E%3B")) != NULL))
				{
					memcpy(parse->toname, iden, (idex - iden) > 1023 ? 1023 : (idex - iden));
					urlDecode(parse->toname);
				}
			}
			else
			{
				if((idex = strstr(ides, "%3B")) != NULL)
				{
					memcpy(parse->toname, ides, (idex - ides) > 1023 ? 1023 : (idex - ides));
					urlDecode(parse->toname);
				}
			}
		}
		if((ides = strstr(data, "&subject=")) != NULL)
		{
			ides += 9;
			if(ides && (iden = strstr(ides, "&")) != NULL)
			{
				memcpy(parse->subject, ides, (iden - ides) > 1023 ? 1023 : (iden - ides));
				urlDecode(parse->subject);
			}
		}
		if((ides = strstr(data, "&text=")) != NULL)
		{
			ides += 6;
			if((iden = strstr(ides, "&")) != NULL)
			{
				memcpy(parse->content, ides, (iden - ides) > 2047 ? 2047 : (iden - ides));
				urlDecode(parse->content);
			}
		}
	}
	#endif
	return 1;
}

/*********************************************************/

int protocol_smtp(void *protocol_parse, u_char *data, int data_len)
{
	PROTOCOL_PARSE *parse = (PROTOCOL_PARSE *)protocol_parse;
	char *idm = NULL, *idn = NULL;
	if(data_len < 40)
		return 0;
	if((idm = strstr(data, "From: ")) != NULL)
	{
		if((idm = strstr(idm, "<")) != NULL && (idn = strstr(idm, ">"))!= NULL)
		{
			idm += 1;
			if(idn - idm > 0)
			{
				memcpy(parse->sendname, idm, idn - idm > 31? 31 : idn - idm);		
				memcpy(parse->username, idm, idn - idm > 31 ? 31 : idn - idm);
			}
		}
		idm = NULL, idn = NULL;
	}
	if((idm = strstr(data, "To: ")) != NULL)
	{
		if((idm = strstr(idm, "<")) != NULL && (idn = strstr(idm, ">"))!= NULL)
		{
			idm += 1;
			if(idn - idm > 0)
				memcpy(parse->toname, idm, idn - idm > 63 ? 63 : idn - idm);		
		}
		idm = NULL, idn = NULL;
	}
	if((idm = strstr(data, "Subject: ")) != NULL)
	{
		idm += 9;
		if(!memcmp(idm, "=?GB", 4) || !memcmp(idm, "=?gb", 4))
		{
			if((idn = strstr(idm, "?="))!= NULL)
			{
				u_char *result = NULL;
				int result_len = 0;
				idm += 11; //=?GB2312?B?
				if(idn - idm > 0)
				{
					u_char subject[512] = {0};
					int ret = 0;
					memcpy(subject, idm, idn - idm > 511 ? 511: idn - idm);
					memset(parse->subject, 0, sizeof(parse->subject));
					
					result = base64Decode(subject, &result_len);
					ret = gbk2utf8(result, strlen(result), parse->subject, sizeof(parse->subject));
					if(ret == -1)					
						memcpy(parse->subject, result, result_len > 63 ? 63 : result_len);
					if(result)	free(result);
				}
			}
		}
		else
		{
			if((idn = memchr(idm, 0x0d, 99)) != NULL || (idn = memchr(idm, 0x0a, 99)) != NULL)
				memcpy(parse->subject, idm, idn - idm > 63 ? 63 : idn - idm);
		}
	}
	
	return 1;
}

/*********************************************************/

int protocol_imap(void *protocol_parse, u_char *data, int data_len)
{	
	PROTOCOL_PARSE *parse = (PROTOCOL_PARSE *)protocol_parse;
	char *idm = NULL, *idn = NULL;

	if(parse->optype == 1)
	{
		if((idm = strstr(data, "LOGIN ")) != NULL)
		{
			idm += 6;
			if((idn = strstr(idm, " ")) != NULL)
				memcpy(parse->username, idm, idn - idm > 31 ? 31 : idn - idm);

			return 1;
		}
	}
	if(!strstr(data, "Received: ") && !strstr(data, "From: ") && !strstr(data, "To: ") && !strstr(data, "Subject: "))
		return 0;
	
	if((idm = strstr(data, "From: ")) != NULL)
	{
		if((idm = strstr(idm, "<")) != NULL && (idn = strstr(idm, ">"))!= NULL)
		{
			idm += 1;
			memcpy(parse->sendname, idm, idn - idm > 63 ? 63 : idn - idm);		
		}
		idm = NULL, idn = NULL;
	}
	if((idm = strstr(data, "To: ")) != NULL)
	{
		idm += 4;
		if((idn = memchr(idm, 0x0d, 99)) != NULL || (idn = memchr(idm, 0x0a, 99)) != NULL)
			memcpy(parse->toname, idm, idn - idm > 63 ? 63 : idn - idm);
		
		if((idm = strstr(parse->toname, "<")) != NULL && (idn = strstr(idm, ">"))!= NULL)
		{
			idm += 1;
			memcpy(parse->toname, idm, idn - idm);
			parse->toname[idn - idm] = '\0';		
		}
		idm = NULL, idn = NULL;
	}
	if((idm = strstr(data, "Subject: ")) != NULL)
	{
		char subject[1024] = {0};
		idm += 9;
		if(!memcmp(idm, "=?GB2312", 8) || !memcmp(idm, "=?gb2312", 8))
		{
			if((idn = strstr(idm, "?=")) != NULL)
			{
				u_char *result = NULL;
				int result_len = 0;
				idm += 11; //=?GB2312?B?
				if(idn - idm > 0)
				{
					u_char subject[512] = {0};
					int ret = 0;
					memcpy(subject, idm, idn - idm > 511 ? 511 : idn - idm);
					memset(parse->subject, 0, sizeof(parse->subject));
					
					result = base64Decode(subject, &result_len);
					ret = gbk2utf8(result, strlen(result), parse->subject, sizeof(parse->subject));
					if(ret == -1)					
						memcpy(parse->subject, result, result_len > 63 ? 63 : result_len);
					if(result)	free(result);
				}
			}
		}
		else if(!memcmp(idm, "=?GBK", 5) || !memcmp(idm, "=?gbk", 5))
		{
			if((idn = strstr(idm, "?="))!= NULL)
			{
				u_char *result = NULL;
				int result_len = 0;
				idm += 8;  //=?GBK?Q?
				u_char subject[1024] = {0};
				int ret = 0;
				if(idn - idm > 0)
				{
					u_char subject[512] = {0};
					int ret = 0;
					memcpy(subject, idm, idn - idm > 511 ? 511 : idn - idm);
					memset(parse->subject, 0, sizeof(parse->subject));
					
					result = base64Decode(subject, &result_len);
					ret = gbk2utf8(result, strlen(result), parse->subject, sizeof(parse->subject));
					if(ret == -1)					
						memcpy(parse->subject, result, result_len > 63 ? 63 : result_len);
					if(result)	free(result);
				}
			}
			idm = NULL, idn = NULL;
		}
		else if(!memcmp(idm, "=?UTF", 5) || !memcmp(idm, "=?utf", 5))
		{
			if((idn = strstr(idm, "?=")) != NULL)
			{
				u_char *result = NULL;
				int result_len = 0;
				idm += 10; //=?UTF-8?B?
				u_char subject[512] = {0};
				int ret = 0;
				if(idn - idm > 0)
				{
					memcpy(subject, idm, idn - idm > 511 ? 511 : idn - idm);
					if(strstr(data, "Content-Transfer-Encoding: base64") 
						|| strstr(data, "Content-Transfer-Encoding: 8Bit"))
					{
						result = base64Decode(subject, &result_len);						
						memcpy(parse->subject, result, result_len > 63 ? 63 : result_len);
						if(result)	free(result);
					}
					else
					{
						//�� "=" ��Ϊ "%"
						replace(subject, "=", "%");
						urlDecode(parse->subject);						
						memcpy(parse->subject, subject, strlen(subject));
					}
				}
			}
		}
		else
		{
			if((idn = memchr(idm, 0x0d, 99)) != NULL || (idn = memchr(idm, 0x0a, 99)) != NULL)
				memcpy(parse->subject, idm, idn - idm > 63 ? 63 : idn - idm);
		}
	}
	
	return 1;
}

/*********************************************************/

int protocol_pop(void *protocol_parse, u_char *data, int data_len)
{	
	PROTOCOL_PARSE *parse = (PROTOCOL_PARSE *)protocol_parse;
	char *idm = NULL, *idn = NULL;

	if((idm = strstr(data, "USER ")) != NULL)
	{
		idm += 5;
		if((idn = memchr(idm, 0x0d, 99)) != NULL || (idn = memchr(idm, 0x0a, 99)) != NULL)
			memcpy(parse->username, idm, idn -idm > 31 ? 31 : idn -idm);
		
		return 1;
	}
	if(!strstr(data, "From: ") && !strstr(data, "To: ") && !strstr(data, "Subject: "))
		return 0;
	
	if((idm = strstr(data, "From: ")) != NULL)
	{
		if((idm = strstr(idm, "<")) != NULL && (idn = strstr(idm, ">"))!= NULL)
		{
			idm += 1;
			if(idn - idm > 0)
				memcpy(parse->sendname, idm, idn - idm > 31 ? 31 : idn - idm);		
		}
		idm = NULL, idn = NULL;
	}
	if((idm = strstr(data, "To: ")) != NULL)
	{
		if((idm = strstr(idm, "<")) != NULL && (idn = strstr(idm, ">"))!= NULL)
		{
			idm += 1;
			if(idn - idm > 0)
				memcpy(parse->toname, idm, idn - idm > 63 ? 63 : idn - idm);		
		}
		idm = NULL, idn = NULL;
	}
	if((idm = strstr(data, "Subject: ")) != NULL)
	{
		idm += 9;
		if(!memcmp(idm, "=?GB2312", 8) || !memcmp(idm, "=?gb2312", 8))
		{
			if((idn = strstr(idm, "?=")) != NULL)
			{
				u_char *result = NULL;
				int result_len = 0;
				idm += 11; //=?GB2312?B?
				if(idn - idm > 0)
				{
					u_char subject[512] = {0};
					int ret = 0;
					memcpy(subject, idm, idn - idm > 511 ? 511 : idn - idm);
					memset(parse->subject, 0, sizeof(parse->subject));
					
					result = base64Decode(subject, &result_len);
					ret = gbk2utf8(result, strlen(result), parse->subject, sizeof(parse->subject));
					if(ret == -1)					
						memcpy(parse->subject, result, result_len > 63 ? 63 : result_len);
					if(result)	free(result);
				}
			}
		}
		else if(!memcmp(idm, "=?gbk", 5) || !memcmp(idm, "=?GBK", 5))
		{
			if((idn = strstr(idm, "?=")) != NULL)
			{
				u_char *result = NULL;
				int result_len = 0;
				idm += 8; //=?gbk?B?
				if(idn - idm > 0)
				{
					u_char subject[512] = {0};
					int ret = 0;
					memcpy(subject, idm, idn - idm > 511 ? 511 : idn - idm);
					memset(parse->subject, 0, sizeof(parse->subject));
					
					result = base64Decode(subject, &result_len);
					ret = gbk2utf8(result, strlen(result), parse->subject, sizeof(parse->subject));
					if(ret == -1)					
						memcpy(parse->subject, result, result_len > 63 ? 63 : result_len);
					if(result)	free(result);
				}
			}
		}
		else if(!memcmp(idm, "=?UTF", 5) || !memcmp(idm, "=?utf", 5))
		{
			if((idn = strstr(idm, "?=")) != NULL)
			{
				u_char *result = NULL;
				int result_len = 0;
				idm += 10; //=?UTF-8?B?
				if(idn - idm > 0)
				{
					memcpy(parse->subject, idm, idn - idm > 63 ? 63 : idn - idm);
					result = base64Decode(parse->subject, &result_len);
					memset(parse->subject, 0, sizeof(parse->subject));
					memcpy(parse->subject, result, result_len > 63 ? 63 : result_len);
					if(result)	free(result);
				}
			}
		}
		else
		{
			if((idn = memchr(idm, 0x0d, 99)) != NULL || (idn = memchr(idm, 0x0a, 99)) != NULL)
				memcpy(parse->subject, idm, idn - idm > 63 ? 63 : idn - idm);
		}
	}
	
	return 1;
}


/*********************************************************/

int protocol_ftp(void *protocol_parse, u_char *data, int data_len)
{
	PROTOCOL_PARSE *parse = (PROTOCOL_PARSE *)protocol_parse;
	u_char *idx = NULL;
	if(data_len < 6)
		return 0;
	data += 5;
	if((idx = memchr(data, 0x0d, 99)) != NULL || (idx = memchr(data, 0x0a, 99)) != NULL)
		memcpy(parse->username, data, idx - data > 31 ? 31 : idx - data);

	return 1;
}

/*********************************************************/

int protocol_telnet(void *protocol_parse, u_char *data, int data_len)
{
	PROTOCOL_PARSE *parse = (PROTOCOL_PARSE *)protocol_parse;
	/* ����ʵ�������⣬��������û�ͬʱ��½ */
	const char loginEndFlag1[1] = {0x0d};
	const char loginEndFlag2[1] = {0x0a};

	if(data_len == 0)
		return 0;
	
	if(parse->optype == 1)
	{
		telnetLogin = 1;		
		memset(telnetUser, 0, sizeof(telnetUser));
		return 1;
	}
	
	if(!telnetLogin)
		return 0;
	
	if(!memcmp(data, loginEndFlag1, 1) || !memcmp(data, loginEndFlag2, 1))
	{
		memcpy(parse->username, telnetUser, strlen(telnetUser));
		memset(telnetUser, 0, sizeof(telnetUser));
		parse->optype = 1;
		telnetLogin = 0;
		return 1;
	}
	strncat(telnetUser, data, 1);
	parse->optype = 1;

	return 1; 
}

/*********************************************************/

int protocol_http(void *protocol_parse, u_char *data, int data_len)
{
	PROTOCOL_PARSE *parse = (PROTOCOL_PARSE *)protocol_parse;
	if(data_len < 6)
		return 0;

	int i = 3, j = 0;
	int url_len = 0, host_len = 0;
	u_char url[512] = {0}, host[64] = {0};

	char *uri = NULL;

	if((uri = memchr(data, 0x20, 6)) != NULL)
	{
		for(; i < data_len; i++) 
		{
			//uri
			if (*(data + i) != 0x0d && *(data + i + 1) != 0x0a)
			{
				url_len ++;
			}
			else
			{
				url_len -= 10;  // HTTP/1.1
				memcpy(url, uri + 1, url_len >= 511 ? 511 : url_len);
				break;
			}
		}
		//uriû�������������ݶ�ʧ
		if(i == data_len)
		{
			//memset((char *)parse->url, 0, 1024);
			memcpy((char *)parse->url, uri + 1, url_len >= 511 ? 511 : url_len);
			return 1;
		}
			
		//Host
		for (i = url_len + 11; i < data_len; i++) 
		{
			if(*(data + i) == 0x48 && *(data + i + 1) == 0x6f && *(data + i + 2) == 0x73 && *(data + i + 3) == 0x74 && *(data + i + 4) == 0x3a)
			{
				i += 6;
				for (j = i; j < data_len; j++) 
				{
					if (*(data + j) != 0x0d && *(data + j + 1) != 0x0a)
					{
						host_len ++;
					}
					else
					{
						memcpy(host, data + i, host_len >= 63 ? 63 : host_len);
						break;
					}
				}
				//Hostû�������������ݶ�ʧ
				if(j == data_len)
				{
					memcpy(host, data + i, host_len >= 63? 63 : host_len);
				}
				break;  //��ȡ������Host֮���Ƴ�ѭ��
			}
		}
#if 0
		//Host�ֶζ�ʧ
		if(i == data_len)
		{
			memcpy((char *)parse->url, url, strlen(url));
			return 1;
		}
#endif
		//url = host + url
		if(strlen(host)< 3)
		{
			return 0;
		}
		memcpy((char *)parse->url, host, strlen(host));
		if('/' != url[0])
		{
			strcat((char *)parse->url,"/");
		}
		strcat((char *)parse->url, url);
	}
	else
		return 0;
	
	return 1;
}

/********************************************************/

int protocol_maopu(void *protocol_parse, u_char *data, int data_len)
{
	PROTOCOL_PARSE *parse = (PROTOCOL_PARSE *)protocol_parse;
	int ret = 0;
	char *idm = NULL, *idn = NULL;
	ret = protocol_http(parse, data, data_len);
	if(!ret || !strlen(parse->url))
		return 0;

	if((idm = strstr(parse->url, "loginName=")) != NULL)
	{
		idm += 10;
		if((idn = strchr(idm, '&')) != NULL)
			memcpy(parse->username, idm, idn - idm > 31 ? 31 : idn - idm);
		else
			memcpy(parse->username, idm, strlen(idm) > 31 ? 31 : strlen(idm));

		urlDecode(parse->username);
	}

	if(!strlen(parse->username))
	{
		memset(parse->proto_name, 0, sizeof(parse->proto_name));
		memcpy(parse->proto_name, "http", 4);
	}
	
	return 1;
}

/********************************************************/

int protocol_baidutieba(void *protocol_parse, u_char *data, int data_len)
{
	PROTOCOL_PARSE *parse = (PROTOCOL_PARSE *)protocol_parse;
	char *psrc = (char *)data;
	char token[]="userName=";
	char token1[]="BDUSS=";
	char token2[]="kw=";//论坛名
	char token3[]="name=";//地点
	char token4[]="&title=";//主题
	char token5[]="content=";//内容
	char token6[]="_phone_imei=";
	char token7[]="brand_type=";
	char token8[]="model=";
	char *out = NULL;
	unsigned int len = 0;
	char tmp[512] = {0};
	protocol_http(parse, data, data_len);
	if (1== get_tab_contant(psrc,token,"&"," ",&out,&len) && NULL != out && len > 0)
	{
		len = (len > 31) ? 31:len;
		memcpy(parse->username,out,len);
		return 1;
	}
	psrc += strlen(parse->url);
	if (1== get_tab_contant(psrc,token2,"&",NULL,&out,&len) && NULL != out && len > 0)
	{
		len = (len > 512) ? 512:len;
		memcpy(tmp,out,len);
		urlDecode(tmp);
		if(strlen(tmp)>32)
			tmp[31]='\0';
		strncpy(parse->keyword,tmp,32);
	}
	if (1== get_tab_contant(psrc,token3,"&",NULL,&out,&len) && NULL != out && len > 0)
	{
		len = (len > 512) ? 512:len;
		memset(tmp,0,512);
		memcpy(tmp,out,len);
		urlDecode(tmp);
		if(strlen(tmp)>64)
			tmp[63]='\0';
		strncpy(parse->toname,tmp,64);
	}
	if (1== get_tab_contant(psrc,token4,"&",NULL,&out,&len) && NULL != out && len > 0)
	{
		len = (len > 512) ? 512:len;
		memset(tmp,0,512);
		memcpy(tmp,out,len);
		urlDecode(tmp);
		if(strlen(tmp)>64)
			tmp[63]='\0';
		strncpy(parse->subject,tmp,64);
	}
	if (1== get_tab_contant(psrc,token5,"&",NULL,&out,&len) && NULL != out && len > 0)
	{
		len = (len > 512) ? 512:len;
		memset(tmp,0,512);
		memcpy(tmp,out,len);
		urlDecode(tmp);
		strncpy(parse->content,tmp,512);
	}
	if (1== get_tab_contant(psrc,token6,"&",NULL,&out,&len) && NULL != out && len > 0)
	{
		len = (len > 32) ? 32:len;
		memcpy(parse->imei,out,len);
	}
	if (1== get_tab_contant(psrc,token7,"&",NULL,&out,&len) && NULL != out && len > 0)
	{
		len = (len > 64) ? 64:len;
		memcpy(parse->devname,out,len);
		urlDecode(parse->devname);
	}
	else if(1== get_tab_contant(psrc,token8,"&",NULL,&out,&len) && NULL != out && len > 0)
	{
		len = (len > 64) ? 64:len;
		memcpy(parse->devname,out,len);
		urlDecode(parse->devname);
	}
	/*
	else if (1== get_tab_contant(psrc,token1,";","\r\n",&out,&len) && NULL != out && len > 0)
	{
		len = (len > 256) ? 256:len;
		memcpy(tmp,out,len);
	}
	char *p1 = NULL;
	char *p2 = NULL;
	unsigned int uilen = 0;

	if (0 !=tmp[0] && strlen(tmp)>110)
	{
		p1 = tmp + 95;
		p2 = strstr(p1,"AAAA");
		uilen = (unsigned int)p2 - (unsigned int)p1;
		if (uilen % 4 != 0)
		{
			uilen = ((uilen % 4) + 1)*4;
		}

		memcpy(parse->username,p1,uilen);
		int len = 0;
		char *re = base64Decode(parse->username,&len);
		memset(parse->username,0,strlen(parse->username));
		strcpy(parse->username,re);
		if (NULL != re)
		{
			free(re);
			re = NULL;
		}
	}
	*/
	return 1;
}

/********************************************************/

int protocol_tianya(void *protocol_parse, u_char *data, int data_len)
{
	PROTOCOL_PARSE *parse = (PROTOCOL_PARSE *)protocol_parse;
	int ret = 0;
	char *idm = NULL, *idn = NULL;
	ret = protocol_http((void *)parse, data, data_len);
	if(!ret || !strlen(parse->url))
		return 0;

	if((idm = strstr(data, "Cookie: ")) != NULL)
	{
		char user[100] = {0};
		idm += 8;
		if((idm = strstr(idm, "user=w=")) != NULL)
		{
			idm += 7;
			if((idn = strchr(idm, '&')) != NULL)
				memcpy(user, idm, idn - idm > 31 ? 31 : idn - idm);
			else
				memcpy(user, idm, strlen(idm) > 31 ? 31 : strlen(idm));
			
			replace(user, "%", "\\");
			ret = unicode2utf8(user, strlen(user), parse->username, sizeof(parse->username));
			if(ret == -1)					
				memcpy(parse->username, user, strlen(user));
		}
	}

	if(!strlen(parse->username))
	{
		memset(parse->proto_name, 0, sizeof(parse->proto_name));
		memcpy(parse->proto_name, "http", 4);
	}
	
	return 1;
}

/********************************************************/

int protocol_chinanet(void *protocol_parse, u_char *data, int data_len)
{
	PROTOCOL_PARSE *parse = (PROTOCOL_PARSE *)protocol_parse;
	int ret = 0;
	char *idm = NULL, *idn = NULL;
	ret = protocol_http((void *)parse, data, data_len);
	if(!ret || !strlen(parse->url))
		return 0;

	if((idm = strstr(parse->url, "username=")) != NULL)
	{
		idm += 9;
		if((idn = strchr(idm, '&')) != NULL)
			memcpy(parse->username, idm, idn - idm > 31 ? 31 : idn - idm);
		else
			memcpy(parse->username, idm, strlen(idm) > 31 ? 31 : strlen(idm));

		urlDecode(parse->username);

	}

	if(!strlen(parse->username))
	{
		memset(parse->proto_name, 0, sizeof(parse->proto_name));
		memcpy(parse->proto_name, "http", 4);
	}
	
	return 1;
}

/********************************************************/

int protocol_kdnet(void *protocol_parse, u_char *data, int data_len)
{
	PROTOCOL_PARSE *parse = (PROTOCOL_PARSE *)protocol_parse;
	int  ret = 0;
	char *idm = NULL, *idn = NULL;
	ret = protocol_http((void *)parse, data, data_len);
	if(!ret || !strlen(parse->url))
		return 0;
	
	if((idm = strstr(data, "Cookie: ")) != NULL)
	{
		idm += 8;
		if((idm = strstr(idm, "kdnet%5Fuser=")) != NULL)
		{
			idm += 13;
			if((idm = strstr(idm, "%2C")) != NULL)
			{
				char user[100] = {0};
				idm += 3;
				if((idn = strstr(idm, "%2C")) != NULL)
					memcpy(user, idm, idn - idm > 31 ? 31 : idn - idm);
				else
					memcpy(user, idm, strlen(idm) > 31 ? 31 : strlen(idm));
				urlDecode(user);
				ret = gbk2utf8(user, strlen(user), parse->username, sizeof(parse->username));
				if(ret == -1)					
					memcpy(parse->username, user, strlen(user));
			}		
		}	
	}
	if(!strlen(parse->username))
	{
		memset(parse->proto_name, 0, sizeof(parse->proto_name));
		memcpy(parse->proto_name, "http", 4);
	}
}

/********************************************************/

int protocol_newsmth(void *protocol_parse, u_char *data, int data_len)
{
	PROTOCOL_PARSE *parse = (PROTOCOL_PARSE *)protocol_parse;
	int ret = 0;
	char *idm = NULL, *idn = NULL;
	ret = protocol_http((void *)parse, data, data_len);
	if(!ret || !strlen(parse->url))
		return 0;

	if((idm = strstr(data, "Cookie: ")) != NULL)
	{
		idm += 8;
		if((idm = strstr(idm, "main[UTMPUSERID]=")) != NULL)
		{
			idm += 17;
			if((idn = strchr(idm, ';')) != NULL)
				memcpy(parse->username, idm, idn - idm > 31 ? 31 : idn - idm);
			else
				memcpy(parse->username, idm, strlen(idm) > 31 ? 31 : strlen(idm));

			urlDecode(parse->username);
		}
	}

	if(!strlen(parse->username))
	{
		memset(parse->proto_name, 0, sizeof(parse->proto_name));
		memcpy(parse->proto_name, "http", 4);
	}
	
	return 1;
}

/********************************************************/

int protocol_people(void *protocol_parse, u_char *data, int data_len)
{
	PROTOCOL_PARSE *parse = (PROTOCOL_PARSE *)protocol_parse;
	int ret = 0;
	char *idm = NULL, *idn = NULL;
	ret = protocol_http((void *)parse, data, data_len);
	if(!ret || !strlen(parse->url))
		return 0;

	if((idm = strstr(data, "userName=")) != NULL)
	{
		idm += 9;
		if((idn = strchr(idm, '&')) != NULL)
			memcpy(parse->username, idm, idn - idm > 31 ? 31 : idn - idm);
		else
			memcpy(parse->username, idm, strlen(idm) > 31 ? 31 : strlen(idm));
		
		urlDecode(parse->username);
	}
	if(!strlen(parse->username))
	{
		memset(parse->proto_name, 0, sizeof(parse->proto_name));
		memcpy(parse->proto_name, "http", 4);
	}
	
	return 1;
}

/********************************************************/

int protocol_fenghuang(void *protocol_parse, u_char *data, int data_len)
{
	PROTOCOL_PARSE *parse = (PROTOCOL_PARSE *)protocol_parse;
	int ret = 0;
	char *idm = NULL, *idn = NULL;
	ret = protocol_http((void *)parse, data, data_len);
	if(!ret || !strlen(parse->url))
		return 0;

	if((idm = strstr(parse->url, "&u=")) != NULL)
	{
		idm += 3;
		if((idn = strchr(idm, '&')) != NULL)
			memcpy(parse->username, idm, idn - idm > 31 ? 31 : idn - idm);
		else
			memcpy(parse->username, idm, strlen(idm) > 31 ? 31 : strlen(idm));
	}
	if(!strlen(parse->username))
	{
		memset(parse->proto_name, 0, sizeof(parse->proto_name));
		memcpy(parse->proto_name, "http", 4);
	}
	
	return 1;
}

/********************************************************/

int protocol_xinhua(void *protocol_parse, u_char *data, int data_len)
{
	PROTOCOL_PARSE *parse = (PROTOCOL_PARSE *)protocol_parse;
	int ret = 0;
	char *idm = NULL, *idn = NULL;
	ret = protocol_http((void *)parse, data, data_len);
	if(!ret || !strlen(parse->url))
		return 0;

	if((idm = strstr(data, "senderName=")) != NULL)
	{
		idm += 11;
		if((idn = strchr(idm, '&')) != NULL)
			memcpy(parse->username, idm, idn - idm > 31 ? 31 : idn - idm);
		else
			memcpy(parse->username, idm, strlen(idm) > 31 ? 31 : strlen(idm));
		
		urlDecode(parse->username);
	}

	if(!strlen(parse->username))
	{
		memset(parse->proto_name, 0, sizeof(parse->proto_name));
		memcpy(parse->proto_name, "http", 4);
	}
	
	return 1;
}

/********************************************************/

int protocol_wangyi(void *protocol_parse, u_char *data, int data_len)
{
	PROTOCOL_PARSE *parse = (PROTOCOL_PARSE *)protocol_parse;
	int ret = 0;
	char *idm = NULL, *idn = NULL;
	ret = protocol_http((void *)parse, data, data_len);
	if(!ret || !strlen(parse->url))
		return 0;

	if((idm = strstr(data, "username=")) != NULL)
	{
		idm += 9;
		if((idn = strchr(idm, '&')) != NULL)
			memcpy(parse->username, idm, idn - idm > 31 ? 31 : idn - idm);
		else
			memcpy(parse->username, idm, strlen(idm) > 31 ? 31 : strlen(idm));
	}

	if(!strlen(parse->username))
	{
		memset(parse->proto_name, 0, sizeof(parse->proto_name));
		memcpy(parse->proto_name, "http", 4);
	}
	
	return 1;
}

/********************************************************/

int protocol_zhilian(void *protocol_parse, u_char *data, int data_len)
{
	PROTOCOL_PARSE *parse = (PROTOCOL_PARSE *)protocol_parse;
	int ret = 0;
	char *idm = NULL, *idn = NULL;
	ret = protocol_http((void *)parse, data, data_len);
	if(!ret || !strlen(parse->url))
		return 0;
	//PC
	char sztmp[128]={0};
	if(strstr(parse->url, "sou.zhaopin.com"))
	{
		if((idm = strstr(parse->url, "&kw=")) != NULL)
		{
			idm += 4;
			if((idn = strchr(idm, '&')) != NULL)
				memcpy(sztmp, idm, idn - idm > 127 ? 127 : idn - idm);
			else
				memcpy(sztmp, idm, strlen(idm) > 127 ? 127 : strlen(idm));
			urlDecode(sztmp);
			strncpy(parse->keyword,sztmp,32);
		}
		if((idm = strstr(data, "JSloginnamecookie=")) != NULL)
		{
			idm += 18;
			if((idn = strchr(idm, ';')) != NULL)
				memcpy(sztmp, idm, idn - idm > 127 ? 127 : idn - idm);
			else
				memcpy(sztmp, idm, strlen(idm) > 127 ? 127 : strlen(idm));
			urlDecode(sztmp);
			strncpy(parse->username,sztmp,32);
		}
	}
	//mobile
	else if(strstr(parse->url, "m.zhaopin.com"))
	{
		if((idm = strstr(parse->url, "KeyWord=")) != NULL)
		{
			idm += 8;
			if((idn = strchr(idm, '&')) != NULL)
				memcpy(sztmp, idm, idn - idm > 127 ? 127 : idn - idm);
			else
				memcpy(sztmp, idm, strlen(idm) > 127 ? 127 : strlen(idm));
			urlDecode(sztmp);
			strncpy(parse->keyword,sztmp,32);
		}
	}

	if(!strlen(parse->keyword))
	{
		memset(parse->proto_name, 0, sizeof(parse->proto_name));
		memcpy(parse->proto_name, "http", 4);
	}
	
	return 1;
}

/********************************************************/

int protocol_liepin(void *protocol_parse, u_char *data, int data_len)
{
	PROTOCOL_PARSE *parse = (PROTOCOL_PARSE *)protocol_parse;
	int ret = 0;
	char *idm = NULL, *idn = NULL;
	ret = protocol_http((void *)parse, data, data_len);
	if(!ret || !strlen(parse->url))
		return 0;
	//PC
	char sztmp[128]={0};
	if(strstr(parse->url, "www.liepin.com"))
	{
		if((idm = strstr(parse->url, "&key=")) != NULL)
		{
			idm += 5;
			if((idn = strchr(idm, '&')) != NULL)
				memcpy(sztmp, idm, idn - idm > 127 ? 127 : idn - idm);
			else
				memcpy(sztmp, idm, strlen(idm) > 127 ? 127 : strlen(idm));
			urlDecode(sztmp);
			strncpy(parse->keyword,sztmp,32);
		}
	}
	//mobile
	else if(strstr(parse->url, "m.liepin.com"))
	{
		if((idm = strstr(parse->url, "keyword=")) != NULL)
		{
			idm += 8;
			if((idn = strchr(idm, '&')) != NULL)
				memcpy(sztmp, idm, idn - idm > 127 ? 127 : idn - idm);
			else
				memcpy(sztmp, idm, strlen(idm) > 127 ? 127 : strlen(idm));
			urlDecode(sztmp);
			strncpy(parse->keyword,sztmp,32);
		}
	}

	if(!strlen(parse->keyword))
	{
		memset(parse->proto_name, 0, sizeof(parse->proto_name));
		memcpy(parse->proto_name, "http", 4);
		return 1;
	}
	
	if((idm = strstr(data, "user_login=")) != NULL)
	{
		idm += 11;
		if((idn = strchr(idm, ';')) != NULL)
		    memcpy(sztmp, idm, idn - idm > 127 ? 127 : idn - idm);
		else
			memcpy(sztmp, idm, strlen(idm) > 127 ? 127 : strlen(idm));
		urlDecode(sztmp);
		strncpy(parse->username,sztmp,32);
	}
	
	return 1;
}

/********************************************************/

int protocol_tmall(void *protocol_parse, u_char *data, int data_len)
{
	PROTOCOL_PARSE *parse = (PROTOCOL_PARSE *)protocol_parse;
	char szuid1[] = "userId=cntaobao";
	char *psrc = (char *)data;
	char *idm = NULL, *idn = NULL;
	char sztmp[128]={0};
	if((idm = strstr(psrc, szuid1)) != NULL)
	{
		idm += strlen(szuid1);
		if((idn = strchr(idm, '&')) != NULL)
			memcpy(sztmp, idm, idn - idm > 127 ? 127 : idn - idm);
		else
			memcpy(sztmp, idm, strlen(idm) > 127 ? 127 : strlen(idm));
		urlDecode(sztmp);
		strncpy(parse->username,sztmp,32);
	}

	protocol_http((void *)parse, data, data_len);

	return 1;
}
int protocol_taobao(void *protocol_parse, u_char *data, int data_len)
{
	PROTOCOL_PARSE *parse = (PROTOCOL_PARSE *)protocol_parse;
	int ret = 0;
	char *idm = NULL, *idn = NULL;
	char szuid[] = "user_id=cntaobao";
	char szuid1[] = "userId=cntaobao";
	char szuid2[]="loginId=cntaobao";
	char sznick[]= "_w_tb_nick=";
	char key[] ="q%22%3A%22";
	char endkey[] = "%22%2C%22";
	char *psrc = (char *)data;
	char *out = NULL;
	unsigned int len = 0;
	char sztmp[128]={0};
	if (1 == parse->optype)
	{
		*(psrc + data_len) = ';';
		if (1== get_tab_contant(psrc,szuid,"&",NULL,&out,&len) && NULL != out && len > 0)
		{
			len = (len > 128) ? 128:len;
			memcpy(sztmp,out,len);
			urlDecode(sztmp);
			strncpy(parse->username,sztmp,32);
		}
		else if (1== get_tab_contant(psrc,szuid1,"&",";",&out,&len) && NULL != out && len > 0)
		{
			len = (len > 128) ? 128:len;
			memcpy(sztmp,out,len);
			urlDecode(sztmp);
			strncpy(parse->username,sztmp,32);
		}
		else if (1== get_tab_contant(psrc,szuid2,"&",NULL,&out,&len) && NULL != out && len > 0)
		{
			len = (len > 128) ? 128:len;
			memcpy(sztmp,out,len);
			urlDecode(sztmp);
			strncpy(parse->username,sztmp,32);
		}
		else
		{
			if (1== get_tab_contant(psrc,sznick,";",NULL,&out,&len) && NULL != out && len > 0)
			{
				len = (len > 128) ? 128:len;
				memcpy(sztmp,out,len);
				urlDecode(sztmp);
				strncpy(parse->username,sztmp,32);
			}
		}
		protocol_http((void *)parse, data, data_len);
		return 1;
	}
	
	if (7 == parse->optype)
	{
		if (1== get_tab_contant(psrc,key,endkey," ",&out,&len) && NULL != out && len > 0)
		{
			len = (len > 256) ? 256:len;
			memcpy(sztmp,out,len);
			urlDecode(sztmp);
			strncpy(parse->keyword,sztmp,32);
		}
	}
	
	//mobile andriod	
	//if(strstr(parse->url, "api.s.m.taobao.com/search.json"))
	{
		if((idm = strstr(psrc, "&q=")) != NULL)
		{
			idm += 3;
			if((idn = strchr(idm, '&')) != NULL)
				memcpy(sztmp, idm, idn - idm > 127 ? 127 : idn - idm);
			else
				memcpy(sztmp, idm, strlen(idm) > 127 ? 127 : strlen(idm));
			urlDecode(sztmp);
			strncpy(parse->keyword,sztmp,32);
		}
		
		//���û���û�������ʹ��imei�����û���
		if(!strlen(parse->username))
		{
			if((idm = strstr(psrc, "&imei=")) != NULL)
			{
				idm += 6;
				if((idn = strchr(idm, '&')) != NULL)
					memcpy(sztmp, idm, idn - idm > 127 ? 127 : idn - idm);
				else
					memcpy(sztmp, idm, strlen(idm) > 127 ? 127 : strlen(idm));
				urlDecode(sztmp);
				strncpy(parse->username,sztmp,32);
				strncpy(parse->imei,sztmp,32);
			}
		}
	}
	
	return 1;
}


/*********************************************************/

int protocol_meituan(void *protocol_parse, u_char *data, int data_len)
{
	PROTOCOL_PARSE *parse = (PROTOCOL_PARSE *)protocol_parse;
	char szCookie[] = "Cookie:";
	char szname[] ="n=";
	char szuid[] = "userid:";
	char szuid1[]="userid=";
	char keyword[]="q=";
	char sztmp[128]={0};
	char *out = NULL;
	unsigned int len = 0;
	char *p = (char *)data;
	protocol_http((void *)parse, data, data_len);
	if (7 == parse->optype)
	{
		if (1== get_tab_contant(p,keyword,"&"," ",&out,&len) && NULL != out && len > 0)
		{
			len = (len > 128) ? 128:len;
			memcpy(sztmp,out,len);
			urlDecode(sztmp);
			strncpy(parse->keyword,sztmp,32);
		}
	
	//	return 1;
	}

	if (1== get_tab_contant(p,szuid,"\r\n",NULL,&out,&len) && NULL != out && len > 0)
	{
		len = (len > 128) ? 128:len;
		memset(sztmp,0,128);
		memcpy(sztmp,out,len);
		urlDecode(sztmp);
		strncpy(parse->username,sztmp,32);
	}
	else 	if (1== get_tab_contant(p,szuid1,"\r\n",NULL,&out,&len) && NULL != out && len > 0)
	{
		len = (len > 128) ? 128 : len;
		memset(sztmp, 0, 128);
		memcpy(sztmp, out, len);
		urlDecode(sztmp);
		strncpy(parse->username, sztmp, 32);
	}
	char *p3 = strstr(p,szCookie);
	if (NULL == p3)
	{
		return 1;
	}
	
	if (1== get_tab_contant(p3,szname,";","\r\n",&out,&len) && NULL != out && len > 0)
	{
		len = (len > 128) ? 128:len;
		memset(sztmp,0,128);
		memcpy(sztmp,out,len);
		urlDecode(sztmp);
		strncpy(parse->username,sztmp,32);
	}

	return 1;
}

/********************************************************/
int protocol_tecent_video(void *protocol_parse, u_char *data, int data_len)
{
	PROTOCOL_PARSE *parse = (PROTOCOL_PARSE *)protocol_parse;
	char *token1 = "qqlog=";
	char *token2 = "nickname=";
	char *token3 = "query=";
	char *imei="imei=";
	char *psrc = (char *)data;
	char *out = NULL;
	unsigned int len = 0;

	char sztmp[128]={0};

	if (1== get_tab_contant(psrc,imei,"&",NULL,&out,&len) && NULL != out && len > 0)
	{
		if(len < 32)
			memcpy(parse->imei,out,len);
	}

	if (1== get_tab_contant(psrc,token1,"&",NULL,&out,&len) && NULL != out && len > 0)
	{
		len = (len > 31) ? 31:len;
		if (0 == IsAllDigital(out,len))
		{
			return 0;
		}
		memcpy(parse->username,out,len);
	}

	if (parse->optype == 1)
	{
		if (1== get_tab_contant(psrc,token2,"&",NULL,&out,&len) && NULL != out && len > 0)
		{
			len = (len > 31) ? 31:len;
			memcpy(parse->username,out,len);
		}
	}
	if (parse->optype == 7)
	{
		if (1== get_tab_contant(psrc,token3,"&",NULL,&out,&len) && NULL != out && len > 0)
		{
			len = (len > 128) ? 128:len;
			memcpy(sztmp,out,len);
			urlDecode(sztmp);
			strncpy(parse->username,sztmp,32);
		}
	}

	protocol_http((void *)parse, data, data_len);
	
	return 1;
}

/***************************************************/
int protocol_qunaer(void  *protocol_parse, u_char *data, u_int data_len)
{
	PROTOCOL_PARSE *parse = (PROTOCOL_PARSE *)protocol_parse;
	char *psrc = (char *)data;
	char token[]="\"UID\" : \"";
	char token1[]="\"UID\":\"";
	char *out = NULL;
	unsigned int len = 0;
	char keyword[]="q=";
	if(7 == parse->optype)
	{
		if (1== get_tab_contant(psrc,keyword," ","\r\n",&out,&len) && NULL != out && len > 0)
		{
			char sztmp[128]={0};
			len = (len > 128) ? 128 : len;
			memcpy(sztmp, out, len);
			urlDecode(sztmp);
			strncpy(parse->keyword, sztmp, 32);
		}
	}

	if (1== get_tab_contant(psrc,token,"\"",NULL,&out,&len) && NULL != out && len > 0)
	{
		len = (len > 31) ? 31:len;
		memcpy(parse->username,out,len);
	}
	if (1== get_tab_contant(psrc,token1,"\"",NULL,&out,&len) && NULL != out && len > 0)
	{
		len = (len > 31) ? 31:len;
		memcpy(parse->username,out,len);
	}
	protocol_http(parse, data, data_len);
	return 1;
}
/********************************************************/
int protocol_dianping(void *protocol_parse, u_char *data, int data_len)
{
	PROTOCOL_PARSE *parse = (PROTOCOL_PARSE *)protocol_parse;
	char *psrc = (char *)data;
	char token[]="ua=";
	char *keyword="keyword=";
	char *out = NULL;
	unsigned int len = 0;
	
	char sztmp[128]={0};
	
	protocol_http((void *)parse, data, data_len);
	if (7 == parse->optype)
	{
		if (1== get_tab_contant(psrc,keyword,"&","\r\n",&out,&len) && NULL != out && len > 0)
		{
			len = (len > 128) ? 128:len;
			memcpy(sztmp,out,len);
			urlDecode(sztmp);
			strncpy(parse->keyword,sztmp,32);
		}
	}
	if (1== get_tab_contant(psrc,token,";","\r\n",&out,&len) && NULL != out && len > 0)
	{
		len = (len > 128) ? 128:len;
		memset(sztmp,0,128);
		memcpy(sztmp,out,len);
		urlDecode(sztmp);
		strncpy(parse->username,sztmp,32);
	}

	return 1;
}

/**************************************************************/
int protocol_qqnews(void *protocol_parse, u_char *data, int data_len)
{
	PROTOCOL_PARSE *parse = (PROTOCOL_PARSE *)protocol_parse;
	char *psrc = (char *)data;
	char token[]="luin=o";
	char *idfa="idfa=";
	char *out = NULL;
	unsigned int len = 0;
	
	if (1== get_tab_contant(psrc,token,";","\r\n",&out,&len) && NULL != out && len > 0)
	{
		len = (len > 31) ? 31:len;
		memcpy(parse->username,out,len);
	}
	if (1== get_tab_contant(psrc,idfa,"&","\r\n",&out,&len) && NULL != out && len > 0)
	{
		memcpy(parse->imei,out,(len > 63) ? 63:len);
	}
	protocol_http((void *)parse, data, data_len);
	return 1;
}
/*********************************************************/
int protocol_qqmail(void *protocol_parse, u_char *data, int data_len)
{
	PROTOCOL_PARSE *parse = (PROTOCOL_PARSE *)protocol_parse;
	char *psrc = (char *)data;
	char token[]="uin=";
	char *out = NULL;
	unsigned int len = 0;
	*(psrc+data_len)=';';
	if (1== get_tab_contant(psrc,token,"&",";",&out,&len) && NULL != out && len > 0)
	{
		len = (len > 31) ? 31:len;
		memcpy(parse->username,out,len);
	}
	protocol_http((void *)parse, data, data_len);
	return 1;
}
/*********************************************************/
int protocol_baidu_app(void *protocol_parse, u_char *data, int data_len)
{
	PROTOCOL_PARSE *parse = (PROTOCOL_PARSE *)protocol_parse;
	char *psrc = (char *)data;
	char token[]="BDUSS=";
	char idfa[]="idfa=";
	char *out = NULL;
	unsigned int len = 0;
	char tmp[256] = {0};
	protocol_http((void *)parse, data, data_len);
	if (1== get_tab_contant(psrc,token,";","\r\n",&out,&len) && NULL != out && len > 0)
	{
		len = (len > 511) ? 511:len;
		memcpy(parse->content,out,len);
	}
	if (1== get_tab_contant(psrc,idfa,"&","\r\n",&out,&len) && NULL != out && len > 0)
	{
		memcpy(parse->imei,out,(len > 63) ? 63:len);
	}

	return 1;
}

/*********************************************************/
int protocol_jd(void *protocol_parse, u_char *data, int data_len)
{
	PROTOCOL_PARSE *parse = (PROTOCOL_PARSE *)protocol_parse;
	char *psrc = (char *)data;
	char token[]="uuid=";
	char token1[]="pin=";
	char key[]="keyword%22%3A%22";
	char *out = NULL;
	unsigned int len = 0;
	protocol_http((void *)parse, data, data_len);
	char sztmp[128]={0};
	if (1== get_tab_contant(psrc,token,"&","\r\n",&out,&len) && NULL != out && len > 0)
	{
		len = (len > 128) ? 128:len;
		memcpy(sztmp,out,len);
		urlDecode(sztmp);
		strncpy(parse->username,sztmp,32);
	}
	else if (1== get_tab_contant(psrc,token1,";","\r\n",&out,&len) && NULL != out && len > 0)
	{
		len = (len > 128) ? 128:len;
		memcpy(sztmp,out,len);
		urlDecode(sztmp);
		strncpy(parse->username,sztmp,32);
	}
	
	if (7 == parse->optype)
	{
		if (1== get_tab_contant(psrc,key,"%22%7D","&",&out,&len) && NULL != out && len > 0)
		{
			len = (len > 128) ? 128:len;
			memset(sztmp,0,128);
			memcpy(sztmp,out,len);
			urlDecode(sztmp);
			strncpy(parse->keyword,sztmp,32);
		}
	}
	
	return 1;
}
/****************************************************/

int protocol_weibo(void *protocol_parse, u_char *data, int data_len)
{
	PROTOCOL_PARSE *parse = (PROTOCOL_PARSE *)protocol_parse;
	char *psrc = (char *)data;
	char token[]="X-Log-Uid:";
	char token1[]="uid=";
	char token2[]="X-Uid:";
	char *out = NULL;
	unsigned int len = 0;
	
	if (1== get_tab_contant(psrc,token,"\r\n",NULL,&out,&len) && NULL != out && len > 0)
	{
		len = (len > 31) ? 31:len;
		memcpy(parse->username,out,len);
	}
	else if (1== get_tab_contant(psrc,token1,"&",NULL,&out,&len) && NULL != out && len > 0)
	{
		len = (len > 31) ? 31:len;
		memcpy(parse->username,out,len);
	}
	else if (1== get_tab_contant(psrc,token1,"&",NULL,&out,&len) && NULL != out && len > 0)
	{
		len = (len > 31) ? 31:len;
		memcpy(parse->username,out,len);
	}

	protocol_http((void *)parse, data, data_len);
	
	return 1;
}

/*****************************************************/
int protocol_58tongcheng(void *protocol_parse, u_char *data, int data_len)
{
	PROTOCOL_PARSE *parse = (PROTOCOL_PARSE *)protocol_parse;
	char *psrc = (char *)data;
	char token[]="username=";
	char token1[]="key=";
	char username[]="UN=";
	char token2[]="tn:";
	char token3[]="imei:";
	char token4[]="ua:";
	char szkey[]="key%22%3a%22";
	char  idfa[]="idfa:";

	char sztmp[128]={0};
	
	char *out = NULL;
	unsigned int len = 0;
	char *nextpos = NULL;
	protocol_http((void *)parse, data, data_len);
	if (7 == parse->optype)
	{
		if (1== get_tab_contant_ex(psrc,token1,"&os"," ",&nextpos,&out,&len) && NULL != out && len > 0)
		{
			len = (len > 128) ? 128:len;
			memcpy(sztmp,out,len);
			urlDecode(sztmp);
			strncpy(parse->keyword,sztmp,32);
		}
		else if(1== get_tab_contant_ex(psrc,token1,"&","\r\n",&nextpos,&out,&len) && NULL != out && len > 0)
		{

			len = (len > 128) ? 128 : len;
			memcpy(sztmp, out, len);
			urlDecode(sztmp);
			strncpy(parse->keyword, sztmp, 32);
		}
		else if(1== get_tab_contant_ex(psrc,szkey,"%22","\r\n",&nextpos,&out,&len) && NULL != out && len > 0)
		{

			len = (len > 128) ? 128 : len;
			memcpy(sztmp, out, len);
			urlDecode(sztmp);
			strncpy(parse->keyword, sztmp, 32);
		}

	}
	
	if (1== get_tab_contant(psrc,token,"&","\r\n",&out,&len) && NULL != out && len > 0)
	{
		len = (len > 128) ? 128:len;
		memcpy(sztmp,out,len);
		urlDecode(sztmp);
		strncpy(parse->username,sztmp,32);
	}
	else if(1== get_tab_contant(psrc,username,"&","\r\n",&out,&len) && NULL != out && len > 0)
	{
		len = (len > 128) ? 128:len;
		memcpy(sztmp,out,len);
		urlDecode(sztmp);
		strncpy(parse->username,sztmp,32);
	}
	
	if (1== get_tab_contant_ex(psrc,token2,"\r\n",NULL,&nextpos,&out,&len) && NULL != out && len > 0)
	{
		len = (len > 128) ? 128:len;
		memset(sztmp,0,128);
		memcpy(sztmp,out,len);
		urlDecode(sztmp);
		strncpy(parse->username,sztmp,32);
	}
	if (nextpos != NULL)
	{
		psrc = nextpos;
	}
	if (1== get_tab_contant_ex(psrc,token3,"\r\n",NULL,&nextpos,&out,&len) && NULL != out && len > 0)
	{
		len = (len > 32) ? 32:len;
		memcpy(parse->imei,out,len);
	}
	if (nextpos != NULL)
	{
		psrc = nextpos;
	}
	if (1== get_tab_contant_ex(psrc,token4,"\r\n",NULL,&nextpos,&out,&len) && NULL != out && len > 0)
	{
		len = (len > 32) ? 32:len;
		memcpy(parse->devname,out,len);
	}
	return 1;
}
/********************************************************/
int protocol_dididache(void *protocol_parse, u_char *data, int data_len)
{
	PROTOCOL_PARSE *parse = (PROTOCOL_PARSE *)protocol_parse;
	char *psrc = (char *)data;
	char token[]="phone=";
	char kimei[]="imei=";
	char idfa[]="idfa=";
	char *out = NULL;
	unsigned int len = 0;

	if (1== get_tab_contant(psrc,token,"&"," ",&out,&len) && NULL != out && len > 0)
	{
		len = (len > 31) ? 31:len;
		memcpy(parse->username,out,len);
	}
	if (1== get_tab_contant(psrc,idfa,"&"," ",&out,&len) && NULL != out && len > 0)
	{
		memcpy(parse->imei,out,(len > 63) ? 63:len);
	}
	else if (1== get_tab_contant(psrc,kimei,"&"," ",&out,&len) && NULL != out && len > 0)
	{
		len = (len > 31) ? 31:len;
		memcpy(parse->imei,out,len);
	}
	protocol_http((void *)parse, data, data_len);
	
	return 1;
}

/********************************************************/
int protocol_ctrip(void *protocol_parse, u_char *data, int data_len)
{
	PROTOCOL_PARSE *parse = (PROTOCOL_PARSE *)protocol_parse;
	char *psrc = (char *)data;
	char token[]="\"UID\" : \"";
	char token1[]="\"UID\":\"";
	char token2[]="muc/";
	char *out = NULL;
	unsigned int len = 0;

	if (1== get_tab_contant(psrc,token,"\"",NULL,&out,&len) && NULL != out && len > 0)
	{
		len = (len > 31) ? 31:len;
		memcpy(parse->username,out,len);
	}
	if (1== get_tab_contant(psrc,token1,"\"",NULL,&out,&len) && NULL != out && len > 0)
	{
		len = (len > 31) ? 31:len;
		memcpy(parse->username,out,len);
	}
	if (1== get_tab_contant(psrc,token2,"/",NULL,&out,&len) && NULL != out && len > 0)
	{
		len = (len > 31) ? 31:len;
		memcpy(parse->username,out,len);
	}
	protocol_http((void *)parse, data, data_len);
	return 1;
}

/********************************************************/
int protocol_bdnuomi(void *protocol_parse, u_char *data, int data_len)
{
	PROTOCOL_PARSE *parse = (PROTOCOL_PARSE *)protocol_parse;
	char *psrc = (char *)data;
	char token[]="bduss=";
	char token1[]="keywords=";
	char token2[]="search_word=";
	char idfa[]="idfa=";
	char *out = NULL;
	unsigned int len = 0;
	char tmp[256] = {0};
	char sztmp[128]={0};
	char *nextpos = NULL;

	if (1== get_tab_contant_ex(psrc,idfa,"&"," ",&nextpos,&out,&len) && NULL != out && len > 0)
	{
		memcpy(parse->username,out,(len > 63) ? 63:len);
	}

	if (1== get_tab_contant_ex(psrc,token,"&"," ",&nextpos,&out,&len) && NULL != out && len > 0)
	{
		len = (len > 511) ? 511:len;
		memcpy(parse->username,out,len);
	}
	

	if (7 == parse->optype)
	{
		if (1== get_tab_contant(psrc,token1,"&"," ",&out,&len) && NULL != out && len > 0)
		{
			len = (len > 128) ? 128:len;
			memcpy(tmp,out,len);
			urlDecode(tmp);
			strncpy(parse->keyword,tmp,32);
		}
		else if (1== get_tab_contant(psrc,token2,"&"," ",&out,&len) && NULL != out && len > 0)
		{
			len = (len > 128) ? 128:len;
			memcpy(tmp,out,len);
			urlDecode(tmp);
			strncpy(parse->keyword,tmp,32);
		}
	}
	
	protocol_http((void *)parse, data, data_len);
	
	return 1;
}
/**********************************************************/
int  protocol_weipin(void *protocol_parse, u_char *data, int data_len)
{
	PROTOCOL_PARSE *parse = (PROTOCOL_PARSE *)protocol_parse;
	char *psrc = (char *)data;
	char token[]="userid=";
	char *out = NULL;
	unsigned int len = 0;
	if (1== get_tab_contant(psrc,token,"&"," ",&out,&len) && NULL != out && len > 0)
	{
		len = (len > 31) ? 31:len;
		memcpy(parse->username,out,len);
	}
	protocol_http((void *)parse, data, data_len);
	return 1;
}
/********************************************************/
int protocol_xunlei(void *protocol_parse, u_char *data, int data_len)
{
	PROTOCOL_PARSE *parse = (PROTOCOL_PARSE *)protocol_parse;
	char *psrc = (char *)data;
	char token[]="key=";
	char keyword[]="key%3D";
	char *out = NULL;
	unsigned int len = 0;
	char sztmp[128]={0};
	if (1== get_tab_contant(psrc,token," ","&",&out,&len) && NULL != out && len > 0)//
	{
		len = (len > 128) ? 128:len;
		memcpy(sztmp,out,len);
		urlDecode(sztmp);
		strncpy(parse->keyword,sztmp,32);
	}
	else if(1== get_tab_contant(psrc,keyword,"%22"," ",&out,&len) && NULL != out && len > 0)
	{
		len = (len > 128) ? 128:len;
		memcpy(sztmp,out,len);
		urlDecode(sztmp);
		strncpy(parse->keyword,sztmp,32);
	}
	protocol_http((void *)parse, data, data_len);
	return 1;
}

int protocol_suningyigou(void *protocol_parse, u_char *data, int data_len)
{
	PROTOCOL_PARSE *parse = (PROTOCOL_PARSE *)protocol_parse;
	char *psrc = (char *)data;
	char token[]="idsLoginUserIdLastTime=";
	char key[] ="keyword=";
	char idfa[]="idfa=";
	char *out = NULL;
	unsigned int len = 0;
	char sztmp[128]={0};


	if (1== get_tab_contant(psrc,idfa,"&",NULL,&out,&len) && NULL != out && len > 0)
	{
		memcpy(parse->imei,out,(len > 63) ? 63:len);
	}
	if (1== get_tab_contant(psrc,token,";","\r\n",&out,&len) && NULL != out && len > 0)
	{
		len = (len > 31) ? 31:len;
		memcpy(parse->username,out,len);
	}
	protocol_http((void *)parse, data, data_len);
	
	if (7 == parse->optype)
	{
		if (1== get_tab_contant(psrc,key,"&"," ",&out,&len) && NULL != out && len > 0)
		{
			len = (len > 128) ? 128:len;
			memcpy(sztmp,out,len);
			urlDecode(sztmp);
			strncpy(parse->keyword,sztmp,32);
		}
	}
	
	return 1;
}


int protocol_360yunpan(void *protocol_parse, u_char *data, int data_len)
{
	PROTOCOL_PARSE *parse = (PROTOCOL_PARSE *)protocol_parse;
	char *psrc = (char *)data;
	char token[]="method=User.login";
	char keywork[]="account=";
	int iret = protocol_http((void *)parse, data, data_len);

	char *out = NULL;
	unsigned int len = 0;

	if(1 == iret)
	{
		out = strstr(parse->url,token);
		if (NULL == out)
		{
			return 0;
		}
	}
	if(1== get_tab_contant(psrc,keywork,"&","\r\n",&out,&len) && NULL != out && len > 0)
	{
		len = (len > 31) ? 31:len;
		memcpy(parse->username,out,len);
	}
	return 1;
}
int protocol_meituawaimai(void *protocol_parse, u_char *data, int data_len)
{
	PROTOCOL_PARSE *parse = (PROTOCOL_PARSE *)protocol_parse;
	char szCookie[] = "Cookie:";
	char szkeyword[]="keyword=";
	char szname[] ="n=";
	char szuid[] = "userid=";
	char sztmp[128]={0};
	char *out = NULL;
	unsigned int len = 0;
	char *p = (char *)data;

	protocol_http((void *)parse, data, data_len);
	
	if (1== get_tab_contant(p,szuid,"&"," ",&out,&len) && NULL != out && len > 0)
	{
		len = (len > 31) ? 31:len;
		memcpy(parse->username,out,len);
	}

	if(7 == parse->optype && 1== get_tab_contant(p,szkeyword,"&"," ",&out,&len) && NULL != out && len > 0)
	{
		len = (len > 128) ? 128:len;
		memcpy(sztmp,out,len);
		urlDecode(sztmp);
		strncpy(parse->keyword,sztmp,32);
	}
	
	char *p3 = strstr(p,szCookie);
	if (NULL == p3)
	{
		return 1;
	}
	
	if (1== get_tab_contant(p3,szname,";","\r\n",&out,&len) && NULL != out && len > 0)
	{
		len = (len > 31) ? 31:len;
		memset((char *)parse->username,0,31);
		memcpy(parse->username,out,len);
	}

	return 1;
}


int protocol_kugou(void *protocol_parse, u_char *data, int data_len)
{
	PROTOCOL_PARSE *parse = (PROTOCOL_PARSE *)protocol_parse;
	char *psrc = (char *)data;
	char token[]="uid=";
	char key[] ="keyword=";
	char *out = NULL;
	unsigned int len = 0;
	char sztmp[128]={0};
	if (1== get_tab_contant(psrc,token,"&"," ",&out,&len) && NULL != out && len > 0)
	{
		len = (len > 31) ? 31:len;
		memcpy(parse->username,out,len);
	}
	protocol_http((void *)parse, data, data_len);

	if (7 == parse->optype)
	{
		if (1== get_tab_contant(psrc,key,"&"," ",&out,&len) && NULL != out && len > 0)
		{
			len = (len > 128) ? 128:len;
			memcpy(sztmp,out,len);
			urlDecode(sztmp);
			strncpy(parse->keyword,sztmp,32);
		}
	}
	return 1;
}
/********************************************************************/
int protocol_aiqiyi(void *protocol_parse, u_char *data, int data_len)
{
	PROTOCOL_PARSE *parse = (PROTOCOL_PARSE *)protocol_parse;
	char *psrc = (char *)data;
	char key[] ="keyword=";
	char idfa[]="idfa=";
	char *out = NULL;
	unsigned int len = 0;
	char sztmp[128]={0};
	protocol_http((void *)parse, data, data_len);

	if(1==get_tab_contant(psrc,idfa,"&","\r\n",&out,&len)&&NULL!=out && len>0)
		memcpy(parse->imei,out,(len>63)?63:len);

	if (7 == parse->optype)
	{
		if (1== get_tab_contant(psrc,key,"&"," ",&out,&len) && NULL != out && len > 0)
		{
			len = (len > 128) ? 128:len;
			memcpy(sztmp,out,len);
			urlDecode(sztmp);
			strncpy(parse->keyword,sztmp,32);
		}
	}
	return 1;
}

/**********************************************************************/

int protocol_youku(void *protocol_parse, u_char *data, int data_len)
{
	PROTOCOL_PARSE *parse = (PROTOCOL_PARSE *)protocol_parse;
	char *psrc = (char *)data;
	char token[]="u=";
	char key[] ="search/";
	char token1[]="k=";
	char key1[] ="keyword=";
	char kimei[]="imei=";
	char *out = NULL;
	unsigned int len = 0;
	char sztmp[128]={0};
	if (1== get_tab_contant(psrc,token,";","\r\n",&out,&len) && NULL != out && len > 0)
	{
		len = (len > 31) ? 31:len;
		memcpy(parse->username,out,len);
	}
	else if(1== get_tab_contant(psrc,token1,";","\r\n",&out,&len) && NULL != out && len > 0){
		len = (len > 31) ? 31:len;
		memcpy(parse->username,out,len);
	}
	if (1== get_tab_contant(psrc,kimei,"&","\r\n",&out,&len) && NULL != out && len > 0)
	{
		len = (len > 31) ? 31:len;
		memcpy(parse->imei,out,len);
	}
	protocol_http((void *)parse, data, data_len);
	
	if (7 == parse->optype)
	{
		if (1== get_tab_contant(psrc,key,"?"," ",&out,&len) && NULL != out && len > 0)
		{
			len = (len > 128) ? 128:len;
			memcpy(sztmp,out,len);
			urlDecode(sztmp);
			strncpy(parse->keyword,sztmp,32);
		}
		else if (1== get_tab_contant(psrc,key1,"&","\r\n",&out,&len) && NULL != out && len > 0)
		{
			len = (len > 128) ? 128:len;
			memcpy(sztmp,out,len);
			urlDecode(sztmp);
			strncpy(parse->keyword,sztmp,32);
		}
	}
	return 1;	
}
/************************************************************/

int protocol_qqmusic(void *protocol_parse, u_char *data, int data_len)
{
	PROTOCOL_PARSE *parse = (PROTOCOL_PARSE *)protocol_parse;
	char *psrc = (char *)data;
	char token[]="uin=o";
    char token1[]="uin=";
	char *out = NULL;
	unsigned int len = 0;
	if (1== get_tab_contant(psrc,token,";","\r\n",&out,&len) && NULL != out && len > 0)
	{
		len = (len > 31) ? 31:len;
		memcpy(parse->username,out,len);
	}
    else if(1== get_tab_contant(psrc,token1,"&",NULL,&out,&len) && NULL != out && len > 0)
    {
        len = (len > 31) ? 31:len;
        memcpy(parse->username,out,len);
    }
	protocol_http((void *)parse, data, data_len);
	
	return 1;	
}
/*********************************************************/
int protocol_letv(void *protocol_parse, u_char *data, int data_len)
{
	PROTOCOL_PARSE *parse = (PROTOCOL_PARSE *)protocol_parse;
	char *psrc = (char *)data;
	char token[]="sso_nickname=";
	char key[] ="wd=";
	char imei[] ="imei=";
	char *out = NULL;
	unsigned int len = 0;
	char *nextpos = NULL;
	char sztmp[128]={0};
	if (7 == parse->optype)
	{
		if (1== get_tab_contant_ex(psrc,key,"&"," ",&nextpos,&out,&len) && NULL != out && len > 0)
		{
			len = (len > 128) ? 128:len;
			memcpy(sztmp,out,len);
			urlDecode(sztmp);
			strncpy(parse->keyword,sztmp,32);
		}
		if (NULL != nextpos)
		{
			psrc = nextpos;
		}
	}
	if (1== get_tab_contant(psrc,token,";","\r\n",&out,&len) && NULL != out && len > 0)
	{
		len = (len > 31) ? 31:len;
		memcpy(parse->username,out,len);
	}
	if (1== get_tab_contant(psrc,imei,"&","\r\n",&out,&len) && NULL != out && len > 0)
	{
		len = (len > 31) ? 31:len;
		memcpy(parse->imei,out,len);
	}
	protocol_http((void *)parse, data, data_len);
	
	return 1;
}
/*********************************************************/
int protocol_mgotv(void *protocol_parse, u_char *data, int data_len)
{
	PROTOCOL_PARSE *parse = (PROTOCOL_PARSE *)protocol_parse;
	char *psrc = (char *)data;
	char token[]="name=";
	char *out = NULL;
	unsigned int len = 0;
	char sztmp[128]={0};
	if (7 == parse->optype)
	{
		if (1== get_tab_contant(psrc,token,"&"," ",&out,&len) && NULL != out && len > 0)
		{
			len = (len > 128) ? 128:len;
			memcpy(sztmp,out,len);
			urlDecode(sztmp);
			strncpy(parse->keyword,sztmp,32);
		}
	}
	protocol_http((void *)parse, data, data_len);
	return 1;
}
/********************************************************/
int protocol_baofeng(void *protocol_parse, u_char *data, int data_len)
{
	PROTOCOL_PARSE *parse = (PROTOCOL_PARSE *)protocol_parse;
	char *psrc = (char *)data;
	char token[]="bf_user_name=";
	char key[] ="search_type/";
    char key1[]="query=";
	char *out = NULL;
	unsigned int len = 0;
	char *nextpos = NULL;
	char sztmp[128]={0};
	if (7 == parse->optype)
	{
		if (1== get_tab_contant_ex(psrc,key,"/"," ",&nextpos,&out,&len) && NULL != out && len > 0)
		{
			len = (len > 128) ? 128:len;
			memcpy(sztmp,out,len);
			urlDecode(sztmp);
			strncpy(parse->keyword,sztmp,32);
		}
        else if (1== get_tab_contant(psrc,key1,"&"," ",&out,&len) && NULL != out && len > 0)
        {
            len = (len > 128) ? 128:len;
            memcpy(sztmp,out,len);
            urlDecode(sztmp);
            strncpy(parse->keyword,sztmp,32);
        }
		if (NULL != nextpos)
		{
			psrc = nextpos;
		}
	}
	if (1== get_tab_contant(psrc,token,";","\r\n",&out,&len) && NULL != out && len > 0)
	{
		len = (len > 128) ? 128:len;
		memcpy(parse->username,out,len);
	}
	protocol_http((void *)parse, data, data_len);
	
	return 1;	
}
/********************************************************/
int protocol_sohutv(void *protocol_parse, u_char *data, int data_len)
{
	PROTOCOL_PARSE *parse = (PROTOCOL_PARSE *)protocol_parse;
	char *psrc = (char *)data;
	char token[]="&key=";
	char *out = NULL;
	unsigned int len = 0;
	char sztmp[128]={0};
	if (7 == parse->optype)
	{
		if (1== get_tab_contant(psrc,token,"&"," ",&out,&len) && NULL != out && len > 0)
		{
			len = (len > 128) ? 128:len;
			memcpy(sztmp,out,len);
			urlDecode(sztmp);
			strncpy(parse->keyword,sztmp,32);
		}
	}
	protocol_http((void *)parse, data, data_len);
	return 1;
}
/********************************************************/
int protocol_tudou(void *protocol_parse, u_char *data, int data_len)
{
	PROTOCOL_PARSE *parse = (PROTOCOL_PARSE *)protocol_parse;
	char *psrc = (char *)data;
	char token1[]="user_id=";
	char token2[]="u_nick=";
	char key[] ="keyword=";
	char idfa[]="idfa=";

	char ioskey[]="/layout/ios/v3/search/direct_all/";

	char *out = NULL;
	unsigned int len = 0;
	char *nextpos = NULL;
	char sztmp[128]={0};

	if (1== get_tab_contant_ex(psrc,idfa,"&","\r\n",&nextpos,&out,&len) && NULL != out && len > 0)
	{
		strncpy(parse->imei,out,(len>63)?63:len);
	}

	if (7 == parse->optype)
	{
		if (1== get_tab_contant_ex(psrc,key,"&"," ",&nextpos,&out,&len) && NULL != out && len > 0)
		{
			len = (len > 128) ? 128:len;
			memcpy(sztmp,out,len);
			urlDecode(sztmp);
			strncpy(parse->keyword,sztmp,32);
		}
		else if (1== get_tab_contant_ex(psrc,ioskey,"?","&",&nextpos,&out,&len) && NULL != out && len > 0)
		{
			len = (len > 128) ? 128:len;
			memcpy(sztmp,out,len);
			urlDecode(sztmp);
			strncpy(parse->keyword,sztmp,32);
		}
		if (NULL != nextpos)
		{
			psrc = nextpos;
		}
	}
	if (1== get_tab_contant(psrc,token2,";","\r\n",&out,&len) && NULL != out && len > 0)
	{
		len = (len > 128) ? 128:len;
		memset(sztmp,0,128);
		memcpy(sztmp,out,len);
		urlDecode(sztmp);
		strncpy(parse->username,sztmp,32);
	}
	else if (1== get_tab_contant(psrc,token1,"|"," ",&out,&len) && NULL != out && len > 0)
	{
		len = (len > 256) ? 256:len;
		int rlen = 0;
		memset(sztmp,0,128);
		memcpy(sztmp,out,len);
		char *re = base64Decode(sztmp,&rlen);
		memset(parse->username,0,strlen(parse->username));
		strncpy(parse->username,re,32);
		if (NULL != re)
		{
			free(re);
			re = NULL;
		}
	}
	protocol_http((void *)parse, data, data_len);
	return 1;
}
/********************************************************************/
int protocol_bdvideo(void *protocol_parse, u_char *data, int data_len)
{
	PROTOCOL_PARSE *parse = (PROTOCOL_PARSE *)protocol_parse;
	char *psrc = (char *)data;
	char token[]="BDUSS=";
	char token1[]="wd=";
	char imei[]="imei=";
	char *out = NULL;
	unsigned int len = 0;
	char tmp[256] = {0};
	char sztmp[128]={0};
	char *nextpos = NULL;
	if (7 == parse->optype)
	{
		if (1== get_tab_contant_ex(psrc,token1,"&"," ",&nextpos,&out,&len) && NULL != out && len > 0)
		{
			len = (len > 128) ? 128:len;
			memcpy(sztmp,out,len);
			urlDecode(sztmp);
			strncpy(parse->keyword,sztmp,32);
		}
		else
		{
			return 0;
		}
		if (NULL != nextpos)
		{
			psrc = nextpos;
		}
	}
	if (1== get_tab_contant(psrc,imei,"&","\r\n",&out,&len) && NULL != out && len > 0)
	{
		len = (len > 32) ? 32:len;
		memcpy(parse->imei,out,len);
	}
	if (1== get_tab_contant(psrc,token,"&"," ",&out,&len) && NULL != out && len > 0)
	{
		len = (len > 256) ? 256:len;
		memcpy(tmp,out,len);
	}
	
	char *p1 = NULL;
	char *p2 = NULL;
	unsigned int uilen = 0;
	
	if (0 !=tmp[0] && strlen(tmp)>110)
	{
		p1 = tmp + 94;
		p2 = strstr(p1,"AAAA");
		uilen = (unsigned int)p2 - (unsigned int)p1;
		if (uilen % 4 != 0)
		{
			uilen = ((uilen % 4) + 1)*4;
		}
		
		memcpy(parse->username,p1,uilen);
		int len = 0;
		char *re = base64Decode(parse->username,&len);
		memset(parse->username,0,strlen(parse->username));
		strncpy(parse->username,re,32);
		if (NULL != re)
		{
			free(re);
			re = NULL;
		}
	}
	protocol_http((void *)parse, data, data_len);
	return 1;
}
/********************************************************************/
int protocol_kuwo(void *protocol_parse, u_char *data, int data_len)
{
	PROTOCOL_PARSE *parse = (PROTOCOL_PARSE *)protocol_parse;
	char *psrc = (char *)data;
	char token[]="all=";
	char *out = NULL;
	unsigned int len = 0;
	char sztmp[128]={0};
	if (7 == parse->optype)
	{
		if (1== get_tab_contant(psrc,token,"&"," ",&out,&len) && NULL != out && len > 0)
		{
			len = (len > 128) ? 128:len;
			memcpy(sztmp,out,len);
			urlDecode(sztmp);
			strncpy(parse->keyword,sztmp,32);
		}
	}
	protocol_http((void *)parse, data, data_len);
	return 1;
}
/********************************************************************/
#if 0
int protocol_weixin(PROTOCOL_PARSE *parse,const u_char *data, const int data_len)
{
	protocol_http(parse, data, data_len);
	unsigned char key[]={0x0d,0x0a,0x0d,0x0a};
	unsigned char *psrc = data;
	unsigned int loc = 0;

	loc = locate_hex_data(psrc,data_len,key,4);
	if(0 == loc)
	{
		return 0;
	}
	psrc += (loc + 4);

	if (0xbf != psrc[0] || 0x5f != psrc[2])
	{
		return 0;
	}
	unsigned int uin = htonl(*(unsigned int *)(psrc + 7));
	sprintf(parse->username,"%u",uin);

/*	char *psrc = (char *)data;
	char token[]="uin=";
	char token2[]="devicetype=";
	char *out = NULL;
	unsigned int len = 0;
	char *nextpos = NULL;
	protocol_http(parse, data, data_len);
	if (1== get_tab_contant_ex(psrc,token,"&"," ",&nextpos,&out,&len) && NULL != out && len > 0)
	{
		len = (len > 256) ? 256:len;
		memcpy(parse->username,out,len);
	}
	
	if (nextpos != NULL)
	{
		psrc = nextpos;
		if (1== get_tab_contant_ex(psrc,token2,"&"," ",&nextpos,&out,&len) && NULL != out && len > 0)
		{
			len = (len > 64) ? 64:len;
			memcpy(parse->devname,out,len);
		}
	}
*/
	return 1;
}
#endif
int protocol_weixin(void *protocol_parse, u_char *data, int data_len)
{
	PROTOCOL_PARSE *parse = (PROTOCOL_PARSE *)protocol_parse;
	u_char *psrc = data;
	int flag = 0;
	unsigned int uin = 0;
	if(data_len < 30)
	{
		return 0;
	}
#if 0
	if(ntohl(*(unsigned int *)psrc) != data_len)
	{
		return 0;
	}
#endif
	if(parse->optype == 1)
	{
		if (*(psrc + 11) != 0x79)//����
		{
			return 0;
		}
	
		psrc += 16;
		
		if (0xbf != psrc[0] || 0x5f != psrc[2])
		{
			if (0x5d == psrc[0] && 0x5f == psrc[2])
			{
				uin = htonl(*(unsigned int *)(psrc + 7));
				sprintf(parse->username,"%u",uin);
				 parse->optype = 4;
				 return 1;
			}
			else
			{
				return 0;
			}
		}
		uin = htonl(*(unsigned int *)(psrc + 7));
		sprintf(parse->username,"%u",uin);
		 return 1;
	}
	else if(parse->optype == 4)
	{
        if(*(psrc + 11) != 0xed  && *(psrc + 11) != 0x13 )//����
        {
            return 0;
        }
        psrc += 16;
        
        if (0xbf != psrc[0] || 0x5f != psrc[2])
        {
            return 0;
        }
        uin  = htonl(*(unsigned int *)(psrc + 7));
        sprintf(parse->username,"%u",uin);
        return 1;
	}
	
	return 0;
}


int protocol_yy(void *protocol_parse, u_char *data, int data_len)
{
	PROTOCOL_PARSE *parse = (PROTOCOL_PARSE *)protocol_parse;
	char *psrc = (char *)data;
	char token[]="imei=";
	char token1[] ="model=";
	char *out = NULL;
	unsigned int len = 0;
	char *nextpos = NULL;
	if (1== get_tab_contant_ex(psrc,token,"&"," ",&nextpos,&out,&len) && NULL != out && len > 0)
	{
		len = (len > 32) ? 32:len;
		memcpy(parse->imei,out,len);
	}
	if (NULL != nextpos)
	{
		psrc = nextpos;
	}
	if (1== get_tab_contant_ex(psrc,token1,"&"," ",&nextpos,&out,&len) && NULL != out && len > 0)
	{
		len = (len > 32) ? 32:len;
		memcpy(parse->devname,out,len);
	}
	protocol_http((void *)parse, data, data_len);

	return 1;
}

int protocol_qqzone(void  *protocol_parse, u_char *data, u_int data_len)
{
	PROTOCOL_PARSE *parse = (PROTOCOL_PARSE *)protocol_parse;
	char *psrc = (char *)data;
	char token[]="uin=";
	char token1[] ="model=";
	char *out = NULL;
	unsigned int len = 0;
	char *nextpos = NULL;
	if (1== get_tab_contant_ex(psrc,token,"&","\r\n",&nextpos,&out,&len) && NULL != out && len > 0)
	{
		len = (len > 32) ? 32:len;
		memcpy(parse->username,out,len);
	}

	if (1== get_tab_contant_ex(psrc,token1,"&","\r\n",&nextpos,&out,&len) && NULL != out && len > 0)
	{
		len = (len > 32) ? 32:len;
		memcpy(parse->devname,out,len);
	}
	protocol_http(parse, data, data_len);

	return 1;
}

int protocol_baidusearch(void *protocol_parse, u_char *data, int data_len)
{
	PROTOCOL_PARSE *parse = (PROTOCOL_PARSE *)protocol_parse;
	char *psrc = (char *)data;
	char token[]="word=";
	char token2[]="wd=";
	char *out = NULL;
	unsigned int len = 0;
	char sztmp[128]={0};
	if (1== get_tab_contant(psrc,token,"&","\r\n",&out,&len) && NULL != out && len > 0)
	{
		len = (len > 128) ? 128:len;
		memcpy(sztmp,out,len);
		urlDecode(sztmp);
		strncpy(parse->keyword,sztmp,32);
	}
	else if (1== get_tab_contant(psrc,token2,"&","\r\n",&out,&len) && NULL != out && len > 0)
	{
		len = (len > 128) ? 128:len;
		memcpy(sztmp,out,len);
		urlDecode(sztmp);
		strncpy(parse->keyword,sztmp,32);
	}

	protocol_http(parse, data, data_len);
	return 1;
}


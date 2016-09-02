/****************************************************
*  file     :  format.c
*  function :  chars format
*  part     :  urlDecode   //url����  %20+%EB ��ʽ
*              urlEncode   //url����
*              uniDecode   //unicode����  \u6211\u7231��ʽ
*              uniEncode   //unicode����
*			   base64Decode  //base64����  emhhbmc=��ʽ
*              base64Encode  //base64����
*              
*****************************************************/
#include "product.h"
#include <iconv.h>

/********************************************************************
*  ����:	urlDecode, urlEncode
*  ����:	url���룬url����
*  ����:	
*  ����:	
*  ˵��:	
*********************************************************************/

/*  ��������ֵ 0-15,�쳣����ֵ -1 */
char hextoi(char hexch)
{	
	if((hexch >= '0') && (hexch <= '9'))
		return hexch - '0';
	else if((hexch >= 'A') && (hexch <= 'F'))
		return hexch - 'A' + 10;
	else if((hexch >= 'a') && (hexch <= 'f'))
		return hexch - 'a' + 10;
	else
		return -1;
}

/*   �� XX ��ʽ��ʮ�����Ƶ�����(ASCII��)ת�����ַ� */
char x2c(const char *hex)
{	
	char digit;
	digit = (hextoi(hex[0]));
	digit =  digit << 4;
	digit =  digit + hextoi(hex[1]);
	return(digit);
} 

/* '+' �滻�� ' '  */
char *plustospace(char* str)
{
	int x;
	if(str == NULL)
	{
		return NULL;
	}	
	for(x = 0; str[x]; x++) 	
	{
		if(str[x] == '+')
			str[x] = ' ';	
	}	
	return(str);
} 

/*   �� URL ����������, %XX ת�����ַ�, Ȼ�� + �滻�ɿո�  */
char *urlDecode(char *s)
{	
	int x, y;
	if(!s)
	{
		return NULL;
	}
	int length = strlen(s) - 2;
	for(x = 0, y = 0; s[y]; ++x, ++y)	
	{
		if((s[x] = s[y] ) == '%')
		{
			s[x] = x2c(&s[y+1]);
			y += 2;
		}
	}
	s[x] = '\0';
	return plustospace(s);
} 

/*   URL ����   */
const char digit[20] = "0123456789ABCDEF";
char *urlEncode(char* buffer, int blen, const char* url)
{
	if(!(url && buffer))
	{
		return NULL;
	}
	int i = 0;
	while((*url) && (i < blen - 1))
	{
		if(((*url > 'a') && (*url < 'z')) || ((*url > 'A') && (*url < 'Z')) || ((*url >'0')&&(*url <'9')))
		{
			buffer[i] = *url;
			i++;
		}
		else
		{
			if(i < blen - 3)
			{
				buffer[i] = '%';
				i++;
				buffer[i] = digit[(*url >> 4) & 0x0f];
				i++;
				buffer[i] = digit[*url & 0x0f];
				i++;
			}
		}
		url ++;
	}
	buffer[i] = '\0';
	return buffer;
} 

/********************************************************************
*  ����:	base64Encode
*  ����:	base64����
*  ����:	value: ��Ҫ������ַ���
*  ����:	result: ������ַ���
*  ˵��:	resultҪ�㹻��Ŀռ��Ž���������Ҫ�Է���ֵ���ڴ��ͷ�
*********************************************************************/
static char basis_64[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"; 
static char basis_641[] = "UMghABCuvwxySTdL3bcVtp0YZGHkl/zno89+KWXORefa12ij567IJmqrsNPQ4DEF";
#define CHAR64(c)  (((c) < 0 || (c) > 127) ? -1 : index_64[(c)])
#define CHAR641(c)  (((c) < 0 || (c) > 127) ? -1 : index_641[(c)])
//char * base64_encode(const unsigned char *value); 
//unsigned char * base64_decode(const char *value, int *rlen);
	 
static signed char index_64[128] = { 
									 -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1, 
									 -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1, 
									 -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,62, -1,-1,-1,63, 
									 52,53,54,55, 56,57,58,59, 60,61,-1,-1, -1,-1,-1,-1, 
									 -1, 0, 1, 2,  3, 4, 5, 6,	7, 8, 9,10, 11,12,13,14, 
									 15,16,17,18, 19,20,21,22, 23,24,25,-1, -1,-1,-1,-1, 
									 -1,26,27,28, 29,30,31,32, 33,34,35,36, 37,38,39,40, 
									 41,42,43,44, 45,46,47,48, 49,50,51,-1, -1,-1,-1,-1
									};
static signed char index_641[128] = { 
	-1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1, 
	-1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1, 
	-1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,35, -1,-1,-1,29, 
	22,44,45,16, 60,48,49,50, 33,34,-1,-1, -1,-1,-1,-1, 
	-1, 4, 5, 6, 61,62,63,25, 26,51,52,36, 15, 1, 57,39, 
	58,59,40,12, 13, 0,19,37, 38,23,24,-1, -1,-1,-1,-1, 
	-1,43,17,18, 14,41,42, 2,  3,46,47,27, 28,53,31,32, 
	21,54,55,56, 20, 7, 8, 9, 10,11,30,-1, -1,-1,-1,-1
	};

/*  base64 ����  */	 
char *base64Encode(const char *value) 
{
	unsigned char oval = 0; 
	int vlen = strlen(value);
	char *result = (char *)malloc((vlen * 4) / 3 + 5); 
	char *out = result; 
	while(vlen >= 3) 
	{ 
		*out++ = basis_64[value[0] >> 2]; 
		*out++ = basis_64[((value[0] << 4) & 0x30) | (value[1] >> 4)]; 
		*out++ = basis_64[((value[1] << 2) & 0x3C) | (value[2] >> 6)]; 
		*out++ = basis_64[value[2] & 0x3F]; 
		value += 3; 
		vlen -= 3; 
	}
	if(vlen > 0) 
	{ 
		*out++ = basis_64[value[0] >> 2]; 
		oval = (value[0] << 4) & 0x30 ; 
		if (vlen > 1) oval |= value[1] >> 4; 
		*out++ = basis_64[oval]; 
		*out++ = (vlen < 2) ? '=' : basis_64[(value[1] << 2) & 0x3C]; 
		*out++ = '='; 
	} 
	*out = '\0';  
	return result; 
} 

int base64_encode(char *src,char *dst)
{
	int Len = 0;
	unsigned int uiCount = 0;
	char *p = src;
	char A = 0x00;
	char B = 0x00;

	if(src == NULL || dst == NULL) 
	{
		return -1;
	}

	Len = strlen((const char *)src);
	//Len = Multilpe_strlen((const char *)src);
	if(Len == 0)
	{
		return -1;
	}

	while(Len > 0)
	{
		/*Դ��һ�ֽ�������λ��ȥ����2λ����2λ����*/
		A = p[0]>>2;
		A = A & 0x3f;
		*dst++ = basis_64[A];

		if(Len > 2)
		{
			/*��һ���ֽڸ�6λȥ��Ȼ��������λ���ڶ����ֽ�������λ��˫�ָ�λΪ1ȥ������Ӽ���*/
			A = (p[0] & 0x03)<<4;
			B = (p[1]>>4) & 0x0f;
			*dst++ = basis_64[A | B];

			/*�ڶ����ֽ�ȥ����4λ��������λ���ø�6λ�����������ֽ�����6λ��ȥ����6λ���õ�2λ������Ӽ���*/
			A = (p[1] & 0x0f)<<2;
			B = (p[2]>>6) & 0x03;

			*dst++ = basis_64[A | B];

			/*Դ�����ֽ�ȥ����2λ,���� */
			A = p[2] & 0x3f;
			*dst++ = basis_64[A];
		}
		else if(Len == 1)
		{
			A = (p[0] & 0x03)<<4;
			B = (p[1]>>4) & 0x0f;
			*dst++ = basis_64[A | B];

			*dst++ = '=';
			*dst++ = '=';
		}
		else if(Len == 2)
		{
			A = (p[0] & 0x03)<<4;
			B = (p[1]>>4) & 0x0f;
			*dst++ = basis_64[A | B];
			
			A = (p[1] & 0x0f)<<2;
			B = (p[2]>>6) & 0x03;
			*dst++ = basis_64[A | B];

			*dst++ = '=';
		}
        *dst = '\0';
		p += 3;
		Len -= 3;
		uiCount += 4;
	}
	
	
	return uiCount;
}

int base64_encode1(char *src,char *dst)
{
	int Len = 0;
	unsigned int uiCount = 0;
	char *p = src;
	char A = 0x00;
	char B = 0x00;

	if(src == NULL || dst == NULL) 
	{
		return -1;
	}

	Len = strlen((const char *)src);
	//Len = Multilpe_strlen((const char *)src);
	if(Len == 0)
	{
		return -1;
	}

	while(Len > 0)
	{
		/*Դ��һ�ֽ�������λ��ȥ����2λ����2λ����*/
		A = p[0]>>2;
		A = A & 0x3f;
		*dst++ = basis_641[A];

		if(Len > 2)
		{
			/*��һ���ֽڸ�6λȥ��Ȼ��������λ���ڶ����ֽ�������λ��˫�ָ�λΪ1ȥ������Ӽ���*/
			A = (p[0] & 0x03)<<4;
			B = (p[1]>>4) & 0x0f;
			*dst++ = basis_641[A | B];

			/*�ڶ����ֽ�ȥ����4λ��������λ���ø�6λ�����������ֽ�����6λ��ȥ����6λ���õ�2λ������Ӽ���*/
			A = (p[1] & 0x0f)<<2;
			B = (p[2]>>6) & 0x03;

			*dst++ = basis_641[A | B];

			/*Դ�����ֽ�ȥ����2λ,���� */
			A = p[2] & 0x3f;
			*dst++ = basis_641[A];
		}
		else if(Len == 1)
		{
			A = (p[0] & 0x03)<<4;
			B = (p[1]>>4) & 0x0f;
			*dst++ = basis_641[A | B];

			*dst++ = '=';
			*dst++ = '=';
		}
		else if(Len == 2)
		{
			A = (p[0] & 0x03)<<4;
			B = (p[1]>>4) & 0x0f;
			*dst++ = basis_641[A | B];
			
			A = (p[1] & 0x0f)<<2;
			B = (p[2]>>6) & 0x03;
			*dst++ = basis_641[A | B];

			*dst++ = '=';
		}
        *dst = '\0';
		p += 3;
		Len -= 3;
		uiCount += 4;
	}
	
	
	return uiCount;
}


/********************************************************************
*  ����:	base64Decode
*  ����:	base64����
*  ����:	value: �����ַ���
*  ���:	rlen: ������ַ�������
*  ����:	������ַ���
*  ˵��:	resultҪ�㹻��Ŀռ��Ž���������Ҫ�Է���ֵ���ڴ��ͷ�
*********************************************************************/	
char *base64Decode(const char *value, int *rlen) 
{  
	int c1, c2, c3, c4;		  
	int vlen = strlen(value); 
	char *result =(char *)malloc((vlen * 3) / 4 + 1); 
	char *out = result; 
	 
	*rlen = 0;
	 
	while(1)
	{ 
		if(value[0] == 0)
		{
			*out = '\0' ; 
			return result; 
		}
		c1 = value[0]; 
		if(CHAR64(c1) == -1) 
			goto base64_decode_error;
		c2 = value[1]; 
		if(CHAR64(c2) == -1) 
			goto base64_decode_error;
		c3 = value[2]; 
		if((c3 != '=') && (CHAR64(c3) == -1)) 
			goto base64_decode_error;
		c4 = value[3]; 
		if((c4 != '=') && (CHAR64(c4) == -1)) 
			goto base64_decode_error; 
		value += 4; 
		*out++ = (CHAR64(c1) << 2) | (CHAR64(c2) >> 4); 
		*rlen += 1; 
		if(c3 != '=')
		{ 
			*out++ = ((CHAR64(c2) << 4) & 0xf0) | (CHAR64(c3) >> 2); 
			*rlen += 1; 
			if(c4 != '=')
			{ 
				*out++ = ((CHAR64(c3) << 6) & 0xc0) | CHAR64(c4); 
				*rlen += 1; 
			}
		} 
	} 
base64_decode_error: 
	*result = 0; 
	*rlen = 0; 
	return result; 
}

char *base64Decode1(const char *value, int *rlen)
{
	int c1, c2, c3, c4;
	int vlen = strlen(value);
	char *result =(char *)malloc((vlen * 3) / 4 + 1);
	char *out = result;

	*rlen = 0;

	while(1)
	{
		if(value[0] == 0)
		{
			*out = '\0' ;
			return result;
		}
		c1 = value[0];
		if(CHAR641(c1) == -1)
			goto base64_decode_error;
		c2 = value[1];
		if(CHAR641(c2) == -1)
			goto base64_decode_error;
		c3 = value[2];
		if((c3 != '=') && (CHAR641(c3) == -1))
			goto base64_decode_error;
		c4 = value[3];
		if((c4 != '=') && (CHAR641(c4) == -1))
			goto base64_decode_error;
		value += 4;
		*out++ = (CHAR641(c1) << 2) | (CHAR641(c2) >> 4);
		*rlen += 1;
		if(c3 != '=')
		{
			*out++ = ((CHAR641(c2) << 4) & 0xf0) | (CHAR641(c3) >> 2);
			*rlen += 1;
			if(c4 != '=')
			{
				*out++ = ((CHAR641(c3) << 6) & 0xc0) | CHAR641(c4);
				*rlen += 1;
			}
		}
	}
	base64_decode_error:
	*result = 0;
	*rlen = 0;
	return result;
}


 char GetIndex(char c)
{
	if((c >= 'A') && (c <= 'Z'))
	{
		return (c - 'A');/*[A~Z]*/
	}
	else if((c >= 'a') && (c <= 'z'))
	{
		return (c - 'a' + 26);/*[A~Z][a~z]*/
	}
	else if((c >= '0') && (c <= '9'))
	{
		return (c - '0' + 52);/*[A~Z][a~z][0~9]*/
	}
	else if(c == '+')
	{
		return 62;/*[A~Z][a~z][0~9]+*/
	}
	else if(c == '/')
	{
		return 63;/*[A~Z][a~z][0~9]+/ */
	}

	return 0;
}

/*decode the string*/
int base64_decode(char *src,char *dst)
{
	static char tmp[4] = {0};
	char *p = src;
	unsigned int uiLen = 0;
	unsigned int uiCount = 0;
	int iRet = 0;

	if(src == NULL || dst == NULL) 
	{
		return -1;
	}

	uiLen = strlen(p);
	iRet = uiLen % 4;/*length = 4 * n*/
	if(iRet != 0)
	{
		return -1;
	}

	while(uiLen>2)
	{
		tmp[0] = GetIndex(p[0]);
		tmp[1] = GetIndex(p[1]);
		tmp[2] = GetIndex(p[2]);
		tmp[3] = GetIndex(p[3]);

		*dst++ = (tmp[0]<<2) | (tmp[1]>>4);
		*dst++ = (tmp[1]<<4) | (tmp[2]>>2);
		*dst++ = (tmp[2]<<6) | (tmp[3]);

		p += 4;
		uiLen -= 4;
		uiCount += 3;
	}

	return uiCount;
}


/********************************************************************
*  ����:	code_convert
*  ����:	��һ�ֱ���תΪ��һ�ֱ���
*  ����:	from_charset:Դ�ַ�������; to_charset:Ŀ���ַ�������
*			inbuf:Դ�ַ���; inlen:Դ�ַ�������
*			outbuf:Ŀ���ַ���; outlen:Ŀ���ַ�������
*  ����:	0: �ɹ�;   -1:ʧ��
*  ˵��:	
*********************************************************************/
int code_convert(char *from_charset, char *to_charset, char *inbuf, size_t inlen, char *outbuf, size_t outlen)
{
	iconv_t cd;
	size_t rc;
	char **pin = &inbuf;
	char **pout = &outbuf;

	cd = iconv_open(to_charset, from_charset);
	if(cd == (iconv_t)-1) 
		return -1;
	
	bzero(outbuf, outlen);
	rc = iconv(cd, pin, &inlen, pout, &outlen);
	if(rc == -1) 
		return -1;
	outbuf[outlen - 1] = '\0';
	iconv_close(cd);
	return 0;
}

/********************************************************************
*  ����:	utf2gbk
*  ����:	utf-8��ת��Ϊgbk��
*  ����:	inbuf:Ҫ�����ַ���; inlen:Ҫ�����ַ�������;
*			outbuf:Ҫת���ַ���; outlen:Ҫת���ַ�������;
*  ����:	0: �ɹ�;   -1:ʧ��
*  ˵��:	outbufҪ�㹻��Ŀռ��Ž����ע�������ڴ�
*********************************************************************/
int utf2gbk(char *inbuf, int inlen, char *outbuf, int outlen)
{
	return code_convert("utf-8", "gb2312", inbuf, (size_t)inlen, outbuf, (size_t)outlen);
}

/********************************************************************
*  ����:	gbk2utf8
*  ����:	gbk��ת��Ϊutf-8��
*  ����:	inbuf:Ҫ�����ַ���; inlen:Ҫ�����ַ�������;
*			outbuf:Ҫת���ַ���; outlen:Ҫת���ַ�������;
*  ����:	0: �ɹ�;   -1:ʧ��
*  ˵��:	outbufҪ�㹻��Ŀռ��Ž����ע�������ڴ�
*********************************************************************/
int gbk2utf8(char *inbuf, int inlen, char *outbuf, int outlen)
{
	return code_convert("gb2312", "utf-8", inbuf, (size_t)inlen, outbuf, (size_t)outlen);
}

/********************************************************************
*  ����:	unicode2utf8
*  ����:	gbk��ת��Ϊutf-8��
*  ����:	inbuf:Ҫ�����ַ���; inlen:Ҫ�����ַ�������;
*			outbuf:Ҫת���ַ���; outlen:Ҫת���ַ�������;
*  ����:	0: �ɹ�;   -1:ʧ��
*  ˵��:	outbufҪ�㹻��Ŀռ��Ž����ע�������ڴ�
*********************************************************************/
int unicode2utf8(char *inbuf, int inlen, char *outbuf, int outlen)
{
	return code_convert("UCS-4", "UTF-8", inbuf, (size_t)inlen, outbuf, (size_t)outlen);
}

/********************************************************************
*  ����:	replace
*  ����:	��һ���ַ����в���һ���Ӵ������Ұ����з��ϵ��Ӵ�����һ���滻�ַ����滻��
*  ����:	p_source:Ҫ���ҵ�ĸ�ַ�����p_seachҪ���ҵ����ַ���; p_repstr���滻���ַ���;
*  ����:	p_source:�滻����ַ���;
*  ˵��:	p_resultҪ�㹻��Ŀռ��Ž�������p_repstr���ܴ���p_seach�ĳ���
*********************************************************************/
char *replace(char *p_source, char *p_seach, char *p_repstr)
{
	int repstr_len = 0;
	int search_len = 0;

	char *p1 = NULL;
	char *psource = p_source;
	char *prep = p_repstr;
	char *pseach = p_seach;
	int nLen = 0;
	char *presult = (char *)malloc(2 * strlen(psource));
	char *ptmpresult = presult;

	repstr_len = strlen(prep);
	search_len = strlen(pseach);

	do{ 
		p1 = strstr(psource, pseach);
		if(p1 == NULL)
		{
			strcpy(presult, psource);
			break;
		}

		//������һ���滻�����һ���滻���м���ַ���
		nLen = p1 - psource;
		memcpy(presult, psource, nLen);

		//������Ҫ�滻���ַ���
		memcpy(presult + nLen, p_repstr, repstr_len);
		presult[nLen + repstr_len] = '\0';

		psource = p1 + search_len;
		presult = presult + nLen + repstr_len;

	}while(p1);
	
	memset(p_source, 0, strlen(p_source));
	memcpy(p_source, ptmpresult, strlen(ptmpresult) > strlen(p_source) ? strlen(ptmpresult) : strlen(p_source));
	free(ptmpresult);
	ptmpresult = NULL;
	
	return p_source;
}


/*
	��һ���ַ����в��ִ�Сд�����Ӵ�
*/
char *strcasestr(char *src,char *find)
{
	if(NULL == src || NULL == find)
	{
		return NULL;
	}
	char c, sc;
	size_t len;
	
	if ((c = *find++) != 0) {
	    c = c|0x20;
		len = strlen(find);
		do {
			do {
				if ((sc = *src++) == 0)
					return (NULL);
			} while ((sc|0x20) != c);
		} while (strncasecmp(src, find, len) != 0);
		src--;
	}
	return ((char *)src);
}




void StringToHex(char *dest, char *src, int len)
{
	int i = 0;
	int j = 0;
	unsigned char temp;

	if(len % 2 != 0)
		return;
						
	while(i < len)
	{
		temp = src[i];
		if((temp >= 0x30) && (temp <= 0x39))
		{
			temp = temp - 0x30;
			dest[j] = temp << 4;

		}
		else if((temp >= 0x41) && (temp <= 0x46))
		{
			temp = temp - 0x41 + 0x0A;
			dest[j] = temp << 4;
		}
		else if((temp >= 0x61) && (temp <= 0x66))
		{
			temp = temp - 0x61 + 0x0A;
			dest[j] = temp << 4;
		}
		else
		{
			dest[j] = 0x00;
		}
		//dest[j] = dest[j] << 4;
		temp = src[i + 1];
																							
		if((temp >= 0x30) && (temp <= 0x39))
		{
			temp = temp - 0x30;
			dest[j] = dest[j] | temp;
		}
		else if((temp >= 0x41) && (temp <= 0x46))
		{
			temp = temp - 0x41 + 0x0A;
			dest[j] = dest[j] | temp;
		}
		else if((temp >= 0x61) && (temp <= 0x66))
		{
			temp = temp - 0x61 + 0x0A;					
			dest[j] = dest[j] | temp;	
		}
		else
		{
			dest[j] = dest[j] | 0x00;
		}

		i = i + 2;
		j = j + 1;
	}
	return;
}

/*
void HextoString(unsigned char *digest,char* buff,int len) 
{
	unsigned int i;
	char *ptr = buff;
	for (i = 0; i < len; i++) 
	{
		sprintf (ptr,"x", digest[i]);
		ptr+=2;
	}
}
*/

void DelSpace(char *dst, const char *src)
{	
	while(*src != '\0') 
	{		
		if(*src == ' '||*src == '\t' ||*src == '\r'||*src == '\n')
			*dst = *(++src);
		else			
			*dst = *src;
		dst++;
		src++;
		
	}
	*dst = '\0';
}


/********************************************************/

char *numToIP(u_int num)
{
	char* buf = (char *)malloc(sizeof("xxx.xxx.xxx.xxx"));
	if(buf == NULL)
		return NULL;
	unsigned char* p = (unsigned char *)&num;
	sprintf(buf, "%d.%d.%d.%d", p[3]&0xff,p[2]&0xff,p[1]&0xff,p[0]&0xff);
	return buf;
}

/********************************************************/
int IsAllDigital(char *buf,unsigned int uiLen)
{
	char *p = buf;
	unsigned int i = 0;
	for(i = 0; i < uiLen; i++)
	{
		if(p[i] > '9' || p[i] < '0')
		{
			/* ���з�����*/
			return 0;
		}
	}	
	/* ������*/
	return 1;
}
/********************************************************/
int get_tab_contant(char *src,char *tag,char *endtag1, char *endtag2,char **out,unsigned int *outlen)
{
	if(NULL == src || NULL == tag || NULL == endtag1 || NULL == out)
	{
		printf("[ERROR]memory out!\n");
		return 0;
	}
	char *p = src;
	unsigned long ulLen = 0;
	char *p1 = strstr(p,tag);
	if(NULL == p1)
	{
		return 0;
	}
	p1 += strlen(tag);
	
	char *p2 = strstr(p1,endtag1);
	if(NULL == p2 && NULL != endtag2)
	{
		p2 = strstr(p1,endtag2);
		if (NULL == p2)
		{
			return 0;
		}
	}
	else
	{
		*out = p1;
		*outlen = 0;
	}
	
	ulLen = (unsigned long)p2 - (unsigned long)p1; 
	*out = p1;
	*outlen = (unsigned int)ulLen;
	
	return 1;
}


int get_tab_contant_ex(char *src,char *tag,char *endtag1, char *endtag2,char **nextpos,char **out,unsigned int *outlen)
{
	if(NULL == src || NULL == tag || NULL == endtag1 || NULL == out)
	{
		printf("[ERROR]memory out!\n");
		return 0;
	}
	char *p = src;
	unsigned long ulLen = 0;
	char *p1 = strstr(p,tag);
	if(NULL == p1)
	{
		return 0;
	}
	p1 += strlen(tag);
	
	char *p2 = strstr(p1,endtag1);
	if(NULL == p2 && NULL != endtag2)
	{
		 p2 = strstr(p1,endtag2);
		 if (NULL == p2)
		 {
			 return 0;
		 }
	}
	
	ulLen = (unsigned long)p2 - (unsigned long)p1; 
	*out = p1;
	*outlen = (unsigned int)ulLen;
	
	if (NULL != nextpos)
	{
		p2 += strlen(endtag1);
		*nextpos = p2;
	}
	return 1;
}	
/********************************************************/



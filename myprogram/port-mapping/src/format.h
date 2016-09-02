
//format.h

#ifndef __FORMAT_H_
#define __FORMAT_H_

char hextoi(char hexch);
char x2c(const char *hex);
char *plustospace(char* str);
char *urlDecode(char *s);

char *urlEncode(char* buffer, int blen, const char* url);

char *base64Encode(const char *value) ;
int base64_encode(char *src,char *dst);
char *base64Decode(const char *value, int *rlen) ;
char *base64Decode1(const char *value, int *rlen);
        int base64_decode(char *src,char *dst);
int base64_encode1(char *src,char *dst);
int utf2gbk(char *inbuf, int inlen, char *outbuf, int outlen);

int gbk2utf8(char *inbuf, int inlen, char *outbuf, int outlen);

int unicode2utf8(char *inbuf, int inlen, char *outbuf, int outlen);

char *replace(char *p_source, char *p_seach, char *p_repstr);

char *strcasestr(char *src,char *find);

void StringToHex(char *dest, char *src, int len);

void DelSpace(char *dst, const char *src);

char *numToIP(u_int num);

int IsAllDigital(char *buf,unsigned int uiLen);

int get_tab_contant(char *src,char *tag,char *endtag1, char *endtag2,char **out,unsigned int *outlen);

int get_tab_contant_ex(char *src,char *tag,char *endtag1, char *endtag2,char **nextpos,char **out,unsigned int *outlen);

unsigned int locate_hex_data(unsigned char *data,unsigned int  d_len,unsigned char *key,unsigned int keylen);



#endif //__FORMAT_H_


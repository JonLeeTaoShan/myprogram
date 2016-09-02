

/*public.h*/

#ifndef __PUBLIC_H_
#define __PUBLIC_H_

#include "product.h"



//#define 	TITLE_LEN			128
#define		PRO_OUTPUT_COUNT	200		    /* protocol parse output count	*/

/*	protocol output  */
#define 	OUTPUT_PATH 	"/tmp/as/"

#pragma pack(2)
typedef struct _tagPROTOCOL_PARSE
{
    char id[64];       //会话ID，场所编码+MAC地址+上机时间
    char wacode[16];   //场所编码
    char watype[4];    //场所服务类型
    char apmac[32];        //AP设备MAC地址
    char apid[32];		   //AP设备编号
    char srcMac[32];   //终端MAC
    char srcIPV6[64];  //源外网IPV6地址
    char dstIPV6[64];  //目地公网IPV6地址
    char longitude[16];//ap经度
    char latitude[16]; //ap纬度
    char proto_name[16];
    char username[32];/*  username      */
    char sendname[32];/*  sendername    */
    char toname[64]; /*  receivername  */
    char subject[64];	/*  subject		  */
    char content[512];	/*  content       */
    char url[576];
    char keyword[32];
    int	 optype;	   /*  login:1, logout:2, online:3, chat:4, sendmail:5, receivemail:6, search:7 */
    u_int ipaddr;      //场所内网IP地址
    u_int srcip;       //源外网IPV4地址
    u_int dstip;	   //目的公网IPV4地址
    u_short port;      //场所内网端口号
    u_short src_p_s;   //源外网IPV4起始端口号
    u_short src_p_e;   //源外网IPV4结束端口号
    u_short src_p_s_v6;//源外网IPV6开始端口号
    u_short src_p_e_v6;//源外网IPV6结束端口号
    u_short dst_port;  //目的公网IPV4端口号
    u_short dst_port_v6;//目的公网IPV6端口号
    u_long  captime;    //记录时间
    char 	devname[64];            //终端名称
    char 	imei[64];               //手机识别码
}PROTOCOL_PARSE;


#pragma pack()


 int initsyslog();
 void writeFile(int flag,PROTOCOL_PARSE *parse,char *buf_raw,char *buf_res_pre,char *buf_res_back);
 void write_heartbeat(char *mac);
 void writeLogInfo(void *pstMacItem);
 int closesyslog();

int  CheckFolderExist(const char *szPath);
void CreateDir(const char *p);

#endif //__PUBLIC_H_





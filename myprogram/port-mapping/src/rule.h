
/*
* rule.h
*defined rule
*/

#ifndef __RULE_H__
#define __RULE_H__

#include "cap.h"

/*	protocol property  */
typedef struct _tagRuleItem
{
	u_char		proto_type;		        /*1,	tcp, udp  */
	u_char		watype;                 /*2net service type*/
	char		protocol_name[16];		/* 3 protocol name  	*/
	int 		proto_id;				/*4*/
	u_int		min_sport;              /*5*/
	u_int		max_sport;				/*6*/
	u_int		min_dport;              /*7*/
	u_int		max_dport;				/*8*/
	u_char		property[128];			/*9  protocol property  	*/
	int 		property_len;           /*10*/
	int 		property_offset;		/*11 property offset     */
	u_char		subproperty[64];		/*12 protocol subproperty */
	int 		subproperty_len;        /*13*/
	int 		subproperty_offset;		/*14 subproperty offset		*/
	int			s2c;			        /*15 Server->Client 1 ; C->S 0*/
	int			optype;		    		/*16 login:1, logout:2, online: 3, chat:4 */
    int         multicap;               /*17 tcp分段数，0不分段，1分段*/
} RULEITEM;

typedef struct _tagHostPattern
{
	int id;
	char proto_name[16];
	char host[32];
}URLPATTERN;

typedef enum _en_PROTO_ID
{
	en_PROTO_ID_HTTP       = 0,
	en_PROTO_ID_WEIXIN,
	en_PROTO_ID_BAIDUSEARCH,
	en_PROTO_ID_BAIDUAPP,
	en_PROTO_ID_BAIDUTIEBA,
	en_PROTO_ID_QQ,
	en_PROTO_ID_QQGAME,
	en_PROTO_ID_TAOBAO,
	//en_PROTO_ID_TMALL,
	en_PROTO_ID_MEITUANTUANGOU,
	en_PROTO_ID_JD,
	en_PROTO_ID_DIANPING,
	en_PROTO_ID_BDNUOMI,
	en_PROTO_ID_MEITUANWAIMAI,
	en_PROTO_ID_WEIBO,
	en_PROTO_ID_58TONGCHENG,
	en_PROTO_ID_DIDIDACHE,
	en_PROTO_ID_360SAFE,
	en_PROTO_ID_CTRIP,
//	en_PROTO_ID_TIANYASHEQU,
//	en_PROTO_ID_MAOPU,
//	en_PROTO_ID_WANGYI,
	en_PROTO_ID_163WEBMAIL,
	en_PROTO_ID_QQNEWS,
	en_PROTO_ID_QQZONE,
	en_PROTO_ID_TECENTVIDEO,
	en_PROTO_ID_ZHIFUBAO,
	en_PROTO_ID_ZHILIAN,
	en_PROTO_ID_LEIPIN,
	en_PROTO_ID_KUGOU,
	en_PROTO_ID_QQMAIL,
	en_PROTO_ID_12306,
	en_PROTO_ID_WEIPIN,
	en_PROTO_ID_YY,
	en_PROTO_ID_QUNAR,
	en_PROTO_ID_VDIAN,
	en_PROTO_ID_DAZHIHUI,
	en_PROTO_ID_XUNLEI,
	en_PROTO_ID_360YUNPAN,
	en_PROTO_ID_JINRITOUTIAO,
	en_PROTO_ID_BAIDUMAP,
	en_PROTO_ID_GAODEMAP,
	en_PROTO_ID_DIDAPINCHE,
	en_PROTO_ID_SUNINGYIGOU,
	en_PROTO_ID_AIQIYI,
	en_PROTO_ID_YOUKU,
	en_PROTO_ID_QQMUSIC,
	en_PROTO_ID_LETV,
	en_PROTO_ID_MGOTV,
	en_PROTO_ID_BAOFENG,
	en_PROTO_ID_SOHUVIDEO,
	en_PROTO_ID_TUDOU,
	en_PROTO_ID_BDVIDEO,
	en_PROTO_ID_KUWO,
	en_PROTO_ID_FTP,
	en_PROTO_ID_IMAP,
	en_PROTO_ID_POP,
	en_PROTO_ID_SMTP,
	en_PROTO_ID_TELNET,
	en_PROTO_ID_MILIAO,
	en_PROTO_ID_DHCP,
	en_PROTO_ID_NULL,
	en_PROTO_ID_MAX
}PROTO_ID;

#define PROTOCOL_AUDIT_NUM  en_PROTO_ID_MAX

typedef struct _tagAUDIT
{
	int (*packet_audit)(void *protocol_parse, u_char *data, int data_len);
}PROTOCOL_AUDIT;

static URLPATTERN g_urlpattern[] = {
		{en_PROTO_ID_NULL,"","xiaoyun.com"},
		{en_PROTO_ID_NULL,"","wireless.mapbar.com"},
		{en_PROTO_ID_NULL,"","apple.com"},
		{en_PROTO_ID_NULL,"","gclick.cn"},
		{en_PROTO_ID_NULL,"","pro.cn"},
		{en_PROTO_ID_NULL,"","bdimg.com"},
		{en_PROTO_ID_NULL,"","weixin.qq.com"},
		{en_PROTO_ID_NULL,"","duba.net"},
		{en_PROTO_ID_NULL,"","mastat.3g.qq.com"},
		{en_PROTO_ID_BAIDUAPP,"baiduapp","m.baidu.com"},
		{en_PROTO_ID_BAIDUTIEBA,"baidutieba","c.tieba.baidu.com"},
		{en_PROTO_ID_TAOBAO,"taobao","m.taobao.com"},
		{en_PROTO_ID_MEITUANTUANGOU,"meituantuangou","api.meituan.com"},
		{en_PROTO_ID_MEITUANWAIMAI,"meituanwaimai","waimaiapi.meituan.com"},
		{en_PROTO_ID_JD,"jd","m.jd.com"},
		{en_PROTO_ID_JD,"jd","m.360buy.com"},
		{en_PROTO_ID_DIANPING,"dianping","api.dianping.com"},
		{en_PROTO_ID_BDNUOMI,"bdnuomi","app.nuomi.com"},
		{en_PROTO_ID_WEIBO,"weibo","wbapp.mobile.sina.cn"},
		{en_PROTO_ID_58TONGCHENG,"58tongcheng","app.58.com"},
		{en_PROTO_ID_DIDIDACHE,"dididache","diditaxi.com.cn"},
		{en_PROTO_ID_DIDIDACHE,"dididache","udache.com"},
		{en_PROTO_ID_360SAFE,"360safe","shouji.360.cn"},
		{en_PROTO_ID_CTRIP,"ctrip","m.ctrip.com"},
		{en_PROTO_ID_QQNEWS,"qqnews","r.inews.qq.com"},
		{en_PROTO_ID_NULL,"","uu.qq.com"},
		{en_PROTO_ID_NULL,"","pingma.qq.com"},
		{en_PROTO_ID_TECENTVIDEO,"tecentvideo","video.qq.com"},
		{en_PROTO_ID_NULL,"","omgmta1.qq.com"},
		{en_PROTO_ID_ZHIFUBAO,"zhifubao","mobilecns.alipay.com"},
		{en_PROTO_ID_ZHIFUBAO,"zhifubao","mdap.alipay.com"},
		{en_PROTO_ID_KUGOU,"kugou","kugou.com"},
		{en_PROTO_ID_QQMAIL,"qqmail","mail.qq.com"},
		{en_PROTO_ID_12306,"12306","domob.cn"},
		{en_PROTO_ID_WEIPIN,"weipin","appvipshop.com"},
		{en_PROTO_ID_YY,"YY","3g.yy.com"},
		{en_PROTO_ID_NULL,"","image.yy.com"},
		{en_PROTO_ID_QUNAR,"qunar","qunar.com"},
		{en_PROTO_ID_VDIAN,"vdian","125.39.222.147:80"},
		{en_PROTO_ID_XUNLEI,"xunlei","m.xunlei.com"},
		{en_PROTO_ID_360YUNPAN,"360yunpan","yunpan.360.cn"},
		{en_PROTO_ID_JINRITOUTIAO,"jinritoutiao","snssdk.com"},
		{en_PROTO_ID_BAIDUMAP,"baidumap","client.map.baidu.com"},
		{en_PROTO_ID_GAODEMAP,"gaodemap","webfiles.amap.com"},
		{en_PROTO_ID_SUNINGYIGOU,"suningyigou","suning.com"},
		{en_PROTO_ID_AIQIYI,"aiqiyi","iqiyi.com"},
		{en_PROTO_ID_YOUKU,"youku","3g.youku.com"},
		{en_PROTO_ID_YOUKU,"youku","mobile.youku.com"},
		{en_PROTO_ID_YOUKU,"youku","m.youku.com"},
		{en_PROTO_ID_QQMUSIC,"qqmusic","music.qq.com"},
		{en_PROTO_ID_LETV,"letv","app.m.letv.com"},
		{en_PROTO_ID_MGOTV,"mgotv","mobile.api.hunantv.com"},
		{en_PROTO_ID_BAOFENG,"baofeng","baofeng.com"},
		{en_PROTO_ID_BAOFENG,"baofeng","baofeng.net"},
		{en_PROTO_ID_SOHUVIDEO,"sohuvideo","tv.sohu.com"},
		{en_PROTO_ID_TUDOU,"tudou","3g.tudou.com"},
		{en_PROTO_ID_BDVIDEO,"bdvideo","video.baidu.com"},
		{en_PROTO_ID_KUWO,"kuwo","kuwo.cn"}

};

PROTOCOL_AUDIT g_proto_audit[PROTOCOL_AUDIT_NUM];

int protocol_audit_reg();

int urlpattern_init(void **ppacsm);

int get_match_rule(u_char pro_type/*tcp/udp*/,u_short sport,u_short dport,u_char *data, int data_len,RULEITEM **ppRuleItem);

#endif



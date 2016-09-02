

#include "product.h"
#include "rule.h"
#include "appmodule.h"
#include "acsmx.h"
#include "sfksearch.h"

RULEITEM g_UdpRule[]=
{
   {UDP_PACKET,0,"DHCP",en_PROTO_ID_DHCP,17408,17408,17152,17152,{0x00},0,0,{0x00},0,0,0,1,0},
   {UDP_PACKET,99,"qq",en_PROTO_ID_QQ, 0, 0, 0, 0,{0x02}, 1,0,{0x08,0x25},2,3,0,1,0},
   {UDP_PACKET,99,"qqyouxi",en_PROTO_ID_QQGAME,0,0,0,0,{0x02,0x50,0x06,0x08,0x27},5,0,{0x03},1,-1,0,1,0},
   {UDP_PACKET,99,"qqyouxi",en_PROTO_ID_QQGAME,0,0,0,0,{0x02,0x50,0x06,0x08,0x36},5,0,{0x03},1,-1,0,1,0}
};

RULEITEM g_TcpRule[]=
{
   {TCP_PACKET,99,"weixin",en_PROTO_ID_WEIXIN,4,65535,0,0,{0x00,0x10,0x00,0x01,0x00,0x00,0x00},7,4,{0x79},1,11,0,1,0},
   {TCP_PACKET,99,"weixin",en_PROTO_ID_WEIXIN,4,65535,0,0,{0x00,0x10,0x00,0x01,0x00,0x00,0x00},7,4,{0xed},1,11,0,4,0},
   {TCP_PACKET,99,"weixin",en_PROTO_ID_WEIXIN,4,65535,0,0,{0x00,0x10,0x00,0x01,0x00,0x00,0x00},7,4,{0x13},1,11,0,4,0},
   {TCP_PACKET,10,"qq",en_PROTO_ID_QQ,4,65535,47873,47873,{0x02},1,2,{0x08,0x25},2,5,0,1,0},
   {TCP_PACKET,10,"qq",en_PROTO_ID_QQ,4,65535,0,0,{0x08,0x01},2,7,{0x00},0,0,0,0,0},
   {TCP_PACKET,10,"qq",en_PROTO_ID_QQ,4,65535,0,0,{0x08,0x02},2,7,{0x00},0,0,0,0,0},
   {TCP_PACKET,10,"qq",en_PROTO_ID_QQ,4,65535,0,0,{0x09,0x01},2,7,{0x00},0,0,0,0,0},
   {TCP_PACKET,10,"qq",en_PROTO_ID_QQ,4,65535,0,0,{0x03,0x01},2,7,{0x00},0,0,0,0,0},
   {TCP_PACKET,10,"qq",en_PROTO_ID_QQ,4,65535,0,0,{0x08,0x00},2,7,{0x00},0,0,0,0,0},
   {TCP_PACKET,10,"qq",en_PROTO_ID_QQ,4,65535,0,0,{0x0b,0x01},2,7,{0x00},0,0,0,0,0},
//   {TCP_PACKET,10,"qq",en_PROTO_ID_QQ,4,65535,0,0,{0x00,0x00,0x00,0xc3,0x00,0x00,0x00,0x08,0x01},9,0,{0x00},0,0,0,1,0},
//   {TCP_PACKET,10,"qq",en_PROTO_ID_QQ,4,65535,0,0,{0x00,0x00,0x00,0xc4,0x00,0x00,0x00,0x08,0x01},9,0,{0x00},0,0,0,1,0},
//   {TCP_PACKET,10,"qq",en_PROTO_ID_QQ,4,65535,0,0,{0x00,0x00,0x01,0xdb,0x00,0x00,0x00,0x08,0x01},9,0,{0x00},0,0,0,1,0},
//   {TCP_PACKET,10,"qq",en_PROTO_ID_QQ,4,65535,0,0,{0x00,0x00,0x01,0xdc,0x00,0x00,0x00,0x08,0x01},9,0,{0x00},0,0,0,1,0},
//   {TCP_PACKET,10,"qq",en_PROTO_ID_QQ,4,65535,0,0,{0x00,0x00,0x01,0x5b,0x00,0x00,0x00,0x08,0x02},9,0,{0x00},0,0,0,1,0},
//   {TCP_PACKET,10,"qq",en_PROTO_ID_QQ,4,65535,0,0,{0x00,0x00,0x03,0x0c,0x00,0x00,0x00,0x08,0x01},9,0,{0x00},0,0,0,1,0},
//   {TCP_PACKET,10,"qq",en_PROTO_ID_QQ,4,65535,0,0,{0x00,0x00,0x00,0xab,0x00,0x00,0x00,0x09,0x01},9,0,{0x00},0,0,0,1,0},
//   {TCP_PACKET,10,"qq",en_PROTO_ID_QQ,4,65535,0,0,{0x00,0x00,0x00,0x9d,0x00,0x00,0x00,0x08,0x00},9,0,{0x00},0,0,0,1,0},
//   {TCP_PACKET,10,"qq",en_PROTO_ID_QQ,4,65535,0,0,{0x00,0x00,0x01,0x6c,0x00,0x00,0x00,0x09,0x01},9,0,{0x00},0,0,0,1,0},
//   {TCP_PACKET,10,"qq",en_PROTO_ID_QQ,4,65535,0,0,{0x00,0x00,0x00,0xac,0x00,0x00,0x00,0x09,0x01},9,0,{0x00},0,0,0,1,0},
//   {TCP_PACKET,10,"qq",en_PROTO_ID_QQ,4,65535,0,0,{0x00,0x00,0x03,0x2c,0x00,0x00,0x00,0x09,0x01},9,0,{0x00},0,0,0,1,0},
//   {TCP_PACKET,10,"qq",en_PROTO_ID_QQ,4,65535,0,0,{0x00,0x00,0x01,0x6c,0x00,0x00,0x00,0x0b,0x01},9,0,{0x00},0,0,0,1,0},
//   {TCP_PACKET,10,"qq",en_PROTO_ID_QQ,4,65535,0,0,{0x00,0x00,0x01,0x14,0x00,0x00,0x00,0x08,0x02},9,0,{0x00},0,0,0,1,0},
//   {TCP_PACKET,1,"baidusearch",en_PROTO_ID_BAIDUSEARCH,4,65535,20480,20480,{"GET /sug/rich?wd="},sizeof("GET /sug/rich?wd=")-1,0,{0x00},0,0,0,7,0},//ycc
   {TCP_PACKET,1,"baidusearch",en_PROTO_ID_BAIDUSEARCH,4,65535,20480,20480,{"GET /s?word="},sizeof("GET /s?word=")-1,0,{0x00},0,0,0,7,0},
   {TCP_PACKET,1,"baidusearch",en_PROTO_ID_BAIDUSEARCH,4,65535,20480,20480,{"GET /from="},sizeof("GET /from=")-1,0,{0x00},0,0,0,7,0},
//   {TCP_PACKET,1,"baidutieba",en_PROTO_ID_BAIDUTIEBA,4,65535,20480,20480,{"GET /tb/static-common/html/pass/v3Jump.html?err_no=0"},sizeof("GET /tb/static-common/html/pass/v3Jump.html?err_no=0")-1,0,{0x00},0,0,0,1,0},
//   {TCP_PACKET,1,"baidutieba",en_PROTO_ID_BAIDUTIEBA,4,65535,20480,20480,{"GET /f/user/json_userinfo?"},sizeof("GET /f/user/json_userinfo?")-1,0,{0x00},0,0,0,1,0},
//   {TCP_PACKET,1,"baidutieba",en_PROTO_ID_BAIDUTIEBA,4,65535,20480,20480,{"POST /c/s/login"},sizeof("POST /c/s/login")-1,0,{0x00},0,0,0,1,0},//ycc
//   {TCP_PACKET,1,"baidutieba",en_PROTO_ID_BAIDUTIEBA,4,65535,20480,20480,{"POST /c/f/forum/search"},sizeof("POST /c/f/forum/search")-1,0,{0x00},0,0,0,7,0},//ycc
//   {TCP_PACKET,1,"baidutieba",en_PROTO_ID_BAIDUTIEBA,4,65535,20480,20480,{"POST /c/s/pv"},sizeof("POST /c/s/pv")-1,0,{0x00},0,0,0,1,0},//ycc
//   {TCP_PACKET,1,"baidutieba",en_PROTO_ID_BAIDUTIEBA,4,65535,20480,20480,{"POST /c/s/pluginsmisconf"},sizeof("POST /c/s/pluginsmisconf")-1,0,{0x00},0,0,0,1,0},//ycc
   {TCP_PACKET,1,"baidutieba",en_PROTO_ID_BAIDUTIEBA,4,65535,20480,20480,{"POST /c/s/sync"},sizeof("POST /c/s/sync")-1,0,{0x00},0,0,0,1,0},
   {TCP_PACKET,1,"baidutieba",en_PROTO_ID_BAIDUTIEBA,4,65535,20480,20480,{"POST /c/c/post/add"},sizeof("POST /c/c/post/add")-1,0,{0x00},0,0,0,5,1},
   {TCP_PACKET,1,"baidutieba",en_PROTO_ID_BAIDUTIEBA,4,65535,20480,20480,{"POST /c/c/thread/add"},sizeof("POST /c/c/thread/add")-1,0,{0x00},0,0,0,5,1},
   {TCP_PACKET,1,"baiduapp",en_PROTO_ID_BAIDUAPP,4,65535,20480,20480,{"POST /searchbox?action=u"},sizeof("POST /searchbox?action=u")-1,0,{0x00},0,0,0,0,0},
   {TCP_PACKET,1,"baiduapp",en_PROTO_ID_BAIDUAPP,4,65535,20480,20480,{"GET /s?tn=zbios&pu="},sizeof("GET /s?tn=zbios&pu=")-1,0,{0x00},0,0,0,1,0},
   {TCP_PACKET,1,"taobao",en_PROTO_ID_TAOBAO,4,65535,20480,20480,{"GET /h5/com.taobao.mcl.fav.checkcollect"},sizeof("GET /h5/com.taobao.mcl.fav.checkcollect")-1,0,{0x00},0,0,0,1,0},
   {TCP_PACKET,1,"taobao",en_PROTO_ID_TAOBAO,4,65535,20480,20480,{"GET /status.taobao"},sizeof("GET /status.taobao")-1,0,{0x00},0,0,0,1,0},
   {TCP_PACKET,1,"taobao",en_PROTO_ID_TAOBAO,4,65535,20480,20480,{"GET /search.json?vm=nw"},sizeof("GET /search.json?vm=nw")-1,0,{0x00},0,0,0,1,0},
   {TCP_PACKET,1,"taobao",en_PROTO_ID_TAOBAO,4,65535,20480,20480,{"POST /api/v2/account/device_token.json"},sizeof("GET /api/v2/account/device_token.json")-1,0,{0x00},0,0,0,1,0},
   {TCP_PACKET,1,"taobao",en_PROTO_ID_TAOBAO,4,65535,0,0,{"GET /imlogingw/tcp60login"},sizeof("GET /imlogingw/tcp60login")-1,0,{0x00},0,0,0,1,0},
   {TCP_PACKET,1,"taobao",en_PROTO_ID_TAOBAO,4,65535,20480,20480,{"GET /gw/mtop.taobao.wsearch.appsearch"},sizeof("GET /gw/mtop.taobao.wsearch.appsearch")-1,0,{0x00},0,0,0,7,0},
 //  {TCP_PACKET,1,"taobao",en_PROTO_ID_TAOBAO,4,65535,20480,20480,{"POST /api/v2/account/device_token.json"},sizeof("POST /api/v2/account/device_token.json")-1,0,{0x00},0,0,0,1,0},
   {TCP_PACKET,1,"meituantuangou",en_PROTO_ID_MEITUANTUANGOU,4,65535,20480,20480,{"GET /group/v1/user/"},sizeof("GET /group/v1/user/")-1,0,{0x00},0,0,0,1,1},//ycc
   {TCP_PACKET,1,"meituantuangou",en_PROTO_ID_MEITUANTUANGOU,4,65535,20480,20480,{"GET /group/v1/recommend/homepage/city"},sizeof("GET /group/v1/recommend/homepage/city")-1,0,{0x00},0,0,0,1,0},
   {TCP_PACKET,1,"meituantuangou",en_PROTO_ID_MEITUANTUANGOU,4,65535,20480,20480,{"GET /group/v4/deal/search/count"},sizeof("GET /group/v4/deal/search/count")-1,0,{0x00},0,0,0,7,0},
//   {TCP_PACKET,1,"meituantuangou",en_PROTO_ID_MEITUANTUANGOU,4,65535,20480,20480,{"GET /group/v3/poi/search/count"},sizeof("GET /group/v3/poi/search/count")-1,0,{0x00},0,0,0,7,0},
//   {TCP_PACKET,1,"meituantuangou",en_PROTO_ID_MEITUANTUANGOU,4,65535,20480,20480,{"GET /group/v3/poi/search/"},sizeof("GET /group/v3/poi/search/")-1,0,{0x00},0,0,0,7,0},//ycc
   {TCP_PACKET,1,"jd",en_PROTO_ID_JD,4,65535,20480,20480,{"POST /client.action?functionId=welcomeHome"},sizeof("POST /client.action?functionId=welcomeHome")-1,0,{0x00},0,0,0,1,0},
   {TCP_PACKET,1,"jd",en_PROTO_ID_JD,4,65535,20480,20480,{"POST /client.action?functionId=search "},sizeof("POST /client.action?functionId=search ")-1,0,{0x00},0,0,0,7,1},
   {TCP_PACKET,1,"jd",en_PROTO_ID_JD,4,65535,20480,20480,{"POST /client.action?functionId=search&"},sizeof("POST /client.action?functionId=search&")-1,0,{0x00},0,0,0,7,0},
   {TCP_PACKET,1,"jd",en_PROTO_ID_JD,4,65535,20480,20480,{"POST /client.action?functionId="},sizeof("POST /client.action?functionId=")-1,0,{0x00},0,0,0,0,0},//ycc
   {TCP_PACKET,1,"dianping",en_PROTO_ID_DIANPING,4,65535,20480,20480,{"POST /mapi/applog/applog.api HTTP/1.1"},sizeof("POST /mapi/applog/applog.api HTTP/1.1")-1,0,{0x00},0,0,0,1,0},//ycc
   {TCP_PACKET,1,"dianping",en_PROTO_ID_DIANPING,4,65535,20480,20480,{"GET /config/v1/keyvalue.json?appname=dianping-nova"},sizeof("GET /config/v1/keyvalue.json?appname=dianping-nova")-1,0,{0x00},0,0,0,1,1},//ycc
   {TCP_PACKET,1,"dianping",en_PROTO_ID_DIANPING,4,65535,20480,20480,{"POST /utm.js?v="},sizeof("POST /utm.js?v=")-1,0,{0x00},0,0,0,0,0},
   {TCP_PACKET,1,"dianping",en_PROTO_ID_DIANPING,4,65535,20480,20480,{"GET /mapi/searchshop.api"},sizeof("GET /mapi/searchshop.api")-1,0,{0x00},0,0,0,7,0},
   {TCP_PACKET,1,"dianping",en_PROTO_ID_DIANPING,4,65535,20480,20480,{"GET /tuan/searchdealgn.bin"},sizeof("GET /tuan/searchdealgn.bin")-1,0,{0x00},0,0,0,7,0},
   {TCP_PACKET,1,"bdnuomi",en_PROTO_ID_BDNUOMI,4,65535,20480,20480,{"GET /naserver/user/cartitemnum"},sizeof("GET /naserver/user/cartitemnum")-1,0,{0x00},0,0,0,1,0},
   {TCP_PACKET,1,"bdnuomi",en_PROTO_ID_BDNUOMI,4,65535,20480,20480,{"GET /naserver/common/gpstogeo"},sizeof("GET /naserver/common/gpstogeo")-1,0,{0x00},0,0,0,1,0},
   {TCP_PACKET,1,"bdnuomi",en_PROTO_ID_BDNUOMI,4,65535,20480,20480,{"POST /naserver/home/poiscene HTTP/1.1"},sizeof("POST /naserver/home/poiscene HTTP/1.1")-1,0,{0x00},0,0,0,1,0},
   {TCP_PACKET,1,"bdnuomi",en_PROTO_ID_BDNUOMI,4,65535,20480,20480,{"GET /naserver/search/searchsugword?"},sizeof("GET /naserver/search/searchsugword?")-1,0,{0x00},0,0,0,7,0},
   {TCP_PACKET,1,"bdnuomi",en_PROTO_ID_BDNUOMI,4,65535,20480,20480,{"GET /naserver/search/searchprophet"},sizeof("GET /naserver/search/searchprophet")-1,0,{0x00},0,0,0,7,0},//ycc
   {TCP_PACKET,1,"bdnuomi",en_PROTO_ID_BDNUOMI,4,65535,20480,20480,{"GET /naserver/user/redpoint"},sizeof("GET /naserver/user/redpoint")-1,0,{0x00},0,0,0,1,0},//ycc
   {TCP_PACKET,1,"bdnuomi",en_PROTO_ID_BDNUOMI,4,65535,20480,20480,{"GET /naserver/search/searchitem"},sizeof("GET /naserver/search/searchitem")-1,0,{0x00},0,0,0,7,0},//ycc
   {TCP_PACKET,1,"meituanwaimai",en_PROTO_ID_MEITUANWAIMAI,4,65535,20480,20480,{"POST /api/v6/app/userinfo"},sizeof("POST /api/v6/app/userinfo")-1,0,{0x00},0,0,0,1,0},
//   {TCP_PACKET,1,"meituanwaimai",en_PROTO_ID_MEITUANWAIMAI,4,65535,20480,20480,{"POST /api/v6/data/binduser"},sizeof("POST /api/v6/data/binduser")-1,0,{0x00},0,0,0,1,0},
//   {TCP_PACKET,1,"meituanwaimai",en_PROTO_ID_MEITUANWAIMAI,4,65535,20480,20480,{"POST /api/v7/poi/search/poi"},sizeof("POST /api/v7/poi/search/poi")-1,0,{0x00},0,0,0,7,1},
   {TCP_PACKET,1,"meituanwaimai",en_PROTO_ID_MEITUANWAIMAI,4,65535,20480,20480,{"POST /api/v7/poi/search/home"},sizeof("POST /api/v7/poi/search/home")-1,0,{0x00},0,0,0,1,1},
   {TCP_PACKET,1,"meituanwaimai",en_PROTO_ID_MEITUANWAIMAI,4,65535,20480,20480,{"POST /api/v6/user/"},sizeof("POST /api/v6/user/coupons/number")-1,0,{0x00},0,0,0,1,1},//ycc
   {TCP_PACKET,1,"meituanwaimai",en_PROTO_ID_MEITUANWAIMAI,4,65535,20480,20480,{"POST /api/v6/checkstatus"},sizeof("POST /api/v6/checkstatus")-1,0,{0x00},0,0,0,1,1},//ycc
   {TCP_PACKET,1,"weibo",en_PROTO_ID_WEIBO,4,65535,20480,20480,{"POST /wbapplua/wbpullad.lua"},sizeof("POST /wbapplua/wbpullad.lua")-1,0,{0x00},0,0,0,1,0},
   {TCP_PACKET,1,"weibo",en_PROTO_ID_WEIBO,4,65535,20480,20480,{"GET /entrance"},sizeof("GET /entrance")-1,0,{0x00},0,0,0,1,0},
   {TCP_PACKET,1,"weibo",en_PROTO_ID_WEIBO,4,65535,20480,20480,{"POST /interface/sdk/sdkad.php"},sizeof("POST /interface/sdk/sdkad.php")-1,0,{0x00},0,0,0,1,0},
   {TCP_PACKET,1,"weibo",en_PROTO_ID_WEIBO,4,65535,20480,20480,{"GET /interface/f/ttt/v3/wbpullad.php"},sizeof("GET /interface/f/ttt/v3/wbpullad.php")-1,0,{0x00},0,0,0,1,0},
   {TCP_PACKET,1,"weibo",en_PROTO_ID_WEIBO,4,65535,20480,20480,{"POST /interface/f/ttt/v3/wbpullad.php"},sizeof("POST /interface/f/ttt/v3/wbpullad.php")-1,0,{0x00},0,0,0,1,0},
//   {TCP_PACKET,1,"58tongcheng",en_PROTO_ID_58TONGCHENG,4,65535,20480,20480,{"GET /api/windex/ucenter/items"},sizeof("GET /api/windex/ucenter/items")-1,0,{0x00},0,0,0,1,0},
//   {TCP_PACKET,1,"58tongcheng",en_PROTO_ID_58TONGCHENG,4,65535,20480,20480,{"POST /api/log/api/sysmsg/"},sizeof("POST /api/log/api/sysmsg/")-1,0,{0x00},0,0,0,1,0},//ycc
   {TCP_PACKET,1,"58tongcheng",en_PROTO_ID_58TONGCHENG,4,65535,20480,20480,{"GET /api/list/shouji/?action=getListInfo&"},sizeof("GET /api/list/shouji/?action=getListInfo&")-1,0,{0x00},0,0,0,7,1},
//   {TCP_PACKET,1,"58tongcheng",en_PROTO_ID_58TONGCHENG,4,65535,20480,20480,{"GET /api/log/api/advertising/latest/"},sizeof("GET /api/log/api/advertising/latest/")-1,0,{0x00},0,0,0,1,0},//ycc
//   {TCP_PACKET,1,"58tongcheng",en_PROTO_ID_58TONGCHENG,4,65535,20480,20480,{"POST /api/windex/mobileapi/checkbind HTTP/1.1"},sizeof("POST /api/windex/mobileapi/checkbind HTTP/1.1")-1,0,{0x00},0,0,0,1,0},//ycc
   {TCP_PACKET,1,"58tongcheng",en_PROTO_ID_58TONGCHENG,4,65535,20480,20480,{"GET /api/list/sou?action=getListInfo"},sizeof("GET /api/list/sou?action=getListInfo")-1,0,{0x00},0,0,0,7,0},//ycc
   {TCP_PACKET,1,"58tongcheng",en_PROTO_ID_58TONGCHENG,4,65535,20480,20480,{"GET /api/windex/api/indexnew"},sizeof("GET /api/windex/api/indexnew")-1,0,{0x00},0,0,0,1,0},
   {TCP_PACKET,1,"58tongcheng",en_PROTO_ID_58TONGCHENG,4,65535,20480,20480,{"POST /api/windex/mobileapi/checkbind"},sizeof("POST /api/windex/mobileapi/checkbind")-1,0,{0x00},0,0,0,1,0},
   {TCP_PACKET,1,"58tongcheng",en_PROTO_ID_58TONGCHENG,4,65535,20480,20480,{"GET /api/windex/mobileapi/getbind"},sizeof("GET /api/windex/mobileapi/getbind")-1,0,{0x00},0,0,0,1,0},
   {TCP_PACKET,1,"58tongcheng",en_PROTO_ID_58TONGCHENG,4,65535,20480,20480,{"GET /api/list/sou"},sizeof("GET /api/list/sou")-1,0,{0x00},0,0,0,7,0},
   {TCP_PACKET,1,"dididache",en_PROTO_ID_DIDIDACHE,4,65535,20480,20480,{"GET /gulfstream/api/v1/passenger"},sizeof("GET /gulfstream/api/v1/passenger")-1,0,{0x00},0,0,0,1,0},
   {TCP_PACKET,1,"dididache",en_PROTO_ID_DIDIDACHE,4,65535,20480,20480,{"GET /passenger/getprofile"},sizeof("GET /passenger/getprofile")-1,0,{0x00},0,0,0,1,0},//ycc
   {TCP_PACKET,1,"360safe",en_PROTO_ID_360SAFE,4,65535,20480,20480,{"POST /v4/splashinfo.php"},sizeof("POST /v4/splashinfo.php")-1,0,{0x00},0,0,0,1,0},
   {TCP_PACKET,1,"ctrip",en_PROTO_ID_CTRIP,4,65535,0,0,{"GET /api/archive/muc/"},sizeof("GET /api/archive/muc/")-1,0,{0x00},0,0,0,1,0},
   {TCP_PACKET,1,"ctrip",en_PROTO_ID_CTRIP,4,65535,20480,20480,{"POST /restapi/soa2/10612/getmessagecount.json"},sizeof("POST /restapi/soa2/10612/getmessagecount.json")-1,0,{0x00},0,0,0,1,0},
//   {TCP_PACKET,1,"tianyashequ",en_PROTO_ID_TIANYASHEQU,4,65535,20480,20480,{"GET /display?f=index&g=pageindex"},sizeof("GET /display?f=index&g=pageindex")-1,0,{0x00},0,0,0,1,0},
//   {TCP_PACKET,1,"maopu",en_PROTO_ID_MAOPU,4,65535,20480,20480,{"GET /ajax/login?callback"},sizeof("GET /ajax/login?callback")-1,0,{0x00},0,0,0,1,0},
//   {TCP_PACKET,1,"wangyi",en_PROTO_ID_WANGYI,4,65535,20480,20480,{"GET /crossdomain.jsp?username="},sizeof("GET /crossdomain.jsp?username=")-1,0,{0x00},0,0,0,1,0},
   {TCP_PACKET,1,"163webmail",en_PROTO_ID_163WEBMAIL,4,65535,20480,20480,{"POST /goldenstate/goldenuser/getClubinfo.m"},sizeof("POST /goldenstate/goldenuser/getClubinfo.m")-1,0,{0x00},0,0,0,1,0},
   {TCP_PACKET,1,"163webmail",en_PROTO_ID_163WEBMAIL,4,65535,20480,20480,{"GET /goldenstate/goldenuser/getClubinfo.m"},sizeof("GET /goldenstate/goldenuser/getClubinfo.m")-1,0,{0x00},0,0,0,1,0},
   {TCP_PACKET,1,"163webmail",en_PROTO_ID_163WEBMAIL,4,65535,20480,20480,{"GET /apptrack/confinfo/searchBindbyappid"},sizeof("GET /apptrack/confinfo/searchBindbyappid")-1,0,{0x00},0,0,0,1,0},
	{TCP_PACKET,1,"163webmail",en_PROTO_ID_163WEBMAIL,4,65535,20480,20480,{"var=%3C%3Fxml%20version%3D"},sizeof("var=%3C%3Fxml%20version%3D")-1,0,{0x00},0,0,0,5,0},
//   {TCP_PACKET,1,"qqnews",en_PROTO_ID_QQNEWS,4,65535,20480,20480,{"POST /userCenterConfig?devid="},sizeof("POST /userCenterConfig?devid=")-1,0,{0x00},0,0,0,1,0},
//   {TCP_PACKET,1,"qqnews",en_PROTO_ID_QQNEWS,4,65535,20480,20480,{"GET /userCenterConfig?uid="},sizeof("GET /userCenterConfig?uid=")-1,0,{0x00},0,0,0,1,0},
//   {TCP_PACKET,1,"qqnews",en_PROTO_ID_QQNEWS,4,65535,20480,20480,{"GET /getAndSetUserSubList?"},sizeof("GET /getAndSetUserSubList?")-1,0,{0x00},0,0,0,1,0},//ycc
   {TCP_PACKET,1,"qqnews",en_PROTO_ID_QQNEWS,4,65535,20480,20480,{"GET /getQQNewsRemoteConfig?"},sizeof("GET /getQQNewsRemoteConfig?")-1,0,{0x00},0,0,0,1,1},//ycc
   {TCP_PACKET,1,"qqnews",en_PROTO_ID_QQNEWS,4,65535,20480,20480,{"POST /getUserInfoPhone?omgid="},sizeof("POST /getUserInfoPhone?omgid=")-1,0,{0x00},0,0,0,1,1},//ycc
 //  {TCP_PACKET,1,"qqzone",en_PROTO_ID_QQZONE,4,65535,20480,20480,{"GET /cgi-bin/qconn_share/check_app_limit.cgi"},sizeof("GET /cgi-bin/qconn_share/check_app_limit.cgi")-1,0,{0x00},0,0,0,1,0},
   {TCP_PACKET,1,"qqzone",en_PROTO_ID_QQZONE,4,65535,20480,20480,{"GET /offline/check?"},sizeof("GET /offline/check?")-1,0,{0x00},0,0,0,1,0},//ycc
//   {TCP_PACKET,1,"tecentvideo",en_PROTO_ID_TECENTVIDEO,4,65535,20480,20480,{"GET /commdatav2?platform="},sizeof("GET /commdatav2?platform=")-1,0,{0x00},0,0,0,1,0},//ycc
//   {TCP_PACKET,1,"tecentvideo",en_PROTO_ID_TECENTVIDEO,4,65535,20480,20480,{"GET /commdatav2?platform="},sizeof("GET /commdatav2?platform=")-1,0,{0x00},0,0,0,1,0},//ycc
//   {TCP_PACKET,1,"tecentvideo",en_PROTO_ID_TECENTVIDEO,4,65535,20480,20480,{"GET /getmfomat?model="},sizeof("GET /getmfomat?model=")-1,0,{0x00},0,0,0,1,0},//ycc
   {TCP_PACKET,1,"tecentvideo",en_PROTO_ID_TECENTVIDEO,4,65535,20480,20480,{"GET /fcgi-bin/payvip?otype=json&platform"},sizeof("GET /fcgi-bin/payvip?otype=json&platform")-1,0,{0x00},0,0,0,1,0},
   {TCP_PACKET,1,"tecentvideo",en_PROTO_ID_TECENTVIDEO,4,65535,20480,20480,{"POST /fcgi-bin/like?uin="},sizeof("POST /fcgi-bin/like?uin=")-1,0,{0x00},0,0,0,1,0},
   {TCP_PACKET,1,"tecentvideo",en_PROTO_ID_TECENTVIDEO,4,65535,20480,20480,{"GET /fcgi-bin/dlib/uptoken?uin="},sizeof("GET /fcgi-bin/dlib/uptoken?uin=")-1,0,{0x00},0,0,0,1,0},
   {TCP_PACKET,1,"tecentvideo",en_PROTO_ID_TECENTVIDEO,4,65535,20480,20480,{"GET /srh_iphone?query"},sizeof("GET /srh_iphone?query")-1,0,{0x00},0,0,0,7,0},
   {TCP_PACKET,1,"tecentvideo",en_PROTO_ID_TECENTVIDEO,4,65535,20480,20480,{"GET /tvideo/fcgi-bin/srh_aphone?"},sizeof("GET /tvideo/fcgi-bin/srh_aphone?")-1,0,{0x00},0,0,0,7,0},
   {TCP_PACKET,1,"zhifubao",en_PROTO_ID_ZHIFUBAO,4,65535,20480,20480,{"GET /loggw/config.do"},sizeof("GET /loggw/config.do")-1,0,{0x00},0,0,0,1,0},
   {TCP_PACKET,1,"zhilian",en_PROTO_ID_ZHILIAN,4,65535,20480,20480,{"GET /jobs/searchresult.ashx"},sizeof("GET /jobs/searchresult.ashx")-1,0,{0x00},0,0,0,7,0},
   {TCP_PACKET,1,"zhilian",en_PROTO_ID_ZHILIAN,4,65535,20480,20480,{"GET /searchjob/search?"},sizeof("GET /searchjob/search?")-1,0,{0x00},0,0,0,7,0},
   {TCP_PACKET,1,"leipin",en_PROTO_ID_LEIPIN,4,65535,20480,20480,{"GET /zhaopin/?searchField="},sizeof("GET /zhaopin/?searchField=")-1,0,{0x00},0,0,0,7,0},
   {TCP_PACKET,1,"leipin",en_PROTO_ID_LEIPIN,4,65535,20480,20480,{"GET /job/search/?"},sizeof("GET /job/search/?")-1,0,{0x00},0,0,0,7,0},
   {TCP_PACKET,1,"kugou",en_PROTO_ID_KUGOU,4,65535,20480,20480,{"GET /new/app/i/singer.php"},sizeof("GET /new/app/i/singer.php")-1,0,{0x00},0,0,0,1,0},
   {TCP_PACKET,1,"kugou",en_PROTO_ID_KUGOU,4,65535,20480,20480,{"GET /Mobile/MobileStat.aspx"},sizeof("GET /Mobile/MobileStat.aspx")-1,0,{0x00},0,0,0,0,0},
   {TCP_PACKET,1,"kugou",en_PROTO_ID_KUGOU,4,65535,20480,20480,{"GET /api/v3/search/song"},sizeof("GET /api/v3/search/song")-1,0,{0x00},0,0,0,7,0},
   {TCP_PACKET,1,"qqmail",en_PROTO_ID_QQMAIL,4,65535,20480,20480,{"POST /cgi-bin/uma_login"},sizeof("POST /cgi-bin/uma_login")-1,0,{0x00},0,0,0,1,0},
   {TCP_PACKET,1,"qqmail",en_PROTO_ID_QQMAIL,4,65535,20480,20480,{"POST /cgi-bin/ftnlogin"},sizeof("POST /cgi-bin/ftnlogin")-1,0,{0x00},0,0,0,1,0},//ycc
//   {TCP_PACKET,1,"qqmail",en_PROTO_ID_QQMAIL,4,65535,20480,20480,{"POST /cgi-bin/uma_email_nickname_sync HTTP/1.1"},sizeof("POST /cgi-bin/uma_email_nickname_sync HTTP/1.1")-1,0,{0x00},0,0,0,1,0},//ycc
//   {TCP_PACKET,1,"12306",en_PROTO_ID_12306,4,65535,20480,20480,{"POST /session?appkey="},sizeof("POST /session?appkey=")-1,0,{0x00},0,0,0,1,0},//ycc
   {TCP_PACKET,1,"12306",en_PROTO_ID_12306,4,65535,20480,20480,{"POST /a/ HTTP/1.1"},sizeof("POST /a/ HTTP/1.1")-1,0,{0x00},0,0,0,1,0},
   {TCP_PACKET,1,"weipin",en_PROTO_ID_WEIPIN,4,65535,20480,20480,{"GET /vips-mobile-tracker/router.do"},sizeof("GET /vips-mobile-tracker/router.do")-1,0,{0x00},0,0,0,0,0},
//   {TCP_PACKET,1,"weipin",en_PROTO_ID_WEIPIN,4,65535,20480,20480,{"GET /apns/device_reg?"},sizeof("GET /apns/device_reg?")-1,0,{0x00},0,0,0,0,0},//ycc
//   {TCP_PACKET,1,"YY",en_PROTO_ID_YY,4,65535,20480,20480,{"GET /switch/info"},sizeof("GET /switch/info")-1,0,{0x00},0,0,0,1,0},//ycc
//   {TCP_PACKET,1,"YY",en_PROTO_ID_YY,4,65535,20480,20480,{"GET /discover/v2/redStar"},sizeof("GET /discover/v2/redStar")-1,0,{0x00},0,0,0,1,0},//ycc
   {TCP_PACKET,1,"YY",en_PROTO_ID_YY,4,65535,20480,20480,{"GET /index/v4/infoList"},sizeof("GET /index/v4/infoList")-1,0,{0x00},0,0,0,1,0},//ycc
   {TCP_PACKET,1,"YY",en_PROTO_ID_YY,4,65535,20480,20480,{"GET /splash/v2/info?"},sizeof("GET /splash/v2/info?")-1,0,{0x00},0,0,0,1,0},
   {TCP_PACKET,1,"qunar",en_PROTO_ID_QUNAR,4,65535,20480,20480,{"GET /js.ng/chan=travel&pg=home&pos=top&site=app.qunar"},sizeof("GET /js.ng/chan=travel&pg=home&pos=top&site=app.qunar")-1,0,{0x00},0,0,0,1,0},
   {TCP_PACKET,1,"qunar",en_PROTO_ID_QUNAR,4,65535,20480,20480,{"POST /ca?qrt=p_login"},sizeof("POST /ca?qrt=p_login")-1,0,{0x00},0,0,0,1,0},//ycc
//   {TCP_PACKET,1,"qunar",en_PROTO_ID_QUNAR,4,65535,20480,20480,{"POST /intention/productlist.htm"},sizeof("POST /intention/productlist.htm")-1,0,{0x00},0,0,0,7,1},//ycc
   {TCP_PACKET,1,"qunar",en_PROTO_ID_QUNAR,4,65535,20480,20480,{"GET /v1/client.qunar.com/a HTTP/1.1"},sizeof("GET /v1/client.qunar.com/a HTTP/1.1")-1,0,{0x00},0,0,0,1,0},//ycc
   {TCP_PACKET,1,"vdian",en_PROTO_ID_VDIAN,4,65535,20480,20480,{"POST /getBasicUserInfo.do"},sizeof("POST /getBasicUserInfo.do")-1,0,{0x00},0,0,0,1,0},
   {TCP_PACKET,1,"vdian",en_PROTO_ID_VDIAN,4,65535,20480,20480,{"POST /loginForPhoneNumber.do"},sizeof("POST /loginForPhoneNumber.do")-1,0,{0x00},0,0,0,1,0},
   {TCP_PACKET,1,"dazhihui",en_PROTO_ID_DAZHIHUI,4,65535,20480,20480,{"GET /wap/data/mobileconfig/navigation/navigation_config.json"},sizeof("GET /wap/data/mobileconfig/navigation/navigation_config.json")-1,0,{0x00},0,0,0,1,0},
   {TCP_PACKET,1,"xunlei",en_PROTO_ID_XUNLEI,4,65535,20480,20480,{"GET /ios_page/iphone/search/"},sizeof("GET /ios_page/iphone/search/")-1,0,{0x00},0,0,0,7,0},//ycc
//   {TCP_PACKET,1,"xunlei",en_PROTO_ID_XUNLEI,4,65535,20480,20480,{"GET /cgi-bin/popularsite"},sizeof("GET /cgi-bin/popularsite")-1,0,{0x00},0,0,0,1,0},//ycc
   {TCP_PACKET,1,"xunlei",en_PROTO_ID_XUNLEI,4,65535,20480,20480,{"GET /cgi-bin/homepage"},sizeof("GET /cgi-bin/homepage")-1,0,{0x00},0,0,0,1,0},
   {TCP_PACKET,1,"xunlei",en_PROTO_ID_XUNLEI,4,65535,20480,20480,{"GET /cgi-bin/friendResource"},sizeof("GET /cgi-bin/friendResource")-1,0,{0x00},0,0,0,7,0},
   {TCP_PACKET,1,"xunlei",en_PROTO_ID_XUNLEI,4,65535,20480,20480,{"GET /ios_page/iphone/search"},sizeof("GET /ios_page/iphone/search")-1,0,{0x00},0,0,0,7,0},
   {TCP_PACKET,1,"xunlei",en_PROTO_ID_XUNLEI,4,65535,20480,20480,{"GET /v2/search/result_v2.htm"},sizeof("GET /v2/search/result_v2.htm")-1,0,{0x00},0,0,0,7,0},
   {TCP_PACKET,1,"360yunpan",en_PROTO_ID_360YUNPAN,4,65535,20480,20480,{"GET /intf.php?method=User.getUserDetail"},sizeof("GET /intf.php?method=User.getUserDetail")-1,0,{0x00},0,0,0,1,0},//ycc
   {TCP_PACKET,1,"360yunpan",en_PROTO_ID_360YUNPAN,4,65535,20480,20480,{"GET /?o=User"},sizeof("GET /?o=User")-1,0,{0x00},0,0,0,1,0},//ycc
   {TCP_PACKET,1,"360yunpan",en_PROTO_ID_360YUNPAN,4,65535,20480,20480,{"GET /?devid="},sizeof("GET /?devid=")-1,0,{0x00},0,0,0,1,0},
   {TCP_PACKET,1,"jinritoutiao",en_PROTO_ID_JINRITOUTIAO,4,65535,20480,20480,{"POST /2/article/extensions"},sizeof("POST /2/article/extensions")-1,0,{0x00},0,0,0,1,0},
   {TCP_PACKET,1,"jinritoutiao",en_PROTO_ID_JINRITOUTIAO,4,65535,20480,20480,{"GET /dongtai/notification/count/"},sizeof("GET /dongtai/notification/count/")-1,0,{0x00},0,0,0,1,0},//ycc
   {TCP_PACKET,1,"baidumap",en_PROTO_ID_BAIDUMAP,4,65535,20480,20480,{"GET /imap/dl/s/UpdateInfo.php?"},sizeof("GET /imap/dl/s/UpdateInfo.php?")-1,0,{0x00},0,0,0,1,0},
   {TCP_PACKET,1,"gaodemap",en_PROTO_ID_GAODEMAP,4,65535,20480,20480,{"POST /usertrack_com_autonavi_amap/get_aid"},sizeof("POST /usertrack_com_autonavi_amap/get_aid")-1,0,{0x00},0,0,0,1,0},
   {TCP_PACKET,1,"gaodemap",en_PROTO_ID_GAODEMAP,4,65535,20480,20480,{"GET /ws/mapapi/appinit?"},sizeof("GET /ws/mapapi/appinit?")-1,0,{0x00},0,0,0,1,0},
   {TCP_PACKET,1,"didapinche",en_PROTO_ID_DIDAPINCHE,4,65535,20480,20480,{"POST /V3/User/getStartupPageUrl HTTP/1.1"},sizeof("POST /V3/User/getStartupPageUrl HTTP/1.1")-1,0,{0x00},0,0,0,1,0},
   {TCP_PACKET,1,"didapinche",en_PROTO_ID_DIDAPINCHE,4,65535,20480,20480,{"GET /dida-web/jifenmall/adEvents"},sizeof("GET /dida-web/jifenmall/adEvents")-1,0,{0x00},0,0,0,1,0},
   {TCP_PACKET,1,"didapinche",en_PROTO_ID_DIDAPINCHE,4,65535,20480,20480,{"POST /V3/User/userInfoGet"},sizeof("POST /V3/User/userInfoGet")-1,0,{0x00},0,0,0,1,0},
//   {TCP_PACKET,1,"suningyigou",en_PROTO_ID_SUNINGYIGOU,4,65535,20480,20480,{"GET /webapp/wcs/stores/servlet/SNiPhoneAppAddressV2View?"},sizeof("GET /webapp/wcs/stores/servlet/SNiPhoneAppAddressV2View?")-1,0,{0x00},0,0,0,1,1},
//   {TCP_PACKET,1,"suningyigou",en_PROTO_ID_SUNINGYIGOU,4,65535,20480,20480,{"POST /mmcs-web/mmcs/public/addIosToken.do"},sizeof("POST /mmcs-web/mmcs/public/addIosToken.do")-1,0,{0x00},0,0,0,1,0},
   {TCP_PACKET,1,"suningyigou",en_PROTO_ID_SUNINGYIGOU,4,65535,20480,20480,{"POST /webapp/wcs/stores/servlet/SNMobileShoppingCartQuery"},sizeof("POST /webapp/wcs/stores/servlet/SNMobileShoppingCartQuery")-1,0,{0x00},0,0,0,1,0},
   {TCP_PACKET,1,"suningyigou",en_PROTO_ID_SUNINGYIGOU,4,65535,20480,20480,{"GET /search/queryRecommendedBrand.do"},sizeof("GET /search/queryRecommendedBrand.do")-1,0,{0x00},0,0,0,7,0},
   {TCP_PACKET,1,"suningyigou",en_PROTO_ID_SUNINGYIGOU,4,65535,20480,20480,{"GET /emall/mobile/mobileSearch.jsonp"},sizeof("GET /emall/mobile/mobileSearch.jsonp")-1,0,{0x00},0,0,0,7,0},
//   {TCP_PACKET,1,"aiqiyi",en_PROTO_ID_AIQIYI,4,65535,20480,20480,{"GET /api/ip2area?key="},sizeof("GET /api/ip2area?key=")-1,0,{0x00},0,0,0,1,0},//ycc
   {TCP_PACKET,1,"aiqiyi",en_PROTO_ID_AIQIYI,4,65535,20480,20480,{"GET /views_search/3.0/search"},sizeof("GET /views_search/3.0/search")-1,0,{0x00},0,0,0,7,1},//ycc
//   {TCP_PACKET,1,"aiqiyi",en_PROTO_ID_AIQIYI,4,65535,20480,20480,{"GET /api/initLogin?"},sizeof("GET /api/initLogin?")-1,0,{0x00},0,0,0,1,0},
   {TCP_PACKET,1,"aiqiyi",en_PROTO_ID_AIQIYI,4,65535,20480,20480,{"GET /proxy/2.0/searchIface"},sizeof("GET /proxy/2.0/searchIface")-1,0,{0x00},0,0,0,7,0},
   {TCP_PACKET,1,"aiqiyi",en_PROTO_ID_AIQIYI,4,65535,20480,20480,{"GET /proxy/3.0/searchIface"},sizeof("GET /proxy/3.0/searchIface")-1,0,{0x00},0,0,0,7,0},
   {TCP_PACKET,1,"youku",en_PROTO_ID_YOUKU,4,65535,20480,20480,{"GET /u/s?keyword"},sizeof("GET /u/s?keyword")-1,0,{0x00},0,0,0,7,0},//ycc
   {TCP_PACKET,1,"youku",en_PROTO_ID_YOUKU,4,65535,20480,20480,{"GET /unicom/free/flow"},sizeof("GET /unicom/free/flow")-1,0,{0x00},0,0,0,1,0},//ycc
   {TCP_PACKET,1,"youku",en_PROTO_ID_TUDOU,4,65535,20480,20480,{"GET /adv/startpage"},sizeof("GET /adv/startpage")-1,0,{0x00},0,0,0,1,0},//ycc
   {TCP_PACKET,1,"youku",en_PROTO_ID_YOUKU,4,65535,20480,20480,{"GET /layout/phone/channels/nav?brand="},sizeof("GET /layout/phone/channels/nav?brand=")-1,0,{0x00},0,0,0,1,0},
   {TCP_PACKET,1,"youku",en_PROTO_ID_YOUKU,4,65535,20480,20480,{"GET /layout/phone/user/center/card?brand="},sizeof("GET /layout/phone/user/center/card?brand=")-1,0,{0x00},0,0,0,1,0},
   {TCP_PACKET,1,"youku",en_PROTO_ID_YOUKU,4,65535,20480,20480,{"GET /layout/android/v5/home?pid="},sizeof("GET /layout/android/v5/home?pid=")-1,0,{0x00},0,0,0,1,0},
   {TCP_PACKET,1,"youku",en_PROTO_ID_YOUKU,4,65535,20480,20480,{"GET /videos/search"},sizeof("GET /videos/search")-1,0,{0x00},0,0,0,7,0},
   {TCP_PACKET,1,"qqmusic",en_PROTO_ID_QQMUSIC,4,65535,20480,20480,{"GET /gdt_mview.fcg?adposcount=1"},sizeof("GET /gdt_mview.fcg?adposcount=1")-1,0,{0x00},0,0,0,1,0},
   {TCP_PACKET,1,"qqmusic",en_PROTO_ID_QQMUSIC,4,65535,20480,20480,{"GET /listen/"},sizeof("GET /listen/")-1,0,{0x00},0,0,0,1,0},
//   {TCP_PACKET,1,"letv",en_PROTO_ID_LETV,4,65535,20480,20480,{"GET /lekan/apilanding_json.so"},sizeof("GET /lekan/apilanding_json.so")-1,0,{0x00},0,0,0,1,1},//ycc
   {TCP_PACKET,1,"letv",en_PROTO_ID_LETV,4,65535,20480,20480,{"GET /android/dynamic.php?mod=audit&ctl=audit"},sizeof("GET /android/dynamic.php?mod=audit&ctl=audit")-1,0,{0x00},0,0,0,1,0},//ycc
   {TCP_PACKET,1,"letv",en_PROTO_ID_LETV,4,65535,20480,20480,{"GET /android/mod/mob/ctl/block/act/index"},sizeof("GET /android/mod/mob/ctl/block/act/index")-1,0,{0x00},0,0,0,1,0},//ycc
   {TCP_PACKET,1,"letv",en_PROTO_ID_LETV,4,65535,20480,20480,{"GET /lekan/apiso_json.so"},sizeof("GET /lekan/apiso_json.so")-1,0,{0x00},0,0,0,7,0},
   {TCP_PACKET,1,"mgotv",en_PROTO_ID_MGOTV,4,65535,20480,20480,{"GET /v3/search/getResult"},sizeof("GET /v3/search/getResult")-1,0,{0x00},0,0,0,7,0},//ycc
   {TCP_PACKET,1,"mgotv",en_PROTO_ID_MGOTV,4,65535,20480,20480,{"GET /mobile/distribute.do"},sizeof("GET /mobile/distribute.do")-1,0,{0x00},0,0,0,7,0},//ycc
   {TCP_PACKET,1,"mgotv",en_PROTO_ID_MGOTV,4,65535,20480,20480,{"GET /v1/search/autocomplete"},sizeof("GET /v1/search/autocomplete")-1,0,{0x00},0,0,0,7,0},//ycc
   {TCP_PACKET,1,"mgotv",en_PROTO_ID_MGOTV,4,65535,20480,20480,{"GET /mobile/getRsaKey"},sizeof("GET /mobile/getRsaKey")-1,0,{0x00},0,0,0,1,0},
   {TCP_PACKET,1,"mgotv",en_PROTO_ID_MGOTV,4,65535,20480,20480,{" GET /mobile/getCategorys"},sizeof(" GET /mobile/getCategorys")-1,0,{0x00},0,0,0,1,0},
   {TCP_PACKET,1,"mgotv",en_PROTO_ID_MGOTV,4,65535,20480,20480,{"GET /search/getResult"},sizeof("GET /search/getResult")-1,0,{0x00},0,0,0,7,0},
//   {TCP_PACKET,1,"baofeng",en_PROTO_ID_BAOFENG,4,65535,20480,20480,{"GET /suggest.php?"},sizeof("GET /suggest.php?")-1,0,{0x00},0,0,0,7,0},//ycc
   {TCP_PACKET,1,"baofeng",en_PROTO_ID_BAOFENG,4,65535,20480,20480,{"GET /logger.php?"},sizeof("GET /logger.php?")-1,0,{0x00},0,0,0,1,1},//ycc
//   {TCP_PACKET,1,"baofeng",en_PROTO_ID_BAOFENG,4,65535,20480,20480,{"POST /api/mlogin/default"},sizeof("POST /api/mlogin/default")-1,0,{0x00},0,0,0,1,0},
   {TCP_PACKET,1,"baofeng",en_PROTO_ID_BAOFENG,4,65535,20480,20480,{"GET /iphone/personal"},sizeof("GET /iphone/personal")-1,0,{0x00},0,0,0,1,0},
   {TCP_PACKET,1,"baofeng",en_PROTO_ID_BAOFENG,4,65535,20480,20480,{"GET /iphone/search_type"},sizeof("GET /iphone/search_type")-1,0,{0x00},0,0,0,7,0},
   {TCP_PACKET,1,"baofeng",en_PROTO_ID_BAOFENG,4,65535,20480,20480,{"GET /search.php?query="},sizeof("GET /search.php?query=")-1,0,{0x00},0,0,0,7,0},
   {TCP_PACKET,1,"sohuvideo",en_PROTO_ID_SOHUVIDEO,4,65535,20480,20480,{"GET /mobile_user/device/clientconf.json"},sizeof("GET /mobile_user/device/clientconf.json")-1,0,{0x00},0,0,0,1,0},//ycc
   {TCP_PACKET,1,"sohuvideo",en_PROTO_ID_SOHUVIDEO,4,65535,20480,20480,{"GET /search/keyword"},sizeof("GET /search/keyword")-1,0,{0x00},0,0,0,7,0},//ycc
//   {TCP_PACKET,1,"sohuvideo",en_PROTO_ID_SOHUVIDEO,4,65535,20480,20480,{"GET /v6/mobile/channel/list.json"},sizeof("GET /v6/mobile/channel/list.json")-1,0,{0x00},0,0,0,1,0},//ycc
   {TCP_PACKET,1,"sohuvideo",en_PROTO_ID_SOHUVIDEO,4,65535,20480,20480,{"GET /mobile_user/push/uploadtoken.json"},sizeof("GET /mobile_user/push/uploadtoken.json")-1,0,{0x00},0,0,0,1,0},
   {TCP_PACKET,1,"sohuvideo",en_PROTO_ID_SOHUVIDEO,4,65535,20480,20480,{"GET /v4/search/all.json"},sizeof("GET /v4/search/all.json")-1,0,{0x00},0,0,0,7,0},
   {TCP_PACKET,1,"tudou",en_PROTO_ID_TUDOU,4,65535,20480,20480,{"GET /v4_7/search/ugc"},sizeof("GET /v4_7/search/ugc")-1,0,{0x00},0,0,0,7,0},//ycc
//   {TCP_PACKET,1,"tudou",en_PROTO_ID_TUDOU,4,65535,20480,20480,{"GET /v4/user/sub/update/num"},sizeof("GET /v4/user/sub/update/num")-1,0,{0x00},0,0,0,1,0},//ycc
   {TCP_PACKET,1,"tudou",en_PROTO_ID_TUDOU,4,65535,20480,20480,{"GET /layout/ios/v3/search/direct_all/"},sizeof("GET /layout/ios/v3/search/direct_all/")-1,0,{0x00},0,0,0,7,0},//ycc
//  {TCP_PACKET,1,"tudou",en_PROTO_ID_TUDOU,4,65535,20480,20480,{"GET /user/viewrecord/get"},sizeof("GET /user/viewrecord/get")-1,0,{0x00},0,0,0,1,0},
//   {TCP_PACKET,1,"tudou",en_PROTO_ID_TUDOU,4,65535,20480,20480,{"GET /adv/startpage"},sizeof("GET /adv/startpage")-1,0,{0x00},0,0,0,1,0},
   {TCP_PACKET,1,"tudou",en_PROTO_ID_TUDOU,4,65535,20480,20480,{"GET /v4_7/search/direct_all"},sizeof("GET /v4_7/search/direct_all")-1,0,{0x00},0,0,0,7,0},
//   {TCP_PACKET,1,"bdvideo",en_PROTO_ID_BDVIDEO,4,65535,20480,20480,{"GET /getmfomat?"},sizeof("GET /getmfomat?")-1,0,{0x00},0,0,0,1,0},//ycc
   {TCP_PACKET,1,"bdvideo",en_PROTO_ID_BDVIDEO,4,65535,20480,20480,{"GET /iphapp_static/auto_channelMenu/"},sizeof("GET /iphapp_static/auto_channelMenu/")-1,0,{0x00},0,0,0,1,0},//ycc
   {TCP_PACKET,1,"bdvideo",en_PROTO_ID_BDVIDEO,4,65535,20480,20480,{"GET /iphnativeindex"},sizeof("GET /iphnativeindex")-1,0,{0x00},0,0,0,1,0},
   {TCP_PACKET,1,"bdvideo",en_PROTO_ID_BDVIDEO,4,65535,20480,20480,{"POST /generalconf/?terminal=adnative&appname=videoandroid"},sizeof("POST /generalconf/?terminal=adnative&appname=videoandroid")-1,0,{0x00},0,0,0,1,0},
   {TCP_PACKET,1,"bdvideo",en_PROTO_ID_BDVIDEO,4,65535,20480,20480,{"GET /p.gif?pid="},sizeof("GET /p.gif?pid=")-1,0,{0x00},0,0,0,7,0},
   {TCP_PACKET,1,"kuwo",en_PROTO_ID_KUWO,4,65535,20480,20480,{"GET /newradio.nr"},sizeof("GET /newradio.nr")-1,0,{0x00},0,0,0,1,0},
//   {TCP_PACKET,1,"kuwo",en_PROTO_ID_KUWO,4,65535,20480,20480,{"POST /music."},sizeof("POST /music.")-1,0,{0x00},0,0,0,1,0},//ycc
   {TCP_PACKET,1,"kuwo",en_PROTO_ID_KUWO,4,65535,20480,20480,{"GET /UnicomFlow/flow/getagents"},sizeof("GET /UnicomFlow/flow/getagents")-1,0,{0x00},0,0,0,1,0},
   {TCP_PACKET,1,"kuwo",en_PROTO_ID_KUWO,4,65535,20480,20480,{"GET /s.c"},sizeof("GET /s.c")-1,0,{0x00},0,0,0,7,0},
   {TCP_PACKET,1,"http",en_PROTO_ID_HTTP,4,65535,20480,20480,{"GET"},3,0,{0x00},0,0,0,0,0},
   {TCP_PACKET,1,"http",en_PROTO_ID_HTTP,4,65535,20480,20480,{"POST"},4,0,{0x00},0,0,0,0,0},
   {TCP_PACKET,7,"ftp",en_PROTO_ID_FTP,0,0,5376,5376,{"USER "},sizeof("USER ")-1,0,{0x00},0,0,0,1,0},
   {TCP_PACKET,5,"imap",en_PROTO_ID_IMAP,0,0,36608,36608,{"LOGIN "},sizeof("LOGIN ")-1,3,{0x00},0,0,0,1,0},
   {TCP_PACKET,5,"imap",en_PROTO_ID_IMAP,36608,36608,0,0,{0x00},0,0,{0x00},0,0,1,6,0},
   {TCP_PACKET,4,"pop",en_PROTO_ID_POP,0,0,28160,28160,{"USER "},sizeof("USER ")-1,0,{0x00},0,0,0,1,0},
   {TCP_PACKET,4,"pop",en_PROTO_ID_POP,28160,28160,0,0,{"Received: "},sizeof("Received: ")-1,0,{0x00},0,0,1,6,0},
   {TCP_PACKET,3,"smtp",en_PROTO_ID_SMTP,0,0,6400,6400,{"Date: "},sizeof("Date: ")-1,0,{0x00},0,0,0,5,0},
   {TCP_PACKET,9,"telnet",en_PROTO_ID_TELNET,0,0,5888,5888,{0xfb},1,1,{0xff,0xfb,0x03},3,-3,0,1,0},
   {TCP_PACKET,9,"telnet",en_PROTO_ID_TELNET,0,0,5888,5888,{0xff,0xfd,0x01},3,0,{0xff,0xfd,0x01},3,-3,0,1,0},
   {TCP_PACKET,9,"telnet",en_PROTO_ID_TELNET,0,0,5888,5888,{0xff,0xfd,0x03},3,0,{0xff,0xfd,0x03},3,-3,0,1,0},
   {TCP_PACKET,9,"telnet",en_PROTO_ID_TELNET,0,0,5888,5888,{0x00},0,0,{0x00},0,0,0,0,0},
   {TCP_PACKET,99,"miliao",en_PROTO_ID_MILIAO,4,65535,26132,26132,{"<message to="},sizeof("<message to=")-1,0,{0x00},0,0,0,3,0},
   {TCP_PACKET,99,"miliao",en_PROTO_ID_MILIAO,26132,26132,4,65535,{"<message chid="},sizeof("<message chid=")-1,0,{0x00},0,0,1,4,0},
   {TCP_PACKET,99,"miliao",en_PROTO_ID_MILIAO,26132,26132,4,65535,{"<bind from='xiaomi.com'"},sizeof("<bind from='xiaomi.com'")-1,0,{0x00},0,0,1,1,0}
};



#define UDP_RULE_NUM (sizeof(g_UdpRule)/sizeof(RULEITEM))
#define TCP_RULE_NUM (sizeof(g_TcpRule)/sizeof(RULEITEM))


int protocol_audit_reg()
{
	int i = 0;
	for(i; i < PROTOCOL_AUDIT_NUM; i++)
	{
		g_proto_audit[i].packet_audit = protocol_null;
	}
	g_proto_audit[en_PROTO_ID_HTTP].packet_audit = protocol_http;
	g_proto_audit[en_PROTO_ID_QQ].packet_audit = protocol_qq;
	g_proto_audit[en_PROTO_ID_WEIXIN].packet_audit = protocol_weixin;
	g_proto_audit[en_PROTO_ID_QQGAME].packet_audit = protocol_qqyouxi;
	g_proto_audit[en_PROTO_ID_BAIDUSEARCH].packet_audit = protocol_baidusearch;
	g_proto_audit[en_PROTO_ID_BAIDUTIEBA].packet_audit = protocol_baidutieba;
	g_proto_audit[en_PROTO_ID_BAIDUAPP].packet_audit = protocol_baidu_app;
	g_proto_audit[en_PROTO_ID_TAOBAO].packet_audit = protocol_taobao;
	//g_proto_audit[en_PROTO_ID_TMALL].packet_audit = protocol_taobao;
	g_proto_audit[en_PROTO_ID_MEITUANTUANGOU].packet_audit = protocol_meituan;
	g_proto_audit[en_PROTO_ID_JD].packet_audit = protocol_jd;
	g_proto_audit[en_PROTO_ID_DIANPING].packet_audit = protocol_dianping;
	g_proto_audit[en_PROTO_ID_BDNUOMI].packet_audit = protocol_bdnuomi;
	g_proto_audit[en_PROTO_ID_MEITUANWAIMAI].packet_audit = protocol_meituawaimai;
	g_proto_audit[en_PROTO_ID_WEIBO].packet_audit = protocol_weibo;
	g_proto_audit[en_PROTO_ID_58TONGCHENG].packet_audit = protocol_58tongcheng;
	g_proto_audit[en_PROTO_ID_DIDIDACHE].packet_audit = protocol_dididache;
	g_proto_audit[en_PROTO_ID_360SAFE].packet_audit = protocol_http;
	g_proto_audit[en_PROTO_ID_CTRIP].packet_audit = protocol_ctrip;
//	g_proto_audit[en_PROTO_ID_TIANYASHEQU].packet_audit = protocol_tianya;
//	g_proto_audit[en_PROTO_ID_MAOPU].packet_audit = protocol_maopu;
//	g_proto_audit[en_PROTO_ID_WANGYI].packet_audit = protocol_wangyi;
	g_proto_audit[en_PROTO_ID_QUNAR].packet_audit = protocol_qunaer;
	g_proto_audit[en_PROTO_ID_163WEBMAIL].packet_audit = protocol_webmail;
	g_proto_audit[en_PROTO_ID_QQNEWS].packet_audit = protocol_qqnews;
	g_proto_audit[en_PROTO_ID_QQZONE].packet_audit = protocol_http;
	g_proto_audit[en_PROTO_ID_TECENTVIDEO].packet_audit = protocol_tecent_video;
	g_proto_audit[en_PROTO_ID_ZHIFUBAO].packet_audit = protocol_http;
	g_proto_audit[en_PROTO_ID_ZHILIAN].packet_audit = protocol_zhilian;
	g_proto_audit[en_PROTO_ID_LEIPIN].packet_audit = protocol_liepin;
	g_proto_audit[en_PROTO_ID_KUGOU].packet_audit = protocol_kugou;
	g_proto_audit[en_PROTO_ID_QQMAIL].packet_audit = protocol_qqmail;
	g_proto_audit[en_PROTO_ID_12306].packet_audit = protocol_http;
	g_proto_audit[en_PROTO_ID_WEIPIN].packet_audit = protocol_weipin;
	g_proto_audit[en_PROTO_ID_YY].packet_audit = protocol_yy;
	g_proto_audit[en_PROTO_ID_QUNAR].packet_audit = protocol_http;
	g_proto_audit[en_PROTO_ID_VDIAN].packet_audit = protocol_http;
	g_proto_audit[en_PROTO_ID_DAZHIHUI].packet_audit = protocol_http;
	g_proto_audit[en_PROTO_ID_XUNLEI].packet_audit = protocol_xunlei;
	g_proto_audit[en_PROTO_ID_360YUNPAN].packet_audit = protocol_360yunpan;
	g_proto_audit[en_PROTO_ID_JINRITOUTIAO].packet_audit = protocol_http;
	g_proto_audit[en_PROTO_ID_BAIDUMAP].packet_audit = protocol_http;
	g_proto_audit[en_PROTO_ID_GAODEMAP].packet_audit = protocol_http;
	g_proto_audit[en_PROTO_ID_DIDAPINCHE].packet_audit = protocol_http;
	g_proto_audit[en_PROTO_ID_SUNINGYIGOU].packet_audit = protocol_suningyigou;
	g_proto_audit[en_PROTO_ID_AIQIYI].packet_audit = protocol_aiqiyi;
	g_proto_audit[en_PROTO_ID_YOUKU].packet_audit = protocol_youku;
	g_proto_audit[en_PROTO_ID_QQZONE].packet_audit = protocol_qqzone;
	g_proto_audit[en_PROTO_ID_QQMUSIC].packet_audit = protocol_qqmusic;
	g_proto_audit[en_PROTO_ID_LETV].packet_audit = protocol_letv;
	g_proto_audit[en_PROTO_ID_MGOTV].packet_audit = protocol_mgotv;
	g_proto_audit[en_PROTO_ID_BAOFENG].packet_audit = protocol_baofeng;
	g_proto_audit[en_PROTO_ID_SOHUVIDEO].packet_audit = protocol_sohutv;
	g_proto_audit[en_PROTO_ID_TUDOU].packet_audit = protocol_tudou;
	g_proto_audit[en_PROTO_ID_BDVIDEO].packet_audit = protocol_bdvideo;
	g_proto_audit[en_PROTO_ID_KUWO].packet_audit = protocol_kuwo;
	g_proto_audit[en_PROTO_ID_FTP].packet_audit = protocol_ftp;
	g_proto_audit[en_PROTO_ID_IMAP].packet_audit = protocol_imap;
	g_proto_audit[en_PROTO_ID_POP].packet_audit = protocol_pop;
	g_proto_audit[en_PROTO_ID_SMTP].packet_audit = protocol_smtp;
	g_proto_audit[en_PROTO_ID_TELNET].packet_audit = protocol_telnet;
	g_proto_audit[en_PROTO_ID_MILIAO].packet_audit = protocol_miliao;
	g_proto_audit[en_PROTO_ID_HTTP].packet_audit = protocol_http;

	return 0;
}


int urlpattern_init(void **ppacsm)
{
	/*
	ACSM_STRUCT * acsm;

	acsm = acsmNew();

	int num = sizeof(g_urlpattern)/sizeof(URLPATTERN);
	int i;
	for (i = 0; i < num; ++i)
	{
		acsmAddPattern(acsm,g_urlpattern[i].host,strlen(g_urlpattern[i].host),(void *)g_urlpattern[i].proto_name,
					   g_urlpattern[i].id,i);
	}

	acsmCompile (acsm);

	*ppacsm = (void *)acsm;
*/
	KTRIE_STRUCT * ts;
	int nocase=1;  // don't care about case
	ts = KTrieNew();
	int num = sizeof(g_urlpattern)/sizeof(URLPATTERN);
	int i;
	for (i = 0; i < num; ++i)
	{

		KTrieAddPattern( ts, (unsigned char *)g_urlpattern[i].host,strlen(g_urlpattern[i].host),
						 (void *)g_urlpattern[i].proto_name, nocase, g_urlpattern[i].id);
	}

	KTrieCompile( ts );

	*ppacsm = (void *)ts;

	printf("[INFO]create trie sucess!\n");

	return 0;
}



int get_match_rule(u_char pro_type/*tcp/udp*/,u_short sport,u_short dport,u_char *data, int data_len,RULEITEM **ppRuleItem)
{
	int i ;
//	printf("%d %d %d %d\n",pro_type,sport,dport,data_len);
	if(TCP_PACKET == pro_type)
	{
		for(i = 0; i < TCP_RULE_NUM; i++)
		{
			if(g_TcpRule[i].property_len == 0)
			{
				if((g_TcpRule[i].min_sport ==  g_TcpRule[i].max_sport) != 0)
				{
					if(sport == g_TcpRule[i].max_sport)
						break;
				}
				if((g_TcpRule[i].min_dport ==  g_TcpRule[i].max_dport) != 0)
				{
					if(dport == g_TcpRule[i].max_dport)
						break;
				}
				continue;
			}
			if(g_TcpRule[i].property_offset >= 0)
			{	
				if(!memcmp(data + g_TcpRule[i].property_offset, g_TcpRule[i].property, g_TcpRule[i].property_len))
				{			
					if(g_TcpRule[i].subproperty_len > 0)
					{
						if(g_TcpRule[i].subproperty_offset >= 0)
						{
							if(memcmp(data + g_TcpRule[i].subproperty_offset, g_TcpRule[i].subproperty, g_TcpRule[i].subproperty_len))
								continue;
						}
						else
						{
							if(memcmp(data + data_len + g_TcpRule[i].subproperty_offset, g_TcpRule[i].subproperty, g_TcpRule[i].subproperty_len))
								continue;
						}
					}
					if(g_TcpRule[i].min_sport !=  g_TcpRule[i].max_sport)
					{
						if(sport > g_TcpRule[i].max_sport || sport < g_TcpRule[i].min_sport)
							continue;
					}
					else
					{
						if(g_TcpRule[i].min_sport != 0 && sport != g_TcpRule[i].min_sport)
							continue;
							
					}
					if(g_TcpRule[i].min_dport !=  g_TcpRule[i].max_dport)
					{
						if(dport > g_TcpRule[i].max_dport || dport < g_TcpRule[i].min_dport)
							continue;
					}
					else
					{
						if(g_TcpRule[i].min_dport != 0 && dport != g_TcpRule[i].min_dport)
							continue;					
					}
					break;
				}			
			}	
		}
		if( i < TCP_RULE_NUM)
		{
			*ppRuleItem = &g_TcpRule[i];
			return 1;
		}
	}
	else if(UDP_PACKET == pro_type)
	{
		for(i = 0; i < UDP_RULE_NUM; i++)
		{
			if(g_UdpRule[i].property_len == 0)
			{
				if((g_UdpRule[i].min_sport ==  g_UdpRule[i].max_sport) != 0)
				{
					if(sport == g_UdpRule[i].max_sport)
						break;
				}
				if((g_UdpRule[i].min_dport ==  g_UdpRule[i].max_dport) != 0)
				{
					if(dport == g_UdpRule[i].max_dport)
						break;
				}
				continue;
			}
			if(g_UdpRule[i].property_offset >= 0)
			{	
				if(!memcmp(data + g_UdpRule[i].property_offset, g_UdpRule[i].property, g_UdpRule[i].property_len))
				{			
					if(g_UdpRule[i].subproperty_len > 0)
					{
						if(g_UdpRule[i].subproperty_offset >= 0)
						{
							if(memcmp(data + g_UdpRule[i].subproperty_offset, g_UdpRule[i].subproperty, g_UdpRule[i].subproperty_len))
								continue;
						}
						else
						{
							if(memcmp(data + data_len + g_UdpRule[i].subproperty_offset, g_UdpRule[i].subproperty, g_UdpRule[i].subproperty_len))
								continue;
						}
					}
					if(g_UdpRule[i].min_sport !=  g_UdpRule[i].max_sport)
					{
						if(sport > g_UdpRule[i].max_sport || sport < g_UdpRule[i].min_sport)
							continue;
					}
					else
					{
						if(g_UdpRule[i].min_sport != 0 && sport != g_UdpRule[i].min_sport)
							continue;
							
					}
					if(g_UdpRule[i].min_dport !=  g_UdpRule[i].max_dport)
					{
						if(dport > g_UdpRule[i].max_dport || dport < g_UdpRule[i].min_dport)
							continue;
					}
					else
					{
						if(g_UdpRule[i].min_dport != 0 && dport != g_UdpRule[i].min_dport)
							continue;					
					}
					break;
				}			
			}	
		}
		if( i < UDP_RULE_NUM)
		{
			*ppRuleItem = &g_UdpRule[i];
			return 1;
		}
	}
	return 0;
}





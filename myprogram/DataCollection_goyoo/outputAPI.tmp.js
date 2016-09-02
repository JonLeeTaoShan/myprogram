var util		= require('util'),
	moment	= require("moment"),
	data	= require('date-utils'),
	getStringbyhttp1= require('./autoJob').getStringbyhttp1,
	exec = require('child_process').exec,
	cheerio = require('cheerio'),
	config = require('./config'),
	getDatabassInfo= require('./myroute').getDatabassInfo;

function lastOnlineByMac(query,mac,father,callback)
{
	query(util.format("select * from upload_db.dev_upload_tb where nowtime = (select max(nowtime) from upload_db.dev_upload_tb where mac = %s) and mac = %s;",parseInt("0x"+mac, 16),parseInt("0x"+mac, 16)),  
		function selectCb(error, results, fields) 
		{ 
			if(error) 
			{
				console.log("lastOnlineByMac Error: " + error.message);
				callback(mac,father,NULL,-1);
			}
			if(results)
			{
				callback(mac,father,results,0);
			}
			else 
				callback(mac,father,NULL,-2);

			
		}
	);
}
function offlineByMac(query,mac,fromnow,callback)
{
	query(util.format("select unix_timestamp(nowtime) nowtime,errtype from upload_db.dev_upload_offline_tb where mac=%d and nowtime >DATE_SUB(now(), INTERVAL (1+%d) DAY)and nowtime < DATE_SUB(now(), INTERVAL (0+%d) DAY) order by nowtime ASC;",parseInt("0x"+mac.replace(/:/g,''), 16),fromnow,fromnow),  
		function selectCb(error, results, fields) 
		{  
			if(error) 
			{
				console.log("offlineByMac Error: " + error.message);
				callback(mac,-1,0,-1);
			}
			if(results)
			{
				var length = results.length;
				var totaltime=0;
				var offlinetimes=0;
				if(length>1)
				{
					for(var i=0;i<length;)
					{
						if((i==0)&&(results[0].errtype=="restart"))
						{
							console.log("first results errtype =restart continue. ");
							i++;
							continue;
						}
						if(!results[i+1])
						{
							i++;
							continue;
						}
						if((results[i].errtype!='error')||(results[i+1].errtype!='restart'))
						{
							console.log("find fail data nowtime=%d,continue.",results[i].nowtime);
							i++;
							continue;
						}
						totaltime += results[i+1].nowtime -results[i].nowtime;
						offlinetimes++;
						//console.log("results.nowtime=%d",results[i].nowtime);
						i=i+2;
					}
					//console.log("totaltime=%d",totaltime);
					callback(mac,totaltime,offlinetimes,0);
				}
				else
				{
					lastOnlineByMac(query,mac,{totaltime:totaltime,offlinetimes:offlinetimes},
						function(mac,father,results,status)
						{
							if(status != 0)
									callback(mac,father.totaltime,father.offlinetimes,status);
							if(results.length==1)
							{
								var old=new Date(results[0].nowtime);
								var to	=new Date();
								if(old.getHoursBetween(to)>6)
								{
									console.log("old date:"+results[0].nowtime);
									callback(mac,father.totaltime,father.offlinetimes,-4);
								}
								else
									callback(mac,father.totaltime,father.offlinetimes,0);
							}
							else
									callback(mac,father.totaltime,father.offlinetimes,-3);
						}
					)
					console.log("can't find mac=%s error,fromnow=%d",mac,fromnow);
					return;
				}
			}
			else
			{
				callback(mac,0,0,-1);
				return;
			}
		}
	);
	
}
function offlineByMacAPI(query,mac,fromnow,callback)
{
	offlineByMac(query,mac,fromnow,function(mac,os,ot,status)
		{
			var from=new Date();
			var to	=new Date();
			from.add({days:-(parseInt(fromnow)+1)});
			to.add({days:-fromnow});
			console.log("form:%s,to:%s .%d",from,to,-(parseInt(fromnow)+1))
			callback(util.format("贵店铺wifi小云设备（%s 至 %s）报告：当前总的小云设备有1台，活跃在线率为:%d%%,%s掉线%d次。今天认证人数为400人，详细信息请点击<a target='_blank' href='http://dev-stat.xiaoyun.com/%s/getDevstat?shoptb=%s&shopid=%s'>http://dev-stat.xiaoyun.com/%s/getDevstat?shoptb=%s&shopid=%s</a> 进行查看。在线快速排查手册<a target='_blank' href='http://dev-stat.xiaoyun.com/getdoc?id=1324124321'>http://dev-stat.xiaoyun.com/getdoc?id=1324124321</a>.技术支持电话：1333333333，邮箱：abc@goyoo.com。",moment(from).format('YYYY-MM-DD HH:mm:ss'),moment(to).format('YYYY-MM-DD HH:mm:ss'),((1-os/(3600*24))*100).toFixed(1),mac,o));
		})
}
function offlineByShopIDAPI(query,shopTB,shopID,fromnow,callback)
{
	query(util.format(" select * from upload_db.%s where shopID=\'%s\'; ",shopTB,shopID),
		function selectCb(error, results,fields) 
		{
			if(error) {
				console.log("ClientReady Error: " + error.message);
				return;
			}
			if(!results)
			{
				console.log("offlineByShopAPI results nill");
				return;
			}
			var length=results.length;
			if(length!=1)
			{
				console.log("offlineByShopAPI length=%d",length);
				return;
			}
			var macList=results[0].macList.substring(1,results[0].macList.length-1).split(',');
			var maclength = macList.length;
			var nn =0;
			var macRes =new Array();
			for(var i=0;i<maclength;i++)
				offlineByMac(query,macList[i],fromnow,
					function(mac,offlinesecond,offlinetimes,status)
					{
						nn++;
						macRes.push({mac:mac,os:offlinesecond,ot:offlinetimes,name:results[0].name,status});
						//console.log("mac=%s,os=%s,ot=%s",mac,offlinesecond,offlinetimes);
						if(nn == maclength)
						{
							console.log("nn ==maclength =%d",nn);
							console.log(macRes);
							callback(macRes);
							return;
						}
					}
				);
			//console.log(macList);
			
		}
	);
	/*offlineByMac(query,mac,fromnow,function(os,ot)
	{
	
	}*/
}
function  getVisitorsByShopID(father,shopid,fromnow)
{
	var day=new Date();
	day.add({days:-fromnow});
	getStringbyhttp1(util.format("",shaopid,moment(day).format('YYYY-MM-DD')),father)
}
function offlineByShopIDAPIChin(query,shopTB,shopID,fromnow,callback)
{
	offlineByShopIDAPI(query,shopTB,shopID,fromnow,
		function(macRes)
		{
			if(macRes)
			{
				var length=macRes.length;
				var totalsecond =0;
				var totaltimes = 0;
				var name=macRes[0].name;
				var perDev="";
				var errDev="";
				var errDevcnt=0;
				for(var i=0;i<length;i++)
				{
					totalsecond +=macRes[i].os;
					totaltimes +=macRes[i].ot;
					if(macRes[i].status<0)
					{
						errDevcnt++;	
						errDev  += macRes[i].mac+',';
					}
					else
						perDev	+=	macRes[i].mac+'掉线时间'+parseInt(macRes[i].os/60)+'分,掉线'+macRes[i].ot+'次';

				}
				var from=new Date();
				var to	=new Date();
				from.add({days:-(parseInt(fromnow)+1)});
				to.add({days:-fromnow});
				callback(util.format("(%d)wifi小云设备（%s 至 %s）报告：当前总的小云设备有%d台，活跃在线率为:%d%%%s长时间不在线%d台,分别为%s当日认证人数为400人<a target='_blank' href='http://dev-stat.xiaoyun.com/%s/getDevstat?shoptb=%s&shopid=%s'>http://dev-stat.xiaoyun.com/%s/getDevstat?shoptb=%s&shopid=%s</a> 进行查看。在线快速排查手册<a target='_blank' href='http://dev-stat.xiaoyun.com/getdoc?id=1324124321'>http://dev-stat.xiaoyun.com/getdoc?id=1324124321</a>.技术支持电话：333333333 ,邮箱：abc@goyoo.com?,name,moment(from).format('YYYY-MM-DD HH:mm:ss'),moment(to).format('YYYY-MM-DD HH:mm:ss'),length,((1-totalsecond/(3600*24*length))*100).toFixed(1),perDev,errDevcnt,errDev));

			}
		}
	);
}
function testdataname(query)
{
	query(" INSERT INTO upload_db.shop_by_channelID SET channelID='56383114a993d04421323a83',name='西贝莜面村;",
	//client.query("select * from upload_db.shop_by_channelID;",
		function selectCb(error, results,fields) {
			if(error) {
				console.log("ClientReady Error: " + error.message);
				return;
			}
			console.log(results);
		 }
	)
}
function getMaclistByShopID(query,shopTB,shopID,callback)
{
	query(util.format(" select * from upload_db.%s where shopID=\'%s\'; ",shopTB,shopID),
		function selectCb(error, results,fields) 
		{
			if(error) {
				console.log("ClientReady Error: " + error.message);
				return;
			}
			if(!results)
			{
				console.log("offlineByShopAPI results nill");
				return;
			}
			var length=results.length;
			if(length!=1)
			{
				console.log("offlineByShopAPI length=%d",length);
				return;
			}
			callback(results[0].macList.substring(1,results[0].macList.length-1).split(','));
		}
	)
}
function handlelastOnlineByShopID(query,shopTB,shopid,callback)
{
	var database = eval("config."+(process.env.NODE_ENV) +".database");

	var databasepasswd	= database.connection.password;
	var databaseuser 	=database.connection.user;
	var 	midurl = eval("config."+(process.env.NODE_ENV) +".server.midurl");
	getMaclistByShopID(query, shopTB, shopid, 
		function (macList)
		{
			var length = macList.length;
		       var macHtmlRes =new Array();
			var totalcout=0;
			for(var i = 0;i<length;i++)
			{
				var cmdStr =util.format("mysql -u%s -p%s -e 'select * from upload_db.dev_upload_tb where nowtime = (select max(nowtime) from upload_db.dev_upload_tb where mac = %s) and mac = %s;'  -H",databaseuser,databasepasswd,parseInt("0x"+macList[i], 16),parseInt("0x"+macList[i], 16));
				exec(cmdStr,
					{
						encoding: 'utf8',
						timeout: 0,
						maxBuffer: 5000 * 1024, // 200 * 1024
						killSignal: 'SIGTERM'
					},
				 function(err,stdout,stderr){
				    if(err) {
				        console.log('get exec api error:'+err);
				    } else {
				       // console.log(stdout);
				       macHtmlRes.push(stdout)
				       totalcout++;
					if(totalcout==length)
					{
						 var myhtml = cheerio.load('<html></html>');
						 var myhead = cheerio.load('<head></head>');
						myhtml('html').append(myhead.html());
						for(var i=0;i<length;i++)
						{
							var mytable = cheerio.load(macHtmlRes[i]);
							myhtml('html').append(mytable.html());

						}
						myhtml('head').append('<title>Get error data list </title>');
						myhtml('head').append('<link href="/'+midurl+'/public/css/mytable.css" rel="stylesheet" type="text/css" />');
						myhtml('html').append('<script src="/'+midurl+'/public/js/jquery-2.1.4.min.js"></script>');
						myhtml('html').append('<script>\n$(function(){$("tr:even").addClass("evenrowcolor");\n$("tr:odd").addClass("oddrowcolor"); });</script>');
						callback(myhtml.html());
						console.log("secceed!");
					}
				    }
				});

			}
					

		}
	)
}
module.exports.testdataname = testdataname;
module.exports.offlineByMacAPI = offlineByMacAPI;
module.exports.offlineByShopIDAPI = offlineByShopIDAPI;
module.exports.offlineByShopIDAPIChin = offlineByShopIDAPIChin;
module.exports.handlelastOnlineByShopID = handlelastOnlineByShopID;

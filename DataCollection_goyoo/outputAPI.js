var util		= require('util'),
	moment	= require("moment"),
	data	= require('date-utils')
	;
function offlineByMac(query,mac,fromnow,callback)
{
	query(  
						  util.format("select unix_timestamp(nowtime) nowtime,errtype from upload_db.dev_upload_offline_tb where mac=%d and nowtime >DATE_SUB(now(), INTERVAL (1+%d) DAY)and nowtime < DATE_SUB(now(), INTERVAL (0+%d) DAY);",parseInt("0x"+mac.replace(/:/g,''), 16),fromnow,fromnow),  
							function selectCb(error, results, fields) 
							{  
								if(error) 
								{
									console.log("ClientReady Error: " + error.message);
									callback(-1,0);
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
	//										console.log(results);
		//									console.log(results.length);
			//								callback(0,0);
				//							return;
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
										callback(totaltime,offlinetimes);
									}
									else
									{
										console.log("can't find mac=%s error,fromnow=%d",mac,fromnow);
										callback(totaltime,offlinetimes);
										return;
									}

								}
								else
								{
									callback(0,0);
									return;
								}
								
							}
	);
	
}
function offlineByMacAPI(query,mac,fromnow,callback)
{
	offlineByMac(query,mac,fromnow,function(os,ot)
		{
			var from=new Date();
			var to	=new Date();
			from.add({days:-(parseInt(fromnow)+1)});
			to.add({days:-fromnow});
			console.log("form:%s,to:%s .%d",from,to,-(parseInt(fromnow)+1))
			callback(util.format("贵店铺wifi小云设备（%s 至 %s）报告：当前总的小云设备有1台，活跃在线率为:%d%%,%s掉线%d次。今天认证人数为400人，详细信息请点击<a target='_blank' href='http://dev-stat.xiaoyun.com/getstatus?id=232341234124'>http://dev-stat.xiaoyun.com/getstatus?id=232341234124</a> 进行查看。在线快速排查手册<a target='_blank' href='http://dev-stat.xiaoyun.com/getdoc?id=1324124321'>http://dev-stat.xiaoyun.com/getdoc?id=1324124321</a>.技术支持电话：1333333333，邮箱：abc@goyoo.com。",moment(from).format('YYYY-MM-DD HH:mm:ss'),moment(to).format('YYYY-MM-DD HH:mm:ss'),((1-os/(3600*24))*100).toFixed(1),mac,ot));
		})
}
function testdataname(query)
{
	query(" INSERT INTO upload_db.shop_by_channelID SET channelID='56383114a993d04421323a83',name='西贝莜面村';",
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
module.exports.testdataname = testdataname;
module.exports.offlineByMacAPI = offlineByMacAPI;

var util	= require('util'),
	url	= require('url');
var http = require('http');
var request = require('request');
var 	config = require('./config'),
	util = require('util'),
	mysql=	require('mysql'),
	moment	= require("moment"),
	async = require("async")
;
function getStringbyhttp1(uri,father,callback)
{
	request(uri, function (error, response, body) {
		 if (!error && response.statusCode == 200) 
	 	{
		// console.log(body);
			 callback(body,father);
		}
	})

}
function CallbackInsertShopTB(query,channelID,table_name,limit,offset)
{
	 getStringbyhttp1(util.format("http://data-api.xiaoyun.com/v1/shop/by/channelID/%s?limit=%d&offset=%d",channelID,limit,offset),channelID,function(data,father)
		{
			var msg =JSON.parse(data);
			if (!msg.meta)
				console.log("error:can't get meta body");
			 var length = msg.meta.length;
			 var remaining = msg.meta.remaining;
			 var myoffset=msg.meta.offset;
			 var Shoplist=msg.list;
			 for (var i = 0; i< length;i++)
			 {
				 // console.log("thisShopname=%s,thisadminPhone=%s,thisid=%s",Shoplist[i].name,Shoplist[i].phoneNumber,Shoplist[i].id);
				var cc =0;
				 getStringbyhttp1(util.format("http://data-api.xiaoyun.com/v1/shop/%s/routers?limit=200&offset=0",Shoplist[i].id),Shoplist[i],
				  	function(data,father)
					{
						var msg1 =JSON.parse(data);
						if (!msg1.meta)
							console.log("error:can't get meta body");
						 var length = msg1.meta.length;
						 var remaining = msg1.meta.remaining;
						 var list=msg1.list;
						 var macList;
						for (var i = 0; i< length;i++)
						{
							if(macList)
								macList+=','+list[i].mac;
							else
								macList=list[i].mac;
						}
						//console.log(util.format("INSERT INTO upload_db.\'%s\' SET ShopID=\'%s\',name=\'%s\',adminPhone=\'%s\',macList=\'%s\' ;",table_name,father.id,father.name,father.phoneNumber,macList));
						cc++;
						//console.log("cc=%d",cc);
						//return 0;
						query(util.format("INSERT INTO upload_db.%s SET ShopID=\'%s\',name=\'%s\',adminPhone=\'%s\',macList=\'%s\' ;",table_name,father.id,father.name,father.phoneNumber,macList),
							function(error,results,fields)
							{
								if(error)
								{
									console.log("InsertShopTB error:"+error);
								}
							}
						);
					  }
				  )

			 }
			 console.log("CallbackInsertShopTB out offset=%d",offset)
	 	}
	 );
}
function InsertShopTB(query,channelID,table_name)
{
	 getStringbyhttp1(util.format("http://data-api.xiaoyun.com/v1/shop/by/channelID/%s?limit=1&offset=0",channelID),channelID,function(data,father)
		{
			var msg =JSON.parse(data);
			if (!msg.meta)
				console.log("error:can't get meta body");
			 var total = parseInt(msg.meta.total);
			 console.log("total:%d",total)
			 for(var i = 0;i<total;i = i+20)
			 {
			 	console.log("i=%d",i)
				CallbackInsertShopTB(query, channelID, table_name,20,i);
			 }
			 console.log("out return")

	 	}
	 );
}
function traversalShopTablesFromChannel(query,backcall)
{
	query("select * from upload_db.shop_by_channelID;",
		function(error,results,fields)
		{
			if(error)
			{
				console.log("TraversalShopTablesFromChannel error:"+error.message)
			}
			if(!results)
			{
				console.log("database can't get shop_by_channelID results;");
				return;
			}
			if(results)
			{
				var length = results.length;
				if(length>=1)
					for(var i =0;i < length;i++)
					{
						if(results[i].table_name)
						{
							backcall(results[i]);
						}
					}
			}
		}
	)
}
function traversalEveryMacFromShopTables(query,backcall)
{
	traversalShopTablesFromChannel(query,
		function(father)
		{
			query(util.format("select * from upload_db.%s;",father.table_name),
				function(error,results,fields)
				{
					if(error)
					{
						console.log("traversalEveryMacFromShopTables error:"+error.message)
					}
					if(!results)
					{
						console.log("database can't get shop info results;");
						return;
					}
					if(length<=0)
					{
						console.log("database can't get shop info;");
						return;
					}
					if(results)
					{
						var length = results.length;
						if(length>=1)
							for(var i =0;i < length;i++)
							{
								if(results[i].macList)
								{
									backcall(results[i].macList);
								}
							}
					}
				}
			)
			
		}
	)
}
function countErrByoneMacEveryday(query,mac,errTablename)
{
	
	query(util.format("select * from upload_db.dev_upload_tb where mac=%d and nowtime >DATE_SUB(now(), INTERVAL 1 DAY)and nowtime < now() order by nowtime ASC;",parseInt("0x"+mac, 16)),
		function(error,results,fields)
		{
			if(error)
			{
				console.log("countErrByoneMacEveryday error:"+error.message)
			}
			if(!results)
			{
				console.log("countErrByoneMacEveryday error: database can't get mac results;");
				return;
			}
			if(results)
			{
				var length = results.length;

				if(length>=1)
				{
					var beforetime = Date.parse(results[0].nowtime)/1000;
					var aftertime;
					var tmp;
					for(var i =1;i < length;i++)
					{
						aftertime =  Date.parse(results[i].nowtime)/1000;
						tmp = aftertime - beforetime;

//console.log("tmp=%d",tmp);
						if( tmp >= 420)
						{
							 console.log('mac = %s updata statue is error!last time is %s,beforetime=%d, aftertime=%d id_b=%d,id_a=%d',results[i].mac.toString(16),results[i-1].nowtime,beforetime ,aftertime,results[i-1].id,results[i].id);
							async.waterfall([
								function(callback){
									var res1=results[i-1],res2=results[i]
									query(util.format('INSERT INTO dev_upload_offline_tb SET bandwidth=\'%s\',errtype=\'error\',erruptime=\'%s\',id=%d, devtype=\'%s\',nowtime=\'%s\', mac=%d, time=%s, propid=\'%s\', wandev=\'%s\', landev=\'%s\',  mem=\'%s\',  cdevlist=\'%s\',  softversion=\'%s\',  others=\'%s\'',
										JSON.stringify(results[i-1].bandwidth),moment(Date.parse(results[i].nowtime)).format('YYYY-MM-DD HH:mm:ss'),results[i-1].id,results[i-1].devtype,moment(Date.parse(results[i-1].nowtime)).format('YYYY-MM-DD HH:mm:ss'),results[i-1].mac,results[i-1].time ,results[i-1].propid,results[i-1].wandev,results[i-1].landev,results[i-1].mem ,results[i-1].cdevlist ,results[i-1].softversion ,results[i-1].others),
										function(error,myresults,fields)
										{
											if(error)
											{
												console.log("countErrByoneMacEveryday 0 error:"+error.message);
											}
											callback(null,res1,res2);
										}
									)
								},
								function(arg1,arg2, callback){
									query(util.format('INSERT INTO dev_upload_offline_tb SET bandwidth=\'%s\',errtype=\'restart\',erruptime=\'%s\',id=%d, devtype=\'%s\',nowtime=\'%s\', mac=%d, time=%s, propid=\'%s\', wandev=\'%s\', landev=\'%s\',  mem=\'%s\',  cdevlist=\'%s\',  softversion=\'%s\',  others=\'%s\'',
										JSON.stringify(arg2.bandwidth),moment(Date.parse(arg2.nowtime)).format('YYYY-MM-DD HH:mm:ss'),arg1.id,arg2.devtype,moment(Date.parse(arg2.nowtime)).format('YYYY-MM-DD HH:mm:ss'),arg2.mac,arg2.time ,arg2.propid,arg2.wandev,arg2.landev,arg2.mem ,arg2.cdevlist ,arg2.softversion ,arg2.others),
										function(error,myresults1,fields)
											{
												if(error)
												{
													console.log("countErrByoneMacEveryday 1 error:"+error.message)
												}
											}
									)
								}
								], function (err, result) {
								console.log("countErrByoneMacEveryday waterfall result:"+result);
								// result now equals 'done'
								//  console.log('4');
								}
							);
						}
						beforetime = aftertime;

					}
				}
			}
		}
	)
	
}
module.exports.getStringbyhttp1 =getStringbyhttp1;
module.exports.traversalEveryMacFromShopTables =traversalEveryMacFromShopTables;
module.exports.traversalShopTablesFromChannel =traversalShopTablesFromChannel;
module.exports.InsertShopTB =InsertShopTB;
module.exports.countErrByoneMacEveryday =countErrByoneMacEveryday;


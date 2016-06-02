var util	= require('util'),
	url	= require('url');
var http = require('http');
var request = require('request');
var 	config = require('./config'),
	util = require('util'),
	mysql=	require('mysql'),
	query;
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
function autoCreatandInsertShopTB(query)
{
	query("select * from upload_db.shop_by_channelID;",
		function(error,results,fields)
		{
			if(error)
			{
				console.log("autoCreatandInsertShopTB error:"+error.message)
			}
			if(!results)
			{
				console.log("database can't get shop_by_channelID results;");
				return;
			}
			if(length<=0)
			{
				console.log("database can't get shop_by_channelID;");
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
							function myhandle(father)
							{
								query(util.format("DROP TABLE IF EXISTS upload_db.%s",father.table_name),
									function(error,results,fields)
									{
										if(error)
										{
											console.log("autoCreatandInsertShopTB error:"+error.message)
										}
										console.log("find table_name=%s channelID=%s ",father.table_name,father.channelID);
										query(util.format("CREATE TABLE `%s` (`id` int(10) unsigned NOT NULL AUTO_INCREMENT,`name` char(100) DEFAULT NULL,`shopID` varchar(48) CHARACTER SET latin1 DEFAULT NULL,`macList` text CHARACTER SET latin1,`adminPhone` varchar(20) CHARACTER SET latin1 DEFAULT NULL,`adminEmail` text CHARACTER SET latin1,PRIMARY KEY (`id`)) ENGINE=MyISAM DEFAULT CHARSET=utf8;",father.table_name),
											function(error,results,fields)
											{
												if(error)
												{
													console.log("Database error:"+error.message)
													return;
												}
												InsertShopTB(query, father.channelID, father.table_name);
											}
										);
										

									}
								);
							}
							myhandle(results[i]);
						}
					}
			}			

		}
	)
}
function connectdatebase(config)
{


	var database = eval("config."+(process.env.NODE_ENV) +".database");
	var pool = mysql.createPool({
			host :database.connection.host,
			user :database.connection.user,
			password : database.connection.password,
			database : 'upload_db',
			charset : database.connection.charset
		});

	query=function(sql,callback){
	    pool.getConnection(function(err,conn){
	        if(err){
	            callback(err,null,null);
	        }else{
	            conn.query(sql,function(qerr,vals,fields){
	                conn.release();
	                callback(qerr,vals,fields);
	            });
	        }
	    });
	};
	if (database.client != 'mysql')
		return -1;
	return 0;
	  
}
//process.env.NODE_ENV = process.env.NODE_ENV || 'development';
//connectdatebase(config);
module.exports.autoCreatandInsertShopTB = autoCreatandInsertShopTB;
module.exports.getStringbyhttp1 =getStringbyhttp1;

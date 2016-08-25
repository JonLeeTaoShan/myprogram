var util	= require('util'),
	url	= require('url');
var http = require('http');
var request = require('request');
var 	config = require('./config'),
	util = require('util'),
	mysql=	require('mysql'),
	InsertShopTB = require('./func').InsertShopTB,
	traversalShopTablesFromChannel =require('./func').traversalShopTablesFromChannel,
	traversalEveryMacFromShopTables = require('./func').traversalEveryMacFromShopTables,
	countErrByoneMacEveryday = require('./func').countErrByoneMacEveryday
	;
function autoCreatandInsertShopTB(query)
{
	traversalShopTablesFromChannel(query,
		function(fathers)
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
			myhandle(fathers);
		}
	)

}

function scheduleCountEverydayError(query)
{
	traversalEveryMacFromShopTables(query,
		function(oneShopmacList)
		{
			var macList =oneShopmacList.substring(1,oneShopmacList.length).split(',');
			for(var i=0;i<macList.length;i++)
			{
				countErrByoneMacEveryday(query,macList[i],"upload_db.dev_upload_offline_tb");
				
			}
		}
	)
}

function scheduleDelDataFromSQL(query)
{
	var schedule = require('node-schedule');
	var rule = new schedule.RecurrenceRule();
	//00:20:00
	rule.hour =0;rule.minute =20;rule.second =0;
	var j = schedule.scheduleJob(rule, function(){
		console.log("clear SQL: now is %s.",new Date());
	query(util.format(" DELETE  from upload_db.dev_upload_tb where nowtime < DATE_SUB(now(), INTERVAL 1 MONTH); "),
		function selectCb(error, results,fields) 
		{
			if(error) {
				console.log("scheduleDelDataFromSQL Error: " + error.message);
				return;
			}
			if(!results)
			{
				console.log("scheduleDelDataFromSQL results nill");
				return;
			}
			console.log("clear upload_db.dev_upload_tb SQL done!")
		});
	query(util.format(" DELETE  from upload_db.dev_upload_offline_tb where nowtime < DATE_SUB(now(), INTERVAL 3 MONTH); "),
		function selectCb(error, results,fields) 
		{
			if(error) {
				console.log("scheduleDelDataFromSQL 2 Error: " + error.message);
				return;
			}
			if(!results)
			{
				console.log("scheduleDelDataFromSQL 2 results nill");
				return;
			}
			console.log("clear upload_db.dev_upload_offline_tb SQL done!")
		});
		autoCreatandInsertShopTB(query);
		scheduleCountEverydayError(query);
	})
}
//process.env.NODE_ENV = process.env.NODE_ENV || 'development';
//connectdatebase(config);
module.exports.autoCreatandInsertShopTB = autoCreatandInsertShopTB;
module.exports.scheduleDelDataFromSQL = scheduleDelDataFromSQL;
module.exports.scheduleCountEverydayError = scheduleCountEverydayError;


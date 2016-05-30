var util	= require('util'),
	url	= require('url');
var http = require('http');
var request = require('request');

function getStringbyhttp1(uri,callback)
{
	request(uri, function (error, response, body) {
		 if (!error && response.statusCode == 200) 
	 	{
		 console.log(body) 
		 callback(body);
		}
	})

}
function autoCreatShopTB(query)
{
	query("DROP TABLE IF EXISTS 'upload_db'.`shop_xibei`;",
		function(error,results,fields)
		{
			if(error)
			{
				console.log("Database error:"+error.message)
			}
			if(results)
			{
				var length = results.length;
				if(length>=1)
					for(var i =0;i < length;i++)
					{
						if(results[i].table_name)
						{
							console.log("find table_name=%s id=%d  ",results[i].table_name,i);
							query(util.format("CREATE TABLE `%s` (`id` int(10) unsigned NOT NULL AUTO_INCREMENT,`name` char(100) DEFAULT NULL,`shopID` varchar(48) CHARACTER SET latin1 DEFAULT NULL,`macList` text CHARACTER SET latin1,`adminPhone` varchar(20) CHARACTER SET latin1 DEFAULT NULL,`adminEmail` text CHARACTER SET latin1,PRIMARY KEY (`id`)) ENGINE=MyISAM DEFAULT CHARSET=utf8;",results[i].table_name),
								function(error,results,fields)
								{
									if(error)
									{
										console.log("Database error:"+error.message)
										return;
									}
									
								}
							)
							
						}
					}
			}
		}
	)
}
 getStringbyhttp1("http://data-api.xiaoyun.com/v1/shop/by/channelID/56383114a993d04421323a83?limit=2&offset=0",function(data)
 	{

 	});

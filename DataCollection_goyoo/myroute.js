var express     = require('express');
var querystring = require('querystring');
var util = require('util');
var 	config = require('./config'),
	moment = require("moment"),
	exec = require('child_process').exec,
	nodemailer = require("nodemailer"),
	cheerio = require('cheerio'),
	serveStatic     = require('express').static,
	path            = require('path'),
	url      = require('url'),
	mysql=	require('mysql'),
	offlineByMacAPI =  require('./outputAPI').offlineByMacAPI,
	testdataname=  require('./outputAPI').testdataname,
	offlineByShopIDAPI=  require('./outputAPI').offlineByShopIDAPI,
	offlineByShopIDAPIChin=  require('./outputAPI').offlineByShopIDAPIChin,
	autoCreatandInsertShopTB=  require('./autoJob').autoCreatandInsertShopTB,
	scheduleDelDataFromSQL=  require('./autoJob').scheduleDelDataFromSQL,
	handlelastOnlineByShopID = require('./outputAPI').handlelastOnlineByShopID,
	getShoptbandShopidbyname= require('./outputAPI').getShoptbandShopidbyname;
//	client;
	var dataCollection; 
	var databaseuser;
	var databasepasswd;
	var midurl
	var query;
function dataCollection(config)
{
	var rootPath=config.rootPath;
}
function sendEmailTousers(config,req)
{
	var fromnow=0;
	var length=100;
	 var params = url.parse(req.url, true).query;
	 //length <= 100 ,offset default is 0
	if(req.query.fromnow)
		fromnow = req.query.fromnow;
	if(req.query.limit)
	{
		var tmp = (req.query.limit.match(/,\d+/))[0].match(/\d+/)[0];
		length = tmp>length?length:tmp;
	}
	else
		req.query.limit="0,100"
	var cmdStr =util.format("mysql -u%s -p%s -e 'select * from upload_db.dev_upload_offline_tb where nowtime >DATE_SUB(now(), INTERVAL (1+%d) DAY)and nowtime < DATE_SUB(now(), INTERVAL (0+%d) DAY) order by mac DESC ,nowtime ASC limit %d,%d;'  -H",databaseuser,databasepasswd,fromnow,fromnow,(req.query.limit.match(/\d+,/))[0].match(/\d+/)[0],length);
	exec(cmdStr,
		{
			encoding: 'utf8',
			timeout: 0,
			maxBuffer: 5000 * 1024, // 默认 200 * 1024
			killSignal: 'SIGTERM'
		},
	function(err,stdout,stderr){
	    if(err) {
	        console.log('get exec api error:'+err);
	    } else {
	       // console.log(stdout);
	       console.log("secceed!");
		var EmailJsons=config.sendToEmail;
		var len=EmailJsons.length;
		 var myhtml = cheerio.load('<html></html>');
		 var myhead = cheerio.load('<head></head>');
		var mytable = cheerio.load(stdout);
		myhtml('html').append(myhead.html());
		myhtml('html').append(mytable.html());
		myhtml('head').append('<title>Get error data list </title>');
		myhtml('head').append('<link href="/handle/public/css/mytable.css" rel="stylesheet" type="text/css" />');
		myhtml('html').append('<script src="/handle/public/js/jquery-2.1.4.min.js"></script>');
		myhtml('html').append('<script>\n$(function(){$("tr:even").addClass("evenrowcolor");\n$("tr:odd").addClass("oddrowcolor"); });</script>');
		for (var i = 0;i<len;i++)
		{
			sendemail(EmailJsons[i].to,EmailJsons[i].subject,myhtml.html());
		}
	    }
	});
	

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
	/*client = new mysql.createConnection(
		{
			host :database.connection.host,
			user :database.connection.user,
			password : database.connection.password,
			database : 'upload_db',
			charset : database.connection.charset
		}
	);*/
/*	console.log("host=%s,user=%s,password=%s,database=%s,charset=%s",database.connection.host,
			database.connection.user,
			database.connection.password,
			'upload_db',
			database.connection.charset);*/
	if (database.client != 'mysql')
		return -1;
	//console.log(database);
	//client.query("SET character_set_client=utf8,character_set_connection=utf8;");
	databaseuser = database.connection.user;
	databasepasswd = database.connection.password;
	return 0;
	  
	/*client.connect(function(error, results) {
	  if(error) {
	    console.log('Connection Error: ' + error.message);
	    return;
	  }
	  console.log('Connected to MySQL');
	});*/
}
function update(req,res)
{
	//
	
	var post = '';
	req.on('data',function(chunk)
			{
				post+=chunk;
			}
	)
	req.on('end',function()
			{
				//post =querystring.parse(post);
				//post = JSON.stringify(post);
				//req.body.data.app.use(express.bodyParser(post));
				post = JSON.parse(post);
				//获取当前时间戳
				var mynow=new Date();
				var timestamp1= Date.parse(mynow)/1000;
				now= moment(mynow).format('YYYY-MM-DD HH:mm:ss');
				
				if (!(post.others))
					post.others="";
				//client.query('INSERT INTO dev_upload_tb SET devtype=\'%s\',nowtime=NOW(), mac=%s, time=%s, propid=\'%s\', wandev=\'%s\', landev=\'%s\',  mem=\'%s\',  cdevlist=\'%s\',  softversion=\'%s\',  others=\'%s\'',
				//	post.devtype,post.mac,post.time ,JSON.stringify(post.propid) ,post.wandev ,JSON.stringify(post.wandev) ,post.mem ,JSON.stringify(post.cdevlist ),post.softversion ,post.others);
			//	console.log('INSERT INTO dev_upload_tb SET devtype=\'%s\',nowtime=NOW(), mac=%s, time=%s, propid=\'%s\', wandev=\'%s\', landev=\'%s\',  mem=\'%s\',  cdevlist=\'%s\',  softversion=\'%s\',  others=\'%s\'',
				//	post.devtype,post.mac,post.time ,JSON.stringify(post.propid) ,JSON.stringify(post.wandev),JSON.stringify(post.landev),post.mem ,JSON.stringify(post.cdevlist ),post.softversion ,post.others);
				query(  
					  util.format("select * from upload_db.dev_upload_tb where nowtime = (select max(nowtime) from upload_db.dev_upload_tb where mac = %s) and mac = %s;",parseInt("0x"+post.mac, 16),parseInt("0x"+post.mac, 16)),  
						function selectCb(error, results, fields) 
						{  
							if(error) 
							{
								console.log("ClientReady Error: " + error.message);
								return;
							}

							query(util.format('INSERT INTO dev_upload_tb SET bandwidth=\'%s\',devtype=\'%s\',nowtime=\'%s\', mac=%s, time=%s, propid=\'%s\', wandev=\'%s\', landev=\'%s\',  mem=\'%s\',  cdevlist=\'%s\',  softversion=\'%s\',  others=\'%s\'',
									JSON.stringify(post.bandwidth),post.devtype,now,parseInt("0x"+post.mac, 16),post.time ,JSON.stringify(post.propid) ,JSON.stringify(post.wandev),JSON.stringify(post.landev),post.mem ,JSON.stringify(post.cdevlist ),post.softversion ,post.others),
								function selectCb(error, results,fields) {
								      if(error) {
								        console.log("ClientReady Error: " + error.message);
								        return;
								      }
								      console.log('Inserted: ' + results.affectedRows + ' row.');
							      console.log('Id inserted: ' + results.insertId);
							    }
							);
							//console.log(post);
							res.send('This my test.');
							//数据非第一次。
							if(results)
							{
								if (results.length>1)
								{
									console.log("mac = %s updata lasttime > 1 ,results.length=%d",results.mac,results.length);
									return;
								}
								if (results[0])
								{
									var lasttime = Date.parse(results[0].nowtime)/1000;
									//大于10秒小于7分钟认为是正常的，否则数据异常产生报警。
									if (((timestamp1 - lasttime)>10)&&((timestamp1 - lasttime)<420) )
										console.log('mac = %s updata statue is succeed!last time is %s,timestamp1 - lasttime=%d ',results[0].mac.toString(16),results[0].nowtime,timestamp1 - lasttime);
									else
									{
										console.log('mac = %s updata statue is error!last time is %s,timestamp1=%d, lasttime=%d ',results[0].mac.toString(16),results[0].nowtime,timestamp1 ,lasttime);
										//把最近一次有效数据存入数据库dev_upload_offline_tb errtype=error
										query(util.format('REPLACE INTO dev_upload_offline_tb SET bandwidth=\'%s\',errtype=\'error\',erruptime=\'%s\',id=%d, devtype=\'%s\',nowtime=\'%s\', mac=%d, time=%s, propid=\'%s\', wandev=\'%s\', landev=\'%s\',  mem=\'%s\',  cdevlist=\'%s\',  softversion=\'%s\',  others=\'%s\'',
												JSON.stringify(results[0].bandwidth),now,results[0].id,results[0].devtype,moment(Date.parse(results[0].nowtime)).format('YYYY-MM-DD HH:mm:ss'),results[0].mac,results[0].time ,results[0].propid,results[0].wandev,results[0].landev,results[0].mem ,results[0].cdevlist ,results[0].softversion ,results[0].others),
											function selectCb(error, results,fields) {
											      if(error) {
											        console.log("ClientReady Error: " + error.message);
											        return;
											      }
												console.log('dev_upload_offline_tb Inserted: ' + results.affectedRows + ' row.');
												console.log('dev_upload_offline_tb Id inserted: ' + results.insertId);
											}
										);
										//把本次正常状态存入数据库dev_upload_offline_tb，errtype=restart .同时id=0.
										query(util.format('REPLACE INTO dev_upload_offline_tb SET bandwidth=\'%s\',errtype=\'restart\', erruptime=\'%s\',id=%d, devtype=\'%s\',nowtime=\'%s\', mac=%d, time=%s, propid=\'%s\', wandev=\'%s\', landev=\'%s\',  mem=\'%s\',  cdevlist=\'%s\',  softversion=\'%s\',  others=\'%s\'',
												JSON.stringify(post.bandwidth),now,0,post.devtype,now,parseInt("0x"+post.mac, 16),post.time ,JSON.stringify(post.propid),JSON.stringify(post.wandev),JSON.stringify(post.landev),post.mem ,JSON.stringify(post.cdevlist ),post.softversion ,post.others),
											function selectCb(error, results,fields) {
											      if(error) {
											        console.log("ClientReady Error: " + error.message);
											        return;
											      }
												console.log('dev_upload_offline_tb Inserted: ' + results.affectedRows + ' row.');
												console.log('dev_upload_offline_tb Id inserted: ' + results.insertId);
											}
										);
									}

								}
								//没有任何记录，证明是数据第一次到达。
								else
								{
									//不做处理
									console.log("new data by this mac=%s",post.mac);
								}
							}
							//没有任何记录，证明是数据第一次到达。
							else
							{
								//不做处理
								console.log("new data by this mac=%s",post.mac);
							}
						}  
					);

			}
	)
}
function updateGetError(req,res)
{
	var fromnow=0;
	var length=100;
	 var params = url.parse(req.url, true).query;
	 //length <= 100 ,offset default is 0
	if(req.query.fromnow)
		fromnow = req.query.fromnow;
	if(req.query.limit)
	{
		var tmp = (req.query.limit.match(/,\d+/))[0].match(/\d+/)[0];
		length = tmp>length?length:tmp;
	}
	else
		req.query.limit="0,100"
	var cmdStr =util.format("mysql -u%s -p%s -e 'select * from upload_db.dev_upload_offline_tb where nowtime >DATE_SUB(now(), INTERVAL (1+%d) DAY)and nowtime < DATE_SUB(now(), INTERVAL (0+%d) DAY) order by mac DESC ,nowtime ASC limit %d,%d;'  -H",databaseuser,databasepasswd,fromnow,fromnow,(req.query.limit.match(/\d+,/))[0].match(/\d+/)[0],length);
	exec(cmdStr,
		{
			encoding: 'utf8',
			timeout: 0,
			maxBuffer: 5000 * 1024, // 默认 200 * 1024
			killSignal: 'SIGTERM'
		},
	 function(err,stdout,stderr){
	    if(err) {
	        console.log('get exec api error:'+err);
	    } else {
	       // console.log(stdout);
	       console.log("secceed!");
		 var myhtml = cheerio.load('<html></html>');
		 var myhead = cheerio.load('<head></head>');
		var mytable = cheerio.load(stdout);
		myhtml('html').append(myhead.html());
		myhtml('html').append(mytable.html());
		myhtml('head').append('<title>Get error data list </title>');
		myhtml('head').append('<link href="/'+midurl+'/public/css/mytable.css" rel="stylesheet" type="text/css" />');
		myhtml('html').append('<script src="/'+midurl+'/public/js/jquery-2.1.4.min.js"></script>');
		myhtml('html').append('<script>\n$(function(){$("tr:even").addClass("evenrowcolor");\n$("tr:odd").addClass("oddrowcolor"); });</script>');
		res.send(myhtml.html());
	    }
	});
}
function updateGetAllByMac(req,res)
{
	var fromnow=0;
	var length=100;
	 var params = url.parse(req.url, true).query;
	 //length <= 100 ,offset default is 0
	if(req.query.fromnow)
		fromnow = req.query.fromnow;
	if(req.query.limit)
	{
		var tmp = (req.query.limit.match(/,\d+/))[0].match(/\d+/)[0];
		length = tmp>length?length:tmp;
	}
	else
		req.query.limit="0,100"
	if(req.query.mac==null)
	{
		res.send("can't get mac req;");
		return;
	}
	var cmdStr =util.format("mysql -u%s -p%s -e 'select * from upload_db.dev_upload_tb where mac = %d and nowtime >DATE_SUB(now(), INTERVAL (1+%d) DAY)and nowtime < DATE_SUB(now(), INTERVAL (0+%d) DAY) order by nowtime DESC limit %d,%d;'  -H",databaseuser,databasepasswd,parseInt("0x"+req.query.mac.replace(/:/g,''), 16),fromnow,fromnow,(req.query.limit.match(/\d+,/))[0].match(/\d+/)[0],length);
	exec(cmdStr,
		{
			encoding: 'utf8',
			timeout: 0,
			maxBuffer: 5000 * 1024, // 默认 200 * 1024
			killSignal: 'SIGTERM'
		},
	 function(err,stdout,stderr){
	    if(err) {
	        console.log('get exec api error:'+err);
	    } else {
	       // console.log(stdout);
	       console.log("secceed!");
		 var myhtml = cheerio.load('<html></html>');
		 var myhead = cheerio.load('<head></head>');
		var mytable = cheerio.load(stdout);
		myhtml('html').append(myhead.html());
		myhtml('html').append(mytable.html());
		myhtml('head').append('<title>Get error data list </title>');
		myhtml('head').append('<link href="/'+midurl+'/public/css/mytable.css" rel="stylesheet" type="text/css" />');
		myhtml('html').append('<script src="/'+midurl+'/public/js/jquery-2.1.4.min.js"></script>');
		myhtml('html').append('<script>\n$(function(){$("tr:even").addClass("evenrowcolor");\n$("tr:odd").addClass("oddrowcolor"); });</script>');
		res.send(myhtml.html());
	    }
	});
}

function sendemail(to,subject,htmlMsg)
{
	var transport = nodemailer.createTransport("SMTP", {
	    host: "smtp.qq.com",
	    secureConnection: true, // use SSL
	    port: 465, // port for secure SMTP
	    auth: {
	        user: "654224414@qq.com",
	        pass: "safuhqvsfgrnbdbi"
	    }
	});
	 
	transport.sendMail({
	    from : "654224414@qq.com",
	    to : to,
	    subject: subject,
	    generateTextFromHTML : true,
	    html : htmlMsg
	}, function(error, response){
	    if(error){
	        console.log(error);
	    }else{
	        console.log("Message sent: " + response.message+" to:"+to);
	    }
	    transport.close();
	});
}
function sendEmail(req,res)
{
	sendEmailTousers(dataCollection,req);
	res.send("sendEmailTousers test!");
}
function testofflineByMac(req,res)
{
	offlineByMacAPI(query,req.query.mac,req.query.fromnow,function(resStr)
		{
			res.send(resStr);
		}
	)
}
function getDatabassInfo()
{
	//return {databaseuser:databaseuser,databasepasswd:databasepasswd};
	return "hah";

}
function mytest(req,res)
{
	testdataname(query);
	res.send("testdataname test");
}
function flashShopTB(req,res)
{
	autoCreatandInsertShopTB(query);
	res.send("flashShopTB succeed");
}
function getDevstatByShopID(req,res)
{
	var fromnow=0;
	if(req.query.fromnow)
			fromnow = req.query.fromnow;
	offlineByShopIDAPIChin(query,req.query.shoptb,req.query.shopid,fromnow,
		function (resString)
		{
			res.send(resString);
		}
	);
	//res.send("getDevstatByShopID succeed");

}
function getDevstatByName(req,res)
{
	if(req.query.Channelname==null||req.query.Shopname==null)
		res.send("input req err!");
	var fromnow=0;
	if(req.query.fromnow)
			fromnow = req.query.fromnow;
	getShoptbandShopidbyname(query,req.query.Channelname,req.query.Shopname,
		function (tbname,shopid)
		{
			offlineByShopIDAPIChin(query,tbname,shopid,fromnow,
				function (resString)
				{
					res.send(resString);
				}
			)
		}
	)
}
function lastOnlineByName(req,res)
{
	if(req.query.Channelname==null||req.query.Shopname==null)
		res.send("input req err!");
	getShoptbandShopidbyname(query,req.query.Channelname,req.query.Shopname,
		function (tbname,shopid)
		{
			handlelastOnlineByShopID(query,tbname,shopid,
				function (resString)
				{
					res.send(resString);
				}
			)
		}
	)
}

function lastOnlineByShopID(req,res)
{
	handlelastOnlineByShopID(query,req.query.shoptb,req.query.shopid,
		function (resString)
		{
			res.send(resString);
		}
	)
}
function myroute (app)
{
	dataCollection= eval("config."+(process.env.NODE_ENV) +".dataCollection");
	midurl = eval("config."+(process.env.NODE_ENV) +".server.midurl");
	app.post('/'+midurl+'/updata',update);
	
	//url: /handle/updategeterror?fromnow=0&limit=1,20
	app.get('/'+midurl+'/updateGetError',updateGetError);
	app.get('/'+midurl+'/updateGetAllByMac',updateGetAllByMac);
	app.get('/'+midurl+'/sendEmailTousers',sendEmail);
	app.get('/'+midurl+'/testofflineByMac',testofflineByMac);
	app.get('/'+midurl+'/testdataname',mytest);
	app.get('/'+midurl+'/flashShopTB',flashShopTB);
	app.get('/'+midurl+'/getDevstat',getDevstatByShopID);
	app.get('/'+midurl+'/lastOnlineByShopID',lastOnlineByShopID);
	app.get('/'+midurl+'/getDevstatByName',getDevstatByName);
	app.get('/'+midurl+'/lastOnlineByName',lastOnlineByName);
	app.use('/'+midurl+'/public',serveStatic(
        path.join(dataCollection.rootPath, '/public'),
        {maxAge: util.ONE_HOUR_MS, fallthrough: false}
    ));
	console.log("This is myroute.");
	connectdatebase(config);
	scheduleDelDataFromSQL(query);
	//console.log(process.config);
}
module.exports = myroute;
module.exports.getDatabassInfo = getDatabassInfo;

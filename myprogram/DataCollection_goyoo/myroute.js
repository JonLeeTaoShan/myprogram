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
	getShoptbandShopidbyname= require('./outputAPI').getShoptbandShopidbyname,
	scheduleCountEverydayError = require('./autoJob').scheduleCountEverydayError;
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
			maxBuffer: 5000 * 1024, // д╛хо 200 * 1024
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
				post = JSON.parse(post);
				if (!(post.others))
					post.others="";
				query(util.format('INSERT INTO dev_upload_tb SET bandwidth=\'%s\',devtype=\'%s\',nowtime=NOW(), mac=%s, time=%s, propid=\'%s\', wandev=\'%s\', landev=\'%s\',  mem=\'%s\',  cdevlist=\'%s\',  softversion=\'%s\',  others=\'%s\'',
						JSON.stringify(post.bandwidth),post.devtype,parseInt("0x"+post.mac, 16),post.time ,JSON.stringify(post.propid) ,JSON.stringify(post.wandev),JSON.stringify(post.landev),post.mem ,JSON.stringify(post.cdevlist ),post.softversion ,post.others),
					function selectCb(error, results,fields) {
					      if(error) {
					        console.log("ClientReady Error: " + error.message);
					        return;
					      }
				      console.log('update inserted succeed mac: %s.' , post.mac);
				    }
				);
				res.send("ok");  
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
	var cmdStr =util.format("mysql -u%s -p%s -e 'select * from upload_db.dev_upload_offline_tb where nowtime >DATE_SUB(now(), INTERVAL (1+%d) DAY)and nowtime < DATE_SUB(now(), INTERVAL (0+%d) DAY) order by mac DESC ,id ASC limit %d,%d;'  -H",databaseuser,databasepasswd,fromnow,fromnow,(req.query.limit.match(/\d+,/))[0].match(/\d+/)[0],length);
	exec(cmdStr,
		{
			encoding: 'utf8',
			timeout: 0,
			maxBuffer: 5000 * 1024, // д╛хо 200 * 1024
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
			maxBuffer: 5000 * 1024, // д╛хо 200 * 1024
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
function forceCountErrByoneMacEveryday(req,res)
{
	scheduleCountEverydayError(query);
	res.send("scheduleCountEverydayError");
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
	app.get('/'+midurl+'/forceCountErrByoneMacEveryday',forceCountErrByoneMacEveryday);
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

var express     = require('express');
var myroutee;
var querystring = require('querystring');
var util = require('util');
var 	config = require('../../config'),
	client;

function connectdatebase(config)
{
	var database = eval("config."+(process.env.NODE_ENV) +".database");
	client = new require('mysql').createConnection(
		{
			host :database.connection.host,
			user :database.connection.user,
			password : database.connection.password,
			database : 'upload_db',
			charset : database.connection.charset
		}
	);
/*	console.log("host=%s,user=%s,password=%s,database=%s,charset=%s",database.connection.host,
			database.connection.user,
			database.connection.password,
			'upload_db',
			database.connection.charset);*/
	if (database.client != 'mysql')
		return -1;
	//console.log(database);
	return 0;
	  
	client.connect(function(error, results) {
	  if(error) {
	    console.log('Connection Error: ' + error.message);
	    return;
	  }
	  console.log('Connected to MySQL');
	});
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
				if (!(post.others))
					post.others="";
				//client.query('INSERT INTO dev_upload_tb SET devtype=\'%s\',nowtime=NOW(), mac=%s, time=%s, propid=\'%s\', wandev=\'%s\', landev=\'%s\',  mem=\'%s\',  cdevlist=\'%s\',  softversion=\'%s\',  others=\'%s\'',
				//	post.devtype,post.mac,post.time ,JSON.stringify(post.propid) ,post.wandev ,JSON.stringify(post.wandev) ,post.mem ,JSON.stringify(post.cdevlist ),post.softversion ,post.others);
			//	console.log('INSERT INTO dev_upload_tb SET devtype=\'%s\',nowtime=NOW(), mac=%s, time=%s, propid=\'%s\', wandev=\'%s\', landev=\'%s\',  mem=\'%s\',  cdevlist=\'%s\',  softversion=\'%s\',  others=\'%s\'',
				//	post.devtype,post.mac,post.time ,JSON.stringify(post.propid) ,JSON.stringify(post.wandev),JSON.stringify(post.landev),post.mem ,JSON.stringify(post.cdevlist ),post.softversion ,post.others);
				
				client.query(util.format('INSERT INTO dev_upload_tb SET devtype=\'%s\',nowtime=NOW(), mac=%s, time=%s, propid=\'%s\', wandev=\'%s\', landev=\'%s\',  mem=\'%s\',  cdevlist=\'%s\',  softversion=\'%s\',  others=\'%s\'',
						post.devtype,parseInt("0x"+post.mac, 16),post.time ,JSON.stringify(post.propid) ,JSON.stringify(post.wandev),JSON.stringify(post.landev),post.mem ,JSON.stringify(post.cdevlist ),post.softversion ,post.others),
					function(error, results) {
					      if(error) {
					        console.log("ClientReady Error: " + error.message);
					        client.end();
					        return;
					      }
					      console.log('Inserted: ' + results.affectedRows + ' row.');
				      console.log('Id inserted: ' + results.insertId);
				    }
				);
				//console.log(post);
				res.send('This my test.');
			}
	)
}
function myroute (app)
{
	//myroutee = express();
	//myroutee.get('update',update);
	app.post('/handle/update',update);
	console.log("This is myroute.");
	connectdatebase(config);
	//console.log(process.config);
}
module.exports = myroute;

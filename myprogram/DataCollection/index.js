var express	= require('express');
var myroute	= require('./myroute');
var config	= require('./config');
var http		= require('http');
var morgan	= require('morgan');
var errorhandler= require('errorhandler'); 
var path	= require('path');
var offlineByMacAPI =  require('./outputAPI').offlineByMacAPI;
process.env.NODE_ENV = process.env.NODE_ENV || 'development';
var port = eval("config."+(process.env.NODE_ENV) +".server.port");
var app = express();
app.set('port',port);
app.use(morgan('dev'));
myroute(app);
if (process.env.NODE_ENV == 'development')
{
	app.use(errorhandler());
}
http.createServer(app).listen(app.get('port'),function ()
	{
		console.log('Express server listening on port ' + app.get('port'));
	}
)

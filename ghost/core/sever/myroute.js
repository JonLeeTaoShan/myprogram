function update(req,res)
{
	console.log(req.param);
}
function myroute (app)
{
app.get('/handle/update',update)
}
Module.exports = myroute;

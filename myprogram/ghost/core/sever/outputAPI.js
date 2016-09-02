function offlineByMac(dataclient,mac,fromnow,callback)
{
	dataclient.query(  
						  util.format("select unix_timestamp(nowtime) nowtime,errtype from upload_db.dev_upload_offline_tb where mac=%d and nowtime >DATE_SUB(now(), INTERVAL (1+%d) DAY)and nowtime < DATE_SUB(now(), INTERVAL (0+%d) DAY);",parseInt("0x"+mac, 16),fromnow,fromnow),  
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
function offlineByMacAPI(dataclient,mac,fromnow,callback)
{
	offlineByMac(dataclient,mac,fromnow,function(os,ot)
		{
			callback(((1-os/(3600*24))*100).toString());
		})
}
module.exports.offlineByMacAPI = offlineByMacAPI;

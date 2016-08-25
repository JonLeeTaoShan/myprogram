#! /usr/bin/env lua
local uci   = require "los.uci"
local _uci = uci.cursor()
local util = require "los.util"
local nixio = require "nixio"
local fs     = require "nixio.fs"
local luci = require "luci"
local uci  = require "los.uci".cursor()
local json = require "los.json"
--local https = require "ssl.https"
local https = require "ssl.https"
local function getPidtable()
	resTable={}
	resTable.dropbear		=	util.exec("pidof dropbear|sed 's/ /,/g'|tr -d '\n'")
	resTable.socat			=	util.exec("pidof socat|sed 's/ /,/g'|tr -d '\n'")
	resTable.pppd			=	util.exec("pidof pppd|sed 's/ /,/g'|tr -d '\n'")
	resTable.levent		=	util.exec("ps|grep levent.lua|grep -v grep |awk '{print $1}'|tr -d '\n'")
	resTable["redis-server"]	=	util.exec("pidof redis-server|sed 's/ /,/g'|tr -d '\n'")
	resTable["port-mirroring"]		=	util.exec("pidof port-mirroring|sed 's/ /,/g'|tr -d '\n'")
	return resTable
end
local function getwanTable()
	local wanTable={}
	for oneWan in util.execi("uci -q show network|grep wan|grep proto|tr -d \\'")
	do
		local wname	=string.match(oneWan,"wan[^.]*")

		local proto		= string.match(string.match(oneWan,"=%w+"),"%w+")

		--print("string="..oneWan)
		if wname and proto then
			local ipaddr
			local gw
			local ifname=util.exec("uci -q get network."..wname..".ifname|tr -d '\n'")
			if proto == "static" or proto == "dhcp" then
				ipaddr	= util.exec("ip add show "..ifname.." |grep 'inet '|awk '{print $2}'|tr -d '\n'")
				gw		= util.exec("route -n |grep "..ifname.." |awk '{print $2}'|grep -v 0.0.0.0|tr -d '\n'")
			else
				ipaddr	= util.exec("ip add show "..proto..'-'..wname.."|grep 'inet '|awk '{print $2}'|tr -d '\n'")
				gw		= util.exec("route -n |grep "..proto..'-'..wname.." |awk '{print $2}'|grep -v 0.0.0.0|tr -d '\n'")
			end
			
			wanTable[wname] = string.format("%s,%s,%s,%s",ifname,proto,ipaddr,gw)
			--print("wname="..wanTable.wname )
		end
	end
	return wanTable
end
local function getssidListdev()
	local ssidListTable={}
	for mydev in util.execi("/script/list |sed '1d'|awk '{print $1,$2,$3,$4,$5,$6,$7,$8,$9}'|sed 's: :,:g'")
	do
		local devmac		= string.match(mydev,"[%w:]+")
		local devinfo		= string.match(string.match(mydev,",[^\n]+"),"%d[^\n]+")
		ssidListTable[devmac] = devinfo
		--print("devmac="..devinfo )
	end
	return ssidListTable
end
local function Entry()
	local res = {}
	local body={}
	local source
	local sink
	--防止高并发
	util.exec("while [ 1 ] ;do [ `expr $(date +%s) % 6` -eq `expr $((0x$(ip link show br-lan|grep 'ether'|awk '{print $2}'|awk -F':' '{print $6}'))) % 6` ]&& break;sleep 1; done")
	url = "https://dev-stat.xiaoyun.com:8080/handle/updata"
	body.devtype =util.exec("cat /etc/model|tr -d '\n'")
	body.mac =util.exec("ip link show br-lan|grep 'ether'|awk '{print $2}'|sed  's/://g'|tr -d '\n'")
	body.time =util.exec("cat /proc/uptime |awk -F '.' '{print $1}'|tr -d '\n'")
	body.propid =getPidtable()
	body.wandev =getwanTable()
	body.landev={dev=util.exec("uci -q get network.lan.ifname|sed 's: :,:g'|tr -d '\n'"),ip=util.exec("ip add show br-lan |grep 'inet '|awk '{print $2}'|tr -d '\n'")}
	body.softversion=util.exec("cat /etc/version|tr -d '\n'")
	body.others=nil
	body.mem=util.exec("free |awk '{print $3,$2}'|sed -n '2p'|tr ' ' '/'|tr -d '\n'")
	body.cdevlist=getssidListdev()
	body.cpu=util.exec("top -n 1 -b|grep CPU:|grep -v grep |awk  '{printf(\"%s:%s,%s:%s,%s:%s,%s:%s,%s:%s,%s:%s,%s:%s\\n\",$3,$2,$5,$4,$7,$6,$9,$8,$11,$10,$13,$12,$15,$14);}'|tr -d '\n'")
	body.bandwidth=json.decode(util.exec("cat /tmp/bandwidth.data ||echo nil")	)
	local bodyjson = json.encode(body)
	local one,code,headers,status = https.request{
		url = url,
		method = "POST",
		headers = { 
			["User-Agent"] = "Mozilla/5.0 (Macintosh; Intel Mac OS X 10_10_5) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/50.0.2661.86 Safari/537.36",
			["Connection"] = "keep-alive",
			["Accept-Language"] = "zh-CN,zh;q=0.8",
			["Accept-Encoding"] = "gzip, deflate, sdch",
			["content-length"] = string.len(bodyjson),
			["Accept"] = "text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,*/*;q=0.8"
            },
 --          key = "/root/client.key",
 --          certificate="/root/client.crt",
 --          cafile="/root/ca.crt"
            source = ltn12.source.string(bodyjson),
       sink = ltn12.sink.table(res)
  	}
       
	--print("code="..code..",headers="..json.encode(headers)..",res="..json.encode(res))
	return code
end
local res = 0            
local i=10                     
while (i > 0 and res ~= 200) 
do                               
	res = Entry()
	if res == nil then     
		https = require "socket.http"
	end                      
	res = Entry()      
	print(res)                     
	https = require "ssl.https"                  
        i = i -1               
        nixio.nanosleep(1, 0)    
end 

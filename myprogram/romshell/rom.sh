#!/bin/ash
#ROM_URL='http://router-api.xiaoyun.com/upgrade/rom/<%= model %>/<%= version %>.bin'
#ROM_FILENAME=$(basename $ROM_URL)
#MD5SUM_URL='http://router-api.xiaoyun.com/upgrade/md5sum/<%= model %>/<%= version %>'
#rm -rf /tmp/*.bin /tmp/md5sums
#rm -rf /tmp/private_cache
#rm -rf /tmp/public_cache
#rm -rf /var/log/messages
#wget -O "/tmp/${ROM_FILENAME}" $ROM_URL
#wget -O /tmp/md5sums $MD5SUM_URL
TMP_FILENAME=$1
ROM_FILENAME=$(basename $TMP_FILENAME)
md5sum $TMP_FILENAME >/tmp/md5sums
cd /tmp
test -z "$(md5sum -c md5sums 2>/dev/null | grep 'OK')" && exit 1

/etc/init.d/cron stop
/etc/init.d/remoteTTY stop

uci -q show network | grep -E 'network.(w|l)an' | grep -E -v 'macaddr|type' | awk '{print "uci set",$0}' > /etc/rc.local
uci -q show multiwan | grep -E 'multiwan.(w|l)an' | grep -E -v 'macaddr|type' | awk '{print "uci set",$0}' >>/etc/rc.local
[ "$(uci -q get dhcp.lan.ignore)" == "1" ] && echo "uci set dhcp.lan.ignore=1" >> /etc/rc.local
uci -q show port-mirroring | grep -E -v 'target|source_ports|rotate_time|rotate_size|rotate_count' | awk '{print "uci set", $0}' >> /etc/rc.local

uci -q show lewifi | grep -E 'lewifi\.(portal|proxy|arpscan)\.enable' | awk '{print "uci set", $0}' >> /etc/rc.local

uci -q show wireless | grep -E 'wifi-iface\[(1|3)\]\.' | grep -E -v 'device|ifname|network|mode' | awk '{print "uci set", $0}' >> /etc/rc.local

MOD=$(cat /etc/model)
VER=$(cat /etc/version)

YUNWIFIVER="02.14.11.30"
YUNWIFINOHIDDEN="02.14.12.03"
PUBLICSSID="YunWiFi"

if [ "$VER" \< "$YUNWIFIVER" -o "$VER" = "$YUNWIFINOHIDDEN" -o "$VER" \> "$YUNWIFINOHIDDEN" ]
then
    PUBLICSSID=$(uci -q get wireless.@wifi-iface[0].ssid)
    echo "/script/lewifi_config set ssid $PUBLICSSID" >> /etc/rc.local
fi
if [ "$VER" \> "$YUNWIFIVER" -a "$VER" \< "$YUNWIFINOHIDDEN" ]
then
    PUBLICSSID=$(uci -q get wireless.@wifi-iface[1].ssid)
    echo "/script/lewifi_config set ssid $PUBLICSSID" >> /etc/rc.local
fi

cat <<EOF >>/etc/rc.local
uci commit
/etc/init.d/network restart
/etc/init.d/dnsmasq restart
/etc/init.d/firewall restart
/etc/init.d/recvcmd restart
/etc/init.d/port-mirroring restart
/etc/init.d/multiwan restart
sleep 15&&/etc/init.d/dropbear restart
cp /rom/etc/rc.local /etc/rc.local
EOF

if [ -e "/etc/user_whitelist" ]
then
    tar zcf /tmp/backup.tgz -C / etc/rc.local etc/hosts etc/shadow etc/user_whitelist
else
    tar zcf /tmp/backup.tgz -C / etc/rc.local etc/hosts etc/shadow
fi

if [ "$MOD" = "a1" -a "$VER" \< "03.15.12.05" ]
then
    sysupgrade -n "/tmp/${ROM_FILENAME}"
else
    sysupgrade -n -f /tmp/backup.tgz "/tmp/${ROM_FILENAME}"
fi


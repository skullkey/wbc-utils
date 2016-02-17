sudo ifconfig $1 down
sudo iw dev $1 set monitor otherbss fcsfail
sudo ifconfig $1 up
sudo iw reg set RU
sudo iwconfig $1 channel 13


sudo ../../../20160215_wifibroadcast/wifibroadcast/rx -p 0 -b 8 -r 4 -f 1024 $1  | socat -b 1024 - UDP4-DATAGRAM:192.168.42.129:5000 &
sleep 2
sudo ../../../20160215_wifibroadcast/wifibroadcast/rx -p 1 -b 8 -r 4 -f 1024 $1  | sudo ./mavlink_json_osd | socat -b 1024 - UDP4-DATAGRAM:192.168.42.129:5001

sudo pkill rx

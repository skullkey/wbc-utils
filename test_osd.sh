sudo /home/pi/wifibroadcast/rx -p 0 -b 8 -r 4 -f 1024 wlan0 > /dev/null &
sudo /home/pi/wifibroadcast/rx -p 1 -b 8 -r 4 -f 1024 wlan0 | sudo ./mavlink_omx_osd_SBS .
sudo pkill rx

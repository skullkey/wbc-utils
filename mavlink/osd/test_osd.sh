sudo	ifconfig wlan0 down
sudo	iw dev wlan0 set monitor otherbss fcsfail
sudo	ifconfig wlan0 up
sudo	iwconfig wlan0 channel 13

sudo	ifconfig wlan1 down
sudo	iw dev wlan1 set monitor otherbss fcsfail
sudo	ifconfig wlan1 up
sudo	iwconfig wlan1 channel 13


sudo /home/pi/wifibroadcast/rx -p 0 -b 8 -r 4 -f 1024 wlan0 wlan1 | /opt/vc/src/hello_pi/hello_video/hello_video.bin > /dev/null &
sudo /home/pi/wifibroadcast/rx -p 1 -b 8 -r 4 -f 1024 wlan0 wlan1 | sudo ./mavlink_omx_osd .
.
sudo pkill rx

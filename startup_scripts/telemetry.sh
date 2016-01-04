#!/bin/bash
# tx script


#adapt these to your needs
NIC="wlan0"
CHANNEL2G="13"
CHANNEL5G="149"

WIDTH=1280
HEIGHT=720
FPS=48
BITRATE=4000000
KEYFRAMERATE=48

##################################

#change these only if you know what you are doing (and remember to change them on both sides)
BLOCK_SIZE=8
FECS=4
PACKET_LENGTH=1024
PORT=0
OSD_PORT=1

##################################

WBC_PATH="/home/pi/wifibroadcast"
NAZA_PATH="/home/pi/naza_telemetry"

# Make sure only root can run our script
if [[ $EUID -ne 0 ]]; then
   echo "This script must be run as root" 1>&2
   exit 1
fi

#wait a bit. this helps automatic starting
sleep 2

stty -F /dev/ttyAMA0 -imaxbel -opost -isig -icanon -echo -echoe -ixoff -ixon 115200
while :
do
	echo "Starting tx for $NIC for telemetry"
	cat /dev/ttyAMA0   | $WBC_PATH/tx -p $OSD_PORT -b $BLOCK_SIZE -r $FECS -f $PACKET_LENGTH $NIC
	killall cat
        ps -ef | grep "tx -p $OSD_PORT" | grep -v grep | awk '{print $1}' | xargs kill -9
done
#>> /home/pi/tx.log 2>&1

#killall raspivid
#killall tx

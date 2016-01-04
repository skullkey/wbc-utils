#!/bin/bash
# tx script


#adapt these to your needs
NIC="wlan0"
CHANNEL2G="13"
CHANNEL5G="149"

WIDTH=1280
HEIGHT=720
FPS=49
BITRATE=3000000
KEYFRAMERATE=8

##################################

#change these only if you know what you are doing (and remember to change them on both sides)
BLOCK_SIZE=8
FECS=4
PACKET_LENGTH=1024
PORT=0
TELEMETRY_PORT=1
##################################

WBC_PATH="/home/pi/wifibroadcast"

function prepare_nic {
	DRIVER=`cat /sys/class/net/$1/device/uevent | grep DRIVER | sed 's/DRIVER=//'`

	case $DRIVER in
		ath9k_htc)
			echo "Setting $1 to channel $CHANNEL2G"
			ifconfig $1 down
			iw dev $1 set monitor otherbss fcsfail
			ifconfig $1 up
			iwconfig $1 channel $CHANNEL2G
		;;
		rt2800usb) echo "$DRIVER new shit"
			echo "Setting $1 to channel $CHANNEL5G"
			ifconfig $1 down
			iw dev $1 set monitor otherbss fcsfail
			ifconfig $1 up
			iw reg set BO
			iwconfig $1 rate 24M
			iwconfig $1 channel $CHANNEL5G
		;;
		*) echo "ERROR: Unknown wifi driver on $1: $DRIVER" && exit
		;;
	esac
}

# Make sure only root can run our script
if [[ $EUID -ne 0 ]]; then
   echo "This script must be run as root" 1>&2
   exit 1
fi

#wait a bit. this helps automatic starting
sleep 2

prepare_nic $NIC


echo "Starting tx for $NIC"
#raspivid -ih -t 0 -w $WIDTH -h $HEIGHT -fps $FPS -b $BITRATE -n -g $KEYFRAMERATE -ex sports -awb horizon  -pf high  -o - | $WBC_PATH/tx -p $PORT -b $BLOCK_SIZE -r $FECS -f $PACKET_LENGTH $NIC

#$WBC_PATH/tx -p $PORT -b $BLOCK_SIZE -r $FECS -f $PACKET_LENGTH -m 64 -s 2  $NIC &
#/home/pi/wifibroadcast_fpv_scripts/telemetry.sh &
stty -F /dev/ttyAMA0 -imaxbel -opost -isig -icanon -echo -echoe -ixoff -ixon 115200
cat /dev/ttyAMA0 | $WBC_PATH/tx -p $TELEMETRY_PORT  -b $BLOCK_SIZE -r $FECS -f $PACKET_LENGTH  $NIC  &
raspivid -ih -t 0 -w $WIDTH -h $HEIGHT -fps $FPS -b $BITRATE -n -g $KEYFRAMERATE -ex sports -awb horizon  -pf high  -o - | $WBC_PATH/tx -p $PORT  -b $BLOCK_SIZE -r $FECS -f $PACKET_LENGTH  $NIC  
killall cat
killall raspivid
killall tx

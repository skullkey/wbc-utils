#!/bin/bash

sleep 5
chvt 2
SIMPLE_PATH=/home/pi/wbc-utils/mavlink/simple-control
cd $SIMPLE_PATH

function clearfb {
           dd if=/dev/zero of=/dev/fb0 &> /dev/null
}

function killall {
    systemctl stop wbcrxd
    pkill ss2d
    cd $SIMPLE_PATH
    clearfb
}

function help {
    clearfb
    echo "1 - Wbcrxd Start"
    echo "2 - Toggle rx status"
    echo "3 - Reset home position"
    echo "4 - Reset latency indicator"
    echo "5 - Wbcrxd Stop"
    echo "6 - Kill OSD"
    echo "7 - Spectral scan"
    echo "0 - Shutdown"
    echo "?"


}

clearfb
while true; do
    read -s -n 1 key

    case "$key" in
	1) killall
	   systemctl restart wbcrxd
	   ;;
	2) echo 'd' > /tmp/OSDFIFO &
	   ;;	
	3) echo 'h' > /tmp/OSDFIFO &
	   ;;	
        4) echo 't' > /tmp/OSDFIFO &
           ;;
	5) systemctl stop wbcrxd
	   ;;
	6) pkill osd
	   ;;
	7) killall
	   sleep 1
	   cd /home/pi/wbc-utils/ath9k-spectral-scan-2d
	   ./ss2d &
	   ;;
	0) killall
           echo "About to shutdown, are you sure? y/n"
           read -s -n 1 sure
           if [ $sure == 'y' ]; then
                 shutdown -h now
           else
                 clear
                 help
           fi
	   ;;
	*) help;;
    esac    
done




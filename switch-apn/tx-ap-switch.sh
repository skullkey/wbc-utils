#! /bin/bash
sudo raspi-gpio set 17 pu

lastval=1
while :
do
	val=`sudo raspi-gpio get 17`
	if [[ "$val" =~ "level=0" ]]
		then 
			if [ $lastval == 1 ]
				then
 					lastval=0
					echo "Turning on APN"
					switch_flip_wbctxd_apn apn
					
				fi 
		fi
        if [[ "$val" =~ "level=1" ]]
                then 
                        if [ $lastval == 0 ]
                                then
                                        lastval=1
                                        echo "Turning on wbctxd"
					switch_flip_wbctxd_apn wbctxd
                                fi 
                fi

        sleep 1
done

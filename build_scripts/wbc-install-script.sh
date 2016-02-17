#!/bin/bash

set -e
sudo apt-get update

#install wbc-utils
cd /home/pi
git clone https://github.com/skullkey/wbc-utils.git

#install ath9k_spectral_scan
cd /home/pi/
cd wbc-utils/ath9k-spectral-scan-2d
git clone https://github.com/kazikcz/ath9k-spectral-scan
cd ath9k-spectral-scan
make
sudo apt-get -y install gnuplot-nox fbi

#patch hello_video - adds the alarm SIGNAL to make sure it gets killed when freezing
cd /home/pi
cd wbc-utils/hello_video
sudo cp video.c /opt/vc/src/hello_pi/hello_video/video.c
cd /opt/vc/src/hello_pi/
sudo ./rebuild.sh

#install openvg
cd /home/pi
cd wbc-utils
sudo apt-get -y install libjpeg8-dev indent libfreetype6-dev ttf-dejavu-core
git clone https://github.com/SamuelBrucksch/openvg
cd openvg
make
make library
sudo make install

#install mavlink clibrary
cd /home/pi
cd wbc-utils
git clone https://github.com/mavlink/c_library

#install mavlink
cd /home/pi
cd wbc-utils/mavlink/osd
make

#install simple-control.service
cd /home/pi
cd wbc-utils/mavlink/simple-control
sudo cp simple-control.service /etc/systemd/system/
sudo systemctl enable simple-control.service

#add improved systemctl script for wbcrxd
cd /home/pi
cd wbc-utils/startup_scripts
sudo cp wbcrxd.service /etc/systemd/system/
sudo systemctl enable simple-control.service

#backup installed startup scripts
cd /home/pi
cd wifibroadcast_fpv_scripts/
mv tx.sh tx.sh.orig
mv rx.sh rx.sh.orig

# install new startup scripts
cd /home/pi
cp wbc-utils/startup_scripts/tx.sh wifibroadcast_fpv_scripts/tx.sh
cp wbc-utils/startup_scripts/rx.sh wifibroadcast_fpv_scripts/rx.sh

# Manually 
echo "You will have to disable the serial port using raspi-config.  Simple type sudo raspi-config"




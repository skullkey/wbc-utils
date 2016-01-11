sudo su

# Build essentials
#apt-get -y install build-essential linux-headers-rpi-rpfv libpcap-dev python-m2crypto libnl-dev libgcrypt11-dev
apt-get -y install python-m2crypto libnl-dev libgcrypt11-dev

#Sources
cd /home/pi
mkdir src
cd src
wget https://www.kernel.org/pub/software/network/crda/crda-3.18.tar.xz
wget https://www.kernel.org/pub/software/network/wireless-regdb/wireless-regdb-2015.04.06.tar.xz
wget https://kernel.org/pub/software/network/iw/iw-3.10.tar.xz
tar xvf crda-3.18.tar.xz
tar xvf wireless-regdb-2015.04.06.tar.xz
tar xvf iw-3.10.tar.xz

#Updated db.txt
cd /home/pi
cd src/wireless-regdb-2015.04.06
mv db.txt db.txt.orig
cp /home/pi/wbc-utils/build_scripts/db.patched.txt db.txt

# make regulatory.bin
make clean
make uninstall
make
./db2bin.py regulatory.bin db.txt ~/.wireless-regdb-root.key.priv.pem
make install

# make CRDA
cd /home/pi
cd src/crda-3.18
cp -v ../wireless-regdb-2015.04.06/*.key.pub.pem ./pubkeys/
make
make install

#make iw
cd /home/pi
cd src/iw-3.10
make
make install

#reboot and test
sudo reboot

Building
========

Prerequisites
------------
1. Pre-built image of wifibroadcast from https://github.com/befinitiv/rpi_wifibroadcast_image_builder/releases/latest   - This guide was compiled using v0.4
2. Transfer the image to an SD card
dd if=2015-09-24-raspbian-jessie_2016-01-09.img of=/dev/mmcXXX bs=8M
where /dev/mmcXXX correspond to the /dev entry for your SD card - not the partition, i.e. not /dev/mmcXXXp1
3. Mount the SD card
mkdir mnt/fat32
mkdir mnt/ext4
sudo mount /dev/mmcXXXp1 mnt/fat32
sudo mount /dev/mmcXXXp2 mnt/ext4

Getting ready to compile
------------------------
It is possible to compile the kernel on the PI, however the steps below assume cross compiling (see https://www.raspberrypi.org/documentation/linux/kernel/building.md for more details)


1. Install build essentials
sudo apt-get update
sudo apt-get install -y  git-core gnupg flex bison gperf libesd0-dev build-essential zip curl libncurses5-dev zlib1g-dev gcc-multilib g++-multilib
sudo dpkg --add-architecture i386
sudo apt-get update
sudo apt-get install lib32z1 lib32ncurses5 lib32bz2-1.0 lib32stdc++6

2.  Clone this repo

git clone https://github.com/skullkey/wbc-utils.git


3. Install the toolchain
git clone https://github.com/raspberrypi/tools

4. Update the path
export PATH=$PATH:tools/arm-bcm2708/gcc-linaro-arm-linux-gnueabihf-raspbian/bin

5. Get the latest kernel source
git clone --depth=1 https://github.com/raspberrypi/linux


A typical folder structure for the kernel patching, configuring and building would look like this after completing the above steps:
~/stuff/
~/stuff/wbc-utils
~/stuff/linux
~/stuff/tools



Patch the kernel for 30dBm output and 2.3-2.4 GHz (optional)
--------------------------------------------------------
The downloaded image is pre-patched with constant power output of 20dBm - this should be sufficient for most uses.  However it is possible to increase the power output (given that your device can deliver) and frequency range.  Check the regulations in your country regarding frequency and power output before proceeding!  This step is at your own risk, please act responsibly.

If you are unsure about the legality or whether your device will be able to survive the increase in power, rather skip this step.

1. Patch the latest kernel source with patches from the wbc-utils/build_scripts/kernel_patches folder
cd linux
cat ../wbc-utils/build_scripts/kernel_patches/*|patch --verbose -p1

Configure & build the kernel
--------------------
1. Config for Pi 1
cd linux
KERNEL=kernel
cp ../wbc-utils/build_scripts/bcmrpi.config.with.ath9k.debugging .config
make ARCH=arm CROSS_COMPILE=arm-linux-gnueabihf- old_config

2. For Pi 2:
cd linux
KERNEL=kernel7
cp ../wbc-utils/build_scripts/bcm2709.config.with.ath9k-debugging .config
make ARCH=arm CROSS_COMPILE=arm-linux-gnueabihf- old_config

3. Then for both:
make -j16 ARCH=arm CROSS_COMPILE=arm-linux-gnueabihf- zImage modules dtbs

4. Install onto the SD card
sudo make ARCH=arm CROSS_COMPILE=arm-linux-gnueabihf- INSTALL_MOD_PATH=mnt/ext4 modules_install
sudo cp mnt/fat32/$KERNEL.img mnt/fat32/$KERNEL-backup.img
sudo scripts/mkknlimg arch/arm/boot/zImage mnt/fat32/$KERNEL.img
sudo cp arch/arm/boot/dts/*.dtb mnt/fat32/
sudo cp arch/arm/boot/dts/overlays/*.dtb* mnt/fat32/overlays/
sudo cp arch/arm/boot/dts/overlays/README mnt/fat32/overlays/


Copy the install script onto the SD Card
----------------------------------------
1. Copy wbc-install-script.sh
cp wbc-utils/build_scripts/wbc-install-script.sh mnt/ext4/home/pi/

2. Copy regdb-crda-install.sh (optional - only if you patched the kernel above)
cp wbc-utils/build_scripts/regdb-crda-install.sh mnt/ext4/home/pi/

3. Unmount the sdcard
sudo umount mnt/fat32
sudo umount mnt/ext4


Boot the pi and run the install script
-------------------------------------
1. This is the basic installer
chmod u+x wbc-install-script.sh
sudo ./wbc-install-script.sh

2. This is optional and only needed if the kernel patches were applied
chmod u+x regdb-crda-install.sh
sudo su
./regdb-crda-install.sh
exit

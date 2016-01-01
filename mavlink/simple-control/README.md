==What is this==
A systemd service file and bash script that starts on tty2 on boot and allows the user to toggle some of the OSD features

==How to use==
Copy the simple-control.service file to install 
sudo cp simple-control.service /etc/systemd/system/

And enable it
sudo systemctl enable simple-control.service

Once rebooted keyboard input will toggle OSD features - see the watch-keys.sh file for details

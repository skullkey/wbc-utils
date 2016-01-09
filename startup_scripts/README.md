What is this?
===========
These scripts should be used to modify the corresponding scripts in the Raspberry Pi's wifibroadcast_fpv_scripts folder.

Details
===========
1. rx.sh will start the mavlink osd
2. To make rx more robust (systemd ignore SIGPIPE by default), replace wbcrxd.service in /etc/systemd/system.  If you are using the hello_video as part of these tools, you must install this
3. tx.sh starts telemetry.sh
4. telemetry.sh sets up the UART, encode naza gps to mavlink and transmits

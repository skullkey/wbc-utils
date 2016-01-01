==What is this?==
Mavlink based OSD for the wifibroadcast system by Befi (https://befinitiv.wordpress.com/wifibroadcast-analog-like-transmission-of-live-video-data/)

Directly based on a dropbox share by <nobie>  https://www.dropbox.com/s/1ehq0ytun5ns6no/osd.zip?dl=0 on https://befinitiv.wordpress.com/2015/07/06/telemetry-osd-for-wifibroadcast/

Nobie used openvg (https://github.com/ajstarks/openvg) to render the osd.


==What's new==
1. Some mods for screen layout added and everything is green
2. rx status display 
3. fifo to toggle display options during flight
* echo 'd' > /tmp/OSDFIFO
  Will toggle the display of rxstatus
* echo 'h' > /tmp/OSDFIFO
  Reset the home position - handy for capturing your take-off point
* echo 't' > /tmp/OSDFIFO
  Sync the receiver and remote time, used to debug latency drift issues.


==Building==
1. Install openvg
Follow the instructions here: https://github.com/ajstarks/openvg

2. Get Mavlink c_library 
cd wbc-utils
git clone https://github.com/mavlink/c_library 

3. And Make
cd mavlink/osd/
make



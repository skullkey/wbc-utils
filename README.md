What is this?
==========
Wifibroadcast developed by Befi (https://befinitiv.wordpress.com/wifibroadcast-analog-like-transmission-of-live-video-data/) is video broadcast system using WiFi equipment.

This repository contains some tools for the wifibroadcast system based on other opensource projects, dropbox shares, code snippets, tutorials and hacks.

Building
======
Some instructions on how to build and dependencies can be found in each of the sub-projects.

In general, grab the latest image from Befi (https://befinitiv.wordpress.com/wifibroadcast-analog-like-transmission-of-live-video-data/).

Then clone this repository into /home/pi and follow the instructions in the readme's

TX
--
On the transmitting side:
1. Some modified startup scripts to hook in the telemetry and have it start with the tx
2. NazaTelemetry - reads NAZA GPS data and converts to mavlink.  Based on the work by Pawelsky for the Arduino (http://www.rcgroups.com/forums/showthread.php?t=1995704) .  Modified and changed into a command line util.

You will need to build a cable for the Naza as described on the site.  However note: Raspberry PI is 3.3V and NAZA is 5V - do not connect directly - you will have to add a level shifter.  It will not work if you just connect it directly!!!
![Do no use this cable directly - add a level shifter](http://static.rcgroups.net/forums/attachments/3/9/3/9/3/6/t6327065-121-thumb-osd_cable.jpg?d=1386792640)
 
RX
--
1. Mavlink  OSD - displays mavlink telemetry and rx status during flight.  Based on a [dropbox link](https://www.dropbox.com/s/1ehq0ytun5ns6no/osd.zip?dl=0) shared by Nobie (on the [wifibroadcast blog](https://befinitiv.wordpress.com/2015/07/06/telemetry-osd-for-wifibroadcast/)) 
2. Ath9k based spectral scan - displays a 2d histogram of frequency usage.  The idea is to "see" how noisy the frequency spectrum is before launching.  Uses the [ath9k-spectral-scan utility](https://github.com/kazikcz/ath9k-spectral-scan) developed by Michał Kazior to draw a 2d histogram with gnuplot.
3. Simple-Control service - Ability to toggle display options and switch between the osd and scanner display. Instead of a full sized keyboard, an Arduino simulated keyboard can also be used. 
4. Hello-video - is a video player included in the Raspbian distribution.  Befi modified the player to read stdin.  During my testing, the player would sometime hang when really bad data is received and the code was modified to make the player a bit more robust by adding an alarm signal in the render/decode loop.




What is it?
==========
The hello_video utility is part of Raspbian and was modified by Befi as part of the wifibroadcast system to enable playback of live streams, the modified code is here: https://bitbucket.org/befi/hello_video

If the broadcasted stream is extremely noisy, the decoder sometimes get stuck, leaving the image frozen for the pilot - without the process getting killed!

To enable systemd to recover from this, an alarm signal was added to the render loop.

How to use?
==========
Replace the video.c file in /opt/vc/src/hello_pi/hello_video in the wifibroadcast image and then rebuild using /opt/vc/src/hello_pi/rebuild.sh

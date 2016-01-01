==What is this?==
A utility for the Raspberry Pi to read Naza GPS and compass data and encode as Mavlink.  

This is based on NazaDecoder20141130 from http://www.rcgroups.com/forums/showthread.php?t=1995704

You will need to make the cable as detailed in that link and connect to the UART on the Raspberry PI
NB! - The Raspberry Pi is 3.3V and the data from the NAZA GPS is 5V - do not connect these!!! Add a level shifter (5V to 3.3V) between the GPS output and the Raspberry PI.



==Build==
To build on the pi:
cd wbc-utils
git clone https://github.com/mavlink/c_library
cd naza_telemetry
make

==Details==
setup.sh is useful to setup the UART during testing

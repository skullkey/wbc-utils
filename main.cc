#include <stdint.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include "NazaDecoderLib.h"
#include <mavlink.h>
#include <sys/time.h>
#include <iostream>
using namespace std;


uint8_t buf[1];
long lastsent=0;
long lastcompass=0;
mavlink_gps_raw_int_t packet_gps;
mavlink_attitude_t packet_attitude;
mavlink_global_position_int_t packet_global_pos;

float ToRad(float degree){
  
   return degree /180.0f * 3.1415927; 
  
}

long millis(){
	struct timeval now;

	long mtime, seconds, useconds;    

	gettimeofday(&now, NULL);

	seconds  = now.tv_sec;  
	useconds = now.tv_usec; 
	mtime = ((seconds) * 1000 + useconds/1000.0) + 0.5;
    return(mtime);
}

int main(int argc, char *argv[]) {
	int system_type = MAV_TYPE_FIXED_WING;
	int autopilot_type = MAV_AUTOPILOT_GENERIC;//MAV_AUTOPILOT_ARDUPILOTMEGA;//

	mavlink_message_t msg;
	mavlink_message_t msga; 
    uint16_t len = 0;
	uint8_t outbuf[MAVLINK_MAX_PACKET_LEN];
    lastsent = millis();
    lastcompass = millis();
    for(;;) {
		    int n = read(STDIN_FILENO, buf, sizeof(buf));


		    if(millis() - lastsent > 1000){                 
			  	  mavlink_msg_heartbeat_pack(100, 200, &msg, system_type, autopilot_type,0,0,0);	
				  len = mavlink_msg_to_send_buffer(outbuf, &msg);
			      fwrite(outbuf,1, len,stdout);
                  fflush(stdout);
			      lastsent = millis();
		    }


            uint8_t decodedMessage = NazaDecoder.decode(buf[0]);
            switch (decodedMessage)
            {
              case NAZA_MESSAGE_GPS:
                //printf("Num sat: %d %f %f %f %f\n",NazaDecoder.getNumSat(),NazaDecoder.getLat(),NazaDecoder.getLon(), NazaDecoder.getGpsAlt(), NazaDecoder.getHeadingNc());

                memset(&packet_gps, 0, sizeof(packet_gps));
            	packet_gps.time_usec = millis()*1000;
            	packet_gps.lat = NazaDecoder.getLat()*10000000.0f;;
            	packet_gps.lon = NazaDecoder.getLon()*10000000.0f;;
            	packet_gps.alt = NazaDecoder.getGpsAlt()*1000.0f;
            	packet_gps.fix_type = NazaDecoder.getFixType();
                packet_gps.vel =  NazaDecoder.getSpeed();
                packet_gps.satellites_visible = NazaDecoder.getNumSat();
      	        mavlink_msg_gps_raw_int_encode(100, 200, &msga, &packet_gps);
				len = mavlink_msg_to_send_buffer(outbuf, &msga);
    	        fwrite(outbuf,1, len,stdout);
                fflush(stdout);
                

				memset(&packet_attitude, 0, sizeof(packet_attitude));
            	packet_attitude.time_boot_ms = millis();
              	packet_attitude.roll = 0;
            	packet_attitude.pitch = 0; 
            	packet_attitude.yaw = ToRad(NazaDecoder.getHeadingNc());
            	packet_attitude.rollspeed = 0;
            	packet_attitude.pitchspeed = 0;
            	packet_attitude.yawspeed = 0;
      	        mavlink_msg_attitude_encode(100, 200, &msga, &packet_attitude);
                len = mavlink_msg_to_send_buffer(outbuf, &msga);
    	        fwrite(outbuf,1, len,stdout);
                fflush(stdout);


				memset(&packet_global_pos,0, sizeof(packet_global_pos));
				packet_global_pos.time_boot_ms = millis();
				packet_global_pos.lat = NazaDecoder.getLat()*10000000.0f;;
				packet_global_pos.lon = NazaDecoder.getLon()*10000000.0f;;
				packet_global_pos.alt = NazaDecoder.getGpsAlt()*1000;
				packet_global_pos.relative_alt = NazaDecoder.getGpsAlt()*1000;
				packet_global_pos.hdg = (NazaDecoder.getHeadingNc());
				mavlink_msg_global_position_int_encode(100, 200, &msga, &packet_global_pos);
				len = mavlink_msg_to_send_buffer(outbuf, &msga);
    	        fwrite(outbuf,1, len,stdout);
                fflush(stdout);


                break;
              case NAZA_MESSAGE_COMPASS:
                if(millis() - lastcompass > 200){
				memset(&packet_attitude, 0, sizeof(packet_attitude));
            	packet_attitude.time_boot_ms = millis();
              	packet_attitude.roll = 0;
            	packet_attitude.pitch = 0; 
            	packet_attitude.yaw = ToRad(NazaDecoder.getHeadingNc());
            	packet_attitude.rollspeed = 0;
            	packet_attitude.pitchspeed = 0;
            	packet_attitude.yawspeed = 0;
      	        mavlink_msg_attitude_encode(100, 200, &msga, &packet_attitude);
                len = mavlink_msg_to_send_buffer(outbuf, &msga);
    	        fwrite(outbuf,1, len,stdout);
                fflush(stdout);
                lastcompass = millis();
                }
                break;
            }

    }

	return 0;
}


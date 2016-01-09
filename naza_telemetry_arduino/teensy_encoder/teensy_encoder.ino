
#include <NazaDecoderLib.h>
#include "../mavlink/include/mavlink.h"

#include <SoftwareSerial.h>
SoftwareSerial mySerial(3,2); // RX, TX

#define PITCHPIN A1
#define ROLLPIN A0
#define VOLTAGEPIN A2
#define VOLT_DIVIDER_CONST 0.28
#define SERIAL mySerial
#define DEBUG Serial

long lastheartbeat=0;
long lastcompass=0;
long lastbattery=0;
int system_type;
int autopilot_type;
mavlink_gps_raw_int_t packet_gps;
mavlink_attitude_t packet_attitude;
mavlink_global_position_int_t packet_global_pos;
mavlink_sys_status_t packet_sys_status;
uint8_t buf[MAVLINK_MAX_PACKET_LEN];
mavlink_message_t msg; 
uint16_t len;

void setup(){

  SERIAL.begin(115200);
  DEBUG.begin(115200);
  Serial1.begin(115200);
  pinMode(PITCHPIN, INPUT_PULLUP);  
  pinMode(ROLLPIN, INPUT_PULLUP); 
  system_type = MAV_TYPE_FIXED_WING;
  autopilot_type = MAV_AUTOPILOT_GENERIC;//MAV_AUTOPILOT_ARDUPILOTMEGA;//
}

void loop(){

  if(millis() - lastheartbeat > 1000){                 

    mavlink_msg_heartbeat_pack(100, 200, &msg, system_type, autopilot_type,0,0,0);
    len = mavlink_msg_to_send_buffer(buf, &msg);
    SERIAL.write(buf, len);
    lastheartbeat = millis();
  }


   if(Serial1.available())
   {
     
     
   uint8_t decodedMessage = NazaDecoder.decode(Serial1.read());
     switch (decodedMessage){
     case NAZA_MESSAGE_GPS:
                memset(&packet_gps, 0, sizeof(packet_gps));
            	packet_gps.time_usec = millis()*1000;
            	packet_gps.lat = NazaDecoder.getLat()*10000000.0f;;
            	packet_gps.lon = NazaDecoder.getLon()*10000000.0f;;
            	packet_gps.alt = NazaDecoder.getGpsAlt()*1000.0f;
            	packet_gps.fix_type = NazaDecoder.getFixType();
                packet_gps.vel =  NazaDecoder.getSpeed();
                packet_gps.satellites_visible = NazaDecoder.getNumSat();
      	        mavlink_msg_gps_raw_int_encode(100, 200, &msg, &packet_gps);
	        len = mavlink_msg_to_send_buffer(buf, &msg);
                SERIAL.write(buf, len);  
                SERIAL.flush();
  
                memset(&packet_global_pos,0, sizeof(packet_global_pos));
                packet_global_pos.time_boot_ms = millis();
				packet_global_pos.lat = NazaDecoder.getLat()*10000000.0f;;
				packet_global_pos.lon = NazaDecoder.getLon()*10000000.0f;;
				packet_global_pos.alt = NazaDecoder.getGpsAlt()*1000;
				packet_global_pos.relative_alt = NazaDecoder.getGpsAlt()*1000;
				packet_global_pos.hdg = (NazaDecoder.getHeadingNc());
				mavlink_msg_global_position_int_encode(100, 200, &msg, &packet_global_pos);
				len = mavlink_msg_to_send_buffer(buf, &msg);
                SERIAL.write(buf, len); 
                               SERIAL.flush(); 
                
                break;
     case NAZA_MESSAGE_COMPASS:
   /*
		memset(&packet_attitude, 0, sizeof(packet_attitude));
            	packet_attitude.time_boot_ms = millis();
              	packet_attitude.roll = ToRad(NazaDecoder_getRoll());;
            	packet_attitude.pitch = ToRad(NazaDecoder_getPitch());
            	packet_attitude.yaw = ToRad(NazaDecoder.getHeadingNc());
            	packet_attitude.rollspeed = 0;
            	packet_attitude.pitchspeed = 0;
            	packet_attitude.yawspeed = 0;
      	        mavlink_msg_attitude_encode(100, 200, &msg, &packet_attitude);
                len = mavlink_msg_to_send_buffer(buf, &msg);
                SERIAL.write(buf, len);
                SERIAL.flush();*/
                break;
     }
   }
   
   if(millis() - lastcompass > 50)
   {
		memset(&packet_attitude, 0, sizeof(packet_attitude));
            	packet_attitude.time_boot_ms = millis();
              	packet_attitude.roll = ToRad(NazaDecoder_getRoll());;
            	packet_attitude.pitch = ToRad(NazaDecoder_getPitch());
            	packet_attitude.yaw = ToRad(NazaDecoder.getHeadingNc());
            	packet_attitude.rollspeed = 0;
            	packet_attitude.pitchspeed = 0;
            	packet_attitude.yawspeed = 0;
      	        mavlink_msg_attitude_encode(100, 200, &msg, &packet_attitude);
                len = mavlink_msg_to_send_buffer(buf, &msg);
                SERIAL.write(buf, len);
                SERIAL.flush();

                lastcompass = millis();
                DEBUG.println(NazaDecoder_getRoll());

   }
   
   if(millis() - lastbattery>1000){
             memset(&packet_sys_status,0, sizeof(packet_sys_status));
               packet_sys_status.voltage_battery = analogRead(VOLTAGEPIN)/1024.0*5.0*1000/VOLT_DIVIDER_CONST;
  	       mavlink_msg_sys_status_encode(100, 200, &msg, &packet_sys_status);
               len = mavlink_msg_to_send_buffer(buf, &msg);
	       SERIAL.write(buf, len);
               SERIAL.flush();
               lastbattery = millis();
 
     
   }
}



float ToRad(float degree){

  return degree /180.0f * 3.1415927; 

}



int8_t pwm2Deg(uint32_t pulseWidth)
{
  return (pulseWidth == 0) ? 0 : map(constrain(pulseWidth, 1000, 2000), 1000, 2000, -90, 90);
}


int readPWM(int pin){

  int m1 = pulseIn(pin, HIGH);
  delay(1);
  int m2 = pulseIn(pin, HIGH);
  return(max(m1,m2));
}

int8_t NazaDecoder_getPitch()
{

  return pwm2Deg(2000-readPWM(PITCHPIN)+1000);
}

int8_t NazaDecoder_getRoll()
{
  return pwm2Deg(readPWM(ROLLPIN));
}



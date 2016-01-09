#include <stdio.h>
#include <stdint.h>  
#include "conf.h"
#include "telemetry.h"
#include "mavlink_parse.h"
#include <mavlink.h>
#include <math.h>


#include <unistd.h>
#include <stdlib.h>





void setHomeVars(telemetry_data_t *td)
 
{
 
  float dstlon, dstlat; 
  long bearing;
 
  if(td->home_location_set == 0 && td->fix_type == 3){
    printf("set home\n");
    td->home_latitude = td->latitude;
    td->home_longitude = td->longitude;
    td->home_altitude = td->altitude;
    
    td->home_location_set = 1;
    printf("osd_home_lat:%f\n",td->home_latitude);
    printf("osd_home_lon:%f\n",td->home_longitude);

  }
  else if(td->home_location_set == 1 && td->home_altitude_set ==0){
    // JRChange: osd_home_alt: check for stable osd_alt (must be stable for 25*120ms = 3s)
    if(td->home_altitude_counter < 25){
      if(fabs(td->home_last_altitude - td->altitude) > 0.5){
        td->home_altitude_counter = 0;
        td->home_last_altitude = td->altitude;
      }
      else
	{
	  if(++td->home_altitude_counter >= 25){
	    td->home_altitude = td->altitude;  // take this stable osd_alt as osd_home_alt
	    td->home_altitude_set = 1;
	  }
	}
    }
  }
  else if(td->home_location_set == 1 && td->home_altitude_set ==1){  
    // shrinking factor for longitude going to poles direction
    float rads = fabs(td->home_latitude) * 0.0174532925;
    double scaleLongDown = cos(rads);
    double scaleLongUp   = 1.0f/cos(rads);

    //DST to Home
    dstlat = fabs(td->home_latitude - td->latitude) * 111319.5;
    dstlon = fabs(td->home_longitude - td->longitude) * 111319.5 * scaleLongDown;
    td->home_distance = sqrt(pow(dstlat,2) + pow(dstlon,2));
    printf("home_dist:%f\n",td->home_distance);

    //printf("dstlat:%f\n",dstlat);
    //printf("dstlon:%f\n",dstlon);



    //DIR to Home
    dstlon = (td->home_longitude - td->longitude); //OffSet_X
    dstlat = (td->home_latitude - td->latitude) * scaleLongUp; //OffSet Y
    bearing = 90 + (atan2(dstlat, -dstlon) * 57.295775); //absolut home direction
    if(bearing < 0) bearing += 360;//normalization
    bearing = bearing - 180;//absolut return direction
    if(bearing < 0) bearing += 360;//normalization
    bearing = bearing - td->heading;//relative home direction
    if(bearing < 0) bearing += 360; //normalization
 
    td->home_direction = bearing;

    /*round((float)(bearing/360.0f) * 16.0f) + 1;//array of arrows =)
 
      if(td->osd_home_direction > 16) td->osd_home_direction = 0;*/
    printf("home_direction:%f\n",td->home_direction);

  }
} 

int mavlink_parse_buffer(telemetry_data_t *td, uint8_t *buf, int buflen) {
  int new_data = 0;
  int i;

  mavlink_message_t msg;
  mavlink_status_t status; 
  printf("mavlink parse buffer\n");

  for(i=0; i<buflen; i++) {
    uint8_t c = buf[i];
    //trying to grab msg
     if(mavlink_parse_char(MAVLINK_COMM_0, c, &msg, &status)){

      //handle msg
      new_data=1;
      switch (msg.msgid) {
        case MAVLINK_MSG_ID_HEARTBEAT:
    		{
		printf("heartbeat\n");           
		uint8_t base_mode = (uint8_t)mavlink_msg_heartbeat_get_base_mode(&msg);
		uint8_t custom_mode = mavlink_msg_heartbeat_get_custom_mode(&msg);
		printf("custom mode=%d\n",custom_mode);
		printf("base mode=%d\n",base_mode); 
	
		switch(custom_mode)
		{
		case 0:	
			{
			td->mode="MANUAL";
			break;
			}
		case 1:
                        {
                        td->mode="CIRCLE";
                        break;
                        }
		case 2:
                        {
                        td->mode="STABILIZE";
                        break;
                        }
		case 3:
                        {
                        td->mode="TRAINING";
                        break;
                        }
		case 4:
                        {
                        td->mode="ACRO";
                        break;
                        }
		case 5:
                        {
                        td->mode="FBWA";
                        break;
                        }
		case 6:
                        {
                        td->mode="FBWB";
                        break;
                        }
		case 7:
                        {
                        td->mode="CRUISE";
                        break;
                        }
		case 8:
                        {
                        td->mode="AUTOTUNE";
                        break;
                        }
		case 10:
                        {
                        td->mode="AUTO";
                        break;
                        }
		case 11:
                        {
                        td->mode="RTL";
                        break;
                        }
		case 12:
                        {
                        td->mode="LOITER";
                        break;
                        }
		case 14:
                        {
                        td->mode="LAND";
                        break;
                        }
		case 15:
                        {
                        td->mode="GUIDED";
                        break;
                        }
		case 16:
                        {
                        td->mode="INITIALISING";
                        break;
                        }

		}

         	break;
          }
          
   case MAVLINK_MSG_ID_SYS_STATUS:
          {
                float battery_v = (mavlink_msg_sys_status_get_voltage_battery(&msg) / 1000.0f);
                float battery_a = (mavlink_msg_sys_status_get_current_battery(&msg) * 10.0f / 1000.0f);
                td->voltage = battery_v;
                td->current = battery_a;
                float battery_remaining =(mavlink_msg_sys_status_get_battery_remaining(&msg));
                td->battery_remaining = battery_remaining;
                printf("Voltage:%f\n", battery_v);

                printf("bat rem:%f\n", battery_remaining); 
                printf("Current:%f\n", battery_a);
		break;
	}


            case MAVLINK_MSG_ID_NAV_CONTROLLER_OUTPUT:
                {
                  td->wp_target_bearing = mavlink_msg_nav_controller_output_get_target_bearing(&msg);
                  td->wp_dist = mavlink_msg_nav_controller_output_get_wp_dist(&msg);
                  break; 
              }

            case MAVLINK_MSG_ID_MISSION_CURRENT:
                {
                    td->wp_number = (uint8_t)mavlink_msg_mission_current_get_seq(&msg);
                    break;   
             }

        case MAVLINK_MSG_ID_GPS_RAW_INT:
          {
		printf("gps raw init\n");           

            	float lat = mavlink_msg_gps_raw_int_get_lat(&msg) / 10000000.0f;
            	float lng = mavlink_msg_gps_raw_int_get_lon(&msg) / 10000000.0f;
            	td->fix_type = mavlink_msg_gps_raw_int_get_fix_type(&msg);
		printf("fixtype %d",td->fix_type);
		//td->osd_fix_type = fix_type;
		switch (td->fix_type)
		{
		case 0:
		td->fix_mode="No fix";
		break;
		case 1:
		td->fix_mode="No fix";
		break;
		case 2:
		td->fix_mode="2D fix";
		break;
		case 3:
		td->fix_mode="3D fix";
		break;
		default:
		td->fix_mode="";

		}
printf("fix:%s\n", td->fix_mode);
            	td->satellites_visible = mavlink_msg_gps_raw_int_get_satellites_visible(&msg);
         	td->latitude = lng;
		td->longitude = lat;
		td->altitude = mavlink_msg_gps_raw_int_get_alt(&msg)*0.001;
		td->ground_speed = mavlink_msg_gps_raw_int_get_vel(&msg);

	        printf("lat:%f\n",lat);
printf("lon:%f\n",lng);
// printf("alt:%f\n",td->altitude); 
          break;
  	  }
          
        case MAVLINK_MSG_ID_VFR_HUD:
          {
		printf("vfr hud\n");           
		float groundspeed = mavlink_msg_vfr_hud_get_groundspeed(&msg);
            	float alt = mavlink_msg_vfr_hud_get_alt(&msg);
         	td->altitude = alt;
		td->ground_speed = groundspeed;
                td->throttle = (uint8_t)mavlink_msg_vfr_hud_get_throttle(&msg);
                td->climb_rate = mavlink_msg_vfr_hud_get_climb(&msg); 
 		td->heading = mavlink_msg_vfr_hud_get_heading(&msg); // 0..360 deg, 0=north

          break;
	  }

	  case MAVLINK_MSG_ID_GLOBAL_POSITION_INT:
          {
                 td->altitude = mavlink_msg_global_position_int_get_alt(&msg)*0.001;
                 break;          
     } 


        case MAVLINK_MSG_ID_ATTITUDE:
          {
	        td->running_time =  mavlink_msg_attitude_get_time_boot_ms(&msg);
		printf("running time %lu millis %lu\n",td->running_time,millis());

		//calc_latency(td);
		printf("attitude\n");           
            	float heading = ToDeg(mavlink_msg_attitude_get_yaw(&msg));
         	td->heading = heading;
                printf("td->heading:%f\n",td->heading);
                
		float pitch = ToDeg(mavlink_msg_attitude_get_pitch(&msg));
		td->pitch = pitch;
                printf("td->pitch:%f\n",td->pitch);

		float roll = ToDeg(mavlink_msg_attitude_get_roll(&msg));
		td->roll=roll;
		printf("td->roll:%f\n",td->roll);
		}
          break;

	  
        default:
	{
	  printf("unknown mavlink msgid:%d\n",msg.msgid);
          break;
	}
      }


      
    }
//else
// printf("%d\n",status.parse_error);

  }

	
  setHomeVars(td); 
return new_data;
}



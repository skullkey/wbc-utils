#include <stdbool.h>

#pragma once


typedef struct{ 
	uint8_t fix_type;
	float osd_alt; 
	uint8_t osd_alt_cnt ; // counter for stable osd_alt
	uint8_t osd_got_home ; // tels if got home position or not
	char* osd_fix_type ; 
	float osd_lat ; // latidude
	float osd_lon ; 
	float osd_home_lat;               // home latidude
	float osd_home_lon ; // home longitude 
	float osd_home_direction; // Arrow direction pointing to home (1-16 to CW loop)
	float osd_home_alt ; 
	float osd_home_distance ;
	uint8_t osd_satellites_visible;
	//float osd_heading ;
	bool haltset ;
	float osd_alt_prev;
	uint8_t armed;
	uint8_t manual;
        uint8_t hil_enable;
	uint8_t stabilized;
	uint8_t guided;
	uint8_t aut;
	uint16_t wp_target_bearing;
	uint16_t wp_dist;
	uint16_t wp_number;
	char* mode;
	double start_lat;
	double start_lng;
	float current;
	float voltage;
	float altitude;
	double longitude;
	double latitude;
	float heading;
	float speed;
	float pitch;
	float roll;
	float battery_remaining;
	float throttle;
	float climb;
	float home_alt;
	float lat,lng,alt;
	
} telemetry_data_t;


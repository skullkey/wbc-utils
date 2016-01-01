#include <stdbool.h>
#include <stdint.h>
#include <time.h>

#pragma once

typedef struct {
        uint32_t received_packet_cnt;
        uint32_t wrong_crc_cnt;
        int8_t current_signal_dbm;
} wifi_adapter_rx_status_t;

typedef struct {
        time_t last_update;
        uint32_t received_block_cnt;
        uint32_t damaged_block_cnt;
        uint32_t tx_restart_cnt;

        uint32_t wifi_adapter_cnt;
        wifi_adapter_rx_status_t adapter[8];
} wifibroadcast_rx_status_t;

#define ADAPTER_MAX 10

typedef struct{    
	uint8_t fix_type;
        uint8_t satellites_visible;
	float altitude, latitude, longitude;

	long running_time;
	long rx_time;
	long tx_time;
	long latency;

	float pitch, roll, heading;
	float ground_speed, air_speed;
	float climb_rate;
	float throttle;

	float voltage,current,rssi,battery_remaining;
	
	uint8_t home_location_set, home_altitude_set;
	float home_altitude, home_latitude, home_longitude;
	float home_direction, home_distance;
	uint8_t home_altitude_counter; // used to check that altitude is stable before setting home altitude
	float home_last_altitude;

	char* mode;
	char* fix_mode;

	// waypoint
	float wp_target_bearing;
	float wp_dist;
	uint8_t wp_number; 

	// rx health stats
	uint8_t display_rx_stats;
	float health_rate[ADAPTER_MAX];
	float packet_rate[ADAPTER_MAX];
	int8_t signal_strength[ADAPTER_MAX];
	
	wifibroadcast_rx_status_t *rx_status;
	


} telemetry_data_t;


wifibroadcast_rx_status_t *telemetry_wbc_status_memory_open(void);
void telemetry_init(telemetry_data_t *td);
long millis();

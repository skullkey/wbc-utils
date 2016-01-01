#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>        /* For mode constants */
#include <fcntl.h>           /* For O_* constants */
#include <stdio.h>
#include <stdlib.h>
#include <string.h> 
#include <sys/time.h>

#include "telemetry.h"
void telemetry_init(telemetry_data_t *td) {

  //memset(td, 0, sizeof(telemetry_data_t));

 td-> fix_type = 0;
 td-> satellites_visible=0;
 td-> altitude=0;
 td-> latitude=0;
 td-> longitude=0;
 

 td-> running_time=0;
 td-> rx_time=0;
 td-> tx_time=0;
 td-> latency=0;
 
 td-> pitch=0;
 td-> roll=0;
 td-> heading=0;
 td-> ground_speed=0;
 td-> air_speed=0;
 td-> climb_rate=0;
 td-> throttle=0;

 td-> voltage=0;
 td-> current=0;
 td-> rssi=0;
 td-> battery_remaining=0;
	
 td-> home_location_set=0;
 td-> home_altitude_set=0;
 td-> home_altitude=0;
 td-> home_latitude=0;
 td-> home_longitude=0;
 td-> home_direction=0;
 td-> home_distance=0;
 td-> home_altitude_counter=0; // used to check that altitude is stable before setting home altitude
 td-> home_last_altitude=0;

 td-> mode = "UNKNOWN";
 td-> fix_mode = "UNKNOWN";

	// waypoint
 td-> wp_target_bearing=0;
 td-> wp_dist=0;
 td-> wp_number=-1;

 td->display_rx_stats=1;
 int i = 0;
 for(i=0;i<ADAPTER_MAX;i++){   
   td-> health_rate[i]=0;
   td-> packet_rate[i]=0;
   td-> signal_strength[i]=0;
 }	


  td->rx_status = telemetry_wbc_status_memory_open();
}



wifibroadcast_rx_status_t *telemetry_wbc_status_memory_open(void) {
  int fd = shm_open("/wifibroadcast_rx_status_0", O_RDWR, S_IRUSR | S_IWUSR);

  if(fd < 0) {
    fprintf(stderr, "Could not open wifibroadcast rx status\n");
    return NULL;
  }

  if (ftruncate(fd, sizeof(wifibroadcast_rx_status_t)) == -1) {
    perror("ftruncate");
    exit(1);
  }

  void *retval = mmap(NULL, sizeof(wifibroadcast_rx_status_t), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
  if (retval == MAP_FAILED) {
    perror("mmap");
    exit(1);
  }


  return (wifibroadcast_rx_status_t*)retval;

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

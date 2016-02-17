#include <stdint.h>
#include "conf.h"
#ifdef RENDER
#include "render.h"
#include "telemetry.h"
#include <stdlib.h>
#include "math.h"
#include <sys/time.h>

uint32_t last_total_count[ADAPTER_MAX];
uint32_t last_damaged_count[ADAPTER_MAX];
long last_time_read;


void render_init()
{
  int i=0;

  last_time_read = millis();
  for(i=0;i<ADAPTER_MAX;i++){
    last_total_count[i] = 0;
    last_damaged_count[i] = 0;
  }

}


void render(telemetry_data_t *td)
{

  fprintf(stdout, "{\"battery\":%.2f,", td->voltage);
  fprintf(stdout, "\"latitude\":%.5f,", td->latitude);
  fprintf(stdout, "\"longitude\":%.5f,", td->longitude);

  if(td->home_altitude_set){
    fprintf(stdout, "\"altitude\":%.2f,", td->altitude - td->home_altitude);
  } else {
    fprintf(stdout, "\"altitude\":\"?\",");
  }
  if(td->home_location_set){
    fprintf(stdout, "\"distance\":%.2f,", td->home_distance);
    fprintf(stdout, "\"direction\":%.2f,", td->home_direction);

  } else {
    fprintf(stdout, "\"distance\":\"?\",");
    fprintf(stdout, "\"direction\":\"?\",");

  }

  render_rx_status(td);
  fprintf(stdout, "\"satellite\":\"%d (%s)\"}", td->satellites_visible, td->fix_mode);
  fflush(stdout);


}


void render_rx_status(telemetry_data_t *td) {
  int best_signal=-100;
  float best_rate=0;
  int i=0;
  if(td->rx_status != NULL ) {
    int i;
    int ac = td->rx_status->wifi_adapter_cnt;
    long duration = millis()-last_time_read;

    for(i=0; i<ac; ++i) {

      if(duration > 1000){

        float new_damaged = (float)td->rx_status->adapter[i].wrong_crc_cnt - last_damaged_count[i];
        float new_total = (float)td->rx_status->adapter[i].received_packet_cnt - last_total_count[i];

        td->health_rate[i] = (new_total-new_damaged)/(new_total+1);
        td->packet_rate[i] = new_total / duration *1000.0f;
        td->signal_strength[i] = td->rx_status->adapter[i].current_signal_dbm;

        last_damaged_count[i] = td->rx_status->adapter[i].wrong_crc_cnt;
        last_total_count[i] = td->rx_status->adapter[i].received_packet_cnt;
        last_time_read = millis();
      }
      if(best_rate < td->packet_rate[i]){
        best_rate = td->packet_rate[i];
      }
      if(best_signal < td->signal_strength[i]){
        best_signal = td->signal_strength[i];
      }
    }
    fprintf(stdout, "\"signal\": %d,", best_signal );
    fprintf(stdout, "\"rate\": %.2f,",best_rate);

  }
}

#endif

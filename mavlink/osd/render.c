#include <stdint.h>
#include "conf.h"
#ifdef RENDER
#include "render.h"
#include "telemetry.h"
#include <stdlib.h>
#include "math.h"
#include <sys/time.h>

int s_width, s_height;


int getWidth(float pos_x_percent)
{
  return(s_width / 100.0 * pos_x_percent);
}

int getHeight(float pos_y_percent)
{
  return(s_height / 100.0 * pos_y_percent);
}


void draw_sat(telemetry_data_t *td, int pos_x, int pos_y, float scale)
{
  char s[100];
  TextEnd(pos_x, pos_y, td->fix_mode, SerifTypeface, s_width/170 * scale);
  sprintf(s,"Sat:%d",td->satellites_visible);
  TextEnd(pos_x, pos_y+getHeight(5), s, SerifTypeface, s_width/170 * scale);  
}

void draw_center(telemetry_data_t *td, int pos_x, int pos_y, float scale)
{

  //center circle
  int radius=3 * scale;
  //StrokeWidth(2);
  //Stroke(255,255,255,1);
  float i;
  VGfloat x[100];
  VGfloat y[100];  
  int z=0;

  for(i=0;i<6.28;i+=0.1)
    {
      x[z]=sin(i) * radius + pos_x;
      y[z]=cos(i) * radius + pos_y;
      z++;
      //printf("sin(%f)=%f\n",i,sin(i));
    }
  x[z] = x[0];
  y[z] = y[0];
  z++;
  //Polyline(x, y, z);
  Fill(0, 0, 0, 1);
  Circle(pos_x,pos_y,10*scale);
  
  //home direction
  float length = 15 * scale;
  float arg = td->home_direction/180.0*PI;
  VGfloat x1 = pos_x + sin(arg) * radius;
  VGfloat y1 = pos_y + cos(arg) * radius;
  VGfloat x2 = pos_x + sin(arg) * (radius + length);
  VGfloat y2 = pos_y + cos(arg) * (radius + length);
  Line(x1, y1, x2, y2);

  printf("home direction %f\n", td->home_direction);
  printf(">> %f %f %f %f %f\n",x1,y1,x2,y2,arg);


  // altitude label
  int space = 20 * scale;
  int width = 40 * scale;
  int height = 180 *  scale;
  int label_height = 15 * scale;
  int pos_x_r = pos_x + width / 2 + s_width / 4 + space;
  int pos_y_r = pos_y;

  x[0] = pos_x_r;
  y[0] = pos_y_r;
  x[1] = pos_x_r + width / 5;
  y[1] = pos_y_r +  label_height / 2;
  x[2] = x[1]+ width;
  y[2] = y[1];
  x[3] = x[2]; 
  y[3] = y[2] - label_height;
  x[4] = x[1];
  y[4] = y[3];
  x[5] = x[0];
  y[5] = y[0];
  Polyline(x, y, 6);
  char s[100];
  sprintf(s,"%.0f", td->altitude);//td->altitude);
  TextMid(pos_x_r + width / 2 + width / 5, pos_y - s_width/170 * scale / 2, s, SerifTypeface,s_width/170 * scale*1.5);
  // velocity label
  int pos_x_l = pos_x - width / 2 - s_width / 4 - space;
  int pos_y_l = pos_y;
  x[0] = pos_x_l;
  y[0] = pos_y_l;
  x[1] = pos_x_l - width / 5;
  y[1] = pos_y_l + label_height / 2;
  x[2] = x[1] - width;
  y[2] = y[1];
  x[3] = x[2];
  y[3] = y[2] - label_height;
  x[4] = x[1];
  y[4] = y[3];
  sprintf(s,"%.0fm/s", td->ground_speed);
  TextMid (pos_x_l - width/2 - width / 5 , pos_y - s_width/170 * scale /2, s, SerifTypeface, s_width/170 * scale*1.5);
  x[5] = x[0];
  y[5] = y[0];
  Polyline(x, y, 6);


  //ladder speed
  int width_ladder = 150 * scale;

  StrokeWidth(2);
  int  width_speed = 50 * scale;
  int  height_speed = 300 * scale; 
  int  length_bar = 5 * scale;
  int  range_speed = 40;

  int k;
  int space_text = 5 * scale;
  int distance = 100 * scale;
  float ratio_speed = height_speed / range_speed;
  for( k = (int)(td->ground_speed - range_speed/2); k <= td->ground_speed + range_speed/2; k++)
    {
      int y = pos_y + (k - td->ground_speed) * ratio_speed;
      sprintf(s, "%d", k);
      if(k%1==0)
	{ 
	  int px = pos_x - width_ladder/2 - distance - width_speed;
	  int px2 = px - length_bar;
	  Line(px, y, px2, y); 
	}
      if(k%5==0)
	{
	  int px =  pos_x - width_ladder/2 - distance - width_speed;
	  int px2 = px - 2*length_bar;
	  Line(px, y, px2, y);
	  if(k>=0)
	    TextEnd(pos_x - width_ladder/2 - distance - width_speed - space_text - 2 * length_bar, y - s_width/600 * scale, s, SerifTypeface, s_width/300 * scale);
    
	}
    }
  //ladder altitude
  int range_altitude = 200;
  float ratio_altitude =  height_speed / range_altitude;
  for( k = (int) (td->altitude - range_altitude/2); k <= td->altitude + range_altitude/2; k++)
    {
      int y = pos_y + ( k - td->altitude ) * ratio_altitude;
      sprintf(s, "%d", k);
      if(k%10==0)
	{
	  int px = pos_x + width_ladder/2 + distance + width_speed;
	  int px2 = px + length_bar;
	  Line(px, y ,px2, y);
	}
      if(k%100==0)
	{
	  int px = pos_x + width_ladder/2 + distance + width_speed;
	  int px2 = px + 2 * length_bar;
	  Line(px, y, px2, y);
	  Text(pos_x + width_ladder/2 + distance + width_speed + space_text + 2 * length_bar, y - s_width/600 * scale, s, SerifTypeface, s_width/300 * scale);
	}
    }
  //ladder middle

  int height_ladder = 300 * scale;
  int range = 20;
  int pike = 4 * scale;
  float ratio = height_ladder / range;
  //printf("min:%d pitch:%d max:%d\n",minAngle, rot, maxAngle); 

  Translate(pos_x, pos_y);
  Rotate(td->roll);
  Translate(-pos_x, -pos_y);

  int y_start=height_ladder;

  for ( k = (int)(td->pitch - range/2); k <= td->pitch + range/2; k++) //Wertebereich  Anzeige Grad
    {
      //printf("k=%d\n",k);
      int  y = pos_y  + (k - td->pitch) * ratio;
      sprintf(s, "%d", k);
      if(k%5==0)
	{
	  TextEnd(pos_x - width_ladder/2 - space_text, y - s_width/600 * scale, s, SerifTypeface, s_width/300 * scale);
	  Text(pos_x + width_ladder/2 + space_text, y - s_width/600 * scale, s, SerifTypeface, s_width/300 * scale);
	}
      if (( k>0 ) && ( k%5 == 0 ))
	{
	  int px = pos_x - width_ladder/2;
	  int px2= pos_x + width_ladder/2;
	  Line(px, y, px + width_ladder / 3 , y);
	  Line(px, y, px, y - pike);
	  Line(px + width_ladder * 2/3, y, px2, y);
	  Line(px2, y, px2, y - pike);
	}
      else if(( k<0 ) && ( k%5 == 0 ))
	{
	  int px  = pos_x - width_ladder/2 + 13/13.0 * width_ladder/3;
	  int px2 = pos_x - width_ladder/2 + 10/13.0 * width_ladder/3;

	  int px3 = pos_x - width_ladder/2 + 8/13.0 * width_ladder/3;
	  int px4 = pos_x - width_ladder/2 + 5/13.0 *  width_ladder/3;

	  int px5 = pos_x - width_ladder/2 + 3/13.0 * width_ladder/3;
	  int px6 = pos_x - width_ladder/2;

	  Line(px, y, px2, y);
	  Line(px3, y, px4, y);
	  Line(px5, y, px6, y);
	  Line(px6, y, px6, y + pike);

	  px =  pos_x + width_ladder/2 - 13/13.0 * width_ladder/3;
	  px2 = pos_x + width_ladder/2 - 10/13.0 * width_ladder/3;

	  px3 = pos_x + width_ladder/2 - 8/13.0 * width_ladder/3;
	  px4 = pos_x + width_ladder/2 - 5/13.0 * width_ladder/3;

	  px5 = pos_x + width_ladder/2 - 3/13.0 * width_ladder/3;
	  px6 = pos_x + width_ladder/2;

	  Line(px, y, px2, y);
	  Line(px3, y, px4, y);
	  Line(px5, y, px6, y);
	  Line(px6, y, px6, y + pike); 
	}
      else if(k==0)
	{
	  Line(pos_x - width_ladder/2, y, pos_x + width_ladder/2, y); 
	}

    }
}

void draw_lat_lon(telemetry_data_t *td, int pos_x, int pos_y, float scale)
{
  char s[100];
  float space = 10 * scale;
  sprintf(s, "Lon:%f", td->longitude);
  Text(pos_x, pos_y, s, SerifTypeface, s_width/170 * scale); 
  sprintf(s, "Lat:%f", td->latitude);
  Text(pos_x, pos_y + space, s, SerifTypeface, s_width/170 * scale);
}

void draw_power(telemetry_data_t *td, int pos_x, int pos_y, float scale)
{
  char s[100];
  float space = 15 * scale;
  sprintf(s, "%3.1fV", td->voltage);
  Text(pos_x, pos_y, s, SerifTypeface, s_width/170 * scale);
  
  sprintf(s, "%3.2fA", td->current);
  
  Text(pos_x, pos_y + space, s, SerifTypeface, s_width/170 * scale);

}
void draw_home_dist(telemetry_data_t *td, int pos_x, int pos_y, float scale)
{
  char s[100];
  if(!td->home_location_set) Stroke(0xff,0,0,1); else Stroke(HUD_R, HUD_G,HUD_B,1);
  sprintf(s, "Dist:%5.0fm", td->home_distance);
  TextMid(pos_x, pos_y, s, SerifTypeface, s_width/170 * scale);
  if(!td->home_altitude_set) Stroke(0xff,0,0,1); else Stroke(HUD_R, HUD_G,HUD_B,1);  
  sprintf(s, "Alt:%5.0fm",(td->altitude - td->home_altitude));
  TextMid(pos_x, pos_y+getHeight(5), s, SerifTypeface, s_width/170 * scale);

  Stroke(HUD_R, HUD_G,HUD_B,1); 
  
}


void draw_mode(telemetry_data_t *td, int pos_x, int pos_y, float scale)
{
  TextEnd(pos_x, pos_y, td->mode, SerifTypeface, s_width/170 * scale);
}

void draw_bat(telemetry_data_t *td, int pos_x, int pos_y, float scale)
{
  int width= 20 * scale;
  int height= 10 * scale;
  int corner = 3 * scale;
  int plus_w = 2 * scale;
  int plus_h = 5 * scale;
  int stroke = 1 * scale;

  Fill(255, 255, 255, 1); 
  Roundrect(pos_x - width / 2, pos_y - height / 2, width, height, corner, corner);
  Rect(pos_x + width / 2, pos_y - plus_h / 2, plus_w, plus_h);
  //if(td->battery_remaining<10)
  //Fill(255,0,0,1);
  //else
  Fill(0,0,0,1);

  Rect(pos_x - width / 2 + stroke + td->battery_remaining / 100.0 * width , pos_y - height / 2 + stroke , width - 2 * stroke - td->battery_remaining /100.0 * width , height - 2 * stroke);
}

void draw_compass(telemetry_data_t *td, int pos_x, int pos_y, float scale)
{
  int width_number =  50 * scale;
  int height_number = 20 * scale;

  VGfloat x[] = {pos_x - width_number / 2, pos_x + width_number / 2, pos_x + width_number/2 , pos_x - width_number / 2, pos_x - width_number / 2};
  VGfloat y[] = {pos_y + height_number / 2, pos_y + height_number / 2, pos_y - height_number / 2, pos_y - height_number / 2, pos_y + height_number / 2};

  StrokeWidth(2);
  //Stroke(75,83,32,1); 
  Polyline(x,y,5);
  //Fill(75,83,32,1);

  char s[100];
  sprintf(s, "%3.0f\xb0", td->heading);
  TextMid(pos_x, pos_y-height_number/4, s , SerifTypeface, height_number/3);

  int width_triangle = 20 * scale;
  int space=50 * scale;
  int p0x = pos_x;
  int p0y = pos_y + space;
  int p1x = pos_x - width_triangle / 2;
  int p1y = pos_y + space + width_triangle / 2;
  int p2x = pos_x + width_triangle / 2;
  int p2y = p1y;

  VGfloat x2[] = {p0x, p1x, p2x, p0x};
  VGfloat y2[] = {p0y, p1y, p2y, p0y};
  Polyline(x2,y2,4);

  int width_comp = 150 * scale;
  int height_comp = 20 * scale;
  float ratio = width_comp / 180.0;

  int i;
  char* c;
  bool draw=false;

  for(i=(int)(td->heading - 90); i <= td->heading + 90; i++)
    {
      int x=pos_x + (i-td->heading) * ratio;
      if(i%15 == 0)
	{
	  Line(x, pos_y + height_comp / 3 + space / 3, x, pos_y + space / 3);
	}
      if(i%30==0)
	{
	  Line(x, pos_y+height_comp / 3  + space / 3 * 1.2, x, pos_y + height_comp / 3 + space / 3);
	}

      int j=i;
      if(j<0)
	j=j+360;
      if(j>=360)
	j=j-360;

      switch(j)
	{
	case 0:
	  {
	    draw=true;
	    c="N";
	    break;
	  }
	case 90:
	  {
	    draw=true;
	    c="W";
	    break;
	  }
	case 180:
	  {
	    draw=true;
	    c="S";
	    break;
	  }
	case 270:
	  {
	    draw=true;
	    c="E";
	    break;
	  }
	}
      if(draw==true)
	{ StrokeWidth(1);
	  TextMid(x, pos_y + height_comp / 3 + space / 3 * 1.6 , c, SerifTypeface, s_width/170 * 2.5 );
	  draw=false;
	}
    }
}




uint32_t last_total_count[ADAPTER_MAX];
uint32_t last_damaged_count[ADAPTER_MAX];
long last_time_read;
long last_latency_print=0;
int cnt_latency=0;
float sum_latency=0;
float latency=0;



void render_init()
{
  init(&s_width,&s_height);
  int i=0;

  last_time_read = millis();
  for(i=0;i<ADAPTER_MAX;i++){
    last_total_count[i] = 0;
    last_damaged_count[i] = 0;
  }

  last_latency_print= millis();
}


void render(telemetry_data_t *td) 
{

  Start(s_width, s_height);

  //Background(0,0,0);
  //draw_bat      (td, getWidth(10), getHeight(90), 3);
 
  Fill(HUD_R,HUD_G,HUD_B,1);
  Stroke(HUD_R,HUD_G,HUD_B,1);
  StrokeWidth(2);
  draw_compass  (td, getWidth(50), getHeight(85), 1.5);
  //draw_power    (td, getWidth(20), getHeight(5),  2);
  draw_sat      (td, getWidth(90), getHeight(90), 3);
	

  StrokeWidth(0.6);
  Stroke(HUD_R,HUD_G,HUD_B,1);

  draw_lat_lon  (td, getWidth(5), getHeight(15), 2.5);
  draw_home_dist(td, getWidth(50), getHeight(5),  3);
  draw_mode     (td, getWidth(90), getHeight(5),  3);
  //Translate(getWidth(50), getHeight(50));
  //Rotate(rot);
  //Translate(-getWidth(50), -getHeight(50));

  
  if(td->display_rx_stats==1) render_rx_status(td);
  
  Stroke(HUD_R,HUD_G,HUD_B,1);
  draw_center(td, getWidth(50), getHeight(50),    2);


  End();                                             // End the picture

}

 
void render_rx_status(telemetry_data_t *td) {
  char text[256];

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

      StrokeWidth(0.6);
      if(td->packet_rate[i] != 0){
        Stroke(HUD_R,HUD_G,HUD_B,1);
      } else {
	Stroke(0xff,0x00,0x00,1);
      }

      sprintf(text, "c%d: %ddBm", i, td->signal_strength[i]);
      Text(getWidth(5), getHeight(5+i*5), text, SerifTypeface, s_width/170*2.5);
      printf("%s\n",text);

      sprintf(text, "t%d: %.2f", i, td->packet_rate[i]);
      Text(getWidth(20), getHeight(5+i*5), text, SerifTypeface, s_width/170*2.5);

      sprintf(text, "h%d: %0.2f", i, td->health_rate[i]);
      Text(getWidth(35), getHeight(5+i*5), text, SerifTypeface, s_width/170*2.5);
     
    }
  }
  if(td->tx_time !=0){
    sprintf(text, "l: %.1f",td->avg_latency);
    Text(getWidth(65), getHeight(5), text, SerifTypeface, s_width/170*2.5);

    
    sum_latency += td->latency;
    cnt_latency ++;
    if(millis() - last_latency_print > 1000 && cnt_latency>100){
      latency = sum_latency/cnt_latency;
      cnt_latency = 0;
      sum_latency = 0;
      last_latency_print = millis();
    }
  }
}

#endif

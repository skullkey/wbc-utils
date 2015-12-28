#include <stdint.h>
#include "conf.h"
#include "render.h"
#include "telemetry.h"
#include <stdlib.h>
#include "math.h"

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
  TextEnd(pos_x, pos_y, td->osd_fix_type, SerifTypeface, s_width/170 * scale);
}

void draw_center(telemetry_data_t *td, int pos_x, int pos_y, float scale)
{

//center circle
  int radius=3 * scale;
  StrokeWidth(2);
  Stroke(255,255,255,1);
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
  Polyline(x, y, z);

//home direction
float length = 15 * scale;
float arg = 
td->osd_home_direction;
VGfloat x1 = pos_x + sin(arg) * radius;
VGfloat y1 = pos_y + cos(arg) * radius;
VGfloat x2 = pos_x + sin(arg) * (radius + length);
VGfloat y2 = pos_y + cos(arg) * (radius + length);
Line(x1, y1, x2, y2);

//printf("home direction %f\n", td->osd_home_direction);



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
  char s[10];
  sprintf(s,"%.0f", td->altitude);//td->altitude);
  TextMid(pos_x_r + width / 2 + width / 5, pos_y - s_width/170 * scale / 2, s, SerifTypeface,s_width/170 * scale);
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
  sprintf(s,"%.0f", td->speed);
  TextMid (pos_x_l - width/2 - width / 5 , pos_y - s_width/170 * scale /2, s, SerifTypeface, s_width/170 * scale);
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
for( k = (int)(td->speed - range_speed/2); k <= td->speed + range_speed/2; k++)
{
  int y = pos_y + (k - td->speed) * ratio_speed;
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
  char s[30];
  float space = 15 * scale;
  sprintf(s, "Lon:%f", td->osd_lon);
  Text(pos_x, pos_y, s, SerifTypeface, s_width/170 * scale); 
  sprintf(s, "Lat:%f", td->osd_lat);
  Text(pos_x, pos_y + space, s, SerifTypeface, s_width/170 * scale);
}

void draw_power(telemetry_data_t *td, int pos_x, int pos_y, float scale)
{
  char s[10];
  float space = 15 * scale;
  sprintf(s, "%3.1fV", td->voltage);
  Text(pos_x, pos_y, s, SerifTypeface, s_width/170 * scale);
  sprintf(s, "%3.2fA", td->current);
  Text(pos_x, pos_y + space, s, SerifTypeface, s_width/170 * scale);

}
void draw_home_dist(telemetry_data_t *td, int pos_x, int pos_y, float scale)
{
  char s[20];
  sprintf(s, "%5.0fm", td->osd_home_distance);
  TextMid(pos_x, pos_y, s, SerifTypeface, s_width/170 * scale);
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

  StrokeWidth(3);
  Stroke(255,255,255,1); 
  Polyline(x,y,5);
  Fill(255,255,255,1);

  char s[5];
  sprintf(s, "%3.0f\xb0", td->heading);
  TextMid(pos_x, pos_y-height_number/4, s , SerifTypeface, height_number/2);

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
    {
      TextMid(x, pos_y + height_comp / 3 + space / 3 * 1.6 , c, SerifTypeface, s_width/170 * scale );
      draw=false;
    }
  }
}



void drawOvg(telemetry_data_t *td)
{
        Start(s_width, s_height);
//Background(0,0,0);
	//draw_bat      (td, getWidth(10), getHeight(90), 3);
	draw_compass  (td, getWidth(50), getHeight(85), 2);
	draw_power    (td, getWidth(20), getHeight(5),  2);
	draw_sat      (td, getWidth(90), getHeight(90), 2);
        draw_lat_lon  (td, getWidth(10), getHeight(10), 3);
	draw_home_dist(td, getWidth(50), getHeight(5),  3);
	draw_mode     (td, getWidth(90), getHeight(5),  3);
      	//Translate(getWidth(50), getHeight(50));
	//Rotate(rot);
	//Translate(-getWidth(50), -getHeight(50));
	draw_center(td, getWidth(50), getHeight(50),    2);
	//Rotate(-rot);
        End();                                             // End the picture
        //fgets(s, 2, stdin);                                // look at the pic, end with [RETURN]
        //finish();                                          // Graphics cleanup
        //exit(0);
}

void render_init()
{
  init(&s_width,&s_height);
}

void render(telemetry_data_t *td) {

/*

	char text[256];
	int text_y = rd->height - RENDER_OFFSET_Y - OFFSET_Y - 17;	
	int text_width = rd->width;
	int wqhd_offset = 0;
	if(text_width == 2048)
		wqhd_offset = 256;

	graphics_resource_fill(rd->img, 0, 0, rd->width, rd->height, GRAPHICS_RGBA32(0,0,0,0x00));

//draw_bat(rd, td);

//draw_rect(rd, 500, 500,10, 10, 1, 0xff, 0xff, 0xff, 0xff);


	//sprintf(text, "Current: %4.1fA", td->current);
        //render_text(text, rd, OFFSET_3D + text_width/2 - RENDER_OFFSET_X - 5.5 * FONT_WIDTH - wqhd_offset, text_y, RENDER_FONT_SIZE, 0xff, 0xff, 0xff);
        //render_text(text, rd, text_width - RENDER_OFFSET_X - 10 * FONT_WIDTH, text_y, RENDER_FONT_SIZE, 0xff, 0xff, 0xff);

	sprintf(text, "home distance: %.0fm", td->osd_home_distance);
        render_text(text, rd, OFFSET_3D + RENDER_OFFSET_X, RENDER_OFFSET_Y + OFFSET_Y, RENDER_FONT_SIZE, 0xff, 0xff, 0xff);
        //render_text(text, rd, RENDER_OFFSET_X + text_width/2 + wqhd_offset, RENDER_OFFSET_Y + OFFSET_Y, RENDER_FONT_SIZE, 0xff, 0xff, 0xff);

        sprintf(text, "home direction: %.0f\xb0", td->osd_home_direction);
        render_text(text, rd, OFFSET_3D + RENDER_OFFSET_X, RENDER_OFFSET_Y + OFFSET_Y + 30, RENDER_FONT_SIZE, 0xff, 0xff, 0xff);

        sprintf(text, "sat fix: %s", td->osd_fix_type);
        render_text(text, rd, OFFSET_3D + RENDER_OFFSET_X, RENDER_OFFSET_Y + OFFSET_Y + 60, RENDER_FONT_SIZE, 0xff, 0xff, 0xff);

        sprintf(text, "sat visible: %d", td->osd_satellites_visible);
        render_text(text, rd, OFFSET_3D + RENDER_OFFSET_X, RENDER_OFFSET_Y + OFFSET_Y + 90, RENDER_FONT_SIZE, 0xff, 0xff, 0xff);


        //render_text(text, rd, RENDER_OFFSET_X + text_width/2 + wqhd_offset, RENDER_OFFSET_Y + OFFSET_Y, RENDER_FONT_SIZE, 0xff, 0xff, 0xff);

        //render_text(text, rd, RENDER_OFFSET_X + text_width/2 + wqhd_offset, RENDER_OFFSET_Y + OFFSET_Y, RENDER_FONT_SIZE, 0xff, 0xff, 0xff);



        sprintf(text, "current: %4.1fA", td->current);
        //sprintf(text, "sat fix: %s", td->osd_fix_type);
        //render_text(text, rd, OFFSET_3D + RENDER_OFFSET_X, RENDER_OFFSET_Y + OFFSET_Y + 60, RENDER_FONT_SIZE, 0xff, 0xff, 0xff);
        render_text(text, rd, OFFSET_3D + RENDER_OFFSET_X, text_y-30, RENDER_FONT_SIZE, 0xff, 0xff, 0xff);
        //render_text(text, rd, RENDER_OFFSET_X + text_width/2 + wqhd_offset, text_y, RENDER_FONT_SIZE, 0xff, td->voltage<MIN_VOLTAGE?0:0xff, td->voltage<MIN_VOLTAGE?0:0xff);


	
	sprintf(text, "voltage: %.1fV", td->voltage);
	render_text(text, rd, OFFSET_3D + RENDER_OFFSET_X, text_y, RENDER_FONT_SIZE, 0xff, td->voltage<MIN_VOLTAGE?0:0xff, td->voltage<MIN_VOLTAGE?0:0xff);
	//render_text(text, rd, RENDER_OFFSET_X + text_width/2 + wqhd_offset, text_y, RENDER_FONT_SIZE, 0xff, td->voltage<MIN_VOLTAGE?0:0xff, td->voltage<MIN_VOLTAGE?0:0xff);

        sprintf(text, "bat remaining: %.0f%%",td->battery_remaining);
        render_text(text, rd, OFFSET_3D + RENDER_OFFSET_X, text_y + 30, RENDER_FONT_SIZE, 0xff, td->battery_remaining<MIN_REMAINING?0:0xff, td->battery_remaining<MIN_REMAINING?0:0xff);
        //render_text(text, rd, RENDER_OFFSET_X + text_width/2 + wqhd_offset, text_y, RENDER_FONT_SIZE, 0xff, td->voltage<MIN_VOLTAGE?0:0xff, td->voltage<MIN_VOLTAGE?0:0xff);


	sprintf(text, "altitude: %3.0fm", td->altitude);
	//render_text(text, rd, OFFSET_3D + text_width/2 - RENDER_OFFSET_X - 4 * FONT_WIDTH - wqhd_offset, RENDER_OFFSET_Y + OFFSET_Y, RENDER_FONT_SIZE, 0xff, 0xff, 0xff);
	render_text(text, rd, text_width - RENDER_OFFSET_X - 14 * FONT_WIDTH, RENDER_OFFSET_Y + OFFSET_Y, RENDER_FONT_SIZE, 0xff, 0xff, 0xff);


        sprintf(text, "groundspeed: %3.0fkm/h", td->speed);
        //render_text(text, rd, OFFSET_3D + text_width/2 - RENDER_OFFSET_X - 4 * FONT_WIDTH - wqhd_offset, RENDER_OFFSET_Y + OFFSET_Y, RENDER_FONT_SIZE, 0xff, 0xff, 0xff);
        render_text(text, rd, text_width - RENDER_OFFSET_X - 14 * FONT_WIDTH, RENDER_OFFSET_Y + OFFSET_Y + 30, RENDER_FONT_SIZE, 0xff, 0xff, 0xff);

        sprintf(text, "mode: %s", td->mode);
        //render_text(text, rd, OFFSET_3D + text_width/2 - RENDER_OFFSET_X - 4 * FONT_WIDTH - wqhd_offset, RENDER_OFFSET_Y + OFFSET_Y, RENDER_FONT_SIZE, 0xff, 0xff, 0xff);
        render_text(text, rd, text_width - RENDER_OFFSET_X - 14 * FONT_WIDTH, RENDER_OFFSET_Y + OFFSET_Y + 60, RENDER_FONT_SIZE, 0xff, 0xff, 0xff);

//printf("homedire %4.0f",td->osd_home_direction);
        //sprintf(text, "home direction: %4.0f°", td->osd_home_direction);
	//render_text(text, rd, OFFSET_3D + text_width/2 - 6.5 * FONT_WIDTH - wqhd_offset/2, RENDER_OFFSET_Y + OFFSET_Y, RENDER_FONT_SIZE, 0xff, 0xff, 0xff);
	//render_text(text, rd, text_width/4 + text_width/2 - 3.5 * FONT_WIDTH + wqhd_offset/2, RENDER_OFFSET_Y + OFFSET_Y, RENDER_FONT_SIZE, 0xff, 0xff, 0xff);

        sprintf(text, "heading: %4.0f\xb0", td->heading);
        render_text(text, rd, OFFSET_3D + text_width/2 - 6.5 * FONT_WIDTH - wqhd_offset/2, RENDER_OFFSET_Y + OFFSET_Y, RENDER_FONT_SIZE, 0xff, 0xff, 0xff);
        //render_text(text, rd, text_width/4 + text_width/2 - 3.5 * FONT_WIDTH + wqhd_offset/2, RENDER_OFFSET_Y + OFFSET_Y, RENDER_FONT_SIZE, 0xff, 0xff, 0xff);


        sprintf(text, "roll: %4.0f\xb0", td->roll);
        render_text(text, rd, OFFSET_3D + text_width - RENDER_OFFSET_Y - 14 * FONT_WIDTH , text_y - 30, RENDER_FONT_SIZE, 0xff, 0xff, 0xff);
        //render_text(text, rd, text_width/4 + text_width/2 - 3.5 * FONT_WIDTH + wqhd_offset/2, RENDER_OFFSET_Y + OFFSET_Y, RENDER_FONT_SIZE, 0xff, 0xff, 0xff);

        sprintf(text, "pitch: %4.0f\xb0", td->pitch);
        render_text(text, rd, OFFSET_3D + text_width  - RENDER_OFFSET_Y - 14 * FONT_WIDTH , text_y - 60, RENDER_FONT_SIZE, 0xff, 0xff, 0xff);
        //render_text(text, rd, text_width/4 + text_width/2 - 3.5 * FONT_WIDTH + wqhd_offset/2, RENDER_OFFSET_Y + OFFSET_Y, RENDER_FONT_SIZE, 0xff, 0xff, 0xff)
      

        sprintf(text,"__ | __");
        render_text(text, rd, OFFSET_3D + text_width/2 - 3.5 * FONT_WIDTH - wqhd_offset/2, rd->height/2, RENDER_FONT_SIZE, 0xff, 0xff, 0xff);


        sprintf(text,"H");
	int length = 150;
        render_text(text, rd, OFFSET_3D + text_width/2 - 3.5 * FONT_WIDTH - wqhd_offset/2 + sin((td->osd_home_direction*3.14/180))*length + 23, rd->height/2 - cos((td->osd_home_direction*3.14/180))*length, RENDER_FONT_SIZE, 0xff, 0xff, 0xff);


  // transparent before display to avoid screen flash
   //graphics_resource_fill(rd->img, 0, 0, width, height, GRAPHICS_RGBA32(0,0,0,0x00));

   //graphics_display_resource(rd->img, 0, LAYER, 0, 0, GRAPHICS_RESOURCE_WIDTH, GRAPHICS_RESOURCE_HEIGHT, VC_DISPMAN_ROT0, 1);
        //sprintf(text,"P");
        //render_text(text, rd,100 ,100 , RENDER_FONT_SIZE, 0xff, 0xff, 0xff);


	//sprintf(text, "%f", td->latitude);
	//render_text(text, rd, RENDER_OFFSET_X + 4 * RENDER_SPACING, text_y, RENDER_FONT_SIZE, 0xff, 0xff, 0xff);

	//sprintf(text, "%f", td->longitude);
	//render_text(text, rd, RENDER_OFFSET_X + 6 * RENDER_SPACING, text_y, RENDER_FONT_SIZE, 0xff, 0xff, 0xff);

   // blue, at the top (y=40)
      graphics_resource_fill(rd->img, 0, 40, rd->width, 1, GRAPHICS_RGBA32(0,0,0xff,0xff));

      // green, at the bottom (y=height-40)
      graphics_resource_fill(rd->img, 0, rd->height-40, rd->width, 1, GRAPHICS_RGBA32(0,0xff,0,0xff));

	graphics_update_displayed_resource(rd->img, 0, 0, 0, 0);
*/
drawOvg(td);

}

//#endif

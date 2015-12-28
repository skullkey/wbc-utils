/*
Copyright (c) 2015, befinitiv
Copyright (c) 2012, Broadcom Europe Ltd
All rights reserved.
Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:
* Redistributions of source code must retain the above copyright
notice, this list of conditions and the following disclaimer.
* Redistributions in binary form must reproduce the above copyright
notice, this list of conditions and the following disclaimer in the
documentation and/or other materials provided with the distribution.
* Neither the name of the copyright holder nor the
names of its contributors may be used to endorse or promote products
derived from this software without specific prior written permission.
THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY
DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CA

#include <stdio.h>ND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/
#include <stdint.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>


#include "conf.h"



#include "telemetry.h"
#include "mavlink_parse.h"
#include "render.h"

telemetry_data_t td;
int main(int argc, char *argv[]) {

  
	//#ifdef RENDER
	//render_data_t rd;
//printf("render is defined\n");
	//#endif

	uint8_t buf[256];
	size_t n;
//	frsky_state_t fs;
	//telemetry_data_t td;


	if(argc != 2) {
		printf("Usage: %s <font path>\n",argv[0]);
		return 1;
	}

	#ifdef RENDER
	render_init();
	#endif


	td.osd_alt_prev=0; 
	td.osd_alt=0;
	td.osd_alt_cnt=0 ; // counter for stable osd_alt
	td.osd_got_home=0 ; // tels if got home position or not
	td.osd_fix_type="no fix" ; 
	td.osd_home_lat=0 ; // home latidude
	td.osd_lat=0 ; // latidude
	td.osd_lon=0 ; 
	td.osd_home_lon=0 ; // home longitude
	td.osd_home_alt=0 ; 
	td.osd_home_distance=0 ; // distance from home
	td.osd_home_direction=0; // Arrow direction pointing to home (1-16 to CW loop)
	td.osd_home_alt=0 ; 
	td.osd_home_distance=0 ; 
	//td.osd_heading=0 ;
	td.haltset=0 ;
	td.osd_alt_prev=0;
        td.osd_satellites_visible=0;
	td.fix_type = 0;
	td.start_lat = 0.0;
	td.start_lng = 0.0;
	td.armed = 0;
	td.current = 0.0;
	td.voltage = 0.0;
	td.altitude = 0.0;
	td.longitude = 0.0;
        td.mode = "Test";
	td.latitude = 0.0;
	td.heading = 0.0;
        td.roll = 0.0;
	td.pitch = 0.0;
	td.speed = 0.0;
	td.battery_remaining = 0;
	td.heading = 0;

        //td.voltage = 30.0;
        //render(&td, &rd);


	for(;;) {
		n = read(STDIN_FILENO, buf, sizeof(buf));
/*
		//printf("%d \n",n);
		if(n == 0) {
		//	break; //EOF
		}

		if(n<0) {
			perror("read");
			exit(-1);
		}
*/
		if(mavlink_parse_buffer(&td, buf, n)) {
			#ifdef RENDER
			printf("mavlink parse ok\n");
			render(&td);
			#endif
		}
//		else
//printf("error parsing\n");
	// td.voltage = 30.0;
	//render(&td, &rd);

	}
	return 0;
}




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
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/select.h>


#include "conf.h"



#include "telemetry.h"
#include "mavlink_parse.h"
#include "render.h"
int main(int argc, char *argv[]) {

	fd_set set;
  	struct timeval timeout;

	uint8_t buf[256];
	size_t n;

	if(argc != 2) {
		printf("Usage: %s <font path>\n",argv[0]);
		return 1;
	}

        #ifdef RENDER
	  render_init();
        #endif
	  
        telemetry_data_t td;
	telemetry_init(&td);

        for(;;) {
	  FD_ZERO(&set);
	  FD_SET(STDIN_FILENO, &set);
	  timeout.tv_sec = 2;
	  timeout.tv_usec = 5e5;
	  n = select(STDIN_FILENO + 1, &set, NULL, NULL, &timeout);

	  if(n > 0) {
	    n = read(STDIN_FILENO, buf, sizeof(buf));
	    //	    printf("n=%d\n",n);
	    if(n == 0) {
	      //	break; //EOF
	    }

	    if(n<0) {
	      perror("read");
	      exit(-1);
	    }
	    if(mavlink_parse_buffer(&td, buf, n)) {
	      printf("mavlink parse ok\n");
	      #ifdef RENDER
	      render(&td);
	      #endif 
	    }
	  } else {
	    printf("skipping\n");
	    #ifdef RENDER
	      render_rx_status(&td,0);
	    #endif  
	  }

	}
	return 0;
}




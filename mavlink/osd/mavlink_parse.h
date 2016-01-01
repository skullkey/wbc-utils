#pragma once
#define ToDeg(x) (x*57.2957795131)  

#include "telemetry.h"


int mavlink_parse_buffer(telemetry_data_t *td, uint8_t *buf, int buflen);




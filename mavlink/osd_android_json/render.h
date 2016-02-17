#pragma once

#ifdef RENDER


#include "telemetry.h"


#define HUD_R 20
#define HUD_G 200
#define HUD_B 20
#define PI 3.1415927

void render_init();
void render(telemetry_data_t *td);
void render_rx_status(telemetry_data_t *rd);

#endif

#pragma once

#ifdef RENDER

#include "bcm_host.h"
#include "vgfont.h"
#include "VG/openvg.h"
#include "VG/vgu.h"
#include "fontinfo.h"
#include "shapes.h"

#include "telemetry.h"


#define HUD_R 20
#define HUD_G 200
#define HUD_B 20
#define PI 3.1415927

void render_init();
void render(telemetry_data_t *td, uint8_t display_rx_stats);
void render_rx_status(telemetry_data_t *rd, uint8_t embedded);

#endif

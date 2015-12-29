#pragma once

#ifdef RENDER

#include "bcm_host.h"
#include "vgfont.h"
#include "VG/openvg.h"
#include "VG/vgu.h"
#include "fontinfo.h"
#include "shapes.h"

#include "telemetry.h"

void render_init();
void render(telemetry_data_t *td);
void render_rx_status(telemetry_data_t *rd, uint8_t embedded);

#endif

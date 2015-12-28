#pragma once

#ifdef RENDER

#include "bcm_host.h"
#include "vgfont.h"
#include "VG/openvg.h"
#include "VG/vgu.h"
#include "fontinfo.h"
#include "shapes.h"

#include "telemetry.h"
//VG_MAX_IMAGE_WIDTH 2600
/*
typedef struct {
	GRAPHICS_RESOURCE_HANDLE img;
	uint32_t width;
	uint32_t height;
} render_data_t;
*/
void render_init();
void render(telemetry_data_t *td);

#endif

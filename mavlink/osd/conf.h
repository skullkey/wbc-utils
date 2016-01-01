#pragma once

#include <stdio.h>

#define RENDER 1

#define RENDER_OFFSET_X 10
#define RENDER_OFFSET_Y 30
#define RENDER_SPACING 100 
#define RENDER_FONT_SIZE 20 


#define DEBUG 1
#define debug_print(fmt, ...) \
            do { if (DEBUG) fprintf(stderr, fmt, __VA_ARGS__); } while (0)


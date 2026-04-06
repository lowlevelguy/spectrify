#pragma once

#include <stdint.h>
#include <stddef.h>

typedef struct {
	uint8_t r,g,b;
} pixel_t;


void render_frame(double* levels, size_t levels_sz,
		pixel_t* frame, size_t frame_width, size_t frame_height);

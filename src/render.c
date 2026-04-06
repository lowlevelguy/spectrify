#include <string.h>
#include <math.h>
#include "render.h"

#define MIN_DB		-80.0
#define MAX_DB		-10.0
#define CLAMP_DB(x)	fmax(fmin((double)(x), MAX_DB), MIN_DB)

void render_frame(double* levels, size_t levels_sz,
		pixel_t* frame, size_t frame_width, size_t frame_height) {
	// Set the frame buffer to black
	memset(frame, 0, sizeof(pixel_t)*frame_width*frame_height);

	// The bar graph occupies the whole of the frame's width
	// and a third of its height; occupying the middle third
	// top-down.
	size_t bar_max_height = frame_height/3,
		   bar_width = (size_t)(frame_width / (levels_sz + (levels_sz-1) / 4.0)),
		   gap_width = bar_width/4,
		   bar_height, x_offset;
	for (int i = 0; i < levels_sz; i++) {
		bar_height = (CLAMP_DB(levels[i])-MIN_DB) * bar_max_height / (MAX_DB-MIN_DB);
		x_offset = (bar_width + gap_width) * i;

		for (int j = 0; j < bar_height; j++)
			memset(&frame[(frame_height * 2/3 - 1 - j) * frame_width + x_offset], 255, bar_width * sizeof(pixel_t));
	}
}

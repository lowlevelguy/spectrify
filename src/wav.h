#pragma once

#include <stdio.h>
#include <stdint.h>

#define ERR_RIFF_MAGIC_MISSING			-1
#define ERR_WAVE_MAGIC_MISSING 			-2
#define ERR_FMT_MAGIC_MISSING 			-3
#define ERR_DATA_MAGIC_MISSING 			-4
#define ERR_WAV_FORMAT_INVALID 			-5
#define ERR_SAMPLE_SIZE_INCOMPATIBLE	-6
#define ERR_BYTE_RATE_INCOMPATIBLE		-7
#define ERR_FREAD_FAIL					INT_MIN

#define WAV_FORMAT_PCM					1
#define WAV_FORMAT_IEEE_FLOAT			3
#define WAV_FORMAT_ALAW					6
#define WAV_FORMAT_MULAW				7
#define WAV_FORMAT_EXTENSIBLE 			0xFFFE

typedef struct {
	uint16_t audio_format, num_channels, sample_size, resolution;
	uint32_t sample_freq, byte_rate, data_size, data_offset;
} wav_info_t;

int wav_read_header(FILE* wav_file, wav_info_t* wav_info);

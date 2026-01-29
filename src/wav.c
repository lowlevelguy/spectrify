#include "wav.h"
#include <stdio.h>
#include <string.h>

int wav_read_header(FILE* wav_file, wav_info_t* wav_info) {
	uint8_t qword[4], dword[2];

	fseek(wav_file, 0, SEEK_SET);

	// Check RIFF magic string
	fread(qword, 1, sizeof qword, wav_file);
	if (memcmp(qword, "RIFF", sizeof qword) != 0)
		return ERR_RIFF_MAGIC_MISSING;

	// Check WAVE magic string
	fseek(wav_file, 4, SEEK_CUR);
	fread(qword, 1, sizeof qword, wav_file);
	if (memcmp(qword, "WAVE", sizeof qword) != 0)
		return ERR_WAVE_MAGIC_MISSING;

	// Check fmt magic string
	fread(qword, 1, sizeof qword, wav_file);
	if (memcmp(qword, "fmt ", sizeof qword) != 0)
		return ERR_FMT_MAGIC_MISSING;

	// Read audio format
	fseek(wav_file, 4, SEEK_CUR);
	fread(dword, 1, sizeof dword, wav_file);
	wav_info->audio_format = dword[1] + (dword[0] << 8);
	if (wav_info->audio_format != WAV_FORMAT_PCM &&
		wav_info->audio_format != WAV_FORMAT_IEEE_FLOAT &&
		wav_info->audio_format != WAV_FORMAT_ALAW &&
		wav_info->audio_format != WAV_FORMAT_MULAW &&
		wav_info->audio_format != WAV_FORMAT_EXTENSIBLE)
		return ERR_WAV_FORMAT_INVALID;

	// Read number of channels
	fread(dword, 1, sizeof dword, wav_file);
	wav_info->num_channels = dword[1] + (dword[0] << 8);

	// Read sample rate
	fread(qword, 1, sizeof qword, wav_file);
	wav_info->sample_freq = qword[3] + 
						(qword[2] << 8) +
						(qword[1] << 16) +
						(qword[0] << 24); 

	// Read byte rate
	fread(qword, 1, sizeof qword, wav_file);
	wav_info->byte_rate = qword[3] + 
						(qword[2] << 8) +
						(qword[1] << 16) +
						(qword[0] << 24); 
	
	// Read sample size (bytes per sample)
	fread(dword, 1, sizeof dword, wav_file);
	wav_info->sample_size = dword[1] + (dword[0] << 8);

	// Read resolution (bits per sample per channel)
	fread(dword, 1, sizeof dword, wav_file);
	wav_info->resolution = dword[1] + (dword[0] << 8);


	/*
	 * Check compatibility between the previous entries
	 * We need:
	 * 		- sample_size == num_channels * resolution/8
	 * 		- byte_rate == sample_rate * sample_size
	 */
	if (wav_info->sample_size != wav_info->num_channels * wav_info->resolution/8)
		return ERR_SAMPLE_SIZE_INCOMPATIBLE;
	
	if (wav_info->byte_rate != wav_info->sample_freq * wav_info->num_channels * wav_info->resolution/8)
		return ERR_BYTE_RATE_INCOMPATIBLE;

	// Check data magic string
	fread(qword, 1, sizeof qword, wav_file);
	if (memcmp(qword, "data", sizeof qword) != 0)
		return ERR_DATA_MAGIC_MISSING;

	// Read data size
	fread(qword, 1, sizeof qword, wav_file);
	wav_info->data_size = qword[3] + 
						(qword[2] << 8) +
						(qword[1] << 16) +
						(qword[0] << 24); 
	
	// Set data offset to current cursor position
	wav_info->data_offset = ftell(wav_file);

	return 0;
}

#include "wav.h"
#include <string.h>

int wav_read_header(FILE* wav_file, wav_info_t* wav_info) {
	uint8_t dword[4], word[2];

	fseek(wav_file, 0, SEEK_SET);

	// Check RIFF magic string
	fread(dword, 1, sizeof dword, wav_file);
	if (memcmp(dword, "RIFF", sizeof dword) != 0)
		return ERR_RIFF_MAGIC_MISSING;

	// Check WAVE magic string
	fseek(wav_file, 4, SEEK_CUR);
	fread(dword, 1, sizeof dword, wav_file);
	if (memcmp(dword, "WAVE", sizeof dword) != 0)
		return ERR_WAVE_MAGIC_MISSING;

	// Check fmt magic string
	fread(dword, 1, sizeof dword, wav_file);
	if (memcmp(dword, "fmt ", sizeof dword) != 0)
		return ERR_FMT_MAGIC_MISSING;

	// Read remaining fmt subchunk size
	fread(dword, 1, sizeof dword, wav_file);
	uint32_t fmt_remaining = dword[0] + 
						(dword[1] << 8) +
						(dword[2] << 16) +
						(dword[3] << 24); 

	// Read audio format
	fread(word, 1, sizeof word, wav_file);
	wav_info->audio_format = word[0] + (word[1] << 8);
	if (wav_info->audio_format != WAV_FORMAT_PCM &&
		wav_info->audio_format != WAV_FORMAT_IEEE_FLOAT &&
		wav_info->audio_format != WAV_FORMAT_ALAW &&
		wav_info->audio_format != WAV_FORMAT_MULAW &&
		wav_info->audio_format != WAV_FORMAT_EXTENSIBLE)
		return ERR_WAV_FORMAT_INVALID;

	// Read number of channels
	fread(word, 1, sizeof word, wav_file);
	wav_info->num_channels = word[0] + (word[1] << 8);

	// Read sample rate
	fread(dword, 1, sizeof dword, wav_file);
	wav_info->sample_freq = dword[0] + 
						(dword[1] << 8) +
						(dword[2] << 16) +
						(dword[3] << 24); 

	// Read byte rate
	fread(dword, 1, sizeof dword, wav_file);
	wav_info->byte_rate = dword[0] + 
						(dword[1] << 8) +
						(dword[2] << 16) +
						(dword[3] << 24); 
	
	// Read sample size (bytes per sample)
	fread(word, 1, sizeof word, wav_file);
	wav_info->block_align = word[0] + (word[1] << 8);

	// Read resolution (bits per sample per channel)
	fread(word, 1, sizeof word, wav_file);
	wav_info->bits_per_sample = word[0] + (word[1] << 8);

	// Skip remaining fields
	fseek(wav_file, fmt_remaining - (4 * sizeof word + 2 * sizeof dword), SEEK_CUR);

	/*
	 * Check compatibility between the previous entries
	 * We need:
	 * 		- sample_size == num_channels * resolution/8
	 * 		- byte_rate == sample_rate * sample_size
	 */
	if (wav_info->block_align != wav_info->num_channels * wav_info->bits_per_sample/8)
		return ERR_SAMPLE_SIZE_INCOMPATIBLE;
	
	if (wav_info->byte_rate != wav_info->sample_freq * wav_info->num_channels * wav_info->bits_per_sample/8)
		return ERR_BYTE_RATE_INCOMPATIBLE;

	// If we encounter any non-data subchunk, skip it
	int skipping_subchunks = 1;
	uint32_t subchunk_remaining;
	while (skipping_subchunks) {
		fread(dword, 1, sizeof dword, wav_file);
		if (memcmp(dword, "data", sizeof dword) == 0)
			skipping_subchunks = 0;
		else {
			fread(dword, 1, sizeof dword, wav_file);
			subchunk_remaining = dword[0] + 
							(dword[1] << 8) +
							(dword[2] << 16) +
							(dword[3] << 24); 
			// WAV chunks are always 2-bytes aligned; so account for padding
			// in the case that subchunk_remaining is odd.
			fseek(wav_file, subchunk_remaining + (subchunk_remaining % 2), SEEK_CUR);
			skipping_subchunks = 1;
		}
	}
	// Read data size
	fread(dword, 1, sizeof dword, wav_file);
	wav_info->data_size = dword[0] + 
						(dword[1] << 8) +
						(dword[2] << 16) +
						(dword[3] << 24); 
	
	// Set data offset to current cursor position
	wav_info->data_offset = ftell(wav_file);

	return 0;
}

int wav_get_frame(FILE* wav_file, wav_info_t wav_info, uint8_t fps, uint32_t frame_offset, void* buffer, uint32_t buffer_size) {
	if (wav_info.audio_format != WAV_FORMAT_PCM)
		return ERR_WAV_FORMAT_UNSUPPORTED;
	
	// Make sure the buffer is large enough to contain the frame data
	uint32_t frame_sz = wav_info.sample_freq / fps;
	if (buffer_size < frame_sz)
		return ERR_BUFFER_TOO_SMALL;

	// Seek to the indicated frame offset, and read one frame
	int ret;
	fseek(wav_file, wav_info.data_offset + wav_info.block_align * frame_sz * frame_offset, SEEK_SET);
	if ((ret = fread(buffer, wav_info.block_align, frame_sz, wav_file)) == 0)
		return ERR_FREAD_FAIL;

	return ret;
}

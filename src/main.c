#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include "wav.h"

int main() {
	printf("Spectrify - Generate Spectrogram from Audio File\n");

	FILE* input = fopen("tests/audio.wav", "rb");
	wav_info_t input_info = {0};

	if (input == NULL) {
		printf("Failed to open file");
		return -1;
	}

	if (wav_read_header(input, &input_info) != 0) {
		printf("Some error was encoutered.\n");
		return -1;
	}

	// Priting WAV file info
	printf("Audio format: %u\n", input_info.audio_format);
	printf("Number of channels: %u\n", input_info.num_channels);
	printf("Sample size: %u\n", input_info.block_align);
	printf("Bits per sample: %u\n", input_info.bits_per_sample);
	printf("Sample frequency: %u\n", input_info.sample_freq);
	printf("Byte rate: %u\n", input_info.byte_rate);
	printf("Data size: %u\n", input_info.data_size);
	printf("Data offset: %u\n", input_info.data_offset);

	// Reading audio frame by frame
	int total_samples = 0, read_samples = 0;
	uint16_t *buffer = malloc(4096 * sizeof(uint16_t));
	size_t fsize, remaining;

	fseek(input, 0, SEEK_END);
	fsize = ftell(input);

	for (int i = 0; read_samples < input_info.data_size; i++) {
		read_samples = wav_get_frame(input, input_info, 24, i, buffer, 4096 * sizeof(uint16_t));
		if (read_samples == 0) {
			printf("Done reading.\n");
			break;
		}
		if (read_samples < 0) {
			printf("Some error occured. Return value: %d.\n", read_samples);
			break;
		}
		total_samples += read_samples;
		remaining = fsize - ftell(input);
		printf("Read %d bytes. %d bytes in total. %lu many bytes left in the file.\n", read_samples * input_info.block_align, total_samples * input_info.block_align, remaining);
	}

	free(buffer);

	return 0;
}

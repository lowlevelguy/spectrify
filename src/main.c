#include <stdio.h>
#include "wav.h"

int main() {
	printf("Spectrify - Generate Spectrogram from Audio File\n");

	FILE* input = fopen("tests/audio.wav", "rb");
	wav_info_t input_info = {0};

	if (input == NULL) {
		printf("Failed to open file");
		return -1;
	}

	if (wav_read_header(input, &input_info) != 0)
		printf("Some error was encoutered.\n");


	printf("Audio format: %u\n", input_info.audio_format);
	printf("Number of channels: %u\n", input_info.num_channels);
	printf("Sample size: %u\n", input_info.block_align);
	printf("Bits per sample: %u\n", input_info.bits_per_sample);
	printf("Sample frequency: %u\n", input_info.sample_freq);
	printf("Byte rate: %u\n", input_info.byte_rate);
	printf("Data size: %u\n", input_info.data_size);
	printf("Data offset: %u\n", input_info.data_offset);

	return 0;
}

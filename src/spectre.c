#include "spectre.h"
#include <math.h>
#include <stdlib.h>

void hann(double* w, size_t w_size) {
	for (int i = 0; i < w_size; i++)
		w[i] = 0.5 - 0.5 * cos(2 * M_PI * i / w_size);
}

int log_power_stft(double* input, size_t input_size, double* output, size_t output_size) {
	fftw_complex* stft = fftw_alloc_complex(output_size);
	double* window = malloc(input_size * sizeof(double)), *w_input = malloc(input_size * sizeof(double));

	hann(window, input_size);
	for (size_t i = 0; i < input_size; i++)
		w_input[i] = input[i] * window[i];
	// perform stft with r2c_1d

	for (size_t i = 0; i < output_size/2+1; i++) {
		output[i] = 10 * log10(stft[i][0]*stft[i][0] + stft[i][1]*stft[i][1]);
		output[output_size-i] = output[i];
	}

	fftw_free(stft);
	free(w_input);
	return 0;
}

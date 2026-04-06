#pragma once

#include <fftw3.h>

int log_power_stft(double* input, size_t input_size, double* output, size_t output_size);

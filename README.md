# Spectrify
A CLI tool to generate spectral waveforms from audio files. This project is unfinished: currently, it can only read a WAV file and output its header information.

## Building
The only dependency of this program is the [`fftw3`](https://www.fftw.org/) project. Make sure to have it installed on your system and visible to the linker, or manually add the library path in the `Makefile`.

Once dependencies are solved, simply run `make` to build the program with `gcc`. If you wish to use a different compiler, you can carry out the necessary changes to the `Makefile`.

The binary will then be generated in the project root folder with the name `spectrify`.

## Formats
Input audio is expected to be in provided in the ["canonical" WAV file format](https://ccrma.stanford.edu/courses/422-winter-2014/projects/WaveFormat/), with only the `fmt` and `data` subchunks in the RIFF header. If an extra subchunk is present between the two, the program will interpret that as an erroneous format.

(To Be Implemented:) The output will be a video file of configurable FPS, presenting the spectral waveform of the input audio as a white bar graph on a black background. What exactly is meant by "spectral waveform" is detailed in the **How It Works** section.

## How It Works (To Be Implemented)
Once the audio data is extracted from the WAV file, it is segmented into chunks of size corresponding to a single frame duration. On every chunk, Short Time Fourier Transform (STFT) is performed with a Hann window to obtain the Fourier coefficients for a number of frequency bins.

Then, for every frequency bin, we compute the corresponding spectral power in dB: `P_dB = 10*log10(P)`, where `P` is the modulus squared of the corresponding Fourier coefficients.

Each chunk's log-power coefficients are then used to draw a white bar graph on a black background on a 1920x1080 canvas; each of these corresponds to a frame in the output video. These frames are then forwarded to FFMPEG to render a final video.

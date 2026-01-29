# Spectrify
A CLI tool to generate spectrograms from audio files.

## Building
The following section assumes compilation is done using the GNU GCC compiler. In any other case, the appropriate changes should be made to the `Makefile`.

### Dependencies
The only dependency of this program is the [`fftw3`](https://www.fftw.org/) project. Make sure to have it installed on your system and visible to the linker; otherwise, you can carry out the necessary changes to the `Makefile`.

### Building
To build the program, simply run:
```
make
```

The binary will then be generated in the project root folder with the name `spectrify`.

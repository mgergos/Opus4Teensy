# Opus4Teensy
Real-time fixed-point implementation of Opus Encoder/Decoder for Teensy 4.0

Notes;
1. Opus prefers 48Khz and frame sizes of 2.5, 5, 10 and 20 ms
2. Opus requires many build flags for compilation, so use platformIO, not Arduino
3. A few tweaks to the audio library are required as shown below;

Edit the following lines in C:\Users\..\.platformio\packages\framework-arduinoteensy\cores\teensy4\AudioStream.h
    #define AUDIO_BLOCK_SAMPLES  960 // Was 128
    #define AUDIO_SAMPLE_RATE_EXACT 48000.0f  // Was 44100

Add the following lines in C:\Users\..\.platformio\packages\framework-arduinoteensy\libraries\Audio\Audio.h
    #include "input_opus_dec.h"
    #include "output_opus_enc.h"

Add the following files to C:\Users\..\.platformio\packages\framework-arduinoteensy\libraries\Audio
input_opus_dec.h
output_opus_enc.h
input_opus_dec.c
output_opus_enc.c

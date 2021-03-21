# Opus4Teensy
## A real-time fixed-point implementation of Opus Encoder/Decoder for Teensy 4.0

Encodes / decodes full band audio to as low as 16 kbs without significant damage.  
Originating from nRF's SDK, the Opus-1.2.1 folder is essentially untouched.  
I've simply wrapped it as per https://www.pjrc.com/teensy/td_libs_AudioNewObjects.html (NB. actually wasn't that simple!)  

The sample loopback main.cpp runs on Teensy 4.0 with Audio Shield Rev D.
The SGTL5000's input is encoded and passed directly to the decoder.
The data rate 64000 by default and can be configured using setBitrate().

A few notes;
1. Opus requires many build flags for compilation, so use platformIO environment
2. Opus prefers 48Khz and frame sizes of 2.5, 5, 10 and 20 ms
3. Many of PJRC's audio libraries are hard coded to 44100, thus may not play nice
4. A few tweaks to the audio library are required as shown below;

Edit the following lines in C:\Users\..\.platformio\packages\framework-arduinoteensy\cores\teensy4\AudioStream.h  
#define AUDIO_BLOCK_SAMPLES  960 // Was 128  
#define AUDIO_SAMPLE_RATE_EXACT 48000.0f  // Was 44100  

Add the following lines to C:\Users\..\.platformio\packages\framework-arduinoteensy\libraries\Audio\Audio.h  
#include "input_opus_dec.h"  
#include "output_opus_enc.h"  

Add the following files to C:\Users\..\.platformio\packages\framework-arduinoteensy\libraries\Audio  
input_opus_dec.h  
output_opus_enc.h  
input_opus_dec.c  
output_opus_enc.c  

Ensure your platformio.ini file contains the opus build flags and compile....  
Happy compressing :)

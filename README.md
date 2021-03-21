# Opus4Teensy  
## Real-time Opus Encoding / Decoding for Teensy 4.0 🚧

Encodes and Decodes full band audio down to 16 kbs without significant artifacts.  
Originating from nRF's SDK, the Opus-1.2.1 folder is essentially untouched.  
I've simply wrapped it as per https://www.pjrc.com/teensy/td_libs_AudioNewObjects.html  

The sample loopback main.cpp runs on Teensy 4.0 with Audio Shield Rev D.  
The SGTL5000's input is encoded and passed directly to the decoder.  
The the default data rate of 64k can be re-configured using setBitrate().  

A few notes;
1. Opus requires numerous build flags for compilation, so recommend platformIO environment
2. Opus prefers 48Khz and frame sizes of 2.5, 5, 10 and 20 ms - default 20 ms for this implementation
3. Many of PJRC's audio libraries are hard coded to 44100, thus they may not play nice anymore
4. A few tweaks to the audio library are required as shown below;

Edit the following lines in C:\Users\..\.platformio\packages\framework-arduinoteensy\cores\teensy4\AudioStream.h  
`#define AUDIO_BLOCK_SAMPLES  960 // Was 128`  
`#define AUDIO_SAMPLE_RATE_EXACT 48000.0f  // Was 44100`  

Add the following lines to C:\Users\..\.platformio\packages\framework-arduinoteensy\libraries\Audio\Audio.h  
`#include "input_opus_dec.h"`  
`#include "output_opus_enc.h"`  

Add the following files to C:\Users\..\.platformio\packages\framework-arduinoteensy\libraries\Audio  
input_opus_dec.h  
output_opus_enc.h  
input_opus_dec.c  
output_opus_enc.c  

Ensure your platformio.ini file contains the opus build flags and compile....  
Happy compressing 😄


![](/assets/Opus_over_LORA.jpg)

# Opus4Teensy  
## Real-time Opus Encoding / Decoding for Teensy 4.0 🚧

Encodes and Decodes fullband audio down to 16 kbs without significant artefacts.  
Originating from nRF's SDK, the Opus-1.2.1 folder is essentially untouched.  
I've simply wrapped it as per PJRC's guidelines https://www.pjrc.com/teensy/td_libs_AudioNewObjects.html  

The sample loopback main.cpp runs on Teensy 4.0 with Audio Shield Rev D.  
The SGTL5000's input is encoded and passed directly to the decoder.  
The default data rate of 64k can be re-configured using setBitrate().  

A few notes;
1. Opus requires numerous build flags for compilation, thus recommend using platformIO environment
2. Opus is optimised for 48Khz and fixed frame sizes - default is 20 ms for this implementation
3. Many of PJRC's audio libraries are hard coded to 44100, thus they may not play nice anymore
4. A few tweaks to the audio library are required as shown below;

**Edit** the following lines in C:\Users...platformio\packages\framework-arduinoteensy\cores\teensy4\AudioStream.h  
`#define AUDIO_BLOCK_SAMPLES  960 // Was 128`  
`#define AUDIO_SAMPLE_RATE_EXACT 48000.0f  // Was 44100`  

**Add** the following lines to C:\Users...platformio\packages\framework-arduinoteensy\libraries\Audio\Audio.h  
`#include "input_opus_dec.h"`  
`#include "output_opus_enc.h"`  

**Move** the following files to C:\Users...platformio\packages\framework-arduinoteensy\libraries\Audio  
input_opus_dec.h  
output_opus_enc.h  
input_opus_dec.c  
output_opus_enc.c  

Ensure your platformio.ini file contains the opus build flags and compile....  

Wanting to decode asynchronous data transmissions? putData() returns the time since update() was last called.  
Decoder synchronisation is then possible by direct manipulation of the denominator within the Audio PLL’s phase fractional divider.  
Minor changes to PLL4 PFD from main() will not impact audio continuity eg;  
`int32_t denominator = CCM_ANALOG_PLL_AUDIO_DENOM;`  
`...`  
`phase = opusDecoder.putData(opusBuffer, opusBufSize);`  
`CCM_ANALOG_PLL_AUDIO_DENOM = (denominator + (phase/100) - 100);`  

Happy compressing 😄  

_Fullband audio over 32 kbs RF link - Opus over LORA_
![](https://github.com/mgergos/assets/blob/main/Opus_over_LORA.jpg?raw=true)

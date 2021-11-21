# Opus4Teensy  
## Real-time Opus Encoding / Decoding for Teensy 4.0 🚧

Encodes and Decodes fullband audio to 64 kbs without significant impact to audio quality.  
Originating from nRF's SDK, the Opus-1.2.1 folder is essentially untouched.   

The sample main.cpp runs on Teensy 4.0 with Audio Shield Rev D.  
The SGTL5000's input is encoded and passed directly to the decoder.  
The default data rate of 64k can be re-configured using setBitrate().  

A few notes;
1. Opus requires numerous build flags for compilation, thus recommend platformIO IDE, not Arduino
2. Opus is optimised for 48Khz and fixed frame sizes - default is 20 ms for this implementation
3. Some PJRC audio libraries are hard coded to 44100, thus they may not play nice anymore
4. A couple tweaks to the audio library are required as shown below;

**Edit** the following lines in C:\Users...platformio\packages\framework-arduinoteensy\cores\teensy4\AudioStream.h  
`#define AUDIO_BLOCK_SAMPLES  960 // Was 128`  
`#define AUDIO_SAMPLE_RATE_EXACT 48000.0f  // Was 44100`

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

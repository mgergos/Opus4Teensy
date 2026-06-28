# Opus4Teensy  
## Real-time Opus Encoding / Decoding for Teensy 4.0/4.1 🚧

Now in Stereo!

Encodes and/or Decodes fullband stereo audio (2ch 16-bit 48 KHz) on the Teensy 4.0/4.1  
Originating from nRF's Smart Remote SDK, the Opus-1.2.1 folder is essentially untouched.   

The sample main.cpp runs on Teensy 4.0/4.1 with Audio Shield Rev D.  
The SGTL5000's input is encoded and passed directly to the decoder.  
The default data rate of 128kbs can be re-configured using setBitrate().  

A few notes;
1. The platformio.ini invokes `teensy@5.1.0` (core 1.6.0) + redefines `AUDIO_BLOCK_SAMPLES 960` and `AUDIO_SAMPLE_RATE_EXACT 48000`
2. Many PJRC audio libraries are hard coded to 44100, thus they may not play nice anymore.
3. Opus is optimised for 48Khz and various frame sizes - default is (960/48000) 20 ms for this implementation.
4. Not a file-based implementation, i.e. does NOT encapsulate packets into an Ogg logical bitstream - See libopusfile

Wanting to decode asynchronous opus data transmissions? putData() returns the time since update() was last called.
Decoder synchronisation is then possible by direct manipulation of the denominator within the Audio PLL’s phase fractional divider.
This elliminats buffer overflows / underflows resulting from different (free-running) audio clocks. Perfect for low bitrate RF links.

Minor changes to PLL4 PFD from main() will not impact audio continuity eg;  
`int32_t denominator = CCM_ANALOG_PLL_AUDIO_DENOM;`  
`...`  
`phase = opusDecoder.putData(opusBuffer, opusBufSize);`  
`CCM_ANALOG_PLL_AUDIO_DENOM = (denominator + (phase/100) - 100);`  

Happy compressing 😄  

_Fullband audio over 32 kbs RF link - Opus over LORA_

![](https://github.com/mgergos/assets/blob/main/Opus_over_LORA.jpg?raw=true)

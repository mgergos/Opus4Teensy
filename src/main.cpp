// Demo code for Opus Encode/Decode for Teensy 4.0

#include <Arduino.h>
#include <Audio.h>
#include <Wire.h>

AudioControlSGTL5000    sgtl5000_1;
AudioInputI2S           i2s_in;
AudioOutputI2S          i2s_out;
AudioOutputOpusEnc      opusEncoder;
AudioInputOpusDec       opusDecoder;
AudioConnection         patchCord1(i2s_in, 0, opusEncoder, 0);
AudioConnection         patchCord2(opusDecoder, 0, i2s_out, 0);
AudioConnection         patchCord3(opusDecoder, 0, i2s_out, 1);

void setup()
{  
  AudioMemory(10);
  sgtl5000_1.enable();  
  sgtl5000_1.inputSelect(AUDIO_INPUT_MIC);
  sgtl5000_1.volume(0.6);
  opusEncoder.initialise(); // Default is 64kbs
  opusDecoder.initialise();
}

void loop()
{
  uint8_t *opusBuffer;
  int32_t opusBufSize;

  opusBufSize = opusEncoder.hasData();

  if(opusBufSize > 0)
  {
    opusBuffer = opusEncoder.getData();
    opusDecoder.putData(opusBuffer, opusBufSize);   
  }
}
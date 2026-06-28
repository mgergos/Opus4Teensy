
#include <Audio.h>
#include <input_opus_dec.h>
#include <output_opus_enc.h>

#if AUDIO_BLOCK_SAMPLES != 960 || AUDIO_SAMPLE_RATE_EXACT !=48000
#warning Recommended frame size is 20ms - Define AUDIO_BLOCK_SAMPLES 960 and AUDIO_SAMPLE_RATE_EXACT 48000 in platformio.ini
#endif

AudioOutputOpusEnc      opusEncoder;  // Create Opus Encoder
AudioInputOpusDec       opusDecoder;  // Create Opus Decoder
AudioControlSGTL5000    sgtl5000_1;

#ifdef USB_AUDIO
AudioInputUSB           usb_in;
AudioOutputUSB          usb_out;
AudioConnection         patchCord1(usb_in, 0, opusEncoder, 0);
AudioConnection         patchCord2(usb_in, 1, opusEncoder, 1);
AudioConnection         patchCord3(opusDecoder, 0, usb_out, 0);
AudioConnection         patchCord4(opusDecoder, 1, usb_out, 1);
#else
AudioInputI2S           i2s_in;
AudioOutputI2S          i2s_out;
AudioConnection         patchCord1(i2s_in, 0, opusEncoder, 0);
AudioConnection         patchCord2(i2s_in, 1, opusEncoder, 1);
AudioConnection         patchCord3(opusDecoder, 0, i2s_out, 0);
AudioConnection         patchCord4(opusDecoder, 1, i2s_out, 1);
#endif

void setup()
{
  AudioMemory(10);
  sgtl5000_1.enable();  
  sgtl5000_1.inputSelect(AUDIO_INPUT_MIC);
  sgtl5000_1.volume(0.5);

  opusEncoder.initialise();
  opusDecoder.initialise();
  //opusEncoder.forceMono(true);                  // Defaults to Auto (i.e. mono < 24000bps < stereo)
  //opusEncoder.setBitrate(32000);                // Do not exceed CONFIG_OPUS_BITRATE_MAX
}

void loop()
{
  uint8_t *opusBuffer;
  int32_t opusBufSize;

  opusBufSize = opusEncoder.hasData();            // Returns the number of encoded bytes available

  if(opusBufSize > 0)                             // If data available...
  {
    opusBuffer = opusEncoder.getData();           // Get the data
    opusDecoder.putData(opusBuffer, opusBufSize); // Pass the data to the Decoder (loopback)
  }
}
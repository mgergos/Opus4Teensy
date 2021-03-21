
#include "input_opus_dec.h"
#include "Arduino.h"
#include "AudioStream.h"
#include "opus.h"
#include "config.h"

uint8_t AudioInputOpusDec::m_opus_decoder[9224];
OpusDecoder * AudioInputOpusDec::m_opus_decoder_state = NULL;

uint8_t AudioInputOpusDec::decoder_frame_buf_compressed[CONFIG_AUDIO_FRAME_SIZE_BYTES];
int32_t AudioInputOpusDec::decoder_decompressed_frame_size = CONFIG_AUDIO_FRAME_SIZE_SAMPLES;
int32_t AudioInputOpusDec::decoder_compressed_frame_size = CONFIG_AUDIO_FRAME_SIZE_BYTES;
bool AudioInputOpusDec::decoderInitialised = false;
elapsedMicros AudioInputOpusDec::inputPacketPhase;

void AudioInputOpusDec::begin(void)
{
	m_opus_decoder_state = (OpusDecoder *)m_opus_decoder;
	//Serial.printf("\r\nCheck Decoder size %d = 9224", opus_decoder_get_size(1));
}

void AudioInputOpusDec::initialise(void)
{
	opus_decoder_init(m_opus_decoder_state, CONFIG_AUDIO_SAMPLING_FREQUENCY, 1);
	decoderInitialised = true;
}

// putData() - Returns the time since update() was last called.
// Very useful for phasing the decoder with an asynchronous encoder.
// Minor changes to PLL4 PFD will not impact audio continuity e.g;
// int32_t denominator = CCM_ANALOG_PLL_AUDIO_DENOM;
// ...
// phase = opusDecoder.putData(opusBuffer, opusBufSize);   
// CCM_ANALOG_PLL_AUDIO_DENOM = (denominator + (phase/100) - 100);
int32_t AudioInputOpusDec::putData(uint8_t* compressedBuffer, int32_t bufferSize)
{
	__disable_irq();
	memcpy(decoder_frame_buf_compressed, compressedBuffer, bufferSize);
	decoder_compressed_frame_size = bufferSize;
	__enable_irq();
	return inputPacketPhase;
}

void AudioInputOpusDec::update(void)
{
	inputPacketPhase = 0;
	audio_block_t *block;
	block = allocate();
	if (block) {
		if(decoderInitialised)
		{
			decoder_decompressed_frame_size = opus_decode(m_opus_decoder_state, decoder_frame_buf_compressed, decoder_compressed_frame_size, block->data, CONFIG_AUDIO_FRAME_SIZE_SAMPLES, 0);
			//Serial.printf(" %d", decoder_decompressed_frame_size);
		}
		transmit(block);
		release(block);
	}
}


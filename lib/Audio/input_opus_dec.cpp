
#include "input_opus_dec.h"

uint8_t AudioInputOpusDec::opus_decoder[17944];
OpusDecoder * AudioInputOpusDec::opus_decoder_state = NULL;

int16_t AudioInputOpusDec::decoder_frame_buf_PCM_LR[AUDIO_BLOCK_SAMPLES][2];
int32_t AudioInputOpusDec::decoder_frame_buf_PCM_LR_size = 0;
uint8_t AudioInputOpusDec::decoder_frame_buf_compressed[AUDIO_BLOCK_SAMPLES * CONFIG_OPUS_BITRATE_MAX / AUDIO_SAMPLE_RATE_EXACT / 8];
int32_t AudioInputOpusDec::decoder_frame_buf_compressed_size = 0;

bool AudioInputOpusDec::decoderInitialised = false;
elapsedMicros AudioInputOpusDec::inputPacketPhase;

void AudioInputOpusDec::begin(void)
{
	opus_decoder_state = (OpusDecoder *)opus_decoder;
	//Serial.printf("\r\nCheck Decoder size %d = 9224", opus_decoder_get_size(1)); // CELT mono
	//Serial.printf("\r\nCheck Decoder size %d = 17944", opus_decoder_get_size(2)); // CELT stereo
}

void AudioInputOpusDec::initialise(void)
{
	if(opus_decoder_init(opus_decoder_state, (int32_t)AUDIO_SAMPLE_RATE_EXACT, 2) == OPUS_OK)
	{
		decoderInitialised = true;
	}else{
		decoderInitialised = false;
	}
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
	__enable_irq();
	decoder_frame_buf_compressed_size = bufferSize;
	return inputPacketPhase;
}

void AudioInputOpusDec::update(void)
{
	inputPacketPhase = 0;
	audio_block_t *blockL, *blockR;
	blockL = allocate();
	blockR = allocate();

	if(decoderInitialised)
	{
		decoder_frame_buf_PCM_LR_size = opus_decode(opus_decoder_state, decoder_frame_buf_compressed, decoder_frame_buf_compressed_size, (opus_int16*)decoder_frame_buf_PCM_LR, AUDIO_BLOCK_SAMPLES, 0);
	}

	if(blockL && blockR)
	{
		__disable_irq();
		for(int i=0; i<AUDIO_BLOCK_SAMPLES; i++)
		{
			blockL->data[i] = decoder_frame_buf_PCM_LR[i][0];
			blockR->data[i] = decoder_frame_buf_PCM_LR[i][1];
		}
		__enable_irq();
		transmit(blockL, 0);
		transmit(blockR, 1);
		release(blockL);
		release(blockR);
	}
}


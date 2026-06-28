
#include "output_opus_enc.h"

uint8_t AudioOutputOpusEnc::opus_encoder[11924];
OpusEncoder * AudioOutputOpusEnc::opus_encoder_state = NULL;

int16_t AudioOutputOpusEnc::encoder_frame_buf_PCM_LR[AUDIO_BLOCK_SAMPLES][2];
int32_t AudioOutputOpusEnc::encoder_frame_buf_PCM_LR_size = AUDIO_BLOCK_SAMPLES;
uint8_t AudioOutputOpusEnc::encoder_frame_buf_compressed[AUDIO_BLOCK_SAMPLES * CONFIG_OPUS_BITRATE_MAX / AUDIO_SAMPLE_RATE_EXACT / 8];
int32_t AudioOutputOpusEnc::encoder_frame_buf_compressed_size = 0;

bool AudioOutputOpusEnc::encoderInitialised = false;

void AudioOutputOpusEnc::begin(void)
{
	opus_encoder_state = (OpusEncoder *)opus_encoder;
	//Serial.printf("\r\nCheck Encoder size %d = 7180", opus_encoder_get_size(1)); // CELT mono
	//Serial.printf("\r\nCheck Encoder size %d = 11924", opus_encoder_get_size(2)); // CELT stereo
}

void AudioOutputOpusEnc::initialise(void)
{
	if(opus_encoder_init(opus_encoder_state, (int32_t)AUDIO_SAMPLE_RATE_EXACT, 2, OPUS_APPLICATION_RESTRICTED_LOWDELAY) == OPUS_OK)
	{
		opus_encoder_ctl(opus_encoder_state, OPUS_SET_BITRATE(CONFIG_OPUS_BITRATE_MAX));
		opus_encoder_ctl(opus_encoder_state, OPUS_SET_BANDWIDTH(OPUS_AUTO));
		opus_encoder_ctl(opus_encoder_state, OPUS_SET_COMPLEXITY(10));
		opus_encoder_ctl(opus_encoder_state, OPUS_SET_SIGNAL(OPUS_AUTO));
		opus_encoder_ctl(opus_encoder_state, OPUS_SET_LSB_DEPTH(16));
		opus_encoder_ctl(opus_encoder_state, OPUS_SET_VBR(0));
		encoderInitialised = true;
	}else{
		encoderInitialised = false;
	}
}

void AudioOutputOpusEnc::setComplexity(uint8_t complexity)
{
	opus_encoder_ctl(opus_encoder_state, OPUS_SET_COMPLEXITY(complexity));
}

void AudioOutputOpusEnc::setBitrate(uint32_t bitrate)
{
	opus_encoder_ctl(opus_encoder_state, OPUS_SET_BITRATE(bitrate));
}

void AudioOutputOpusEnc::forceMono(bool mono)
{
	if(mono)
	{
		opus_encoder_ctl(opus_encoder_state, OPUS_SET_FORCE_CHANNELS(1));
	}
	else
	{
		opus_encoder_ctl(opus_encoder_state, OPUS_SET_FORCE_CHANNELS(OPUS_AUTO));
	}	
}

int32_t AudioOutputOpusEnc::hasData()
{
	return 	encoder_frame_buf_compressed_size;
}

uint8_t *AudioOutputOpusEnc::getData()
{
	encoder_frame_buf_compressed_size = 0;
	return encoder_frame_buf_compressed;
}

void AudioOutputOpusEnc::update(void)
{
	audio_block_t *blockL;
	audio_block_t *blockR;
	blockL = receiveReadOnly(0);
	blockR = receiveReadOnly(1);

	if(blockL && blockR)
	{
		__disable_irq();
		for(int i=0; i<AUDIO_BLOCK_SAMPLES; i++)
		{
			encoder_frame_buf_PCM_LR[i][0] = blockL->data[i];
			encoder_frame_buf_PCM_LR[i][1] = blockR->data[i];
		}
		__enable_irq();
		release(blockL);
		release(blockR);
	}

	if(encoderInitialised)
	{
		encoder_frame_buf_compressed_size = opus_encode(opus_encoder_state, (opus_int16*)encoder_frame_buf_PCM_LR, encoder_frame_buf_PCM_LR_size, encoder_frame_buf_compressed, sizeof(encoder_frame_buf_compressed));
	}
}

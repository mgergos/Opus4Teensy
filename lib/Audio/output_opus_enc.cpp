
#include "output_opus_enc.h"
#include "Arduino.h"
#include "AudioStream.h"
#include "opus.h"
#include "config.h"

uint8_t AudioOutputOpusEnc::m_opus_encoder[7180];
OpusEncoder * AudioOutputOpusEnc::m_opus_encoder_state = NULL;

int16_t AudioOutputOpusEnc::encoder_frame_buf_uncompressed[CONFIG_AUDIO_FRAME_SIZE_SAMPLES];
uint8_t AudioOutputOpusEnc::encoder_frame_buf_compressed[CONFIG_AUDIO_FRAME_SIZE_BYTES];
int32_t AudioOutputOpusEnc::encoder_compressed_frame_size = 0;
bool AudioOutputOpusEnc::encoderInitialised = false;

void AudioOutputOpusEnc::begin(void)
{
	m_opus_encoder_state = (OpusEncoder *)m_opus_encoder;
	//Serial.printf("\r\nCheck Encoder size %d = 7180", opus_encoder_get_size(1));
}

void AudioOutputOpusEnc::initialise(void)
{
	opus_encoder_init(m_opus_encoder_state, CONFIG_AUDIO_SAMPLING_FREQUENCY, 1, OPUS_APPLICATION_RESTRICTED_LOWDELAY);
	opus_encoder_ctl(m_opus_encoder_state, OPUS_SET_BITRATE(CONFIG_OPUS_BITRATE));
	opus_encoder_ctl(m_opus_encoder_state, OPUS_SET_BANDWIDTH(OPUS_AUTO));
	opus_encoder_ctl(m_opus_encoder_state, OPUS_SET_COMPLEXITY(CONFIG_OPUS_COMPLEXITY));
	opus_encoder_ctl(m_opus_encoder_state, OPUS_SET_SIGNAL(OPUS_AUTO));
	opus_encoder_ctl(m_opus_encoder_state, OPUS_SET_LSB_DEPTH(16));
	opus_encoder_ctl(m_opus_encoder_state, OPUS_SET_VBR(0));
	encoderInitialised = true;
}

void AudioOutputOpusEnc::setComplexity(uint8_t complexity)
{
	opus_encoder_ctl(m_opus_encoder_state, OPUS_SET_COMPLEXITY(complexity));
}

void AudioOutputOpusEnc::setBitrate(uint32_t bitrate)
{
	opus_encoder_ctl(m_opus_encoder_state, OPUS_SET_BITRATE(bitrate));
}

int32_t AudioOutputOpusEnc::hasData()
{
	return 	encoder_compressed_frame_size;
}

uint8_t *AudioOutputOpusEnc::getData()
{
	encoder_compressed_frame_size = 0;
	return encoder_frame_buf_compressed;
}

void AudioOutputOpusEnc::update(void)
{
	audio_block_t *block;

	block = receiveReadOnly(0);
	if (block) {
		if(encoderInitialised)
			{
			memcpy(encoder_frame_buf_uncompressed, block->data, CONFIG_AUDIO_FRAME_SIZE_SAMPLES * sizeof(int16_t));
			encoder_compressed_frame_size = opus_encode(m_opus_encoder_state, encoder_frame_buf_uncompressed, CONFIG_AUDIO_FRAME_SIZE_SAMPLES, encoder_frame_buf_compressed, sizeof(encoder_frame_buf_compressed));
			//Serial.printf(" %d", encoder_compressed_frame_size);
			}
		release(block);
	}
}

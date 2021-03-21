
#ifndef output_opus_enc_h_
#define output_opus_enc_h_

#include "Arduino.h"
#include "AudioStream.h"
#include "opus.h"
#include "config.h"

class AudioOutputOpusEnc : public AudioStream
{
public:
	AudioOutputOpusEnc(void) : AudioStream(1, inputQueueArray) {begin();}
	void begin(void);
	virtual void update(void);
	void initialise(void);
	void setComplexity(uint8_t complexity);
	void setBitrate(uint32_t bitrate);
	int32_t hasData();
	uint8_t *getData();

	static uint8_t m_opus_encoder[7180];
	static OpusEncoder * m_opus_encoder_state;

	static int16_t encoder_frame_buf_uncompressed[CONFIG_AUDIO_FRAME_SIZE_SAMPLES];
	static uint8_t encoder_frame_buf_compressed[CONFIG_AUDIO_FRAME_SIZE_BYTES];
	static int32_t encoder_compressed_frame_size;
	static bool encoderInitialised;

protected:

private:
	audio_block_t *inputQueueArray[1];
};

#endif
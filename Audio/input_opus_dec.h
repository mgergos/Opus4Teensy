
#ifndef input_opus_dec_h_
#define input_opus_dec_h_

#include "Arduino.h"
#include "AudioStream.h"
#include "opus.h"
#include "config.h"

class AudioInputOpusDec : public AudioStream
{
public:
	AudioInputOpusDec(void) : AudioStream(1, inputQueueArray) {begin();}
	void begin(void);
	virtual void update(void);
	void initialise(void);
	int32_t putData(uint8_t* compressedBuffer, int32_t bufferSize);

	static uint8_t m_opus_decoder[9224];
	static OpusDecoder * m_opus_decoder_state;

	static uint8_t decoder_frame_buf_compressed[CONFIG_AUDIO_FRAME_SIZE_BYTES];
	static int32_t decoder_decompressed_frame_size;
	static int32_t decoder_compressed_frame_size;
	static bool decoderInitialised;
	static elapsedMicros inputPacketPhase;

protected:

private:
	audio_block_t *inputQueueArray[1];
};

#endif
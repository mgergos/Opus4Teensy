/******************************************************************************
 * Opus4Teensy Library
 * Copyright (c) 2021, Mick Gergos https://github.com/mgergos/Opus4Teensy
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice, development funding notice, and this permission
 * notice shall be included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 *******************************************************************************
 *  input_opus_dec.cpp
 *  Tested on:
 *  	1. Teensy 4.0
 * 		2. Audio Adaptor Board Rev D
 *
 * 	Purpose:    Decode Opus encoded data for use by Teensy Audio Library
 *
 *******************************************************************************/
#ifndef INPUT_OPUS_DEC_H_
#define INPUT_OPUS_DEC_H_
/*******************************************************************************/

#include "Arduino.h"
#include "AudioStream.h"
#include "opus.h"

class AudioInputOpusDec : public AudioStream
{
public:
	AudioInputOpusDec(void) : AudioStream(0, NULL) {begin();}
	void begin(void);
	void initialise(void);
	int32_t putData(uint8_t* compressedBuffer, int32_t bufferSize);
	virtual void update(void);

	static uint8_t opus_decoder[17944];
	static OpusDecoder * opus_decoder_state;

	static int16_t decoder_frame_buf_PCM_LR[AUDIO_BLOCK_SAMPLES][2];
	static int32_t decoder_frame_buf_PCM_LR_size;
	static uint8_t decoder_frame_buf_compressed[AUDIO_BLOCK_SAMPLES * CONFIG_OPUS_BITRATE_MAX / AUDIO_SAMPLE_RATE_EXACT / 8];
	static int32_t decoder_frame_buf_compressed_size;
	
	static bool decoderInitialised;
	static elapsedMicros inputPacketPhase;

protected:

private:

};

#endif // INPUT_OPUS_DEC_H_

// OPUS modes:
#define CONFIG_OPUS_MODE_CELT                   1
#define CONFIG_OPUS_MODE_SILK                   2   // Not working - drops out
#define CONFIG_OPUS_MODE_HYBRID                 (CONFIG_OPUS_MODE_CELT | CONFIG_OPUS_MODE_SILK)  //Not working - drops out

// <h> Opus Options
// <o> Mode
// <i> SILK mode is specifically dedicated for voice but requires more CPU and memory resources than CELT.
// <i> CELT is a general-purpose mode that is more power efficient and uses less resources.
// <i> Hybrid mode is not supported.
// <i> Note: When using Keil, use the option "Rebuild all target files" to change stack size between CELT/SILK.
//  <1=>CELT Only
//  <2=>SILK Only
#define CONFIG_OPUS_MODE CONFIG_OPUS_MODE_CELT

// <o> Sampling Frequency
// <i> Select audio sampling frequency.
// <i> Note that not all combinations of sampling frequency and codec are supported.
//  <8000=>8 kHz
//  <16000=>16 kHz
//  <24000=>24 kHz
//  <32000=>32 kHz
//  <48000=>48 kHz
#define CONFIG_AUDIO_SAMPLING_FREQUENCY 48000

// <o> Bit Rate
// <i> CBR - Constant Bit Rate allows you to set a specific bit rate that remains the same throughout the transmission.
//  <8000=>CBR: 16 kbit/s - Only available in OPUS_APPLICATION_RESTRICTED_LOWDELAY mode
//  <16000=>CBR: 16 kbit/s - Only available in OPUS_APPLICATION_RESTRICTED_LOWDELAY mode
//  <24000=>CBR: 24 kbit/s
//  <32000=>CBR: 32 kbit/s
//  <40000=>CBR: 40 kbit/s
//  <48000=>CBR: 48 kbit/s
//  <56000=>CBR: 56 kbit/s
//  <64000=>CBR: 64 kbit/s
//  <80000=>CBR: 80 kbit/s
//  <96000=>CBR: 96 kbit/s
//  <112000=>CBR: 112 kbit/s
//  <128000=>CBR: 128 kbit/s
//
#define CONFIG_OPUS_BITRATE 64000 // Max bitrate - used to calc compressed buffer size

// <o> Complexity <0-10>
// <i> A number from range 0-10. Higher complexity assures better quality but also higher CPU and memory resources consumption.
#define CONFIG_OPUS_COMPLEXITY 10 // Default

// <o> Audio Frame Size - Must be value below!
// <i> CELT supports 5 ms - 40 ms audio frames. SILK provides support for 10 ms - 60 ms frame sizes.
//  <5=>5 ms
//  <10=>10 ms
//  <20=>20 ms
//  <40=>40 ms
//  <60=>60 ms
//#define CONFIG_AUDIO_FRAME_SIZE_MS 20
# define CONFIG_AUDIO_FRAME_SIZE_SAMPLES   (CONFIG_AUDIO_SAMPLING_FREQUENCY / 50)  // 20mS

// Calculate audio parameters.
# define CONFIG_AUDIO_FRAME_SIZE_BYTES         (CONFIG_AUDIO_FRAME_SIZE_SAMPLES * CONFIG_OPUS_BITRATE) / (8 * CONFIG_AUDIO_SAMPLING_FREQUENCY)

#ifndef FLACDECODER_H
#define FLACDECODER_H

#include <FLAC++/decoder.h>
#include <QBuffer>

#include "audiooutput.h"

class AudioOutput;

#define DEFAULT_FLAC_IN_FILE "/tmp/in1.flac"

class FlacDecoder : public FLAC::Decoder::Stream
{
public:
    FlacDecoder(bool UseFile = false, AudioOutput *anAudioOutput = 0);
    ::FLAC__StreamDecoderReadStatus read_callback(FLAC__byte buffer[], size_t *bytes);
    void error_callback(FLAC__StreamDecoderErrorStatus status);
    ::FLAC__StreamDecoderWriteStatus write_callback(const ::FLAC__Frame *frame, const FLAC__int32 * const buffer[]);

    bool m_UseFile;
    FILE *m_FlacInFile;
    QBuffer *m_Buffer;

    AudioOutput *m_AudioOutput;
};

#endif // FLACDECODER_H

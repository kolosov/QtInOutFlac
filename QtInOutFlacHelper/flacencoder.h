#ifndef FLACENCODER_H
#define FLACENCODER_H
#include <QFile>
#include <QByteArray>
#include <QBuffer>
#include <QDataStream>

#include "audioinput.h"
#include <FLAC++/encoder.h>

#define DEFAULT_FLAC_OUT_FILE "/tmp/out1.flac"

class AudioInput;

class FlacEncoder : public FLAC::Encoder::Stream
{
public:
    FlacEncoder(AudioInput *anAudioInput, bool UseFile = false);

    bool m_UseFile;
    QFile m_OutFile;
    FILE *m_FlacOutFile;
    char *m_FlacBuf;

    AudioInput *m_AudioInput;

    QDataStream *m_DataStream;

    ::FLAC__StreamEncoderWriteStatus write_callback(const FLAC__byte buffer[], size_t bytes, unsigned samples, unsigned current_frame);

};

#endif // FLACENCODER_H

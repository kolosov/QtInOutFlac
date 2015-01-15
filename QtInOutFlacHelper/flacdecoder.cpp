#include "flacdecoder.h"
#include <QDebug>

FlacDecoder::FlacDecoder(bool UseFile, AudioOutput *anAudioOutput)
{
    m_UseFile = UseFile;
    if(anAudioOutput) m_AudioOutput = anAudioOutput;
}

::FLAC__StreamDecoderReadStatus FlacDecoder::read_callback(FLAC__byte buffer[], size_t *bytes)
{
        const size_t requested_bytes = *bytes;
        qDebug() << "Decoder: read_callback" << " requested_bytes = " << requested_bytes;

        //qint64 l = m_Buffer->read((char*)buffer, requested_bytes);
        int l = m_AudioOutput->readMoreFlacData((char*)buffer, requested_bytes);
        if(l == 0) {
            qDebug() << "FlacDecoder::read_callback, return status abort";
            return ::FLAC__STREAM_DECODER_READ_STATUS_ABORT; /* abort to avoid a deadlock */
        }

        *bytes = (size_t) l;
        qDebug() << "Decoder, read " << l << " == " << *bytes << " buffer";

        return ::FLAC__STREAM_DECODER_READ_STATUS_CONTINUE;

}

void FlacDecoder::error_callback(FLAC__StreamDecoderErrorStatus status)
{
    qDebug() << "ERROR: got error callback: err = " << (unsigned)status <<  " " << ::FLAC__StreamDecoderErrorStatusString[status];

}

FLAC__StreamDecoderWriteStatus FlacDecoder::write_callback(const FLAC__Frame *frame, const FLAC__int32 * const buffer[])
{
    unsigned int block_len = frame->header.blocksize;
    unsigned int frame_numb = frame->header.number.frame_number;
    unsigned int sample_numb = frame->header.number.sample_number;
    unsigned int chan_len = frame->header.channels;

    qDebug() << "Decoder: Write callback, block_len=" << block_len
               << " frame_num=" << frame_numb << " sample_numb=" << sample_numb
                << " chanels=" << chan_len;

    short *tmp_buf = new short[block_len]; //16bit
    /* write decoded PCM samples */
    for(unsigned int i = 0; i < block_len; i++) {
        tmp_buf[i] = (FLAC__int16)buffer[0][i];

    }
    m_AudioOutput->writeMoreRaw((char *)tmp_buf, block_len * 2);

    return ::FLAC__STREAM_DECODER_WRITE_STATUS_CONTINUE;
}

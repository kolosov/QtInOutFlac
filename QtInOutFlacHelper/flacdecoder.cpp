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

        if(m_UseFile) {
            if(feof(m_FlacInFile)) {
                *bytes = 0;
                return ::FLAC__STREAM_DECODER_READ_STATUS_END_OF_STREAM;
            }
            else if(requested_bytes > 0) {
                *bytes = ::fread(buffer, 1, requested_bytes, m_FlacInFile);
                if(*bytes == 0) {
                    if(feof(m_FlacInFile))
                        return ::FLAC__STREAM_DECODER_READ_STATUS_END_OF_STREAM;
                    else
                        return ::FLAC__STREAM_DECODER_READ_STATUS_CONTINUE;
                }
                else {
                    return ::FLAC__STREAM_DECODER_READ_STATUS_CONTINUE;
                }
            }
            else
                return ::FLAC__STREAM_DECODER_READ_STATUS_ABORT; /* abort to avoid a deadlock */
        } else {
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
}

void FlacDecoder::error_callback(FLAC__StreamDecoderErrorStatus status)
{
    qDebug() << "ERROR: got error callback: err = " << (unsigned)status <<  " " << ::FLAC__StreamDecoderErrorStatusString[status];

}

FLAC__StreamDecoderWriteStatus FlacDecoder::write_callback(const FLAC__Frame *frame, const FLAC__int32 * const buffer[])
{
    int block_len = frame->header.blocksize;

    qDebug() << "Decoder: Write callback, block_len=" << block_len;

    short *tmp_buf = new short[block_len]; //16bit
    /* write decoded PCM samples */
    for(int i = 0; i < block_len; i++) {
        tmp_buf[i] = (FLAC__int16)buffer[0][i];

        /*
            if(
                    !write_little_endian_int16(f, (FLAC__int16)buffer[0][i]) ||
                    !write_little_endian_int16(f, (FLAC__int16)buffer[1][i])
            ) {
                    QDebug << "ERROR: write_callback error";
                    return FLAC__STREAM_DECODER_WRITE_STATUS_ABORT;
            }*/
    }
    m_AudioOutput->writeMoreRaw((char *)tmp_buf, block_len * 2);

    return ::FLAC__STREAM_DECODER_WRITE_STATUS_CONTINUE;
}

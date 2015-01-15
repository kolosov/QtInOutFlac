#include "flacencoder.h"
#include <QDebug>

FlacEncoder::FlacEncoder(AudioInput *anAudioInput, bool UseFile)
{
    m_UseFile = UseFile;
    m_AudioInput = anAudioInput;

    qDebug() << "FlacEncoder created";

}

::FLAC__StreamEncoderWriteStatus FlacEncoder::write_callback(const FLAC__byte buffer[], size_t bytes, unsigned samples, unsigned current_frame)
{
    (void)samples, (void)current_frame;

    //qDebug() << "Encoder: Write callback: bytes:" << bytes;
    if(m_UseFile) {
        if(fwrite(buffer, 1, bytes, m_FlacOutFile) != bytes)
            return ::FLAC__STREAM_ENCODER_WRITE_STATUS_FATAL_ERROR;
        else
            return ::FLAC__STREAM_ENCODER_WRITE_STATUS_OK;
    } else { //use buffer

        QByteArray ByteBuffer;
        ByteBuffer.resize((int)bytes);
        memcpy((void*)ByteBuffer.data(), (const char*)buffer, bytes);
        //= QByteArray((const char*)buffer, (int)bytes);
        //qDebug() << "Encoder: sending buffer " << bytes << " bytes";

        m_AudioInput->putNewFlacData(ByteBuffer);

        return ::FLAC__STREAM_ENCODER_WRITE_STATUS_OK;
    }
}

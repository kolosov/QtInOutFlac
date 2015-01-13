#include "audiooutput.h"
#include <QDebug>
#include <QTimer>

AudioOutput::AudioOutput(QObject *parent, int Index, bool UseCopy, bool UseFlacFile, bool UseFlacBuf) :
    QObject(parent)
{

    m_Format.setSampleRate(48000);
    m_Format.setChannelCount(1);
    m_Format.setSampleSize(16);
    m_Format.setCodec("audio/pcm");
    m_Format.setByteOrder(QAudioFormat::LittleEndian);
    m_Format.setSampleType(QAudioFormat::SignedInt);

    m_UseCopy = UseCopy;    
    m_UseFlacFile = UseFlacFile;
    m_UseFlacBuf = UseFlacBuf;

    m_MinBufSize = 8192;

    m_ByteBuffer = new QByteArray;

    QAudioDeviceInfo infoOut(QAudioDeviceInfo::defaultOutputDevice());
    if(Index != -1) {
        infoOut = QAudioDeviceInfo::availableDevices(QAudio::AudioOutput)[Index];
    }

    if (!infoOut.isFormatSupported(m_Format))
    {
        qDebug()<< "Default format not supported - trying to use nearest";
        m_Format = infoOut.nearestFormat(m_Format);
    }

    //flac start
    if(m_UseFlacBuf || m_UseFlacFile) {
        qDebug() << "Create FLAC decoder";
        m_FlacDecoder = new FlacDecoder(m_UseFlacFile, this);

        qDebug() << "Init flac";
        m_FlacDecoder->init();
    }
    //flac end

    qDebug() << "Output device is: " << infoOut.deviceName();
    m_AudioOutput = new QAudioOutput(infoOut, m_Format, this);

}

int AudioOutput::getByteBufferSize()
{
    return m_ByteBuffer->size();
}


void AudioOutput::writeMoreRaw(char *buf, int len)
{
    short* resData = (short*)buf;

    short *out=resData;
    out[0] = resData[0];

    qint64 l = m_Output->write((char*)out, len);
    qDebug() << "AudioOutput, " << l << " bytes were written";
}

int AudioOutput::readMoreFlacData(char *buf, int len)
{
    int readBytes = len;
    qDebug() << "AudioOutput::readMoreFlacData, needs "
             << len << "bytes, audio buf size = "
             << m_ByteBuffer->size();
    if(m_ByteBuffer->size() < len) readBytes = m_ByteBuffer->size();
    memcpy(buf, m_ByteBuffer->data(), readBytes);
    m_ByteBuffer->remove(0,readBytes);

    return readBytes;
}

void AudioOutput::writeMoreFlacData(QByteArray flacData)
{
    qDebug() << "AudioOutput::writeMoreFlacData, added "
             << flacData.size() << " bytes";
    m_ByteBuffer->append(flacData);

    emit newFlacDataReady();
}

void AudioOutput::processMore()
{
    while(m_ByteBuffer->size() > m_MinBufSize) {
        qDebug() << "Start flac process";
        m_FlacDecoder->process_single();
    }
}


void AudioOutput::start()
{
    qDebug() << "AudioOutput::start Starting output";


    m_Output = m_AudioOutput->start();

    if(m_UseFlacBuf || m_UseFlacFile) {
        connect(this, SIGNAL(newFlacDataReady()),SLOT(processMore()));
    }
}

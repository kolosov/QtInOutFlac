#include "audioinput.h"
#include <QDebug>
#include <QAudioDeviceInfo>



AudioInput::AudioInput(QObject *parent, int Index, bool UseCopy, bool UseFlacFile, bool UseFlacBuf) :
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

    QAudioDeviceInfo infoIn(QAudioDeviceInfo::defaultInputDevice());
    if(Index != -1) {
        infoIn = QAudioDeviceInfo::availableDevices(QAudio::AudioInput)[Index];
    }

    if (!infoIn.isFormatSupported(m_Format))
    {
        qDebug()<< "Default format not supported - trying to use nearest";
        m_Format = infoIn.nearestFormat(m_Format);
    }

    //flac start
    if(m_UseFlacFile || m_UseFlacBuf) {
        qDebug() << "Create FLAC encoder";

        m_FlacEncoder = new FlacEncoder(this, m_UseFlacFile);

        m_FlacEncoder->set_channels(1);
        m_FlacEncoder->set_sample_rate(48000);
        m_FlacEncoder->set_bits_per_sample(16);
    }
    //flac end

    qDebug() << "Input device is: " << infoIn.deviceName();

    m_AudioInput = new QAudioInput(infoIn, m_Format, this);
}

void AudioInput::start(QIODevice *anOutput)
{
    qDebug() << "Starting copy input";
    if(m_UseCopy) {
        if (anOutput) m_Output = anOutput;
        else qDebug() <<  " Error: UseCopy is set, but Output device is not set";

    }

    m_Input = m_AudioInput->start();

    qDebug() << "AudioInput:start, is seq = " << m_Input->isSequential();
    connect(m_Input, SIGNAL(readyRead()), SLOT(readMore()));

    //flac start
    if(m_UseFlacBuf || m_UseFlacFile) {
        if(m_UseFlacFile) {
            qDebug() << "Opening out FLAC file";
            m_FlacEncoder->m_FlacOutFile = fopen(DEFAULT_FLAC_OUT_FILE, "w+");
            if(!(m_FlacEncoder->m_FlacOutFile)) {
                qDebug() << "Cannot open " <<  DEFAULT_FLAC_OUT_FILE << " for writing";
                return;
            }
        }
        qDebug() << "Initialization FLAC";
        m_FlacEncoder->init();
    }
    //flac end
}

void AudioInput::putNewFlacData(QByteArray buffer)
{
    //emit signal, new data is ready
    emit newFlacDataReady(buffer);

}

void AudioInput::readMore()
{

    qint64 len = m_AudioInput->bytesReady();
    //qDebug() << "AudioInput: bytesReady: " << len;

    if(len > BUF_SIZE)
        len = BUF_SIZE;

    char *data_ptr = m_CharBuf;

    qint64 l = m_Input->read(data_ptr, len);
    //qDebug() << "read " << l << " bytes" << len;

    //flac start
    FLAC__int32 samples_array[BUF_SIZE/2];
    short *short_buf = (short*)data_ptr;
    unsigned samples = l/2 ; // sample size is 2 bytes

    if(m_UseFlacBuf || m_UseFlacFile) {
        for(unsigned int i=0;i<samples;i++)
        {
            samples_array[i]= *short_buf++;
        }
    }
    //flac end


    if(l > 0)
    {
        if(m_UseCopy) {
            short* resData = (short*)m_CharBuf;

            short *out=resData;
            out[0] = resData[0];

            m_Output->write((char*)out, len);
        }
        //flac start
        if(m_UseFlacBuf || m_UseFlacFile) {
            m_FlacEncoder->process_interleaved(samples_array, samples);
        }
        //flac end
    }
}

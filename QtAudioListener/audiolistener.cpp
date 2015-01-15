#include "audiolistener.h"

AudioListener::AudioListener(QObject *parent) :
    QObject(parent)
{
    m_Socket = new QUdpSocket(this);
    m_Socket->bind(QHostAddress::Any, BROADCAST_PORT);
    m_UseFile=false;
    //m_flac_should_start = false;
    m_flac_is_playing = false;

    if(m_UseFile) m_OutFile = new QFile(QString(OUT_FLAC_FILE));
}

void AudioListener::start()
{
    if(m_UseFile) {
        bool res = m_OutFile->open(QIODevice::WriteOnly);
        if(!res) qDebug() << "Cannot open file" << QString(OUT_FLAC_FILE);
    }

    m_AudioOutput = new AudioOutput(this, -1, false, m_UseFile, true);

    connect(m_Socket, SIGNAL(readyRead()), this, SLOT(readDataFromSocket()));

    m_AudioOutput->start();
    m_flac_is_playing = true;
}

void AudioListener::readDataFromSocket()
{
    //qDebug() << "AudioListener::readDataFromSocket";
    QByteArray datagram;
    datagram.resize(m_Socket->pendingDatagramSize());

    QHostAddress SenderHost;
    quint16 SenderPort;

    m_Socket->readDatagram(datagram.data(), datagram.size(), &SenderHost, &SenderPort);

    //qDebug() << "AudioListener::readDataFromSocket, recieved " << datagram.size() << " bytes";
    if(m_UseFile) m_OutFile->write(datagram);

    m_AudioOutput->writeMoreFlacData(datagram);
}

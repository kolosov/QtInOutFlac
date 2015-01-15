#include "audiobroadcaster.h"

#include <QByteArray>
#include <QDebug>

#define BROADCAST_PORT 5577

AudioBroadcaster::AudioBroadcaster(QObject *parent) :
    QObject(parent)
{
    m_Socket = new QUdpSocket(this);

    m_AudioInput = new AudioInput(this, -1, false, false, true);
}

void AudioBroadcaster::start()
{

    connect(m_AudioInput, SIGNAL(newFlacDataReady(QByteArray)), this, SLOT(readyReadNewFlacData(QByteArray)));
    m_AudioInput->start();
}

/*
void AudioBroadcaster::readyRead()
{
    qDebug() << "AudioBroadcaster::readyRead";    
}*/

void AudioBroadcaster::readyReadNewFlacData(QByteArray buffer)
{
    qint64 l = m_Socket->writeDatagram(buffer.data(), buffer.size(),
                            QHostAddress::Broadcast, BROADCAST_PORT);

    /*qDebug() << "AudioBroadcaster::readyReadNewFlacData, "
             << l << " bytes were sent";*/
    if(l<0)
        qDebug() << "Socket error: " <<  m_Socket->error() << " " << m_Socket->errorString();

}

#ifndef AUDIOBROADCASTER_H
#define AUDIOBROADCASTER_H

#include <QObject>
#include <QUdpSocket>
#include <QBuffer>

#include "audioinput.h"

class AudioBroadcaster : public QObject
{
    Q_OBJECT
public:
    explicit AudioBroadcaster(QObject *parent = 0);
    AudioInput *m_AudioInput;

    void start();

signals:

public slots:
    //void readyRead();
    void readyReadNewFlacData(QByteArray buffer);

private:
    QUdpSocket *m_Socket;    

};

#endif // AUDIOBROADCASTER_H

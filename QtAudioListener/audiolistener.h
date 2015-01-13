#ifndef AUDIOLISTENER_H
#define AUDIOLISTENER_H

#include <QUdpSocket>
#include <QObject>
#include <QFile>

#include <audiooutput.h>

#define BROADCAST_PORT 5577
#define OUT_FLAC_FILE "/tmp/net_out.flac"

#define START_PLAYING_BUFFER_SIZE 65536

class AudioListener : public QObject
{
    Q_OBJECT
public:
    explicit AudioListener(QObject *parent = 0);
    AudioOutput *m_AudioOutput;

    void start();

signals:

public slots:
    void readDataFromSocket();

private:
    QUdpSocket *m_Socket;
    bool m_UseFile;
    QFile *m_OutFile;
    //bool m_flac_should_start;
    bool m_flac_is_playing;

};

#endif // AUDIOLISTENER_H

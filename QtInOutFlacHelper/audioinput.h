#ifndef AUDIOINPUT_H
#define AUDIOINPUT_H

#include <QObject>
#include <QAudioDeviceInfo>
#include <QAudioInput>
#include <QFile>
#include <QByteArray>

#include <flacencoder.h>

#define BUF_SIZE 8192

class FlacEncoder;

class AudioInput : public QObject
{
    Q_OBJECT
public:
    explicit AudioInput(QObject *parent = 0, int Index = -1, bool UseCopy = true,
                        bool UseFlacFile = false, bool UseFlacBuf = false);
    void start(QIODevice *anOutput = 0);
    //bool start(QString anOutFileName);    

signals:
    void newFlacDataReady(QByteArray buffer);

public slots:
    void putNewFlacData(QByteArray buffer);

private:
    QAudioDeviceInfo m_InputDevInfo;
    QAudioFormat m_Format;
    QAudioInput *m_AudioInput;
    QIODevice *m_Input;

    char m_CharBuf[BUF_SIZE];

    QIODevice *m_Output;//set from Output
    bool m_UseCopy;
    bool m_UseFlacFile;
    bool m_UseFlacBuf;

    QFile *m_OutFile;
    FlacEncoder *m_FlacEncoder;

private slots:
    void readMore();
};

#endif // AUDIOINPUT_H

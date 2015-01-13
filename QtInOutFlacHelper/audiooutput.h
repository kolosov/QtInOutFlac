#ifndef AUDIOOUTPUT_H
#define AUDIOOUTPUT_H

#include <QObject>
#include <QAudioDeviceInfo>
#include <QAudioOutput>
#include <QFile>
#include <QBuffer>
#include <QByteArray>

#include "flacdecoder.h"

class FlacDecoder;

class AudioOutput : public QObject
{
    Q_OBJECT
public:
    explicit AudioOutput(QObject *parent = 0, int Index = -1,  bool UseCopy = true,
                         bool UseFlacFile = false, bool UseFlacBuf = false);

    int getByteBufferSize();

signals:
    void newFlacDataReady();

public slots:
    void start();
    void writeMoreRaw(char *buf, int len);
    int readMoreFlacData(char *buf, int len);
    void writeMoreFlacData(QByteArray flacData);

    void processMore();


private:
    QAudioDeviceInfo m_OutputDevInfo;
    QAudioFormat m_Format;
    QAudioOutput *m_AudioOutput;
    QIODevice *m_Output;

    bool m_UseCopy;
    bool m_UseFlacFile;
    bool m_UseFlacBuf;

    int m_MinBufSize;

    QByteArray *m_ByteBuffer;

    FlacDecoder *m_FlacDecoder;
private slots:

};

#endif // AUDIOOUTPUT_H

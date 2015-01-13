#include "audioinfo.h"
#include <QDebug>
#include <QAudioDeviceInfo>

AudioInfo::AudioInfo(QObject *parent) :
    QObject(parent)
{
}

void AudioInfo::printDevices(bool In)
{
    int i=0;
    if(In) {
        //print input devices
        foreach (const QAudioDeviceInfo &deviceInfo,  QAudioDeviceInfo::availableDevices(QAudio::AudioInput)) {
            qDebug() << "Index: " << i++ << ", audio input device name: " << deviceInfo.deviceName();
        }
    } else {
        foreach (const QAudioDeviceInfo &deviceInfo,  QAudioDeviceInfo::availableDevices(QAudio::AudioOutput)) {
            qDebug() << "Index: " << i++ << ", audio output device name: " << deviceInfo.deviceName();
        }
    }
}


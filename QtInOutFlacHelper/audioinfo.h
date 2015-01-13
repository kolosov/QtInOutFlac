#ifndef AUDIOINFO_H
#define AUDIOINFO_H

#include <QObject>

class AudioInfo : public QObject
{
    Q_OBJECT
public:
    explicit AudioInfo(QObject *parent = 0);
    static void printDevices(bool In);
signals:

public slots:

};

#endif // AUDIOINFO_H

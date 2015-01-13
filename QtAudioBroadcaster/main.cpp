#include <QCoreApplication>
#include "audiobroadcaster.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    AudioBroadcaster br;
    br.start();

    return a.exec();
}

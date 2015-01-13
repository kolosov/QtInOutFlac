#include <QCoreApplication>
#include "audiolistener.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    AudioListener listener;

    listener.start();


    return a.exec();
}

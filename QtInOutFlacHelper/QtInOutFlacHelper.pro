#-------------------------------------------------
#
# Project created by QtCreator 2015-01-12T12:15:26
#
#-------------------------------------------------

QT       += multimedia

QT       -= gui

TARGET = QtInOutFlacHelper
TEMPLATE = lib
CONFIG += staticlib

SOURCES += qtinoutflachelper.cpp \
    audioinput.cpp \
    audiooutput.cpp \
    audioinfo.cpp \
    flacencoder.cpp \
    flacdecoder.cpp

HEADERS += \


HEADERS += qtinoutflachelper.h \
    audioinput.h \
    audiooutput.h \
    audioinfo.h \
    flacencoder.h \
    flacdecoder.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

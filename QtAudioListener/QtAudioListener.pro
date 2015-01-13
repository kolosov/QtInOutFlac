#-------------------------------------------------
#
# Project created by QtCreator 2015-01-12T11:50:54
#
#-------------------------------------------------

QT       += core network multimedia

QT       -= gui

TARGET = QtAudioListener
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

INCLUDEPATH += /usr/include/FLAC++
LIBS += -lFLAC++ -lFLAC

SOURCES += main.cpp \
    audiolistener.cpp

HEADERS += \
    audiolistener.h

unix:!macx: LIBS += -L$$PWD/../build-QtInOutFlacHelper-Desktop-Debug/ -lQtInOutFlacHelper

INCLUDEPATH += $$PWD/../QtInOutFlacHelper
DEPENDPATH += $$PWD/../QtInOutFlacHelper

unix:!macx: PRE_TARGETDEPS += $$PWD/../build-QtInOutFlacHelper-Desktop-Debug/libQtInOutFlacHelper.a

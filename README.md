# QtInOutFlac

QtInOutFlac is simple application to record/play/broadcast audio by FLAC stream.

There are three components:

##QtInOutFlacHelper

The QtInOutFlacHelper is a class library to recoard/play sound and encode/decode FLAC stream.

##QtAudioBroadcaster
QtAudioBroadcaster is a simple application to record audio from microphone encode stream to FLAC and broadcast it.

##QtAudioListener
QtAudioListener is a simple application to listen FLAC stream, decode it and play.

#Requirements
Install Qt5 dev and libflac++ dev packages

#Building
##Build QtInOutFlacHelper:
    cd QtInOutFlacHelper && qmake && make
  
##Build QtAudioBroadcaster and QtAudioListener
  Before building please check that QtAudioBroadcaster's QtAudioBroadcaster.pro and QtAudioListener's QtAudioListener.pro 
  files content correct pathes to QtInOutFlacHelper's library(*.a) and includes.
  
    cd QtAudioBroadcaster && qmake && make
    cd QtAudioListener && qmake && make
    
#Run
First start QtAudioListener and then QtAudioBroadcaster
  

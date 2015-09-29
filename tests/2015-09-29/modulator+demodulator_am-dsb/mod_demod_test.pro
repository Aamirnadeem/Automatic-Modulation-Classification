#-------------------------------------------------
#
# Project created by QtCreator 2015-09-25T13:00:43
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

TARGET = mod_demod_test
TEMPLATE = app

CONFIG += c++11

LIBS += -luhd\
        -lboost_system\
        -lboost_thread\
        -lboost_chrono\
        -lfftw3\
        -lfftw3f\
        -lliquid

SOURCES += main.cpp\
        mainwindow.cpp \
    amc.cpp \
    efunction.cpp \
    fftgenerator.cpp \
    qcustomplot.cpp \
    uhdmock.cpp \
    uhdread.cpp \
    amcrecv.cpp \
    cosfunction.cpp \
    amdemod.cpp \
    amfunction.cpp

HEADERS  += mainwindow.h \
    amc.h \
    efunction.h \
    fftgenerator.h \
    qcustomplot.h \
    sharedbuffer.h \
    sharedqvector.h \
    sharedvector.h \
    streamer.h \
    streamfunction.h \
    uhdmock.h \
    uhdread.h \
    amcdemodulator.h \
    amcrecv.h \
    realstreamfunction.h \
    cosfunction.h \
    amdemod.h \
    amfunction.h

FORMS    += mainwindow.ui

DISTFILES += \
    mod_demod_test.pro.user

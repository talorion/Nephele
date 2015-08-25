#-------------------------------------------------
#
# Project created by QtCreator 2015-01-08T12:00:34
#
#-------------------------------------------------

QT       += core gui network printsupport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = DeviceTester
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    tcpdriver.cpp \
    flowcontrollerview.cpp \
    flowcontrollerbackend.cpp \
    analogvalue.cpp \
    analogview.cpp \
    ../../libs/qcustomplot/qcustomplot.cpp \
    tcpcommandqueue.cpp \
    qvmbackend.cpp

HEADERS  += mainwindow.h \
    tcpdriver.h \
    flowcontrollerview.h \
    flowcontrollerbackend.h \
    analogvalue.h \
    analogview.h \
    ../../libs/qcustomplot/qcustomplot.h \
    tcpcommandqueue.h \
    qvmbackend.h



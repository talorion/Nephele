#-------------------------------------------------
#
# Project created by QtCreator 2016-12-08T15:40:57
#
#-------------------------------------------------

#include(../../../../common_cxxflags.pri)
#include(../../../../common_flags.pri)

QT       += core gui script network scripttools
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport
QT       += widgets

TARGET = TcpBoxSimulator
TEMPLATE = lib
CONFIG += dll

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

DEFINES += TCPBOXSIMULATOR_LIBRARY

SOURCES += \
    abstract_simulated_box.cpp \
    simulated_ao_box.cpp \
    simulated_ao.cpp \
    simulated_av_box.cpp \
    simulated_av.cpp \
    simulated_fc_box.cpp \
    simulated_fc.cpp \
    tcp_box_connection.cpp \
    tcp_box_server.cpp \
    tcp_box_simulator_thread.cpp \
    tcp_box_simulator.cpp

HEADERS +=\
        tcpboxsimulator_global.hpp \
    simulated_ao.hpp \
    abstract_simulated_box.hpp \
    simulated_ao_box.hpp \
    simulated_av_box.hpp \
    simulated_av.hpp \
    simulated_fc_box.h \
    simulated_fc.h \
    tcp_box_connection.h \
    tcp_box_server.h \
    tcp_box_simulator_thread.hpp \
    tcp_box_simulator.hpp

unix {
    target.path = /usr/lib
    INSTALLS += target
}

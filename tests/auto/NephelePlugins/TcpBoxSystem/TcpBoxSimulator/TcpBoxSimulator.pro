#-------------------------------------------------
#
# Project created by QtCreator 2016-12-08T15:40:57
#
#-------------------------------------------------

include(../../../../../common_cxxflags.pri)

TARGET = TcpBoxSimulator
TEMPLATE = lib
CONFIG += dll

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

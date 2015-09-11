QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = tcp_box_simulator
TEMPLATE = app

unix: QMAKE_CXXFLAGS += -isystem $$[QT_INSTALL_HEADERS]
unix: QMAKE_CXXFLAGS += -isystem $$[QT_INSTALL_HEADERS]/QtCore
unix: QMAKE_CXXFLAGS += -isystem $$[QT_INSTALL_HEADERS]/QtGui
unix: QMAKE_CXXFLAGS += -isystem $$[QT_INSTALL_HEADERS]/QtScript
unix: QMAKE_CXXFLAGS += -isystem $$[QT_INSTALL_HEADERS]/QtWidgets
unix: QMAKE_CXXFLAGS += -Wall
unix: QMAKE_CXXFLAGS += -Wextra
unix: QMAKE_CXXFLAGS += -ansi
unix: QMAKE_CXXFLAGS += -pedantic
unix: QMAKE_CXXFLAGS += -Wshadow
unix: QMAKE_CXXFLAGS += -Weffc++
unix: QMAKE_CXXFLAGS += -Wstrict-aliasing

CONFIG += c++11


SOURCES += main.cpp\
        tcp_box_simulator.cpp \
    tcp_box_server.cpp \
    tcp_box_connection.cpp \
    simulated_fc.cpp \
    simulated_fc_box.cpp \
    simulated_av_box.cpp \
    simulated_av.cpp \
    abstract_simulated_box.cpp

HEADERS  += tcp_box_simulator.hpp \
    tcp_box_server.h \
    tcp_box_connection.h \
    simulated_fc.h \
    simulated_fc_box.h \
    simulated_av_box.hpp \
    simulated_av.hpp \
    abstract_simulated_box.hpp

#-------------------------------------------------
#
# Project created by QtCreator 2015-08-25T18:34:12
#
#-------------------------------------------------

QT       += core gui

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
        tcp_box_simulator.cpp

HEADERS  += tcp_box_simulator.hpp


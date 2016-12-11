unix | win32-g++:{
    QMAKE_CXXFLAGS += -isystem $$[QT_INSTALL_HEADERS]
    QMAKE_CXXFLAGS += -isystem $$[QT_INSTALL_HEADERS]/QtCore
    QMAKE_CXXFLAGS += -isystem $$[QT_INSTALL_HEADERS]/QtGui
    QMAKE_CXXFLAGS += -isystem $$[QT_INSTALL_HEADERS]/QtScript
    QMAKE_CXXFLAGS += -isystem $$[QT_INSTALL_HEADERS]/QtWidgets
    QMAKE_CXXFLAGS += -isystem $$[QT_INSTALL_HEADERS]/QtTest
    #QMAKE_CXXFLAGS += -isystem /tmp/qt-everywhere-opensource-src-5.5.1/qtimageformats/src/3rdparty/jasper/src/libjasper/base/jas_stream.c
    QMAKE_CXXFLAGS += -Wall
    QMAKE_CXXFLAGS += -Wextra
    QMAKE_CXXFLAGS += -ansi
    QMAKE_CXXFLAGS += -pedantic
    QMAKE_CXXFLAGS += -Wshadow
    QMAKE_CXXFLAGS += -Weffc++
    QMAKE_CXXFLAGS += -Wstrict-aliasing
}

CONFIG += warn_on

CONFIG += c++11
CONFIG += c++14
CONFIG += c++1z

#CONFIG += static

QT       += core gui script network scripttools
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

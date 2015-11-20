#-------------------------------------------------
#
# Project created by QtCreator 2015-11-02T18:41:48
#
#-------------------------------------------------


QT       += core
QT       -= gui

TARGET = qtofdaq
TEMPLATE = lib
CONFIG += staticlib

SOURCES += \
    tofdaqdll.cpp \
    twtooldll.cpp \
    twutils.cpp \
    shared_memory_descriptor.cpp \
    shared_memory_pointer.cpp

HEADERS += \
    tofdaqdll.hpp \
    twtooldll.hpp \
    twutils.hpp \
    shared_memory_descriptor.hpp \
    shared_memory_pointer.hpp
unix {
    target.path = /usr/lib
    INSTALLS += target
}

unix | win32-g++:{
    QMAKE_CXXFLAGS += -isystem $$[QT_INSTALL_HEADERS]
    QMAKE_CXXFLAGS += -isystem $$[QT_INSTALL_HEADERS]/QtCore
    QMAKE_CXXFLAGS += -isystem $$[QT_INSTALL_HEADERS]/QtGui
    QMAKE_CXXFLAGS += -isystem $$[QT_INSTALL_HEADERS]/QtScript
    QMAKE_CXXFLAGS += -isystem $$[QT_INSTALL_HEADERS]/QtWidgets
    QMAKE_CXXFLAGS += -isystem '$ORIGIN'/../../../libs/qcustomplot
    QMAKE_CXXFLAGS += -Wall
    QMAKE_CXXFLAGS += -Wextra
    QMAKE_CXXFLAGS += -ansi
    QMAKE_CXXFLAGS += -pedantic
    QMAKE_CXXFLAGS += -Wshadow
    QMAKE_CXXFLAGS += -Weffc++
    QMAKE_CXXFLAGS += -Wstrict-aliasing
}

CONFIG += c++11


unix: INCLUDEPATH += $$PWD/../../../libs/TofDaq_1.97_API/include
unix: DEPENDPATH += $$PWD/../../../libs/TofDaq_1.97_API/include

win32-g++: INCLUDEPATH += $$PWD/../../../libs/TofDaq_1.97_API/include
win32-g++: DEPENDPATH += $$PWD/../../../libs/TofDaq_1.97_API/include

win32-g++: INCLUDEPATH += $$PWD/../../../../../../Tofwerk/TofDaq_1.97_API/include
win32-g++: DEPENDPATH += $$PWD/../../../../../../Tofwerk/TofDaq_1.97_API/include

win32-g++: INCLUDEPATH += $$PWD/../../../../Apis/TofDaq_1.95_API/include
win32-g++: DEPENDPATH += $$PWD/../../../../Apis/TofDaq_1.95_API/include

unix: INCLUDEPATH += $$PWD/../../../../Apis/TofDaq_1.95_API/include
unix: DEPENDPATH += $$PWD/../../../../Apis/TofDaq_1.95_API/include

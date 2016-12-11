#-------------------------------------------------
#
# Project created by QtCreator 2016-12-11T13:06:07
#
#-------------------------------------------------

include(../../common_cxxflags.pri)

TARGET = qtofdaqdll
TEMPLATE = lib

DEFINES += QTOFDAQDLL_LIBRARY

SOURCES += QTofDaqDll.cpp

HEADERS += QTofDaqDll.hpp\
        qtofdaqdll_global.hpp

unix {
    target.path = /usr/lib
    INSTALLS += target
}

#===================
INCLUDEPATH += $$PWD/../TofDaq/TofDaq_1.98_API/include
DEPENDPATH += $$PWD/../TofDaq/TofDaq_1.98_API/include

unix | win32-g++:{
    QMAKE_CXXFLAGS += -isystem $$PWD/../TofDaq/TofDaq_1.98_API/include
}
#===================

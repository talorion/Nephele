include(../../../common_cxxflags.pri)

TARGET = LoggingSystem
TEMPLATE = lib
CONFIG += staticlib

DEFINES += LOGGING_SYSTEM_LIBRARY

HEADERS += \
    LoggingSystem.hpp \
    LoggingSystem_global.hpp

SOURCES += \
    LoggingSystem.cpp

## ================
unix:!macx: LIBS += -L$$OUT_PWD/../../NepheleCore/ -lNepheleCore

INCLUDEPATH += $$PWD/../../NepheleCore
DEPENDPATH += $$PWD/../../NepheleCore

unix:!macx: PRE_TARGETDEPS += $$OUT_PWD/../../NepheleCore/libNepheleCore.a
## ================


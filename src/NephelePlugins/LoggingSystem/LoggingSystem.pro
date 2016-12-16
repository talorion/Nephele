include(../../../common_cxxflags.pri)

TARGET = LoggingSystem
TEMPLATE = lib
#CONFIG += staticlib
CONFIG += dll

DEFINES += LOGGING_SYSTEM_LIBRARY

HEADERS += \
    LoggingSystem.hpp \
    LoggingSystem_global.hpp

SOURCES += \
    LoggingSystem.cpp

## ================
win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../NepheleCore/release/ -lNepheleCore
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../NepheleCore/debug/ -lNepheleCore
else:unix:!macx: LIBS += -L$$OUT_PWD/../../NepheleCore/ -lNepheleCore

INCLUDEPATH += $$PWD/../../NepheleCore
DEPENDPATH += $$PWD/../../NepheleCore

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../../NepheleCore/release/libNepheleCore.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../../NepheleCore/debug/libNepheleCore.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../../NepheleCore/release/NepheleCore.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../../NepheleCore/debug/NepheleCore.lib
else:unix:!macx: PRE_TARGETDEPS += $$OUT_PWD/../../NepheleCore/libNepheleCore.a
## ================


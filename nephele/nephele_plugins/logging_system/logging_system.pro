QT       += core network

TARGET = logging_system
TEMPLATE = lib
CONFIG += staticlib

DEFINES += LOGGING_SYSTEM_LIBRARY

#CONFIG += plugin

#win32-g++: CONFIG(release, debug|release) :DLLDESTDIR += ../release/plugins
#else:unix: CONFIG(release, debug|release) :DESTDIR += $$OUT_PWD/../release/plugins

#win32-g++: CONFIG(debug, debug|release): DLLDESTDIR += ../debug/plugins
#else:unix: CONFIG(debug, debug|release): DESTDIR += $$OUT_PWD/../debug/plugins

include(../../../common_cxxflags.pri)

HEADERS += \
    logging_system.hpp \
    logging_system_global.hpp

SOURCES += \
    logging_system.cpp

## ================
win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../nephele_core/release/ -lnephele_core
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../nephele_core/debug/ -lnephele_core
else:unix: LIBS += -L$$OUT_PWD/../../nephele_core/ -lnephele_core

INCLUDEPATH += $$PWD/../../nephele_core
DEPENDPATH += $$PWD/../../nephele_core

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../../nephele_core/release/libnephele_core.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../../nephele_core/debug/libnephele_core.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../../nephele_core/release/nephele_core.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../../nephele_core/debug/nephele_core.lib
else:unix: PRE_TARGETDEPS += $$OUT_PWD/../../nephele_core/libnephele_core.a
## ================

QT       += core network

TARGET = tcpbox_system
TEMPLATE = lib
CONFIG += staticlib

DEFINES += TCPBOX_SYSTEM_LIBRARY

#CONFIG += plugin

#win32-g++: CONFIG(release, debug|release) :DLLDESTDIR += ../release/plugins
#else:unix: CONFIG(release, debug|release) :DESTDIR += $$OUT_PWD/../release/plugins

#win32-g++: CONFIG(debug, debug|release): DLLDESTDIR += ../debug/plugins
#else:unix: CONFIG(debug, debug|release): DESTDIR += $$OUT_PWD/../debug/plugins

include(../../../common_cxxflags.pri)

HEADERS += \
    tcpbox_system.hpp \
    tcpbox_factory.hpp \
    tcpbox_client.hpp \
    commands/ecmd_help.hpp \
    commands/abstract_ecmd_command.hpp \
    commands/emcd_version.hpp \
    tcpbox_system_thread.hpp \
    ecmd_connection/ecmd_connection.hpp \
    ecmd_connection/ecmd_connection_manager.hpp

SOURCES += \
    tcpbox_system.cpp \
    tcpbox_factory.cpp \
    tcpbox_client.cpp \
    commands/ecmd_help.cpp \
    commands/abstract_ecmd_command.cpp \
    commands/emcd_version.cpp \
    tcpbox_system_thread.cpp \
    ecmd_connection/ecmd_connection.cpp \
    ecmd_connection/ecmd_connection_manager.cpp


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


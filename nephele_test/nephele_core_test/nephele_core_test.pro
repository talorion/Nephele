#-------------------------------------------------
#
# Project created by QtCreator 2015-11-20T18:26:02
#
#-------------------------------------------------

QT       += testlib

QT       -= gui

TARGET = tst_entity_manager
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

include(../../common_cxxflags.pri)


SOURCES += tst_entity_manager.cpp \
    main.cpp \
    tst_system_manager.cpp \
    system_dummy.cpp \
    tst_system.cpp
DEFINES += SRCDIR=\\\"$$PWD/\\\"

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../nephele/nephele_core/release/ -lnephele_core
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../nephele/nephele_core/debug/ -lnephele_core
else:unix: LIBS += -L$$OUT_PWD/../../nephele/nephele_core/ -lnephele_core

INCLUDEPATH += $$PWD/../../nephele/nephele_core
DEPENDPATH += $$PWD/../../nephele/nephele_core

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../../nephele/nephele_core/release/libnephele_core.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../../nephele/nephele_core/debug/libnephele_core.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../../nephele/nephele_core/release/nephele_core.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../../nephele/nephele_core/debug/nephele_core.lib
else:unix: PRE_TARGETDEPS += $$OUT_PWD/../../nephele/nephele_core/libnephele_core.a

HEADERS += \
    tst_entity_manager.hpp \
    tst_system_manager.hpp \
    system_dummy.hpp \
    tst_system.hpp

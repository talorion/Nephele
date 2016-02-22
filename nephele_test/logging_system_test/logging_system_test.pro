QT       += testlib
QT       -= gui


TARGET = logging_system_test
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

include(../../common_cxxflags.pri)

SOURCES += main.cpp \
    tst_logging_system.cpp

HEADERS += \
    tst_logging_system.hpp


win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../nephele/nephele_plugins/logging_system/release/ -llogging_system
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../nephele/nephele_plugins/logging_system/debug/ -llogging_system
else:unix: LIBS += -L$$OUT_PWD/../../nephele/nephele_plugins/logging_system/ -llogging_system

INCLUDEPATH += $$PWD/../../nephele/nephele_plugins/logging_system
DEPENDPATH += $$PWD/../../nephele/nephele_plugins/logging_system

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../../nephele/nephele_plugins/logging_system/release/liblogging_system.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../../nephele/nephele_plugins/logging_system/debug/liblogging_system.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../../nephele/nephele_plugins/logging_system/release/logging_system.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../../nephele/nephele_plugins/logging_system/debug/logging_system.lib
else:unix: PRE_TARGETDEPS += $$OUT_PWD/../../nephele/nephele_plugins/logging_system/liblogging_system.a

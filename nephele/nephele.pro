#-------------------------------------------------
#
# Project created by QtCreator 2015-08-25T18:40:47
#
#-------------------------------------------------

QT       += core gui script printsupport network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = nephele
TEMPLATE = app

unix: QMAKE_CXXFLAGS += -isystem $$[QT_INSTALL_HEADERS]
unix: QMAKE_CXXFLAGS += -isystem $$[QT_INSTALL_HEADERS]/QtCore
unix: QMAKE_CXXFLAGS += -isystem $$[QT_INSTALL_HEADERS]/QtGui
unix: QMAKE_CXXFLAGS += -isystem $$[QT_INSTALL_HEADERS]/QtScript
unix: QMAKE_CXXFLAGS += -isystem $$[QT_INSTALL_HEADERS]/QtWidgets
unix: QMAKE_CXXFLAGS += -isystem '$ORIGIN'/../../../libs/qcustomplot
unix: QMAKE_CXXFLAGS += -Wall
unix: QMAKE_CXXFLAGS += -Wextra
unix: QMAKE_CXXFLAGS += -ansi
unix: QMAKE_CXXFLAGS += -pedantic
unix: QMAKE_CXXFLAGS += -Wshadow
unix: QMAKE_CXXFLAGS += -Weffc++
unix: QMAKE_CXXFLAGS += -Wstrict-aliasing

CONFIG += c++11

#message(Qt version: $$[QT_VERSION])
#message(Qt is installed in $$[QT_INSTALL_PREFIX])
#message(Qt resources can be found in the following locations:)
#message(Documentation: $$[QT_INSTALL_DOCS])
#message(Header files: $$[QT_INSTALL_HEADERS])
#message(Libraries: $$[QT_INSTALL_LIBS])
#message(Binary files (executables): $$[QT_INSTALL_BINS])
#message(Plugins: $$[QT_INSTALL_PLUGINS])
#message(Data files: $$[QT_INSTALL_DATA])
#message(Translation files: $$[QT_INSTALL_TRANSLATIONS])
#message(Settings: $$[QT_INSTALL_SETTINGS])
#message(Examples: $$[QT_INSTALL_EXAMPLES])
#message(Demonstrations: $$[QT_INSTALL_DEMOS])


SOURCES += main.cpp\
    gui_system/nephele_main_window.cpp \
    core/event_manager.cpp \
    script_system/scripting_thread.cpp \
    script_system/scripting_worker.cpp \
    script_system/script_handler/script_act_handler.cpp \
    script_system/script_handler/script_dialoges_handler.cpp \
    script_system/script_handler/script_set_handler.cpp \
    script_system/script_handler/script_util_handler.cpp \
    gui_system/analogvalue.cpp \
    gui_system/analogview.cpp \
    gui_system/flowcontrollerbackend.cpp \
    gui_system/flowcontrollerview.cpp \
    gui_system/qvmbackend.cpp \
    gui_system/tcpcommandqueue.cpp \
    gui_system/tcpdriver.cpp

HEADERS  += gui_system/nephele_main_window.hpp \
    core/event_manager.hpp \
    script_system/scripting_thread.h \
    script_system/scripting_worker.h \
    script_system/script_handler/script_act_handler.h \
    script_system/script_handler/script_dialoges_handler.h \
    script_system/script_handler/script_set_handler.h \
    script_system/script_handler/script_util_handler.h \
    gui_system/analogvalue.h \
    gui_system/analogview.h \
    gui_system/flowcontrollerbackend.h \
    gui_system/flowcontrollerview.h \
    gui_system/qvmbackend.h \
    gui_system/tcpcommandqueue.h \
    gui_system/tcpdriver.h

SUBDIRS += \
    gui_system/DeviceTester.pro


win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../qcustomplot/release/ -lqcustomplot
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../qcustomplot/debug/ -lqcustomplot
else:unix: LIBS += -L$$OUT_PWD/../qcustomplot/ -lqcustomplot

INCLUDEPATH += $$PWD/../qcustomplot
DEPENDPATH += $$PWD/../qcustomplot

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../qcustomplot/release/libqcustomplot.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../qcustomplot/debug/libqcustomplot.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../qcustomplot/release/qcustomplot.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../qcustomplot/debug/qcustomplot.lib
else:unix: PRE_TARGETDEPS += $$OUT_PWD/../qcustomplot/libqcustomplot.a

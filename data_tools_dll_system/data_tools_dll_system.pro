#-------------------------------------------------
#
# Project created by QtCreator 2015-10-28T19:46:07
#
#-------------------------------------------------

QT       -= gui
QT       += core network

TARGET = data_tools_dll_system

TEMPLATE = lib

DEFINES += DATA_TOOLS_DLL_SYSTEM_LIBRARY

CONFIG += plugin

win32-g++: CONFIG(release, debug|release) :DLLDESTDIR += ../release/plugins
else:unix: CONFIG(release, debug|release) :DESTDIR += $$OUT_PWD/../release/plugins

win32-g++: CONFIG(debug, debug|release): DLLDESTDIR += ../debug/plugins
else:unix: CONFIG(debug, debug|release): DESTDIR += $$OUT_PWD/../debug/plugins

SOURCES +=    dtd_config_widget/dtd_config_widget.cpp \
    data_tools_dll_system.cpp \
    twtoolswrapper.cpp


HEADERS +=    dtd_config_widget/dtd_config_widget.hpp \
    data_tools_dll_system.hpp \
    twtoolswrapper.h

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


#=====================================
win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../nephele_core/release/ -lnephele_core
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../nephele_core/debug/ -lnephele_core
else:unix: LIBS += -L$$OUT_PWD/../nephele_core/ -lnephele_core

INCLUDEPATH += $$PWD/../nephele_core
DEPENDPATH += $$PWD/../nephele_core

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../nephele_core/release/libnephele_core.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../nephele_core/debug/libnephele_core.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../nephele_core/release/nephele_core.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../nephele_core/debug/nephele_core.lib
else:unix: PRE_TARGETDEPS += $$OUT_PWD/../nephele_core/libnephele_core.a
#=====================================

#=====================================
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
#=====================================

DISTFILES += \
    data_tools_dll_system.json

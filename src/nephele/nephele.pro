#-------------------------------------------------
#
# Project created by QtCreator 2015-08-25T18:40:47
#
#-------------------------------------------------

include(../common_cxxflags.pri)
include(../common_flags.pri)

win32: {
#RC_FILE = nephele.rc
RC_ICONS = nephele.ico
}

RESOURCES += Resources.qrc

TARGET = nephele
TEMPLATE = app
#VERSION = 0.0.$$BUILD_NUMBER
#VERSION = 2.0.0
VERSION = 0.$$member(BUILDDATE,1).$$BUILD_NUMBER
message(VERSION: $$VERSION)

CONFIG(debug, debug|release) {
  QMAKE_TARGET_PRODUCT = "Nephele (debug mode)"
  QMAKE_TARGET_DESCRIPTION = "Control Software for PTR3 (debug mode)"
} else {
  QMAKE_TARGET_PRODUCT = "Nephele"
  QMAKE_TARGET_DESCRIPTION = "Control Software for PTR3"
}
QMAKE_TARGET_COMPANY = "https://github.com/talorion/Nephele"
QMAKE_TARGET_COPYRIGHT = "Copyright (C) 2015 by Gregor Mayramhof"

#==========Deploy
win32: {
    TARGET_CUSTOM_EXT = .exe

    CONFIG( debug, debug|release ) {
        # debug
        DEPLOY_TARGET = $$shell_quote($$shell_path($${OUT_PWD}/debug/$${TARGET}$${TARGET_CUSTOM_EXT}))
        DLLDESTDIR  = $$shell_quote($$shell_path($${OUT_PWD}/out/debug/))
    } else {
        # release
        DEPLOY_TARGET = $$shell_quote($$shell_path($${OUT_PWD}/release/$${TARGET}$${TARGET_CUSTOM_EXT}))
        DLLDESTDIR  = $$shell_quote($$shell_path($${OUT_PWD}/out/release/))
    }

    DEPLOY_COMMAND = windeployqt
    QMAKE_POST_LINK = $${DEPLOY_COMMAND} --dir $${DLLDESTDIR} --no-translations $${DEPLOY_TARGET}
}
#==========================================


DISTFILES += \
    nephele.ico \
    build_number.txt \
    nephele.rc \
    nephele.qss

SOURCES += main.cpp

HEADERS  += version.hpp
            #build_number.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

## ================
win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../ext/qcustomplot/qcustomplot-sharedlib/sharedlib-compilation/release/ -lqcustomplot2
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../ext/qcustomplot/qcustomplot-sharedlib/sharedlib-compilation/debug/ -lqcustomplotd2
else:unix:!macx:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../ext/qcustomplot/qcustomplot-sharedlib/sharedlib-compilation/ -lqcustomplot
else:unix:!macx:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../ext/qcustomplot/qcustomplot-sharedlib/sharedlib-compilation/ -lqcustomplotd

INCLUDEPATH += $$PWD/../../ext/qcustomplot
DEPENDPATH += $$PWD/../../ext/qcustomplot
unix | win32-g++:{
    QMAKE_CXXFLAGS += -isystem $$PWD/../../ext/qcustomplot
}
## ================

## ================
win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../nepheleLib/release/ -lnepheleLib
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../nepheleLib/debug/ -lnepheleLibd
else:unix:!macx:CONFIG(release, debug|release): unix:!macx: LIBS += -L$$OUT_PWD/../nepheleLib/ -lnepheleLib
else:unix:!macx:CONFIG(debug, debug|release): unix:!macx: LIBS += -L$$OUT_PWD/../nepheleLib/ -lnepheleLibd

INCLUDEPATH += $$PWD/../nepheleLib
DEPENDPATH += $$PWD/../nepheleLib

win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../nepheleLib/release/nepheleLib.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../nepheleLib/debug/nepheleLibd.lib
## ================


## ================
win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../libs/qtofdaqdll/release/ -lqtofdaqdll
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../libs/qtofdaqdll/debug/ -lqtofdaqdll
else:unix:!macx: LIBS += -L$$OUT_PWD/../libs/qtofdaqdll/ -lqtofdaqdll

INCLUDEPATH += $$PWD/../libs/qtofdaqdll
DEPENDPATH += $$PWD/../libs/qtofdaqdll

win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../libs/qtofdaqdll/release/qtofdaqdll.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../libs/qtofdaqdll/debug/qtofdaqdll.lib
else:unix:!macx: PRE_TARGETDEPS += $$OUT_PWD/../libs/qtofdaqdll/libqtofdaqdll.a
## ================


## ================
win32:!win32-g++:CONFIG(debug, debug|release): LIBS += -L$$PWD/'../../../../../Program Files (x86)/Visual Leak Detector/lib/Win64/' -lvld

win32:!win32-g++:INCLUDEPATH += $$PWD/'../../../../../Program Files (x86)/Visual Leak Detector/include'
win32:!win32-g++:DEPENDPATH += $$PWD/'../../../../../Program Files (x86)/Visual Leak Detector/include'
## ================S

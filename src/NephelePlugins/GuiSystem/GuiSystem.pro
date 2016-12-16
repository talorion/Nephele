#-------------------------------------------------
#
# Project created by QtCreator 2016-12-09T11:40:33
#
#-------------------------------------------------

include(../../../common_cxxflags.pri)

TARGET = GuiSystem
TEMPLATE = lib
#CONFIG += staticlib
CONFIG += dll

DEFINES += GUISYSTEM_LIBRARY

#CONFIG += plugin
#====== DEPLOY =====
#win32: DLLDESTDIR += $$OUT_PWD/../../nephele/plugins
#unix:!macx: DESTDIR += $$OUT_PWD/../../nephele/plugins
#===================

SOURCES += GuiSystem.cpp \
    NepheleMainWindow/NepheleMainWindow.cpp \
    NepheleMainWindow/CustomWidgets/SpectraPlot.cpp \
    NepheleMainWindow/CustomWidgets/AbstractPlot.cpp \
    NepheleMainWindow/CustomWidgets/TracesPlot.cpp \
    Style.cpp

HEADERS += GuiSystem.hpp\
        guisystem_global.hpp \
    NepheleMainWindow/NepheleMainWindow.hpp \
    NepheleMainWindow/CustomWidgets/SpectraPlot.hpp \
    NepheleMainWindow/CustomWidgets/AbstractPlot.hpp \
    NepheleMainWindow/CustomWidgets/TracesPlot.hpp \
    Style.hpp

#unix {
#    target.path = /usr/lib
#    INSTALLS += target
#}

DISTFILES += \
    GuiSystem.json

# install
#target.path = $$OUT_PWD/../../nephele/plugins
#INSTALLS += target

CONFIG += install_ok  # Do not cargo-cult this!


## ================
win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../NepheleCore/release/ -lNepheleCore
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../NepheleCore/debug/ -lNepheleCore
else:unix:!macx: LIBS += -L$$OUT_PWD/../../NepheleCore/ -lNepheleCore

INCLUDEPATH += $$PWD/../../NepheleCore
DEPENDPATH += $$PWD/../../NepheleCore

win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../../NepheleCore/release/NepheleCore.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../../NepheleCore/debug/NepheleCore.lib
else:unix:!macx: PRE_TARGETDEPS += $$OUT_PWD/../../NepheleCore/libNepheleCore.a
## ================

## ================
#DEFINES += QCUSTOMPLOT_USE_LIBRARY
win32:!win32-g++:CONFIG(release, debug|release):    LIBS += -L$$OUT_PWD/../../libs/qcustomplot/qcustomplot-sharedlib/sharedlib-compilation/release/ -lqcustomplot2
else:win32:!win32-g++:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../libs/qcustomplot/qcustomplot-sharedlib/sharedlib-compilation/debug/ -lqcustomplotd2
else:unix:!macx: LIBS += -L$$OUT_PWD/../../libs/qcustomplot/qcustomplot-sharedlib/sharedlib-compilation/ -lqcustomplotd

INCLUDEPATH += $$PWD/../../libs/qcustomplot
DEPENDPATH += $$PWD/../../libs/qcustomplot

unix | win32-g++:{
    QMAKE_CXXFLAGS += -isystem $$PWD/../../libs/qcustomplot
}

win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS     += $$OUT_PWD/../../libs/qcustomplot/qcustomplot-sharedlib/sharedlib-compilation/release/qcustomplot2.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS  += $$OUT_PWD/../../libs/qcustomplot/qcustomplot-sharedlib/sharedlib-compilation/debug/qcustomplotd2.lib
## ================


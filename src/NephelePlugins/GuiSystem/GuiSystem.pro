#-------------------------------------------------
#
# Project created by QtCreator 2016-12-09T11:40:33
#
#-------------------------------------------------

include(../../../common_cxxflags.pri)

TARGET = GuiSystem
TEMPLATE = lib
#CONFIG += staticlib

DEFINES += GUISYSTEM_LIBRARY

#CONFIG += plugin
#====== DEPLOY =====
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

unix {
    target.path = /usr/lib
    INSTALLS += target
}

DISTFILES += \
    GuiSystem.json


## ================
unix:!macx: LIBS += -L$$OUT_PWD/../../NepheleCore/ -lNepheleCore

INCLUDEPATH += $$PWD/../../NepheleCore
DEPENDPATH += $$PWD/../../NepheleCore

unix:!macx: PRE_TARGETDEPS += $$OUT_PWD/../../NepheleCore/libNepheleCore.a
## ================

## ================
unix:!macx: LIBS += -L$$OUT_PWD/../../libs/qcustomplot/qcustomplot-sharedlib/sharedlib-compilation/ -lqcustomplotd

INCLUDEPATH += $$PWD/../../libs/qcustomplot
DEPENDPATH += $$PWD/../../libs/qcustomplot

unix | win32-g++:{
    QMAKE_CXXFLAGS += -isystem $$PWD/../../libs/qcustomplot
}

## ================

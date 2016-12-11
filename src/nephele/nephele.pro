
TARGET = nephele
TEMPLATE = app

include(../../common_cxxflags.pri)


SOURCES += \
    main.cpp


## ================
unix:!macx: LIBS += -L$$OUT_PWD/../NepheleCore/ -lNepheleCore

INCLUDEPATH += $$PWD/../NepheleCore
DEPENDPATH += $$PWD/../NepheleCore

#unix:!macx: PRE_TARGETDEPS += $$OUT_PWD/../NepheleCore/libNepheleCore.a
## ================

## ================
unix:!macx: LIBS += -L$$OUT_PWD/../NephelePlugins/GuiSystem/ -lGuiSystem

INCLUDEPATH += $$PWD/../NephelePlugins/GuiSystem
DEPENDPATH += $$PWD/../NephelePlugins/GuiSystem

#unix:!macx: PRE_TARGETDEPS += $$OUT_PWD/../NephelePlugins/GuiSystem/libGuiSystem.a
## ================


### ================
unix:!macx: LIBS += -L$$OUT_PWD/../libs/qcustomplot/qcustomplot-sharedlib/sharedlib-compilation/ -lqcustomplotd

INCLUDEPATH += $$PWD/../libs/qcustomplot
DEPENDPATH += $$PWD/../libs/qcustomplot

unix | win32-g++:{
    QMAKE_CXXFLAGS += -isystem $$PWD/../libs/qcustomplot
}
### ================

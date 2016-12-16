include(../../common_cxxflags.pri)
CONFIG += static

TARGET = nephele
TEMPLATE = app

SOURCES += \
    main.cpp


## ================
win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../NepheleCore/release/ -lNepheleCore
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../NepheleCore/debug/ -lNepheleCore
else:unix:!macx: LIBS += -L$$OUT_PWD/../NepheleCore/ -lNepheleCore

INCLUDEPATH += $$PWD/../NepheleCore
DEPENDPATH += $$PWD/../NepheleCore

win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../NepheleCore/release/NepheleCore.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../NepheleCore/debug/NepheleCore.lib
#unix:!macx: PRE_TARGETDEPS += $$OUT_PWD/../NepheleCore/libNepheleCore.a
## ================

## ================
win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../NephelePlugins/GuiSystem/release/ -lGuiSystem
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../NephelePlugins/GuiSystem/debug/ -lGuiSystem
else:unix:!macx: LIBS += -L$$OUT_PWD/../NephelePlugins/GuiSystem/ -lGuiSystem

INCLUDEPATH += $$PWD/../NephelePlugins/GuiSystem
DEPENDPATH += $$PWD/../NephelePlugins/GuiSystem

win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../NephelePlugins/GuiSystem/release/GuiSystem.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../NephelePlugins/GuiSystem/debug/GuiSystem.lib
#unix:!macx: PRE_TARGETDEPS += $$OUT_PWD/../NephelePlugins/GuiSystem/libGuiSystem.a
## ================


## ================
#DEFINES += QCUSTOMPLOT_USE_LIBRARY
win32:!win32-g++:CONFIG(release, debug|release):    LIBS += -L$$OUT_PWD/../libs/qcustomplot/qcustomplot-sharedlib/sharedlib-compilation/release/ -lqcustomplot2
else:win32:!win32-g++:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../libs/qcustomplot/qcustomplot-sharedlib/sharedlib-compilation/debug/ -lqcustomplotd2
else:unix:!macx: LIBS += -L$$OUT_PWD/../libs/qcustomplot/qcustomplot-sharedlib/sharedlib-compilation/ -lqcustomplotd

INCLUDEPATH += $$PWD/../libs/qcustomplot
DEPENDPATH += $$PWD/../libs/qcustomplot

unix | win32-g++:{
    QMAKE_CXXFLAGS += -isystem $$PWD/../libs/qcustomplot
}

win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS     += $$OUT_PWD/../libs/qcustomplot/qcustomplot-sharedlib/sharedlib-compilation/release/qcustomplot2.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS  += $$OUT_PWD/../libs/qcustomplot/qcustomplot-sharedlib/sharedlib-compilation/debug/qcustomplotd2.lib
## ================

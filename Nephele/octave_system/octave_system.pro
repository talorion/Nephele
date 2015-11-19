QT       -= gui
QT       += core network

TARGET = data_aquisition_dll_system
TEMPLATE = lib

DEFINES += DATA_AQUISITION_DLL_SYSTEM_LIBRARY

CONFIG += plugin

win32-g++: CONFIG(release, debug|release) :DLLDESTDIR += ../release/plugins
else:unix: CONFIG(release, debug|release) :DESTDIR += $$OUT_PWD/../release/plugins

win32-g++: CONFIG(debug, debug|release): DLLDESTDIR += ../debug/plugins
else:unix: CONFIG(debug, debug|release): DESTDIR += $$OUT_PWD/../debug/plugins


SOURCES += \
    octave_system.cpp \
    octave_worker.cpp \
    octave_config_widget/octave_config_widget.cpp

HEADERS += \
    octave_system.hpp \
    octave_worker.hpp \
    octave_config_widget/octave_config_widget.hpp

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

### ==============================================================
### ================ +++ OCTAVE RELATED STUFF +++ ================
### ==============================================================
### on ubuntu i used sudo
### apt-add-repository ppa:octave/stable
### sudo apt-get update
### sudo apt-get install octave
###
### on windows i used the installer from here https://ftp.gnu.org/gnu/octave/windows/
### note if you compile with your own mingw you need to use qt5 or higher
### and put the bin folder of OCTAVE_HOME at the begin of your PATH
### todo: find a better solution for that (compile octave on my own)
###
## comment this out if you need a different version of OCTAVE,
## and set set OCT_HOME accordingly as an environment variable
OCT_VERSION =4.0.0

#MKOCTFILE = mkoctfile --link-stand-alone
MKOCTFILE = mkoctfile
CONFIG(debug, debug|release): MKOCTFILE += -g
OCT_HOME = $$system($$MKOCTFILE -p OCTAVE_HOME)
win32-g++ {
    OCT_HOME =                $$system_path(c:\Octave\Octave-4.0.0)
    #OCT_VERSION =3.6.4
    #OCT_HOME =                $$system_path(C:\Octave\Octave3.6.4_gcc4.6.2)
    MKOCTFILE =               $$system_path($$OCT_HOME/bin/mkoctfile)
    message("OCT_HOME =" $$OCT_HOME)
    message("MKOCTFILE =" $$MKOCTFILE)
}

########################
### include headers and libraries for OCTAVE
OCT_LIBS = $$system($$MKOCTFILE -p LIBS)
#win32-g++ {
#    OCT_LIBS +=-lreadline
#}
#message("OCT_LIBS =" $$OCT_LIBS)

OCT_OCTAVE_LIBS = $$system($$MKOCTFILE -p OCTAVE_LIBS)
win32-g++ {
    OCT_OCTAVE_LIBS = -lliboctave
    #OCT_OCTAVE_LIBS += -lliboctgui
    OCT_OCTAVE_LIBS += -lliboctinterp
}
#message("OCT_OCTAVE_LIBS =" $$OCT_OCTAVE_LIBS)

OCT_INCFLAGS = $$system($$MKOCTFILE -p INCFLAGS)
win32-g++ {
    OCT_INCFLAGS = -I$$system_path($$OCT_HOME/include/octave-$$OCT_VERSION/octave/..)
    OCT_INCFLAGS += -I$$system_path($$OCT_HOME/include/octave-$$OCT_VERSION/octave)
    #OCT_INCFLAGS += -I$$system_path($$OCT_HOME/include)

    QMAKE_CXXFLAGS += -isystem $$system_path($$OCT_HOME/include/octave-$$OCT_VERSION/octave/..)
    QMAKE_CXXFLAGS += -isystem $$system_path($$OCT_HOME/include/octave-$$OCT_VERSION/octave)
    #QMAKE_CXXFLAGS += -isystem $$system_path($$OCT_HOME/include)
}
unix {
    QMAKE_CXXFLAGS += -isystem $$system($$MKOCTFILE -p OCTINCLUDEDIR)/..
    QMAKE_CXXFLAGS += -isystem $$system($$MKOCTFILE -p OCTINCLUDEDIR)
}
#message("OCT_INCFLAGS =" $$OCT_INCFLAGS)

OCT_LFLAGS = $$system($$MKOCTFILE -p LFLAGS)
win32-g++ {
    OCT_LFLAGS = -L$$system_path($$OCT_HOME/lib/octave/$$OCT_VERSION)
    OCT_LFLAGS += -L$$system_path($$OCT_HOME/lib)
    OCT_LFLAGS += -L$$system_path($$OCT_HOME/lib/gcc/i686-w64-mingw32/4.9.2)
    #OCT_LFLAGS += -L$$system_path($$OCT_HOME/lib/pstoedit)
}
#message("OCT_LFLAGS =" $$OCT_LFLAGS)

OCT_OCTLIBDIR = $$system($$MKOCTFILE -p OCTLIBDIR)
win32-g++ {
    OCT_OCTLIBDIR = $$system_path($$OCT_HOME/lib/octave/$$OCT_VERSION)
}
#message("OCT_OCTLIBDIR =" $$OCT_OCTLIBDIR)

OCT_XTRA_CFLAGS = $$system($$MKOCTFILE -p XTRA_CFLAGS)
#message("OCT_XTRA_CFLAGS =" $$OCT_XTRA_CFLAGS)

OCT_CXXFLAGS = $$system($$MKOCTFILE -p CXXFLAGS)
#message("OCT_CXXFLAGS =" $$OCT_CXXFLAGS)

OCT_LDFLAGS = $$system($$MKOCTFILE -p LDFLAGS)
win32-g++ {
    OCT_LDFLAGS = -Wl,-rpath-link,$$system_path($$OCT_HOME/lib)
    OCT_LDFLAGS += -L$$system_path($$OCT_HOME/lib)
    OCT_LDFLAGS += -Wl,--export-all-symbols
}
#message("OCT_LDFLAGS =" $$OCT_LDFLAGS)

OCT_DL_LDFLAGS= $$system($$MKOCTFILE -p DL_LDFLAGS)
win32-g++ {
}

OCT_BLAS_LIBS = $$system($$MKOCTFILE -p BLAS_LIBS)
#win32-g++ {
    #OCT_BLAS_LIBS =-llibopenblas
    #OCT_BLAS_LIBS =-llibblas
#}

OCT_ALL_CXXFLAGS = $$OCT_INCFLAGS $$OCT_XTRA_CFLAGS $$OCT_CXXFLAGS
#message("OCT_ALL_CXXFLAGS =" $$OCT_ALL_CXXFLAGS)

OCT_ALL_LDFLAGS = $$OCT_LDFLAGS
#message("OCT_ALL_LDFLAGS =" $$OCT_ALL_LDFLAGS)

OCT_BLAS =                 $$OCT_BLAS_LIBS
#message("OCT_BLAS =" $$OCT_BLAS)
########################

### compiler etc settings used in default make rules
QMAKE_CXXFLAGS +=   $$OCT_ALL_CXXFLAGS
QMAKE_LFLAGS +=     $$OCT_ALL_LDFLAGS
LIBS += $$OCT_LFLAGS $$OCT_OCTAVE_LIBS $$OCT_LIBS $$OCT_BLAS
### ==============================================================

DISTFILES += \
    octave_system.json

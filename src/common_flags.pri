CONFIG += warn_on

unix | win32-g++:{
    CONFIG += c++1z
}else: {
    CONFIG += c++14
}


QT       += core gui script network scripttools serialport
QT      += concurrent
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport
QT += widgets

unix:!macx: {
    CONFIG += static
}else:{
    CONFIG += static
    DEFINES += STATIC
    STATIC = 1
}

#CONFIG += debug_and_release build_all

#BUILD_NUMBER = 695
#DEFINES += BUILD_DATE='"$(shell date)"'
#DEFINES += BUILD='"$$BUILD_NUMBER"'

#SHELL_DATE = '"$(shell date)"'
#message(SHELL_DATE: $$_DATE_)

win32: {
    BUILDDATE = $$system( date /t ) $$system( time /t )
    BUILD_DATE = $$member(BUILDDATE,0)_$$member(BUILDDATE,1)
    BUILDDATE = $$split(BUILDDATE, /)
    BUILDDATE = $$split(BUILDDATE, :)
    BUILDDATE = $$split(BUILDDATE, .)
}
unix:!macx: {
    #BUILDDATE = $$system( date "+%d.%m.%Y_%H:%M:%S" )
    BUILD_DATE = $$system( date "+%d.%m.%Y_%H:%M:%S" )
    BUILDDATE = $$system( date $$shell_quote("+%d %m %Y %H %M %S") )
}

#BUILD_NUMBER = $$member(BUILDDATE,1)$$member(BUILDDATE,0)$$member(BUILDDATE,3)$$member(BUILDDATE,4)
#BUILD_NUMBER = $$member(BUILDDATE,1)$$member(BUILDDATE,0)$$member(BUILDDATE,3)
#BUILD_NUMBER = $$member(BUILDDATE,0)$$member(BUILDDATE,3)
BUILD_NUMBER = $$member(BUILDDATE,0).$$member(BUILDDATE,3)$$member(BUILDDATE,4)
#BUILD_NUMBER = 695

#message(BUILDDATE: $$BUILDDATE)
#message(BUILD_DATE: $$BUILD_DATE)
#message(BUILD_NUMBER: $$BUILD_NUMBER)

DEFINES += BUILD_DATE='"$(shell date)"'
DEFINES += BUILD='"$$BUILD_NUMBER"'

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

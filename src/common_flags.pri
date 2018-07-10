#CONFIG += warn_on

#unix | win32-g++:{
unix {
    CONFIG += c++1z
}else: {
    CONFIG += c++14
}


#QT      += core gui script network scripttools serialport
QT      += core
QT      += gui
QT      += script
QT      += network
QT      += scripttools
QT      += serialport
QT      += concurrent
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport
QT += widgets

unix| win32-g++:!macx {
    CONFIG += static
}else:{
    CONFIG += static
    DEFINES += STATIC
    STATIC = 1
}

#CONFIG += debug_and_release build_all

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

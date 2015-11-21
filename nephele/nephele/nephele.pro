QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = nephele
TEMPLATE = app

include(../../common_cxxflags.pri)

SOURCES += \
    main.cpp

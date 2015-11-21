QT       -= gui

TARGET = nephele_core
TEMPLATE = lib
CONFIG += staticlib

include(../../common_cxxflags.pri)

HEADERS += \
    entity_manager/entity_manager.hpp

SOURCES += \
    entity_manager/entity_manager.cpp

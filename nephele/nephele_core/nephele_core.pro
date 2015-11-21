QT       -= gui

TARGET = nephele_core
TEMPLATE = lib
CONFIG += staticlib

include(../../common_cxxflags.pri)

HEADERS += \
    entity_manager/entity_manager.hpp \
    entity_manager/entity_manager_db.hpp

SOURCES += \
    entity_manager/entity_manager.cpp \
    entity_manager/entity_manager_db.cpp

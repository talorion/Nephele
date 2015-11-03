#-------------------------------------------------
#
# Project created by QtCreator 2015-10-28T18:31:59
#
#-------------------------------------------------

QT       -= gui

TARGET = nephele_core
TEMPLATE = lib
CONFIG += staticlib

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

SOURCES += \
    abstract_scriptable_object.cpp \
    system_manager.cpp \
    entity_manager/entity_manager.cpp \
    entity_manager/abstract_entity_manager.cpp \
    entity_manager/entity_manager_locator.cpp \
    entity_manager/null_entity_manager.cpp \
    event_manager/event_manager.cpp \
    event_manager/event_manager_locator.cpp \
    event_manager/abstract_event_manager.cpp \
    event_manager/null_event_manager.cpp

HEADERS += \
    abstract_configuration_widget.hpp \
    abstract_scriptable_object.hpp \
    abstract_system.hpp \
    system_manager.hpp \
    entity_manager/entity_manager.hpp \
    entity_manager/abstract_entity_manager.hpp \
    entity_manager/entity_manager_locator.hpp \
    entity_manager/null_entity_manager.hpp \
    event_manager/event_manager.hpp \
    event_manager/event_manager_locator.hpp \
    event_manager/abstract_event_manager.hpp \
    event_manager/null_event_manager.hpp

unix {
    target.path = /usr/lib
    INSTALLS += target
}

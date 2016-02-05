QT       -= gui

TARGET = nephele_core
TEMPLATE = lib
CONFIG += staticlib

include(../../common_cxxflags.pri)

HEADERS += \
    entity_manager/entity_manager.hpp \
    entity_manager/entity_manager_db.hpp \
    system_manager/system_manager.hpp \
    system_manager/abstract_system.hpp \
    system_manager/system_states/abstract_system_state.hpp \
    system_manager/system_states/undefined_state.hpp \
    system_manager/system_states/initialized_state.hpp \
    system_manager/system_states/disposed_state.hpp \
    system_manager/system_states/started_state.hpp \
    system_manager/system_states/error_state.hpp

SOURCES += \
    entity_manager/entity_manager.cpp \
    entity_manager/entity_manager_db.cpp \
    system_manager/system_manager.cpp \
    system_manager/abstract_system.cpp \
    system_manager/system_states/abstract_system_state.cpp \
    system_manager/system_states/undefined_state.cpp \
    system_manager/system_states/initialized_state.cpp \
    system_manager/system_states/disposed_state.cpp \
    system_manager/system_states/started_state.cpp \
    system_manager/system_states/error_state.cpp

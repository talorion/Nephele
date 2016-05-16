QT       -= gui
QT       -= widget


TARGET = nephele_core
TEMPLATE = lib
CONFIG += staticlib

include(../../common_cxxflags.pri)

HEADERS += \
    entity_manager/entity_manager.hpp \
    entity_manager/entity_manager_db.hpp \
    system_manager/system_manager.hpp \
    system_manager/abstract_system.hpp \
    system_manager/configuration_widget/abstract_configuration_widget.hpp \
    entity_manager/static_components.hpp \
    event_manager/event_manager.hpp

SOURCES += \
    entity_manager/entity_manager.cpp \
    entity_manager/entity_manager_db.cpp \
    system_manager/system_manager.cpp \
    system_manager/abstract_system.cpp \
    system_manager/configuration_widget/abstract_configuration_widget.cpp \
    event_manager/event_manager.cpp

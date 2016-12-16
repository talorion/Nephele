include(../../common_cxxflags.pri)

TARGET = NepheleCore
TEMPLATE = lib
CONFIG += staticlib


HEADERS += \
    EntityManager/EntityManager.hpp \
    EntityManager/EntityManagerDb.hpp \
    EventManager/EventManager.hpp \
    SystemManager/AbstractSystem.hpp \
    SystemManager/SystemManager.hpp \
    SystemManager/ConfigurationWidget/AbstractConfigurationWidget.hpp \
    EntityManager/StaticComponentId.hpp \
    ManagerLocator.hpp \
    SystemManager/SystemEntityFactory.hpp \
    SystemManager/SystemState.hpp \
    SystemManager/SystemView/AbstractSystemView.hpp

SOURCES += \
    EntityManager/EntityManager.cpp \
    EntityManager/EntityManagerDb.cpp \
    EventManager/EventManager.cpp \
    SystemManager/AbstractSystem.cpp \
    SystemManager/SystemManager.cpp \
    SystemManager/ConfigurationWidget/AbstractConfigurationWidget.cpp \
    ManagerLocator.cpp \
    SystemManager/SystemEntityFactory.cpp \
    SystemManager/SystemView/AbstractSystemView.cpp

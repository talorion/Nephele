#include "EventManager.hpp"
#include <QMetaType>

namespace talorion {

  EventManager::EventManager(QObject *par):QObject(par){
    qRegisterMetaType<EntityManager::EntityID>("EntityManager::EntityID");
    qRegisterMetaType<EntityManager::ComponentID>("EntityManager::ComponentID");
    qRegisterMetaType<EntityManager::ComponentData>("EntityManager::ComponentData");
  }

}



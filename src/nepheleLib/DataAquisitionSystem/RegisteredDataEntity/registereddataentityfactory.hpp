#ifndef REGISTEREDDATAENTITYFACTORY_HPP
#define REGISTEREDDATAENTITYFACTORY_HPP

#include <QUuid>

#include <core/EntityManager.hpp>
#include <core/ManagerLocator.hpp>

#include "userdatasource.hpp"

namespace talorion {

  class RegisteredDataEntityFactory
  {
  private:
      explicit RegisteredDataEntityFactory();
      virtual ~RegisteredDataEntityFactory() = default;

  public:
    static QUuid registeredDataEntityUid() {return QUuid("{8907a558-c850-4c84-be98-a6c0632f25ba}");}
    static QUuid registeredSourceEntityUid(){return QUuid("{801f6bfc-b9d3-4af0-87d2-bbef2ae153cf}");}

    static EntityManager::ComponentID serialVersionUidComponentId(){return SERIAL_VERSION_UID_COMPONENT;}
    static EntityManager::ComponentID nameComponentId(){return NAME_COMPONENT;}
    static EntityManager::ComponentID analogActValueComponentId(){return ANALOG_ACT_VALUE_COMPONENT;}

  public:
    static EntityManager::EntityList getAllRegisteredDataEntites();
    static EntityManager::EntityList getAllRegisteredSourceEntities();
    
    static EntityManager::EntityID createNewRegisteredDataEntity(QString name, double analogActValue, EntityManager::EntityID parent  = EntityManager::invalid_id);
    static EntityManager::EntityID createNewRegisteredSourceEntity(QString name);
    static EntityManager::EntityID createNewRegisteredSourceEntity(const UserDataSource& src);

    static EntityManager::EntityID registeredDataEntityExists(QString dataname, EntityManager::EntityID dataparent);
    static EntityManager::EntityID registeredSourceEntityExists(QString sourcename);

  private:
    static EntityManager& entMng(){ return ManagerLocator::entityManager();}
    static EventManager& evtMng(){ return ManagerLocator::eventManager();}

  };

} // namespace talorion

#endif // REGISTEREDDATAENTITYFACTORY_HPP

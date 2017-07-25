#ifndef REGISTEREDDATAENTITY_HPP
#define REGISTEREDDATAENTITY_HPP

#include <core/EntityManager.hpp>
#include <core/ManagerLocator.hpp>

namespace talorion {

  class RegisteredDataEntity
  {
  public:
    RegisteredDataEntity(EntityManager::EntityID registeredDataEntity);

    EntityManager::EntityID parent() const;

    QString name() const;

    double analogActValue() const;
    void setAnalogActValue(double anActValue);

    QUuid serialVersionUid() const;

    EntityManager::EntityID entity() const;

    bool isValid()const;

  private:
    static EntityManager& entMng(){ return ManagerLocator::entityManager();}
    static EventManager& evtMng(){ return ManagerLocator::eventManager();}

  private:
    const EntityManager::EntityID m_entity;

  };

} // namespace talorion

#endif // REGISTEREDDATAENTITY_HPP

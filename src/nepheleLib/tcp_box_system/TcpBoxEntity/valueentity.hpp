#ifndef VALUEENTITY_HPP
#define VALUEENTITY_HPP

#include "core/EntityManager.hpp"

#include <core/ManagerLocator.hpp>

namespace talorion {

  class ValueEntity
  {
  public:
    ValueEntity(EntityManager::EntityID en);
    virtual ~ValueEntity() = default;

    QString name() const;
    QUuid serialVersionUid() const;
    EntityManager::EntityID parent() const;
    EntityManager::EntityID entity() const;

    int id() const;
    int boxId() const;

    static_component_id userData() const;

    QString units() const;

  protected:
    static EntityManager& entMng(){ return ManagerLocator::entityManager();}
    static EventManager& evtMng(){ return ManagerLocator::eventManager();}

  private:
    const EntityManager::EntityID m_entity;

  };

} // namespace talorion

#endif // VALUEENTITY_HPP

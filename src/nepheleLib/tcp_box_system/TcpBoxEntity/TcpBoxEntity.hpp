#ifndef TCPBOXENTITY_HPP
#define TCPBOXENTITY_HPP

#include "core/EntityManager.hpp"

#include <core/ManagerLocator.hpp>

namespace talorion {

  class TcpBoxEntity
  {
  public:
    explicit TcpBoxEntity(EntityManager::EntityID tcpBoxEntity);

    QString name() const;
    EntityManager::EntityID boxId() const;
    QString host() const;
    quint16 port() const;
    bool connectionState() const;
    QUuid serialVersionUid() const;
    int backendType() const;
    bool autoReconnect() const;

    bool isValid() const;

    EntityManager::EntityID entity() const;

  private:
    static EntityManager& entMng(){ return ManagerLocator::entityManager();}
    static EventManager& evtMng(){ return ManagerLocator::eventManager();}

  private:
    const EntityManager::EntityID m_entity;

  };
}

#endif // TCPBOXENTITY_HPP

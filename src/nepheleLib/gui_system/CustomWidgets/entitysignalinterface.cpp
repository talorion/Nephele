#include "entitysignalinterface.hpp"

namespace talorion {

  EntityManager::EntityID EntitySignalInterface::entity() const
  {
    return m_entity;
  }

  bool EntitySignalInterface::belongsToMe(EntityManager::EntityID en) const
  {
    if(EntityManager::isValid(en) == false )
      return false;

    if(isValid() == false)
      return false;

    if(en == entity())
      return false;

    return true;
  }

  bool EntitySignalInterface::isValid() const
  {
    if(EntityManager::isValid(entity()) == false )
      return false;

    return true;
  }



} // namespace talorion

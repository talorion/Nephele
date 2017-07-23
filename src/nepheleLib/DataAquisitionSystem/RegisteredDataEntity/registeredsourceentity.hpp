#ifndef REGISTEREDSOURCEENTITY_HPP
#define REGISTEREDSOURCEENTITY_HPP

#include <QString>
#include <userdatasource.hpp>
#include <core/EntityManager.hpp>


namespace talorion {

  class RegisteredSourceEntity
  {
  public:
    RegisteredSourceEntity(EntityManager::EntityID registeredSourceEntity);

    QString name() const;
    QUuid serialVersionUid() const;
    EntityManager::EntityList children() const;
    EntityManager::EntityID entity() const;

    void updateChildren(UserDataSource src) const;

    bool isValid()const;

  private:
    static EntityManager& entMng(){ return ManagerLocator::entityManager();}
    static EventManager& evtMng(){ return ManagerLocator::eventManager();}

  private:
    const EntityManager::EntityID m_entity;
  };

} // namespace talorion

#endif // REGISTEREDSOURCEENTITY_HPP

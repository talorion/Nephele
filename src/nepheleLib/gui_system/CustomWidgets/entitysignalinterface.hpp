#ifndef ENTITYSIGNALINTERFACE_HPP
#define ENTITYSIGNALINTERFACE_HPP

#include <QObject>
#include "core/EntityManager.hpp"

namespace talorion {

  class EntitySignalInterface : private QObject
  {
    Q_OBJECT
  public:
    explicit EntitySignalInterface(EntityManager::EntityID en, QObject *par = Q_NULLPTR):QObject(par), m_entity(en){}
    virtual ~EntitySignalInterface() = default;

    EntityManager::EntityID entity() const;

    bool belongsToMe(EntityManager::EntityID en)const;

    bool isValid()const;

  public slots:
    virtual void setpointChanged(EntityManager::EntityID en, double actValue) = 0;
    virtual void actValueChanged(EntityManager::EntityID en, double setValue) = 0;

  private:
    const EntityManager::EntityID m_entity;


  };

} // namespace talorion

#endif // ENTITYSIGNALINTERFACE_HPP

#include "registereddataentity.hpp"
#include "registereddataentityfactory.hpp"
#include "registeredsourceentity.hpp"

namespace talorion {

  RegisteredSourceEntity::RegisteredSourceEntity(EntityManager::EntityID registeredSourceEntity)
    :m_entity(registeredSourceEntity)
  {

  }

  QString RegisteredSourceEntity::name() const
  {
    QString ret;
    auto var = entMng().componentDataForEntity(RegisteredDataEntityFactory::nameComponentId(), entity());
    if(var.canConvert<QString>()){
        ret = var.value<QString>();
      }
    return ret;
  }
  
  QUuid RegisteredSourceEntity::serialVersionUid() const
  {
    QUuid ret;
    auto var = entMng().componentDataForEntity(RegisteredDataEntityFactory::serialVersionUidComponentId(), entity());
    if(var.canConvert<QUuid>()){
        ret = var.value<QUuid>();
      }
    return ret;
  }
  
  EntityManager::EntityList RegisteredSourceEntity::children() const
  {
    return entMng().getChildren(entity());
  }

  EntityManager::EntityID RegisteredSourceEntity::entity() const
  {
    return m_entity;
  }

  void RegisteredSourceEntity::updateChildren(UserDataSource src) const
  {
    int index = 0;
    double analogActValue;
    foreach (auto ent, children()) {
        RegisteredDataEntity data(ent);
        if(data.isValid() == false)
          continue;

        index = src.descriptions().indexOf(data.name());
        if(index < 0)
          continue;

        analogActValue = src.data().at(index);
        data.setAnalogActValue(analogActValue);

      }
  }

  bool RegisteredSourceEntity::isValid() const
  {
    if(EntityManager::isValid(entity()) == false )
      return false;

    if(serialVersionUid() == RegisteredDataEntityFactory::registeredSourceEntityUid())
      return true;

    return false;
  }
  
} // namespace talorion

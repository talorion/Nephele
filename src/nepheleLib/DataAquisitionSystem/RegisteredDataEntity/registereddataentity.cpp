#include "registereddataentity.hpp"
#include "registereddataentityfactory.hpp"

#include <QtGlobal>

namespace talorion {

  RegisteredDataEntity::RegisteredDataEntity(EntityManager::EntityID registeredDataEntity)
    :m_entity(registeredDataEntity)
  {

  }
  
  EntityManager::EntityID RegisteredDataEntity::parent() const
  {
    return entMng().getParent(entity());
  }
  
  QString RegisteredDataEntity::name() const
  {
    QString ret;
    auto var = entMng().componentDataForEntity(RegisteredDataEntityFactory::nameComponentId(), entity());
    if(var.canConvert<QString>()){
        ret = var.value<QString>();
      }
    return ret;
  }
  
  double RegisteredDataEntity::analogActValue() const
  {
    double ret = qQNaN();
    auto var = entMng().componentDataForEntity(RegisteredDataEntityFactory::analogActValueComponentId(), entity());
    if(var.canConvert<double>()){
        ret = var.value<double>();
      }
    return ret;
  }
  
  void RegisteredDataEntity::setAnalogActValue(double anActValue)
  {
    auto oldval = analogActValue();
    if(anActValue != oldval){
        //auto tmp = QVariant::fromValue(analogActValue);
        //entMng().setComponentDataForEntity(RegisteredDataEntityFactory::analogActValueComponentId(), entity(), tmp);
        emit evtMng().change_analogAct_component(entity() ,anActValue);
      }
  }
  
  QUuid RegisteredDataEntity::serialVersionUid() const
  {
    QUuid ret;
    auto var = entMng().componentDataForEntity(RegisteredDataEntityFactory::serialVersionUidComponentId(), entity());
    if(var.canConvert<QUuid>()){
        ret = var.value<QUuid>();
      }
    return ret;
  }
  
  EntityManager::EntityID RegisteredDataEntity::entity() const
  {
    return m_entity;
  }

  bool RegisteredDataEntity::isValid() const
  {
    if(EntityManager::isValid(entity()) == false )
      return false;

    if(serialVersionUid() == RegisteredDataEntityFactory::registeredDataEntityUid())
      return true;

    return false;
  }
  
} // namespace talorion

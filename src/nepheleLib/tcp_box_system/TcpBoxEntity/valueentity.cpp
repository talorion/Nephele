#include "valueentity.hpp"

namespace talorion {

  ValueEntity::ValueEntity(EntityManager::EntityID en)
    :m_entity(en)
  {

  }

  QString ValueEntity::name() const
  {
    QString ret;
    auto var = entMng().componentDataForEntity(NAME_COMPONENT, entity());
    if(var.canConvert<QString>()){
        ret = var.value<QString>();
      }
    return ret;
  }

  QUuid ValueEntity::serialVersionUid() const
  {
    QUuid ret;
    auto var = entMng().componentDataForEntity(SERIAL_VERSION_UID_COMPONENT, entity());
    if(var.canConvert<QUuid>()){
        ret = var.value<QUuid>();
      }
    return ret;
  }

  int ValueEntity::id() const
  {
    int ret = 0;
    auto var = entMng().componentDataForEntity(ID_COMPONENT, entity());
    if(var.canConvert<int>()){
        ret = var.value<int>();
      }
    return ret;
  }

  int ValueEntity::boxId() const
  {
    int ret = 0;
    auto var = entMng().componentDataForEntity(BOX_ID_COMPONENT, entity());
    if(var.canConvert<int>()){
        ret = var.value<int>();
      }
    return ret;
  }


  static_component_id ValueEntity::userData() const
  {
    static_component_id ret = USER_DATA_COMPONENT;
    auto var = entMng().componentDataForEntity(USER_DATA_COMPONENT, entity());
    if(var.canConvert<static_component_id>()){
        ret = var.value<static_component_id>();
      }
    return ret;
  }

  QString ValueEntity::units() const
  {
    QString ret;
    auto var = entMng().componentDataForEntity(UNITS_COMPONENT, entity());
    if(var.canConvert<QString>()){
        ret = var.value<QString>();
      }
    return ret;
  }

  EntityManager::EntityID ValueEntity::parent() const
  {
    return entMng().getParent(entity());
  }

  EntityManager::EntityID ValueEntity::entity() const
  {
    return m_entity;
  }



} // namespace talorion

#include "TcpBoxEntity.hpp"

namespace talorion {

  TcpBoxEntity::TcpBoxEntity(EntityManager::EntityID tcpBoxEntity)
    :m_entity(tcpBoxEntity)
  {

  }

  QString TcpBoxEntity::name() const
  {
    QString ret;
    auto var = entMng().componentDataForEntity(NAME_COMPONENT, entity());
    if(var.canConvert<QString>()){
        ret = var.value<QString>();
      }
    return ret;
  }

  EntityManager::EntityID TcpBoxEntity::boxId() const
  {
    EntityManager::EntityID ret = EntityManager::invalid_id;
    auto var = entMng().componentDataForEntity(BOX_ID_COMPONENT, entity());
    if(var.canConvert<EntityManager::EntityID>()){
        ret = var.value<EntityManager::EntityID>();
      }
    return ret;
  }

  QString TcpBoxEntity::host() const
  {
    QString ret;
    auto var = entMng().componentDataForEntity(IP_ADDRESS_COMPONENT, entity());
    if(var.canConvert<QString>()){
        ret = var.value<QString>();
      }
    return ret;
  }

  quint16 TcpBoxEntity::port() const
  {
    quint16 ret = 0;
    auto var = entMng().componentDataForEntity(PORT_COMPONENT, entity());
    if(var.canConvert<quint16>()){
        ret = var.value<quint16>();
      }
    return ret;
  }

  bool TcpBoxEntity::connectionState() const
  {
    bool ret = false;
    auto var = entMng().componentDataForEntity(CONNECTION_STATE_COMPONENT, entity());
    if(var.canConvert<bool>()){
        ret = var.value<bool>();
      }
    return ret;
  }

  QUuid TcpBoxEntity::serialVersionUid() const
  {
    QUuid ret;
    auto var = entMng().componentDataForEntity(SERIAL_VERSION_UID_COMPONENT, entity());
    if(var.canConvert<QUuid>()){
        ret = var.value<QUuid>();
      }
    return ret;
  }

  int TcpBoxEntity::backendType() const
  {
    int ret = 0;
    auto var = entMng().componentDataForEntity(TCP_BOX_BACKEND_COMPONENT, entity());
    if(var.canConvert<int>()){
        ret = var.value<int>();
      }
    return ret;
  }

  bool TcpBoxEntity::autoReconnect() const
  {
    bool ret = false;
    auto var = entMng().componentDataForEntity(AUTO_RECONNECT_COMPONENT, entity());
    if(var.canConvert<bool>()){
        ret = var.value<bool>();
      }
    return ret;
  }

  bool TcpBoxEntity::isValid() const
  {
    if(EntityManager::isValid(entity()) == false )
      return false;

     if(serialVersionUid() == entMng().get_TcpBox_uid())
       return true;

     return false;
  }

  EntityManager::EntityID TcpBoxEntity::entity() const
  {
    return m_entity;
  }

}

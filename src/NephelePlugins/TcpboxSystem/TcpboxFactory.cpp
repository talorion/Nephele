#include "TcpboxFactory.hpp"

#include "TcpboxSystem.hpp"
#include "TcpboxClient.hpp"

#include "EntityManager/StaticComponentId.hpp"

namespace talorion {

  TcpboxFactory::TcpboxFactory(QObject *par) : QObject(par)
  {

  }

  QUuid TcpboxFactory::getTcpboxUid() const            {
    return ("{99060fb8-676f-47d8-b9f1-c9c492721009}");
  }

  EntityManager::ComponentID TcpboxFactory::nameComponentId()
  {
    return NAME_COMPONENT;
  }

  EntityManager::ComponentID TcpboxFactory::hostNameComponentId()
  {
    return HOST_NAME_COMPONENT;
  }

  EntityManager::ComponentID TcpboxFactory::portComponentId()
  {
    return PORT_COMPONENT;
  }

  EntityManager::ComponentID TcpboxFactory::boxIdComponentId()
  {
    return BOX_ID_COMPONENT;
  }

  EntityManager::ComponentID TcpboxFactory::timeoutComponentId()
  {
    return TIMEOUT_COMPONENT;
  }

  EntityManager::ComponentID TcpboxFactory::connectionStateComponentId()
  {
    return CONNECTION_STATE_COMPONENT;
  }

  EntityManager::ComponentID TcpboxFactory::serialVersionUidComponentId()
  {
    return SERIAL_VERSION_UID_COMPONENT;
  }

  EntityManager::ComponentID TcpboxFactory::commandStateComponentId()
  {
    return COMMAND_STATE_COMPONENT;
  }

  TcpboxFactory::tcpbox_container_t TcpboxFactory::getAllTcpboxes(const TcpboxSystem &sys) const
  {
    EntityManager& mng = sys.entityManager();
    auto uid= QVariant::fromValue(getTcpboxUid());
    //auto boxes = mng.get_entities_by_components_value(serial_version_uid_component_id(), uid);
    TcpboxFactory::tcpbox_container_t boxes;
    auto all_ents= mng.allEntities();
    foreach (auto entity, all_ents) {
        auto ent_uid = mng.componentDataForEntity(serialVersionUidComponentId(),entity).toUuid();
        if(ent_uid == uid){
            boxes<<entity;
          }
      }

    return boxes;
  }

  TcpboxFactory &TcpboxFactory::getInstance()
  {
    // Since it's a static variable, if the class has already been created,
    // It won't be created again.
    // And it **is** thread-safe in C++11.

    static TcpboxFactory myInstance;

    // Return a reference to our instance.
    return myInstance;
  }

  TcpboxFactory::tcpbox_t TcpboxFactory::createNewTcpbox(TcpboxSystem& sys,const QString &box_name, const QString &host_name, quint16 port, qint32 box_id) const
  {
    EntityManager& mng = sys.entityManager();
    tcpbox_t tcpbox=mng.createNewEntity();

    mng.createComponentAndAddTo(nameComponentId(), tcpbox);
    mng.createComponentAndAddTo(hostNameComponentId(), tcpbox);
    mng.createComponentAndAddTo(portComponentId(), tcpbox);
    mng.createComponentAndAddTo(boxIdComponentId(), tcpbox);
    mng.createComponentAndAddTo(timeoutComponentId(), tcpbox);
    mng.createComponentAndAddTo(connectionStateComponentId(), tcpbox);
    mng.createComponentAndAddTo(serialVersionUidComponentId(), tcpbox);
    mng.createComponentAndAddTo(commandStateComponentId(), tcpbox);

    mng.setComponentDataForEntity(nameComponentId(),               tcpbox, box_name);
    mng.setComponentDataForEntity(hostNameComponentId(),          tcpbox, host_name);
    mng.setComponentDataForEntity(portComponentId(),               tcpbox, port);
    mng.setComponentDataForEntity(boxIdComponentId(),             tcpbox, box_id);
    mng.setComponentDataForEntity(timeoutComponentId(),            tcpbox, 5000);
    mng.setComponentDataForEntity(connectionStateComponentId(),   tcpbox, QAbstractSocket::UnconnectedState);
    mng.setComponentDataForEntity(commandStateComponentId(),      tcpbox, COMMAND_STATE_UNKNOWN);
    auto uid= QVariant::fromValue(getTcpboxUid());
    mng.setComponentDataForEntity(serialVersionUidComponentId(), tcpbox, uid);
    sys.addBox(tcpbox);

    return tcpbox;
  }

  TcpboxFactory::tcpbox_command_container_t TcpboxFactory::getAllTcpboxCommands(const TcpboxSystem &sys, TcpboxFactory::tcpbox_t tcpbox) const
  {
    Q_UNUSED(tcpbox);

    EntityManager& mng = sys.entityManager();
    auto uid= QVariant::fromValue(getTcpboxUid());
    //auto boxes = mng.get_entities_by_components_value(serial_version_uid_component_id(), uid);
    TcpboxFactory::tcpbox_container_t boxes;
    auto all_ents= mng.allEntities();
    foreach (auto entity, all_ents) {
        auto ent_uid = mng.componentDataForEntity(serialVersionUidComponentId(),entity).toUuid();
        if(ent_uid == uid){
            boxes<<entity;
          }
      }

    return boxes;
  }

  TcpboxFactory::tcpbox_command_t TcpboxFactory::createNewTcpboxCommand(TcpboxSystem& sys, tcpbox_t tcpbox ) const
  {
    Q_UNUSED(tcpbox);

    EntityManager& mng = sys.entityManager();
    tcpbox_command_t tcpbox_cmd=mng.createNewEntity();

    return tcpbox_cmd;
  }


} // namespace talorion

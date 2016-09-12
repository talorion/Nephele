#include "tcpbox_factory.hpp"

#include "tcpbox_system.hpp"
#include "tcpbox_client.hpp"

#include"entity_manager/static_components.hpp"

namespace talorion {

  tcpbox_factory::tcpbox_factory(QObject *par) : QObject(par)
  {

  }

  QUuid tcpbox_factory::get_TcpBox_uid() const            {
    return ("{99060fb8-676f-47d8-b9f1-c9c492721009}");
  }

  entity_manager::component_id_t tcpbox_factory::name_component_id()
  {
    return NAME_COMPONENT;
  }

  entity_manager::component_id_t tcpbox_factory::host_name_component_id()
  {
    return HOST_NAME_COMPONENT;
  }

  entity_manager::component_id_t tcpbox_factory::port_component_id()
  {
    return PORT_COMPONENT;
  }

  entity_manager::component_id_t tcpbox_factory::box_id_component_id()
  {
    return BOX_ID_COMPONENT;
  }

  entity_manager::component_id_t tcpbox_factory::timeout_component_id()
  {
    return TIMEOUT_COMPONENT;
  }

  entity_manager::component_id_t tcpbox_factory::connection_state_component_id()
  {
    return CONNECTION_STATE_COMPONENT;
  }

  entity_manager::component_id_t tcpbox_factory::serial_version_uid_component_id()
  {
    return SERIAL_VERSION_UID_COMPONENT;
  }

  entity_manager::component_id_t tcpbox_factory::command_state_component_id()
  {
    return COMMAND_STATE_COMPONENT;
  }

  tcpbox_factory::tcpbox_container_t tcpbox_factory::get_all_tcpboxes(const tcpbox_system &sys) const
  {
    entity_manager& mng = sys.get_entity_manager();
    auto uid= QVariant::fromValue(get_TcpBox_uid());
    //auto boxes = mng.get_entities_by_components_value(serial_version_uid_component_id(), uid);
    tcpbox_factory::tcpbox_container_t boxes;
    auto all_ents= mng.get_all_entities();
    foreach (auto entity, all_ents) {
        auto ent_uid = mng.get_component_data_for_entity(serial_version_uid_component_id(),entity).toUuid();
        if(ent_uid == uid){
            boxes<<entity;
          }
      }

    return boxes;
  }

  tcpbox_factory &tcpbox_factory::get_instance()
  {
    // Since it's a static variable, if the class has already been created,
    // It won't be created again.
    // And it **is** thread-safe in C++11.

    static tcpbox_factory myInstance;

    // Return a reference to our instance.
    return myInstance;
  }

  tcpbox_factory::tcpbox_t tcpbox_factory::create_new_tcpbox(tcpbox_system& sys,const QString &box_name, const QString &host_name, quint16 port, qint32 box_id) const
  {
    entity_manager& mng = sys.get_entity_manager();
    tcpbox_t tcpbox=mng.create_new_entity();

    mng.create_component_and_add_to(name_component_id(), tcpbox);
    mng.create_component_and_add_to(host_name_component_id(), tcpbox);
    mng.create_component_and_add_to(port_component_id(), tcpbox);
    mng.create_component_and_add_to(box_id_component_id(), tcpbox);
    mng.create_component_and_add_to(timeout_component_id(), tcpbox);
    mng.create_component_and_add_to(connection_state_component_id(), tcpbox);
    mng.create_component_and_add_to(serial_version_uid_component_id(), tcpbox);
    mng.create_component_and_add_to(command_state_component_id(), tcpbox);


    mng.set_component_data_for_entity(name_component_id(),               tcpbox, box_name);
    mng.set_component_data_for_entity(host_name_component_id(),          tcpbox, host_name);
    mng.set_component_data_for_entity(port_component_id(),               tcpbox, port);
    mng.set_component_data_for_entity(box_id_component_id(),             tcpbox, box_id);
    mng.set_component_data_for_entity(timeout_component_id(),            tcpbox, 5000);
    mng.set_component_data_for_entity(connection_state_component_id(),   tcpbox, QAbstractSocket::UnconnectedState);
    mng.set_component_data_for_entity(command_state_component_id(),      tcpbox, COMMAND_STATE_UNKNOWN);
    auto uid= QVariant::fromValue(get_TcpBox_uid());
    mng.set_component_data_for_entity(serial_version_uid_component_id(), tcpbox, uid);
    sys.add_box(tcpbox);

    return tcpbox;
  }

  tcpbox_factory::tcpbox_command_container_t tcpbox_factory::get_all_tcpbox_commands(const tcpbox_system &sys, tcpbox_factory::tcpbox_t tcpbox) const
  {
    entity_manager& mng = sys.get_entity_manager();
    auto uid= QVariant::fromValue(get_TcpBox_uid());
    //auto boxes = mng.get_entities_by_components_value(serial_version_uid_component_id(), uid);
    tcpbox_factory::tcpbox_container_t boxes;
    auto all_ents= mng.get_all_entities();
    foreach (auto entity, all_ents) {
        auto ent_uid = mng.get_component_data_for_entity(serial_version_uid_component_id(),entity).toUuid();
        if(ent_uid == uid){
            boxes<<entity;
          }
      }

    return boxes;
  }
  }

  tcpbox_factory::tcpbox_command_t tcpbox_factory::create_new_tcpbox_command(tcpbox_system& sys, tcpbox_t tcpbox ) const
  {
    entity_manager& mng = sys.get_entity_manager();
    tcpbox_command_t tcpbox_cmd=mng.create_new_entity();

    return tcpbox_cmd;
  }


} // namespace talorion

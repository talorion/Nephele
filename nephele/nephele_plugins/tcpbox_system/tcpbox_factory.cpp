#include "tcpbox_factory.hpp"

#include "tcpbox_system.hpp"
#include "tcpbox_client.hpp"

#include"entity_manager/static_components.hpp"

namespace talorion {

  tcpbox_factory::tcpbox_factory(QObject *par) : QObject(par)
  {

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

    mng.create_component_and_add_to(NAME_COMPONENT, tcpbox);
    mng.create_component_and_add_to(HOST_NAME_COMPONENT, tcpbox);
    mng.create_component_and_add_to(PORT_COMPONENT, tcpbox);
    mng.create_component_and_add_to(BOX_ID_COMPONENT, tcpbox);
    mng.create_component_and_add_to(TIMEOUT_COMPONENT, tcpbox);
    mng.create_component_and_add_to(CONNECTION_STATE_COMPONENT, tcpbox);

    mng.set_component_data_for_entity(NAME_COMPONENT,               tcpbox, box_name);
    mng.set_component_data_for_entity(HOST_NAME_COMPONENT,          tcpbox, host_name);
    mng.set_component_data_for_entity(PORT_COMPONENT,               tcpbox, port);
    mng.set_component_data_for_entity(BOX_ID_COMPONENT,             tcpbox, box_id);
    mng.set_component_data_for_entity(TIMEOUT_COMPONENT,            tcpbox, 5000);
    mng.set_component_data_for_entity(CONNECTION_STATE_COMPONENT,   tcpbox, QAbstractSocket::UnconnectedState);
    sys.add_box(tcpbox);

    return tcpbox;
  }


} // namespace talorion

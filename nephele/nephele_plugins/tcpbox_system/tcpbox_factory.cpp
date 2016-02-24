#include "tcpbox_factory.hpp"

#include "tcpbox_system.hpp"



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

  entity_manager::entity_id_t tcpbox_factory::create_new_tcpbox(tcpbox_system& sys) const
  {
    entity_manager& mng = sys.get_entity_manager();
    tcpbox_system::tcpbox new_id=mng.create_new_entity();

    //mng.create_component_and_add_to(NAME_COMPONENT, new_id);
//    createComponentAndAddTo(NAME_COMPONENT, new_id );
//    createComponentAndAddTo(BOX_ID_COMPONENT, new_id );
//    createComponentAndAddTo(IP_ADDRESS_COMPONENT, new_id );
//    createComponentAndAddTo(PORT_COMPONENT, new_id );
//    createComponentAndAddTo(CONNECTION_STATE_COMPONENT, new_id );
//    createComponentAndAddTo(SERIAL_VERSION_UID_COMPONENT, new_id);
//    createComponentAndAddTo(TCP_BOX_BACKEND_COMPONENT, new_id);

//    setComponentDataForEntity(NAME_COMPONENT,               new_id, nameVal);
//    setComponentDataForEntity(BOX_ID_COMPONENT,             new_id, new_id);
//    setComponentDataForEntity(IP_ADDRESS_COMPONENT,         new_id, ip);
//    setComponentDataForEntity(PORT_COMPONENT,               new_id, port);
//    setComponentDataForEntity(CONNECTION_STATE_COMPONENT,   new_id, false);
//    setComponentDataForEntity(SERIAL_VERSION_UID_COMPONENT, new_id, get_TcpBox_uid());
//    setComponentDataForEntity(TCP_BOX_BACKEND_COMPONENT, new_id, 0);

    sys.add_box(new_id);

    return new_id;
  }

} // namespace talorion

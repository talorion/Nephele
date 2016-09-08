#include "abstract_system.hpp"

#include "configuration_widget/abstract_configuration_widget.hpp"

#include "entity_manager/entity_manager.hpp"
#include "event_manager/event_manager.hpp"

namespace talorion {

  event_manager abstract_system::s_evt_mng(Q_NULLPTR);
  entity_manager abstract_system::s_ent_mng(Q_NULLPTR);


  abstract_system::abstract_system(QObject *par) :
    QObject(par)
  {
    if(!s_ent_mng.is_connected_to_event_manager())
      s_ent_mng.connect_to_event_manager(&s_evt_mng);

  }


  void abstract_system::initialize(){

    for(auto comp_nme: get_default_components()){
        get_entity_manager().create_new_component(comp_nme);
      }

    do_initialize();
  }

  void abstract_system::dispose()
  {
    do_dispose();
  }

  void abstract_system::start()
  {
    do_start();
  }

  entity_manager &abstract_system::get_entity_manager() const
  {
    return s_ent_mng;
  }

  event_manager &abstract_system::get_event_manager() const
  {
    return s_evt_mng;
  }

  QStringList abstract_system::get_default_components() const
  {
    QStringList tmp{""};

    tmp.append(do_get_default_components());

    return tmp;
  }

}

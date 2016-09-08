#include "event_manager.hpp"
#include <QMetaType>

namespace talorion {

  event_manager::event_manager(QObject *par):QObject(par){
    qRegisterMetaType<entity_manager::entity_id_t>("entity_manager::entity_id_t");
    qRegisterMetaType<entity_manager::component_id_t>("entity_manager::component_id_t");
    qRegisterMetaType<entity_manager::component_data_t>("entity_manager::component_data_t");
  }

}



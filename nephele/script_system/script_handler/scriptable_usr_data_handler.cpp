#include "scriptable_usr_data_handler.hpp"


#include <QScriptValue>
#include <QDebug>

#include "core/event_manager.hpp"
#include "core/entity_manager.hpp"

namespace talorion {

  scriptable_usr_data_handler::scriptable_usr_data_handler(QObject *par) : abstract_scriptable_object("scriptable_usr_data",par)
  {
    connect(this,SIGNAL(set_value_changed(int,double)),event_manager::get_instance(),SIGNAL(change_analogAct_component(int,double)),Qt::UniqueConnection);
    connect(event_manager::get_instance(),SIGNAL(analogAct_component_changed(int)),this,SLOT(slot_set_value_changed(int)));

  }

  void scriptable_usr_data_handler::set_usr_val(QString name, double value)
  {
    QList<int> entities=entity_manager::get_instance()->get_entity_by_name(name);
    if(!entities.isEmpty()){
        int entity = entities.at(0);
        emit set_value_changed(entity, value);
      }
  }

  void scriptable_usr_data_handler::slot_set_value_changed(int entity)
  {
    QString nme = entity_manager::get_instance()->get_name_component(entity);
    double valu = entity_manager::get_instance()->get_analogSetValue_component(entity);
    if(getScrip_value() != NULL)
        getScrip_value()->setProperty(nme, valu);
  }

  void scriptable_usr_data_handler::create_usr_val(QString name)
  {
    QList<int> entities=entity_manager::get_instance()->get_entity_by_name(name);
    int entity =-1;
    if(entities.isEmpty()){
        entity =entity_manager::get_instance()->create_scriptable_usr_data(name);
      }else{
        entity = entities.at(0);
        qDebug()<<"cannot create another usr val with name "+name;
      }

  }

} // namespace talorion

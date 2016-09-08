#include "entity_manager.hpp"

#include "event_manager/event_manager.hpp"

#include "entity_manager_db.hpp"

#include <QUuid>



namespace talorion {

  const entity_manager::component_data_t entity_manager::invalid_data= QVariant();//Constructs an invalid variant.
  bool entity_manager::is_valid(const entity_manager::component_data_t &component_data){return component_data.isValid();}

  entity_manager::entity_manager(QObject *par) :
    QObject(par),
    db_ptr(new entity_manager_db()),
    m_evt_mng()
  {
    m_evt_mng.clear();

  }

  entity_manager::~entity_manager(){

  }

  QList<entity_manager::entity_id_t> entity_manager::get_all_entities() const
  {
    return db_ptr->entities().keys();
  }

  QList<entity_manager::component_id_t> entity_manager::get_all_components() const
  {
    return db_ptr->components().keys();
  }

  entity_manager::entity_id_t entity_manager::create_new_entity()
  {
    entity_id_t new_id = get_next_entity_id();
    db_ptr->add_entity(new_id, QString());
    return new_id;
  }

  bool entity_manager::entity_exists(entity_id_t entity_id) const{
    return db_ptr->entities().contains(entity_id);
  }


  entity_manager::component_id_t entity_manager::create_new_component(component_id_t component_id, QString component_name)
  {
    if(!is_valid(component_id)){
        component_id = get_next_component_id();
      }

    if(!component_name.isNull() && component_exists(component_name)){
        component_id= get_component_by_name(component_name);
      }

    if(!component_exists(component_id)){
        db_ptr->add_component(component_id, component_name);
      }

    return component_id;
  }

  entity_manager::component_id_t entity_manager::create_new_component(const QString &component_name)
  {
    return create_new_component(invalid_id, component_name);
  }

  entity_manager::component_id_t entity_manager::create_component_and_add_to(component_id_t component_id, entity_manager::entity_id_t entity_id)
  {
    component_id =create_new_component(component_id);  //make sure we have a valid component
    component_data_id_t component_data_id = get_component_data_id(entity_id,component_id);

    db_ptr->add_component_to_entity(entity_id, component_id, component_data_id, invalid_data);
    return component_id;
  }

  entity_manager::component_id_t entity_manager::get_component_by_name(const QString &component_name) const
  {
    auto component_id =invalid_id;

    for(auto it=db_ptr->components().cbegin(); it!=db_ptr->components().cend();++it){
        if(QString::compare(it.value(),component_name, Qt::CaseInsensitive)==0){
            component_id = it.key();
            break;
          }
      }

    return component_id;
  }

  bool entity_manager::component_exists(entity_manager::component_id_t component_id) const
  {
    return db_ptr->components().contains(component_id);
  }

  bool entity_manager::component_exists(QString component_name) const
  {
    auto component_id = get_component_by_name(component_name);
    return component_exists(component_id);
  }

  bool entity_manager::entity_has_component(entity_manager::entity_id_t entity_id, entity_manager::component_id_t component_id) const
  {
    component_data_id_t component_data_id = get_component_data_id(entity_id,component_id);
    return db_ptr->component_data_table().contains(component_data_id);
  }

  void entity_manager::remove_entity(entity_manager::entity_id_t entity_id)
  {
    if(entity_exists(entity_id)){
        remove_all_components_from_entity(entity_id);
        db_ptr->delete_entity(entity_id);
      }
  }

  void entity_manager::remove_all_components_from_entity(entity_manager::entity_id_t entity_id)
  {
    QList<component_id_t> component_ids = db_ptr->entity_components().values(entity_id);
    //for(auto component_id : component_ids){
    foreach (auto component_id, component_ids) {
        remove_component_from_entity(component_id, entity_id);
      }
  }

  void entity_manager::remove_component_from_entity(entity_manager::component_id_t component_id, entity_manager::entity_id_t entity_id)
  {
    component_data_id_t component_data_id = get_component_data_id(entity_id,component_id);
    db_ptr->delete_component_from_entity(entity_id, component_data_id);
  }

  QList<entity_manager::component_id_t> entity_manager::get_all_components_of_entity(entity_manager::entity_id_t entity_id) const
  {
    return db_ptr->entity_components().values(entity_id);
  }

  void entity_manager::set_component_data_for_entity(entity_manager::component_id_t component_id, entity_manager::entity_id_t entity_id, const entity_manager::component_data_t &component_data)
  {
    component_data_id_t component_data_id = get_component_data_id(entity_id,component_id);
    db_ptr->set_data(component_data_id, component_data);
  }

  bool entity_manager::is_connected_to_event_manager() const
  {
    return !(m_evt_mng.isNull());
  }

  void entity_manager::connect_to_event_manager(event_manager *evt_mng)
  {
    //    if(evt_mng==Q_NULLPTR){
    //      m_evt_mng.clear();
    //      return;
    //      }

    if(is_connected_to_event_manager())
      return;

    m_evt_mng = evt_mng;

    connect(m_evt_mng.data(),SIGNAL(change_component_data_for_entity(entity_manager::component_id_t,entity_manager::entity_id_t,entity_manager::component_data_t)),this,SLOT(slot_change_component_data_for_entity(entity_manager::component_id_t,entity_manager::entity_id_t,entity_manager::component_data_t)));

  }

  event_manager *entity_manager::get_event_manager() const
  {
    if(!is_connected_to_event_manager())
      return Q_NULLPTR;

    return m_evt_mng.data();
  }

  QList<entity_manager::entity_id_t> entity_manager::get_entities_by_components_value(entity_manager::component_id_t component_id,  entity_manager::component_data_t component_data) const
  {
    QList<entity_manager::entity_id_t> ents;
    auto tmp_cont=get_all_entities();
    foreach (entity_id_t id, tmp_cont) {
        if(entity_has_component(id, component_id)){
            auto data = get_component_data_for_entity(id,component_id);
            if(data == component_data){
                ents<<id;
              }
          }
      }

    return ents;
  }

  QVariant entity_manager::get_component_data_for_entity(entity_manager::component_id_t component_id, entity_manager::entity_id_t entity_id) const
  {
    component_data_id_t component_data_id = get_component_data_id(entity_id,component_id);
    auto data = db_ptr->component_data_table().value(component_data_id, invalid_data);
    return data;
  }

  entity_manager::id_t entity_manager::get_next_id_from(const QList<id_t> &container) const
  {
    id_t next_id = invalid_id;
    //for(auto e : container)     {next_id = qMax(next_id, e);}
    foreach (auto e, container){next_id = qMax(next_id, e);}
    next_id++;
    return next_id;
  }

  entity_manager::entity_id_t entity_manager::get_next_entity_id() const
  {
    id_t next_id = get_next_id_from(db_ptr->entities().keys());
    if(next_id>max_entity_id)
      next_id = invalid_id;
    return next_id;
  }

  entity_manager::component_id_t entity_manager::get_next_component_id() const
  {
    id_t next_id = get_next_id_from(db_ptr->components().keys());
    if(next_id>max_component_id)
      next_id = invalid_id;
    return next_id;
  }

  void entity_manager::slot_change_component_data_for_entity(component_id_t component_id, entity_id_t entity_id, const component_data_t &component_data)
  {
    set_component_data_for_entity(component_id, entity_id, component_data);
  }

} // namespace talorion


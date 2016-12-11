#include "EntityManagerDb.hpp"


namespace talorion {
  EntityManagerDb::EntityManagerDb(QObject *par) :
    QObject(par),
    m_entities(),
    m_components(),
    m_entity_components(),
    m_component_data_table(),
    m_component_doublevectordata_table(),
    m_component_entitylistdata_table()
  {

  }

  void EntityManagerDb::addEntity(EntityManager::EntityID entity_id, QString human_readable_label)
  {
    m_entities.insert(entity_id, human_readable_label);
  }

  void EntityManagerDb::addComponent(EntityManager::ComponentID component_id, QString official_name)
  {
    m_components.insert(component_id, official_name);
  }

  void EntityManagerDb::addComponentToEntity(EntityManager::EntityID entity_id, EntityManager::ComponentID component_id, EntityManager::ComponentDataID component_data_id, const EntityManager::ComponentData &component_data)
  {
    m_entity_components.insert(entity_id, component_id);
    setData(component_data_id, component_data);
    //m_component_data_table.insert(component_data_id, component_data);
  }

  void EntityManagerDb::deleteEntity(EntityManager::EntityID entity_id)
  {
    m_entities.remove(entity_id);
  }

  void EntityManagerDb::deleteComponent(EntityManager::ComponentID component_id)
  {
    m_components.remove(component_id);
  }

  void EntityManagerDb::deleteComponentFromEntity(EntityManager::EntityID entity_id, EntityManager::ComponentDataID component_data_id)
  {
    m_entity_components.remove(entity_id);
    m_component_data_table.remove(component_data_id);
  }

  void EntityManagerDb::setData(EntityManager::ComponentDataID component_data_id, const EntityManager::ComponentData &component_data)
  {
    m_component_data_table.insert(component_data_id, component_data);
  }

  void EntityManagerDb::setData(EntityManager::ComponentDataID component_data_id, QVector<double> *component_data)
  {
    if(component_data != Q_NULLPTR)
      m_component_doublevectordata_table.insert(component_data_id, component_data);
  }

  void EntityManagerDb::setData(EntityManager::ComponentDataID component_data_id, QList<EntityManager::EntityID> *component_data)
  {
    if(component_data != Q_NULLPTR)
      m_component_entitylistdata_table.insert(component_data_id, component_data);
  }
}

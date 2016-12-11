#ifndef ENTITY_MANAGER_DB_HPP
#define ENTITY_MANAGER_DB_HPP

#include <QObject>
#include <QMap>
#include <QMultiMap>
#include <QVector>


#include "EntityManager.hpp"

namespace talorion {


class EntityManagerDb : public QObject
{
    Q_OBJECT
public:
    explicit EntityManagerDb(QObject *par = Q_NULLPTR);
    virtual ~EntityManagerDb()=default;

public:
    const QMap<EntityManager::EntityID, QString>& entities()const{return m_entities;}
    const QMap<EntityManager::ComponentID, QString>& components() const{return m_components;}
    const QMultiMap<EntityManager::EntityID, EntityManager::ComponentID>& entityComponents() const{return m_entity_components;}
    const QMap<EntityManager::ComponentDataID, EntityManager::ComponentData>& componentDataTable() const{return m_component_data_table;}
    const QMap<EntityManager::ComponentDataID,  QVector<double>* >& componentDoubleVectorDataTable() const{return m_component_doublevectordata_table;}
    const QMap<EntityManager::ComponentDataID,  QList<EntityManager::EntityID>* >& componentEntityListDataTable() const{return m_component_entitylistdata_table;}

    void addEntity(EntityManager::EntityID entity_id,QString human_readable_label);
    void addComponent(EntityManager::ComponentID component_id, QString official_name);
    void addComponentToEntity(EntityManager::EntityID entity_id, EntityManager::ComponentID component_id,  EntityManager::ComponentDataID component_data_id, const EntityManager::ComponentData& component_data);

    void deleteEntity(EntityManager::EntityID entity_id);
    void deleteComponent(EntityManager::ComponentID component_id);
    void deleteComponentFromEntity(EntityManager::EntityID entity_id, EntityManager::ComponentDataID component_data_id);

    void setData(EntityManager::ComponentDataID component_data_id,  const EntityManager::ComponentData& component_data);
    void setData(EntityManager::ComponentDataID component_data_id,  QVector<double>* component_data);
    void setData(EntityManager::ComponentDataID component_data_id,  QList<EntityManager::EntityID>* component_data);

private:

    QMap<EntityManager::EntityID, QString> m_entities;                                //entity_id     | human-readable label
    QMap<EntityManager::ComponentID, QString> m_components;                           //component_id  | official name          ///| human-readable description | table-name

    QMultiMap<EntityManager::EntityID, EntityManager::ComponentID> m_entity_components;      //entity_id 	| component_id |  component_data_id
    QMap<EntityManager::ComponentDataID, EntityManager::ComponentData> m_component_data_table;           //component_data_id                  /// | [1..M columns, one column for each piece of data in your component]

    QMap<EntityManager::ComponentDataID,  QVector<double>* > m_component_doublevectordata_table;           //component_data_id                  /// | [1..M columns, one column for each piece of data in your component]
    QMap<EntityManager::ComponentDataID,  QList<EntityManager::EntityID>* > m_component_entitylistdata_table;           //component_data_id                  /// | [1..M columns, one column for each piece of data in your component]

};
}
#endif // ENTITY_MANAGER_DB_HPP

#ifndef ENTITY_MANAGER_DB_HPP
#define ENTITY_MANAGER_DB_HPP

#include <QObject>
#include <QMap>
#include <QMultiMap>


#include "entity_manager.hpp"

namespace talorion {
    class entity_manager_db : public QObject
    {
        Q_OBJECT
    public:
        explicit entity_manager_db(QObject *par = 0);
        ~entity_manager_db();
        Q_DISABLE_COPY(entity_manager_db)

        const QMap<entity_manager::entity_id_t, QString>& entities()const{return m_entities;}
        const QMap<entity_manager::component_id_t, QString>& components() const{return m_components;}
        const QMultiMap<entity_manager::entity_id_t, entity_manager::component_id_t>& entity_components() const{return m_entity_components;}
        const QMap<entity_manager::component_data_id_t, entity_manager::component_data_t>& component_data_table() const{return m_component_data_table;}

        void add_entity(entity_manager::entity_id_t entity_id,QString human_readable_label);
        void add_component(entity_manager::component_id_t component_id, QString official_name);
        void add_component_to_entity(entity_manager::entity_id_t entity_id, entity_manager::component_id_t component_id,  entity_manager::component_data_id_t component_data_id, const entity_manager::component_data_t& component_data);

        void delete_entity(entity_manager::entity_id_t entity_id);
        void delete_component(entity_manager::component_id_t component_id);
        void delete_component_from_entity(entity_manager::entity_id_t entity_id, entity_manager::component_data_id_t component_data_id);

        void set_data(entity_manager::component_data_id_t component_data_id,  const entity_manager::component_data_t& component_data);

    private:

        QMap<entity_manager::entity_id_t, QString> m_entities;                                //entity_id     | human-readable label
        QMap<entity_manager::component_id_t, QString> m_components;                           //component_id  | official name          ///| human-readable description | table-name

        QMultiMap<entity_manager::entity_id_t, entity_manager::component_id_t> m_entity_components;      //entity_id 	| component_id |  component_data_id
        QMap<entity_manager::component_data_id_t, entity_manager::component_data_t> m_component_data_table;           //component_data_id                  /// | [1..M columns, one column for each piece of data in your component]

    };
}
#endif // ENTITY_MANAGER_DB_HPP

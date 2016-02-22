#include "entity_manager_db.hpp"


namespace talorion {
    entity_manager_db::entity_manager_db(QObject *par) :
        QObject(par),
        m_entities(),
        m_components(),
        m_entity_components(),
        m_component_data_table()
    {

    }

    void entity_manager_db::add_entity(entity_manager::entity_id_t entity_id, QString human_readable_label)
    {
        m_entities.insert(entity_id, human_readable_label);
    }

    void entity_manager_db::add_component(entity_manager::component_id_t component_id, QString official_name)
    {
        m_components.insert(component_id, official_name);
    }

    void entity_manager_db::add_component_to_entity(entity_manager::entity_id_t entity_id, entity_manager::component_id_t component_id, entity_manager::component_data_id_t component_data_id, const entity_manager::component_data_t &component_data)
    {
        m_entity_components.insert(entity_id, component_id);
        set_data(component_data_id, component_data);
        //m_component_data_table.insert(component_data_id, component_data);
    }

    void entity_manager_db::delete_entity(entity_manager::entity_id_t entity_id)
    {
        m_entities.remove(entity_id);
    }

    void entity_manager_db::delete_component(entity_manager::component_id_t component_id)
    {
        m_components.remove(component_id);
    }

    void entity_manager_db::delete_component_from_entity(entity_manager::entity_id_t entity_id, entity_manager::component_data_id_t component_data_id)
    {
        m_entity_components.remove(entity_id);
        m_component_data_table.remove(component_data_id);
    }

    void entity_manager_db::set_data(entity_manager::component_data_id_t component_data_id, const entity_manager::component_data_t &component_data)
    {
        m_component_data_table.insert(component_data_id, component_data);
    }
}

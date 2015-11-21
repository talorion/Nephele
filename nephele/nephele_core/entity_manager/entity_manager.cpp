#include "entity_manager.hpp"

namespace talorion {

    entity_manager::entity_manager(QObject *par) :
        QObject(par),
        entities(),
        components()
    {
    }

    entity_manager::~entity_manager()
    {
    }

    QList<entity_manager::entity_id_t> entity_manager::get_all_entities() const
    {
        return entities.keys();
    }

    QList<entity_manager::component_id_t> entity_manager::get_all_components() const
    {
        return components.keys();
    }

    entity_manager::entity_id_t entity_manager::create_new_entity()
    {
return 1;
    }



} // namespace talorion


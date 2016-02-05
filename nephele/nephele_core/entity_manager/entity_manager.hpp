#ifndef TALORION_ENTITY_MANAGER_HPP
#define TALORION_ENTITY_MANAGER_HPP

#include <QObject>
#include <QScopedPointer>
#include <QVariant>

//QT_BEGIN_NAMESPACE
//class QVariant;
//QT_END_NAMESPACE

namespace talorion {

    class entity_manager_db;

    class entity_manager : public QObject
    {
        Q_OBJECT
    public:     //typedefs
        typedef int id_t;
        typedef id_t entity_id_t;
        typedef id_t component_id_t;
        typedef id_t component_data_id_t;

        typedef QVariant component_data_t;

    public:     //constants
        //static const id_t invalid_id{0};
        static const id_t invalid_id=0;
        //static const entity_id_t default_entity{0};
        //static const entity_id_t max_entity_id{997};    //for component_data_id hash should ba a prime
        static const entity_id_t max_entity_id=997;    //for component_data_id hash should ba a prime
        //static const entity_id_t max_component_id{1009}; //for component_data_id hash should ba a prime
        static const entity_id_t max_component_id=1009; //for component_data_id hash should ba a prime

        static const component_data_t invalid_data;

    public:     //convenient fnctions
        static bool is_valid(id_t id){return id > invalid_id;}
        static component_data_id_t get_component_data_id(entity_id_t entity_id, component_id_t component_id){return (component_id*max_entity_id + entity_id)*max_component_id;}

        static bool is_valid(const component_data_t& component_data);

    public:
        explicit entity_manager(QObject *par = 0);
        ~entity_manager();
        Q_DISABLE_COPY(entity_manager)

        QList<entity_id_t> get_all_entities()const;
        QList<component_id_t> get_all_components() const;

        entity_id_t create_new_entity();
        bool entity_exists(entity_id_t entity_id) const;

        component_id_t create_new_component(component_id_t component_id=invalid_id);
        component_id_t create_component_and_add_to(component_id_t component_id, entity_id_t entity_id);
        bool component_exists(component_id_t component_id) const;

        bool entity_has_component(entity_id_t entity_id, component_id_t component_id) const;

        void remove_entity(entity_id_t entity_id);

        void remove_all_components_from_entity(entity_id_t entity_id);
        void remove_component_from_entity(component_id_t component_id, entity_id_t entity_id);

        QList<component_id_t> get_all_components_of_entity(entity_id_t entity_id)const;

        component_data_t get_component_data_for_entity(component_id_t component_id, entity_id_t entity_id) const;
        void set_component_data_for_entity(component_id_t component_id, entity_id_t entity_id,  const component_data_t &component_data);

    private:
        id_t get_next_id_from(const QList<id_t>& container)const;
        entity_id_t get_next_entity_id()const;
        component_id_t get_next_component_id()const;

    private:
        QScopedPointer<entity_manager_db> const db_ptr;
    };

} // namespace talorion

#endif // TALORION_ENTITY_MANAGER_HPP

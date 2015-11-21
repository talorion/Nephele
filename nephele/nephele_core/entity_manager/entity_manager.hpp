#ifndef TALORION_ENTITY_MANAGER_HPP
#define TALORION_ENTITY_MANAGER_HPP

#include <QMap>
#include <QObject>

namespace talorion {

    class entity_manager : public QObject
    {
        Q_OBJECT
    public:     //typedefs
        typedef int entity_id_t;
        typedef int component_id_t;
        typedef int component_data_id_t;

    public:     //constants
        static const int invalid_id{0};
        //static const int default_entity{invalid_id};

    public:     //convenient fnctions
        static bool is_valid(int id){return id>invalid_id;}

    public:
        explicit entity_manager(QObject *par = 0);
        ~entity_manager();
        Q_DISABLE_COPY(entity_manager)

        QList<entity_id_t> get_all_entities()const;
        QList<component_id_t> get_all_components() const;

        entity_id_t create_new_entity();

    signals:

    public slots:

    private:

        QMap<entity_id_t, QString> entities;                                //entity_id     | human-readable label
        QMap<component_id_t, QString> components;                           //component_id  | official name          ///| human-readable description | table-name

        // QMultiMap<entity_id_t, component_data_id_t> entity_components;      //entity_id 	| component_id |  component_data_id
        //QMap<component_data_id_t, QVariant> component_data_table;           //component_data_id                  /// | [1..M columns, one column for each piece of data in your component]

    };

} // namespace talorion

#endif // TALORION_ENTITY_MANAGER_HPP

#ifndef TALORION_ENTITY_MANAGER_HPP
#define TALORION_ENTITY_MANAGER_HPP

#include <QObject>

#include <QMap>

QT_BEGIN_NAMESPACE
class QMutex;
class QSignalMapper;
class QVariant;
QT_END_NAMESPACE



namespace talorion {

    class analogValue;

    typedef struct comonent_t_{
        comonent_t_():component_id(0),official_name(), human_readable_description(), table_name(){}
        int component_id;
        QString official_name;
        QString human_readable_description;
        QString table_name;
    }comonent_t;

    typedef struct entity_t_{
        entity_t_():entity_id(0), human_readable_label(){}
        int entity_id;
        QString human_readable_label;
    }entity_t;

    typedef struct entity_components_t_{
        entity_components_t_():entity_id(0), component_id(0), component_data_id(0){}
        int entity_id;
        int component_id;
        int component_data_id;
    }entity_components_t;

//    typedef struct component_data_t_{
//        int component_data_id;
//        QVariantMap component_data;
//    }component_data_t;

    typedef enum component_id { IP_ADDRESS_COMPONENT, PORT_COMPONENT, BOX_ID_COMPONENT, NAME_COMPONENT} component_id;

    class entity_manager : public QObject
    {
        Q_OBJECT

    private:
        explicit entity_manager(QObject *parent = 0);
        ~entity_manager();
        Q_DISABLE_COPY(entity_manager)

    public:
        static entity_manager *get_instance();
        static void destroy();

        int createNewEntity(QString human_readable_label=QString());

        void createComponentAndAddTo(component_id comp_id, int entity_id);

        //        QVariantMap getComponentDataForEntity(int component_id, int entity_id);

        //        void setComponentDataForEntity(int component_id, int entity_id,  QVariantMap component_data);

        //analogValue* createNewAnalogValue(QString nameVal, QString unitsVal, double smin, double smax, double amin, double amax, double setVal, int id, int box_id ) ;
        int createNewAnalogValue(QString nameVal, QString unitsVal, double smin, double smax, double amin, double amax, double setVal, int id, int box_id ) ;

        double get_actValue_component(int entity)const;
        double get_setValue_component(int entity)const;
        double get_setMin_component(int entity)const;
        double get_setMax_component(int entity)const;
        double get_actMin_component(int entity)const;
        double get_actMax_component(int entity)const;
        int get_id_component(int entity)const;
        QString get_name_component(int entity)const;
        QString get_units_component(int entity)const;

        int get_entity_by_name(const QString& name) const;

        //int create_new_fc_box_connection(QString nameVal, int box_id, QString ip_address, quint16 port);
        //QString get_ip_address_component(int entity)const;
        //quint16 get_port_component(int entity)const;
        //quint16 get_box_id_component(int entity)const;

    signals:
        void set_value_changed(int entity );
        void act_value_changed(int entity);

        void newAnalogValue(int);

    private:
        analogValue* get_analogValue(int entity)const;

        void set_actValue_component(int entity, double val);
        void set_setValue_component(int entity, double val);
        void set_setMin_component(int entity, double val);
        void set_setMax_component(int entity, double val);
        void set_actMin_component(int entity, double val);
        void set_actMax_component(int entity, double val);
        void set_name_component(int entity, QString val);
        void set_units_component(int entity, QString val);

    private slots:
        void slot_change_act_value(int entity, double value);
        void slot_change_set_value(int entity, double value);

    private:
        QMap<int, analogValue*> analog_values;
        QSignalMapper *act_value_signalMapper;
        QSignalMapper *set_value_signalMapper;
        int current_identity_id;

        QMap<int, comonent_t> components;                  //component_id | official name | human-readable description | table-name
        QMap<int, entity_t> entities;                    //entity_id | human-readable label FOR DEBUGGING ONLY
        QMap<int, entity_components_t> entity_components;               //entity_id | component_id | component_data_id
        //QMap<int, QVariant> component_data_table_N;     //component_data_id | [1..M columns, one column for each piece of data in your component]


    private:
        static QAtomicPointer<entity_manager> _instance;
        static QMutex _mutex;

        static const int P1=997;
        static const int P2=1009;
    };

} // namespace talorion

#endif // TALORION_ENTITY_MANAGER_HPP

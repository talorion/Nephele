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

    class abstract_configuration_widget;

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

    typedef enum static_component_id {
        IP_ADDRESS_COMPONENT,
        PORT_COMPONENT,
        NAME_COMPONENT,
        UNITS_COMPONENT,
        SET_MIN_COMPONENT,
        SET_MAX_COMPONENT,
        ACT_MIN_COMPONENT,
        ACT_MAX_COMPONENT,
        ANALOG_SET_VALUE_COMPONENT,
        ANALOG_ACT_VALUE_COMPONENT,
        ID_COMPONENT,
        BOX_ID_COMPONENT,
        DIGITAL_SET_VALUE_COMPONENT,
        DIGITAL_ACT_VALUE_COMPONENT,
        SYSTEM_CONFIGURAION_WIDGET_COMPONENT,
        IS_SYSTEM_COMPONENT,
        CONNECTION_STATE_COMPONENT
    } static_component_id;

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

        //=== DB Functions
        int createNewEntity(QString human_readable_label=QString());
        void delete_entity(int entity_id);
        void createComponentAndAddTo(static_component_id comp_id, int entity_id);
        QVariant getComponentDataForEntity(int component_id, int entity_id) const;
        void setComponentDataForEntity(int component_id, int entity_id,  const QVariant &component_data);

        //===

        //=== Factory methods
        int createNewSystem(QString nameVal, abstract_configuration_widget* sys_cfg_wdg);
        int createNewAnalogValue(QString nameVal, QString unitsVal, double smin, double smax, double amin, double amax, double setVal, int id, int box_id ) ;
        int createNewDigitalValue(QString nameVal, bool setVal, int id, int box_id ) ;
        int createNewTcpBox(QString nameVal="New Box", QString ip="localhost", quint16 port=2701);
        //===

        abstract_configuration_widget* get_systemConfigurationWidget_component(int entity) const;


        double get_analogActValue_component(int entity)const;
        double get_analogSetValue_component(int entity)const;
        bool get_digitalActValue_component(int entity)const;
        bool get_digitalSetValue_component(int entity)const;
        double get_setMin_component(int entity)const;
        double get_setMax_component(int entity)const;
        double get_actMin_component(int entity)const;
        double get_actMax_component(int entity)const;
        int get_id_component(int entity)const;
        QString get_name_component(int entity)const;
        QString get_units_component(int entity)const;
        QString get_ip_address_component(int entity)const;
        quint16 get_port_component(int entity)const;
        int get_box_id_component(int entity)const;
        bool get_connection_state_component(int entity)const;


        //QList<int> get_all_systems()const;
        int get_entity_by_name(const QString& name) const;
        QList<int> get_all_components_of_entity(int entity)const;

    public slots:
        void slot_change_name_component(int entity, QString value);
        void slot_change_ip_address_component(int entity, QString value);
        void slot_change_port_component(int entity, quint16 value);
        void slot_connection_state_component(int entity, quint16 value);

    signals:
        void newSystem(int entity);

        void component_changed(int entity, int component);

        void connection_state_component_changed(int entity);

        void analogSet_component_changed(int entity);
        void analogAct_component_changed(int entity);

        void digitalSet_component_changed(int entity);
        void digitalAct_component_changed(int entity);

        void newAnalogValue(int);
        void newDigitalValue(int);
        void newTcpBox(int);

    private:
        void set_analogActValue_component(int entity, double val);
        void set_analogSetValue_component(int entity, double val);
        void set_digitalActValue_component(int entity, bool val);
        void set_digitalSetValue_component(int entity, bool val);
        void set_setMin_component(int entity, double val);
        void set_setMax_component(int entity, double val);
        void set_actMin_component(int entity, double val);
        void set_actMax_component(int entity, double val);
        void set_name_component(int entity, QString val);
        void set_units_component(int entity, QString val);
        void set_id_component(int entity, int val);
        void set_ip_address_component(int entity, QString val);
        void set_port_component(int entity, quint16 val);
        void set_box_id_component(int entity, int val);
        void set_connection_state_component(int entity, bool val);
        void set_systemConfigurationWidget_component(int entity_id, abstract_configuration_widget *wdgt);

        int calc_enity_component_hash(int entity_id, int comp_id)const{return (comp_id*P1 + entity_id)*P2;}

    private slots:
        void slot_change_analogAct_component(int entity, double value);
        void slot_change_analogSet_component(int entity, double value);

        void slot_change_digitalAct_component(int entity, bool value);
        void slot_change_digitalSet_component(int entity, bool value);

    private:

        int current_identity_id;

        QMap<int, comonent_t> components;                       //component_id | official name | human-readable description | table-name
        QMap<int, entity_t> entities;                           //entity_id | human-readable label FOR DEBUGGING ONLY
        QMap<int, entity_components_t> entity_components;       //enity_component_hash | entity_id | component_id
        QMap<int, QVariant> component_data_table_N;             //enity_component_hash | [1..M columns, one column for each piece of data in your component]
        QMap<int, abstract_configuration_widget*> component_widget_table;

    private:
        //static QAtomicPointer<entity_manager> _instance;
        static entity_manager* _instance;
        static QMutex _mutex;

        static const int P1=997;
        static const int P2=1009;
    };

} // namespace talorion

#endif // TALORION_ENTITY_MANAGER_HPP

#ifndef TALORION_ENTITY_MANAGER_HPP
#define TALORION_ENTITY_MANAGER_HPP

#include "abstract_entity_manager.hpp"

#include <QObject>
#include <QMap>
#include <QUuid>

QT_BEGIN_NAMESPACE
class QMutex;
class QSignalMapper;
//class QVariant;
//class QScriptEngine;
QT_END_NAMESPACE


namespace talorion {

    //class abstract_configuration_widget;
    //class abstract_scriptable_object;

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
        FIRST_STATIC_COMPONENT =1,

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
        CONNECTION_STATE_COMPONENT,
        SERIAL_VERSION_UID_COMPONENT,
        TCP_BOX_BACKEND_COMPONENT,
        SYSTEM_VERSION_UID_COMPONENT,
        QT_SCRIPT_ENGINE_COMPONENT,
        SCRIPT_FILE_COMPONENT,
        DATA_AQUISITION_DLL_COMPONENT,
        TIMEOUT_COMPONENT,
        SCRIPTABLE_OBJECT_COMPONENT,
        FILEVERSION_COMPONENT,
        PRODUCTVERSION_COMPONENT,
        COMPANYNAME_COMPONENT,
        UPDATERATE_COMPONENT,
        USER_DATA_COMPONENT,
        USER_DATA_PATH_COMPONENT,

        NUM_OF_STATIC_COMPONENTS
    } static_component_id;

    class entity_manager : public abstract_entity_manager
    {
        Q_OBJECT

    public:
        explicit entity_manager(QObject *parent = 0);
        ~entity_manager();
        Q_DISABLE_COPY(entity_manager)

    public:
        static const int invalid_id{0};
        static const int default_entity{invalid_id};
        static const int reserverd_id_max{254};
        static const int id_max{std::numeric_limits<int>::max()};

    public:
        static bool is_valid(int id){return id>invalid_id;}
        static bool is_invalid(int id){return !is_valid(id);}
        static bool is_reserved(int id){return  is_valid(id) && id<=reserverd_id_max;}
        static bool is_non_reserved(int id){return !is_reserved(id);}

    public:
//        static entity_manager *get_instance();
//        static void destroy();

        virtual void initialize() Q_DECL_OVERRIDE;
        virtual void dispose() Q_DECL_OVERRIDE;


        //=== DB Functions
        virtual int createNewEntity(QString human_readable_label=QString(), int entity=invalid_id, bool isSystem = false) Q_DECL_OVERRIDE;
        virtual bool entity_exists(int entity_id) const Q_DECL_OVERRIDE;
        virtual void delete_entity(int entity_id) Q_DECL_OVERRIDE;
        int createNewComponent(int comp_id = invalid_id, QString official_name=QString(), QString human_readable_description=QString(), QString table_name=QString());
        void createComponentAndAddTo(static_component_id comp_id, int entity_id) ;
        virtual void createComponentAndAddTo(int comp_id, int entity_id) Q_DECL_OVERRIDE;
        virtual void removeComponentFrom(int comp_id, int entity_id) Q_DECL_OVERRIDE;
        virtual bool component_exists(int comp_id) const Q_DECL_OVERRIDE;
        virtual bool hasComponent(int comp_id, int entity_id) const Q_DECL_OVERRIDE;
        virtual QVariant getComponentDataForEntity(int component_id, int entity_id) const Q_DECL_OVERRIDE;
        virtual void setComponentDataForEntity(int component_id, int entity_id,  const QVariant &component_data) Q_DECL_OVERRIDE;
        //===

        //===
        //bool entity_exists(int entity_id)const;
        //==

        //=== Factory methods
        virtual int createNewSystem(QUuid suid, QString nameVal, abstract_configuration_widget* sys_cfg_wdg) Q_DECL_OVERRIDE;
        virtual int createNewAnalogInputValue(QString nameVal, QString unitsVal, double amin, double amax, int id, int box_id , int entity =-1 ) Q_DECL_OVERRIDE;
        virtual int createNewAnalogOutputValue(QString nameVal, QString unitsVal, double smin, double smax, double setVal, int id, int box_id , int entity =-1) Q_DECL_OVERRIDE;
        virtual int createNewAnalogValue(QString nameVal, QString unitsVal, double smin, double smax, double amin, double amax, double setVal, int id, int box_id ) Q_DECL_OVERRIDE;
        virtual int createNewDigitalInputValue(QString nameVal, QString unitsVal,  int id, int box_id , int entity =-1) Q_DECL_OVERRIDE;
        virtual int createNewDigitalOutputValue(QString nameVal,  bool setVal, int id, int box_id , int entity =-1) Q_DECL_OVERRIDE;
        virtual int createNewDigitalValue(QString nameVal, QString unitsVal,  bool setVal, int id, int box_id ) Q_DECL_OVERRIDE;
        virtual int createNewDigitalValue(QString nameVal, bool setVal, int id, int box_id ) Q_DECL_OVERRIDE;
        virtual int createNewTcpBox(QString nameVal="New Box", QString ip="localhost", quint16 port=2701) Q_DECL_OVERRIDE;
        virtual int createQtScriptEngine(QString nameVal="Qt Script Engine", QScriptEngine* engine=NULL) Q_DECL_OVERRIDE;
        virtual int createTofDaqDll(QString nameVal="TofDaqDll", QString pathVal="C:\\Tofwerk\\TofDaq_1.97_noHW\\TofDaqDll.dll", int timeout = 5000, int updaterate=1000, QString user_data_path="/USER_DATA") Q_DECL_OVERRIDE;

        virtual int createScriptableObject(QString nameVal, abstract_scriptable_object* comp) Q_DECL_OVERRIDE;
        //===

        virtual bool is_analog_value(int entity) const Q_DECL_OVERRIDE;
        virtual bool is_digital_value(int entity) const Q_DECL_OVERRIDE;

//        int add_scriptable_component(int entity, abstract_scriptable_object* comp);
//        int remove_scriptable_component(int entity);

//        //=== Factory constants
//        QUuid get_AnalogInputValue_uid()const   {return ("{340c5f10-6d53-4c15-8fc4-4c559580cfbc}");}
//        QUuid get_AnalogOutputValue_uid()const  {return ("{c81ab50c-417c-404f-87b3-f9a8e474a6b1}");}
//        QUuid get_AnalogValue_uid()const        {return ("{6ddc030e-2001-4a38-a8ce-57b309f902ff}");}
//        QUuid get_DigitalInputValue_uid()const  {return ("{231fe499-ed93-4f66-a2ed-5ca5063a0d86}");}
//        QUuid get_DigitalOutputValue_uid()const {return ("{bb3e0e52-fb29-429b-9668-b2e3414e9db3}");}
//        QUuid get_DigitalValue_uid()const       {return ("{837c326e-e5fb-4271-97e8-8a3161cfc02c}");}
//        QUuid get_TcpBox_uid()const             {return ("{99060fb8-676f-47d8-b9f1-c9c492721009}");}
//        QUuid get_System_uid()const             {return ("{b50224fa-8908-4503-8296-8b0c8531e1ce}");}
//        QUuid get_Qt_Script_Engine_uid()const   {return ("{3a31ae25-c7f4-4e79-93c4-2a4f7b675298}");}
//        QUuid get_TofDaqDll_uid()const          {return ("{e16e6a36-4f76-4746-995d-62996c86f74a}");}
//        QUuid get_ScriptableObject_uid()const   {return ("{592400d5-7684-4c31-9bfa-7cd806c40589}");}
//        //

//        QList<int> get_all_tcpBoxes()const{return get_entity_by_serialVersionUID(get_TcpBox_uid());}
//        QList<int> get_all_AnalogInputValues()const{return get_entity_by_serialVersionUID(get_AnalogInputValue_uid());}
//        QList<int> get_all_AnalogOutputValues()const{return get_entity_by_serialVersionUID(get_AnalogOutputValue_uid());}
//        QList<int> get_all_AnalogValues()const{return get_entity_by_serialVersionUID(get_AnalogValue_uid());}
//        virtual QList<int> get_all_Values()const;
//        QList<int> get_all_DigitalInputValues()const{return get_entity_by_serialVersionUID(get_DigitalInputValue_uid());}
//        QList<int> get_all_DigitalOutputValues()const{return get_entity_by_serialVersionUID(get_DigitalOutputValue_uid());}
//        QList<int> get_all_DigitalValues()const{return get_entity_by_serialVersionUID(get_DigitalValue_uid());}
//        virtual QList<int> get_all_DValues()const;

//        QList<int> get_all_Systems()const{return get_entity_by_serialVersionUID(get_System_uid());}
//        QList<int> get_all_Qt_Script_Engines()const{return get_entity_by_serialVersionUID(get_Qt_Script_Engine_uid());}
//        QList<int> get_all_TofDaqDlls()const{return get_entity_by_serialVersionUID(get_TofDaqDll_uid());}
//        QList<int> get_all_ScriptableObjects()const{return get_entity_by_serialVersionUID(get_ScriptableObject_uid());}

        virtual QList<int> get_entities_with_scriptable_components()const Q_DECL_OVERRIDE;
        virtual QList<int> get_entities_with_userdata_components()const Q_DECL_OVERRIDE;

        virtual abstract_configuration_widget* get_systemConfigurationWidget_component(int entity_id) const Q_DECL_OVERRIDE;
        virtual QScriptEngine* get_qt_script_engine_component(int entity_id) const Q_DECL_OVERRIDE;
        virtual abstract_scriptable_object* get_scriptable_object_component(int entity_id) const Q_DECL_OVERRIDE;


        virtual double get_analogActValue_component(int entity)const Q_DECL_OVERRIDE;
        virtual double get_analogSetValue_component(int entity)const Q_DECL_OVERRIDE;
        virtual bool get_digitalActValue_component(int entity)const Q_DECL_OVERRIDE;
        virtual bool get_digitalSetValue_component(int entity)const Q_DECL_OVERRIDE;
        virtual double get_setMin_component(int entity)const Q_DECL_OVERRIDE;
        virtual double get_setMax_component(int entity)const Q_DECL_OVERRIDE;
        virtual double get_actMin_component(int entity)const Q_DECL_OVERRIDE;
        virtual double get_actMax_component(int entity)const Q_DECL_OVERRIDE;
        virtual int get_id_component(int entity)const Q_DECL_OVERRIDE;
        virtual QString get_name_component(int entity)const Q_DECL_OVERRIDE;
        virtual QString get_units_component(int entity)const Q_DECL_OVERRIDE;
        virtual QString get_ip_address_component(int entity)const Q_DECL_OVERRIDE;
        virtual quint16 get_port_component(int entity)const Q_DECL_OVERRIDE;
        virtual int get_box_id_component(int entity)const Q_DECL_OVERRIDE;
        virtual bool get_connection_state_component(int entity)const Q_DECL_OVERRIDE;
        virtual QUuid get_serialVersionUID_component(int entity)const Q_DECL_OVERRIDE;
        virtual QUuid get_systemVersionUID_component(int entity)const Q_DECL_OVERRIDE;
        virtual int get_tcp_box_backend_component(int entity)const Q_DECL_OVERRIDE;
        virtual QString get_script_file_component(int entity)const Q_DECL_OVERRIDE;
        virtual QString get_data_aquistion_dll_component(int entity)const Q_DECL_OVERRIDE;
        virtual int get_timeout_component(int entity)const Q_DECL_OVERRIDE;
        virtual int get_updaterate_component(int entity)const Q_DECL_OVERRIDE;
        virtual int get_userdata_component(int entity)const Q_DECL_OVERRIDE;
        virtual QString get_user_data_path_component(int entity)const Q_DECL_OVERRIDE;


        //QList<int> get_all_systems()const;
        virtual QList<int> get_entity_by_name(const QString& name) const Q_DECL_OVERRIDE;
        virtual QList<int> get_entity_by_serialVersionUID(const QUuid& uid) const Q_DECL_OVERRIDE;
        virtual QList<int> get_entity_by_systemVersionUID(const QUuid& uid) const Q_DECL_OVERRIDE;
        virtual QList<int> get_entity_by_component(int comp_id) const Q_DECL_OVERRIDE;

        virtual QList<int> get_all_entities()const Q_DECL_OVERRIDE;
        virtual QList<int> get_all_components_of_entity(int entity)const Q_DECL_OVERRIDE;

        QList<int> get_all_components()const;

    public slots:
        virtual void slot_change_name_component(int entity, QString value) Q_DECL_OVERRIDE;
        virtual void slot_change_ip_address_component(int entity, QString value) Q_DECL_OVERRIDE;
        virtual void slot_change_port_component(int entity, quint16 value) Q_DECL_OVERRIDE;
        virtual void slot_connection_state_component(int entity, bool value) Q_DECL_OVERRIDE;
        virtual void slot_change_tcp_box_backend_component(int entity, int value) Q_DECL_OVERRIDE;

    signals:
        void newSystem(int entity);

        void component_changed(int entity, int component);

        void connection_state_component_changed(int entity);

        void analogSet_component_changed(int entity);
        void analogAct_component_changed(int entity);

        void digitalSet_component_changed(int entity);
        void digitalAct_component_changed(int entity);

        void name_component_changed(int entity);

        void timeout_component_changed(int entity);
        void updaterate_component_changed(int entity);

        void script_file_component_changed(int entity);

        void data_aquistion_dll_component_changed(int entity);

        void user_data_path_changed(int entity);

        void newAnalogInputValue(int);
        void newAnalogOutputValue(int);
        void newAnalogValue(int);
        void newDigitalInputValue(int);
        void newDigitalOutputValue(int);
        void newDigitalValue(int);
        void newTcpBox(int);
        void newQtScriptEngine(int);
        void newTofDaqDll(int);

        void newScriptableObject(int);

        //void register_scritable_component(int);
        //void unregister_scritable_component(int);

    private:
        int get_new_valid_component_id()const;
        int get_new_valid_entity_id(int lower_bound=invalid_id, int upper_bound=id_max)const;
        int create_default_entity();

        int createNewVersionInformation();


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
        void set_serialVersionUID_component(int entity, QUuid val);
        void set_tcp_box_backend_component(int entity, int val);
        void set_systemConfigurationWidget_component(int entity_id, abstract_configuration_widget *wdgt);
        void set_qt_script_engine_component(int entity_id, QScriptEngine *engine);
        void set_scriptable_object_component(int entity_id, abstract_scriptable_object *engine);
        void set_script_file_component(int entity, QString val);
        void set_data_aquistion_dll_component(int entity, QString val);
        void set_timeout_component(int entity, int val);
        void set_updaterate_component(int entity, int val);
        void set_user_data_path_component(int entity, QString val);

        int calc_enity_component_hash(int entity_id, int comp_id)const{return (comp_id*P1 + entity_id)*P2;}

    private slots:
        void slot_change_analogAct_component(int entity, double value);
        void slot_change_analogSet_component(int entity, double value);

        void slot_change_digitalAct_component(int entity, bool value);
        void slot_change_digitalSet_component(int entity, bool value);

        void slot_change_script_file_component(int entity, QString value);

        void slot_change_data_aquistion_dll_component(int entity, QString value);

        void slot_change_timeout_component(int entity, int value);

        void slot_change_updaterate_component(int entity, int value);

        void slot_change_userDataPath_component(int entity, QString value);

    private:

        int current_identity_id;

        QMap<int, comonent_t> components;                       //component_id | official name | human-readable description | table-name
        QMap<int, entity_t> entities;                           //entity_id | human-readable label FOR DEBUGGING ONLY
        QMap<int, entity_components_t> entity_components;       //enity_component_hash | entity_id | component_id
        QMap<int, QVariant> component_data_table_N;             //enity_component_hash | [1..M columns, one column for each piece of data in your component]
        QMap<int, abstract_configuration_widget*> component_widget_table;
        QMap<int, QScriptEngine*> component_script_engine_table;
        QMap<int, abstract_scriptable_object*> component_scriptable_object_table;

    private:
        //static QAtomicPointer<entity_manager> _instance;
        static entity_manager* _instance;
        static QMutex _mutex;

        static const int P1=997;
        static const int P2=1009;
    };



} // namespace talorion

#endif // TALORION_ENTITY_MANAGER_HPP

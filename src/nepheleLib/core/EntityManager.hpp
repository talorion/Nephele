#ifndef TALORION_ENTITY_MANAGER_HPP
#define TALORION_ENTITY_MANAGER_HPP

#include <QObject>
#include <QMap>
#include <QUuid>
#include <QPointer>
#include <QList>
#include <QVariant>

QT_BEGIN_NAMESPACE
class QMutex;
class QSignalMapper;
class QVariant;
class QScriptEngine;
QT_END_NAMESPACE

#include "StaticComponentId.hpp"
//#include "abstract_system.hpp"

#include "systemviewscontainer.h"
//#include "tcp_box_system/JsonRpc/JrpcDatatype.hpp"

namespace talorion {

class EventManager;

class abstract_configuration_widget;
class abstract_scriptable_object;

//class abstract_system;
//class abstract_system::SystemViewsContainer;

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

//    typedef enum static_component_id {
//        IP_ADDRESS_COMPONENT,
//        PORT_COMPONENT,
//        NAME_COMPONENT,
//        UNITS_COMPONENT,
//        SET_MIN_COMPONENT,
//        SET_MAX_COMPONENT,
//        ACT_MIN_COMPONENT,
//        ACT_MAX_COMPONENT,
//        ANALOG_SET_VALUE_COMPONENT,
//        ANALOG_ACT_VALUE_COMPONENT,
//        ID_COMPONENT,
//        BOX_ID_COMPONENT,
//        DIGITAL_SET_VALUE_COMPONENT,
//        DIGITAL_ACT_VALUE_COMPONENT,
//        SYSTEM_CONFIGURAION_WIDGET_COMPONENT,
//        IS_SYSTEM_COMPONENT,
//        CONNECTION_STATE_COMPONENT,
//        SERIAL_VERSION_UID_COMPONENT,
//        TCP_BOX_BACKEND_COMPONENT,
//        SYSTEM_VERSION_UID_COMPONENT,
//        QT_SCRIPT_ENGINE_COMPONENT,
//        SCRIPT_FILE_COMPONENT,
//        DATA_AQUISITION_DLL_COMPONENT,
//        TIMEOUT_COMPONENT,
//        SCRIPTABLE_OBJECT_COMPONENT,
//        FILEVERSION_COMPONENT,
//        PRODUCTVERSION_COMPONENT,
//        COMPANYNAME_COMPONENT,
//        UPDATERATE_COMPONENT,
//        USER_DATA_COMPONENT,
//        USER_DATA_PATH_COMPONENT,
//    } static_component_id;

class EntityManager : public QObject
{
    Q_OBJECT
    friend class ManagerLocator;
public:     //typedefs
    using ID=int;
    using EntityID=ID ;
    using ComponentID=ID ;
    using ComponentDataID=ID ;
    using ComponentData=QVariant;
    using EntityList=QList<EntityID>;
    using ComponentList=QList<ComponentID>;

public:     //constants
    static const ID invalid_id=-1;
    static const EntityID max_entity_id=997;    //for component_data_id hash should ba a prime
    static const EntityID max_component_id=1009; //for component_data_id hash should ba a prime
    static const ComponentData invalid_data;
    static QVector<double> invalid_doubleVector_data;
    static EntityList invalid_entityList_data;

public:     //convenient fnctions
    static bool isValid(ID id){return id > invalid_id;}
    static ComponentDataID getcomponentDataId(EntityID entity_id, ComponentID component_id){return (component_id*max_entity_id + entity_id)*max_component_id;}
    static bool isValid(const ComponentData& component_data);

public:
    explicit EntityManager(QObject *parent = 0);
    ~EntityManager();
    Q_DISABLE_COPY(EntityManager)

public:
    static EntityManager *get_instance();
    static void destroy();

    void initialize();
    void dispose();

    //=== DB Functions
    ComponentID createNewComponent(ComponentID component_id=invalid_id, QString component_name=QString());
    ComponentID createNewComponent(const QString& component_name);
    int createNewEntity(QString human_readable_label=QString(), int entity=-1, bool isSystem = false);
    bool entityExists(EntityID entity_id) const;
    void removeEntity(int entity_id);
    void removeAllComponentsFromEntity(EntityID entity_id);
    ComponentID createComponentAndAddTo(static_component_id comp_id, int entity_id);
    ComponentID createComponentAndAddTo(int comp_id, int entity_id);
    ComponentID createComponentAndAddTo(ComponentID comp_id, EntityID entity_id, bool ignore_exclusive);
    ComponentID getComponentByName(const QString& component_name)const;
    void removeComponentFrom(int comp_id, int entity_id);
    void removeComponentFromEntity(ComponentID component_id, EntityID entity_id);
    bool componentExists(int comp_id) const;
    bool componentExists(QString component_name) const;
    bool entityHasComponent(EntityID entity_id, ComponentID component_id) const;
    bool hasComponent(int comp_id, int entity_id) const;
    QVariant componentDataForEntity(int component_id, int entity_id) const;
    void setComponentDataForEntity(int component_id, int entity_id,  const QVariant &component_data);
    void setComponentDataForEntity(ComponentID component_id, EntityID entity_id, const ComponentData &component_data, bool ignore_exclusive);
    void setComponentDataForEntity(ComponentID component_id, EntityID entity_id, QVector<double> &component_data);
    void setComponentDataForEntity(ComponentID component_id, EntityID entity_id, EntityList &component_data);
    void setComponentDataForEntity(ComponentID component_id, EntityID entity_id, EntityList *component_data, bool ignore_exclusive);
    //void setComponentDataForEntity(ComponentID component_id, EntityID entity_id, JrpcDatatypeList &component_data);
    //void setComponentDataForEntity(ComponentID component_id, EntityID entity_id, JrpcDatatypeList *component_data, bool ignore_exclusive);


    void setData(EntityManager::ComponentDataID component_data_id,  QVector<double>* component_data);
    void setData(EntityManager::ComponentDataID component_data_id, EntityList *component_data);
    //void setData(EntityManager::ComponentDataID component_data_id, JrpcDatatypeList *component_data);

    void addComponent(EntityManager::ComponentID component_id, QString official_name);

    bool componentIsExclusive(ComponentID component_id)const;
    //===

    //=== Evt Manager
    bool isConnectedToEventManager()const;
    void connectToEventManager(EventManager* evt_mng);
    EventManager* eventManager()const;
    //===


    EntityID tcpBoxExists(QString serverName, quint16 serverPort);

    //=== Factory methods
    int createNewSystem(QUuid suid, QString nameVal, abstract_configuration_widget* sys_cfg_wdg, SystemViewsContainer &systemViews);
    int createNewAnalogInputValue(QString nameVal, QString unitsVal, double amin, double amax, int id, int box_id= invalid_id, int entity =-1 ) ;
    int createNewAnalogOutputValue(QString nameVal, QString unitsVal, double smin, double smax, double setVal, int id, int box_id= invalid_id , int entity =-1) ;
    int createNewAnalogValue(QString nameVal, QString unitsVal, double smin, double smax, double amin, double amax, double setVal, int id, int box_id= invalid_id ) ;
    int createNewDigitalInputValue(QString nameVal, QString unitsVal,  int id, int box_id= invalid_id , int entity =-1) ;
    int createNewDigitalOutputValue(QString nameVal,  bool setVal, int id, int box_id= invalid_id , int entity =-1) ;
    int createNewDigitalValue(QString nameVal, QString unitsVal,  bool setVal, int id, int box_id= invalid_id ) ;
    int createNewDigitalValue(QString nameVal, bool setVal, int id, int box_id= invalid_id ) ;
    int createNewBox(QString nameVal="New Box", QString ip="localhost", quint16 port=2701, int transport=0);
    int createNewTcpBox(QString nameVal="New Box", QString ip="localhost", quint16 port=2701);
    int createNewSerialBox(QString nameVal="New Box", QString ip="COM1");
    int createQtScriptEngine(QString nameVal="Qt Script Engine", QScriptEngine* engine=NULL);
    int createTofDaqDll(QString nameVal="TofDaqDll", QString pathVal="C:\\Tofwerk\\TofDaq_1.97_noHW\\TofDaqDll.dll", int timeout = 5000, int updaterate=1000, QString user_data_path="/NEPHELE");
    int create_scriptable_usr_data(QString nameVal, double setval=0.0);


    int createScriptableObject(QString nameVal, abstract_scriptable_object* comp);

    int createQuickScript(QString nameVal, QString path);
    //===

    bool is_analog_value(int entity) const;
    bool is_digital_value(int entity) const;

    //        int add_scriptable_component(int entity, abstract_scriptable_object* comp);
    //        int remove_scriptable_component(int entity);

    //=== Factory constants
    QUuid get_AnalogInputValue_uid()const   {return ("{340c5f10-6d53-4c15-8fc4-4c559580cfbc}");}
    QUuid get_AnalogOutputValue_uid()const  {return ("{c81ab50c-417c-404f-87b3-f9a8e474a6b1}");}
    QUuid get_AnalogValue_uid()const        {return ("{6ddc030e-2001-4a38-a8ce-57b309f902ff}");}
    QUuid get_DigitalInputValue_uid()const  {return ("{231fe499-ed93-4f66-a2ed-5ca5063a0d86}");}
    QUuid get_DigitalOutputValue_uid()const {return ("{bb3e0e52-fb29-429b-9668-b2e3414e9db3}");}
    QUuid get_DigitalValue_uid()const       {return ("{837c326e-e5fb-4271-97e8-8a3161cfc02c}");}
    QUuid get_TcpBox_uid()const             {return ("{99060fb8-676f-47d8-b9f1-c9c492721009}");}
    QUuid get_System_uid()const             {return ("{b50224fa-8908-4503-8296-8b0c8531e1ce}");}
    QUuid get_Qt_Script_Engine_uid()const   {return ("{3a31ae25-c7f4-4e79-93c4-2a4f7b675298}");}
    QUuid get_TofDaqDll_uid()const          {return ("{e16e6a36-4f76-4746-995d-62996c86f74a}");}
    QUuid get_ScriptableObject_uid()const   {return ("{592400d5-7684-4c31-9bfa-7cd806c40589}");}
    QUuid get_QuickScript_uid()const        {return ("{9ed7ca3b-4304-465c-9fe9-c5c6ba483a6c}");}
    QUuid get_ScriptableUsrData_uid()const   {return ("{d9b14945-2e1a-438c-b647-2ca630ed5558}");}
    QUuid get_JsonMethodDescriptor_uid()const   {return ("{ef3568df-2bde-445e-a4c0-ef4c8b1fd540}");}

    //

    QList<int> get_all_tcpBoxes()const{return get_entity_by_serialVersionUID(get_TcpBox_uid());}
    QList<int> get_all_AnalogInputValues()const{return get_entity_by_serialVersionUID(get_AnalogInputValue_uid());}
    QList<int> get_all_AnalogOutputValues()const{return get_entity_by_serialVersionUID(get_AnalogOutputValue_uid());}
    QList<int> get_all_AnalogValues()const{return get_entity_by_serialVersionUID(get_AnalogValue_uid());}
    QList<int> get_all_Values()const;
    QList<int> get_all_DigitalInputValues()const{return get_entity_by_serialVersionUID(get_DigitalInputValue_uid());}
    QList<int> get_all_DigitalOutputValues()const{return get_entity_by_serialVersionUID(get_DigitalOutputValue_uid());}
    QList<int> get_all_DigitalValues()const{return get_entity_by_serialVersionUID(get_DigitalValue_uid());}
    QList<int> get_all_DValues()const;

    QList<int> get_all_JsonMethodDescriptors()const{return get_entity_by_serialVersionUID(get_JsonMethodDescriptor_uid());}

    QList<int> get_all_Systems()const{return get_entity_by_serialVersionUID(get_System_uid());}
    QList<int> get_all_Qt_Script_Engines()const{return get_entity_by_serialVersionUID(get_Qt_Script_Engine_uid());}
    QList<int> get_all_TofDaqDlls()const{return get_entity_by_serialVersionUID(get_TofDaqDll_uid());}
    QList<int> get_all_ScriptableObjects()const{return get_entity_by_serialVersionUID(get_ScriptableObject_uid());}

    QList<int> get_all_quickScripts()const{return get_entity_by_serialVersionUID(get_QuickScript_uid());}

    QList<int> get_entities_with_scriptable_components()const{return get_entity_by_component(SCRIPTABLE_OBJECT_COMPONENT);}
    QList<int> get_entities_with_userdata_components()const{return get_entity_by_component(USER_DATA_COMPONENT);}

    QVector<double>& doubleVectorDataForEntity(ComponentID component_id, EntityID entity_id) const;
    EntityList& entityListDataForEntity(ComponentID component_id, EntityID entity_id) const;
    abstract_configuration_widget* get_systemConfigurationWidget_component(int entity_id) const;
    QScriptEngine* get_qt_script_engine_component(int entity_id) const;
    abstract_scriptable_object* get_scriptable_object_component(int entity_id) const;


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
    QUuid get_serialVersionUID_component(int entity)const;
    QUuid get_systemVersionUID_component(int entity)const;
    int get_tcp_box_backend_component(int entity)const;
    QString get_script_file_component(int entity)const;
    QString get_data_aquistion_dll_component(int entity)const;
    int get_timeout_component(int entity)const;
    int get_updaterate_component(int entity)const;
    int get_userdata_component(int entity)const;
    QString get_user_data_path_component(int entity)const;
    QVariant get_metadata_component(EntityID entity)const;
    int getTransportComponent(int entity)const;

    //QList<int> get_all_systems()const;
    QList<int> get_entity_by_name(const QString& name) const;
    EntityList entitiesByComponentsValue(ComponentID component_id, ComponentData component_data)const;
    QList<int> get_entity_by_serialVersionUID(const QUuid& uid) const;
    QList<int> get_entity_by_systemVersionUID(const QUuid& uid) const;
    QList<int> get_entity_by_component(int comp_id) const;

    QList<int> allEntities()const;
    ComponentList allComponents()const;
    QList<int> get_all_components_of_entity(int entity)const;

    //=== Compositum
    bool entiyHasChildren(EntityID entity_id) const;
    bool entiyHasParent(EntityID entity_id) const;
    void addChildren(EntityID entity_id, EntityList &children);
    void addChild(EntityID entity_id, EntityID &child);
    void removeChildren(EntityID entity_id, EntityList children);
    void removeChild(EntityID parent_entity_id, EntityID child_entity_id);
    EntityList& getChildren(EntityID entity_id) const;
    void setParent(EntityID child_entity_id, EntityID parent_entity_id);
    void removeParent(EntityID child_entity_id);
    EntityID getParent(EntityID child_entity_id);
    //===

public slots:
    void slot_change_name_component(int entity, QString value);
    void slot_change_ip_address_component(int entity, QString value);
    void slot_change_port_component(int entity, quint16 value);
    void slot_connection_state_component(int entity, bool value);
    void slot_change_tcp_box_backend_component(int entity, int value);

    void slot_change_userDataPath_component(int entity, QString value);

    void slot_change_metadata_component(EntityID entity, QVariant value);

signals:
    void entityCreated(EntityID id);
    void entityRemoved(EntityID id, QUuid serialVersionUid = QUuid() );

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
    void removedAnalogInputValue(int);
    void newAnalogOutputValue(int);
    void removedAnalogOutputValue(int);
    void newAnalogValue(int);
    void removedAnalogValue(int);

    void newDigitalInputValue(int);
    void removedDigitalInputValue(int);
    void newDigitalOutputValue(int);
    void removedDigitalOutputValue(int);
    void newDigitalValue(int);
    void removedDigitalValue(int);

    void newTcpBox(int);
    void tcpBoxRemoved(int);
    void newQtScriptEngine(int);
    void newTofDaqDll(int);

    void newScriptableObject(int);

    void newQuickScript(int);

    //void register_scritable_component(int);
    //void unregister_scritable_component(int);

private:
    int createNewVersionInformation();

    ID nextIdFrom(const QList<ID>& container)const;
    ComponentID nextComponentId()const;

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

    void setComponentDataForEntity(ComponentID component_id, EntityID entity_id,  EntityList &component_data, bool ignore_exclusive);

    //int calc_enity_component_hash(int entity_id, int comp_id)const{return (comp_id*max_entity_id + entity_id)*max_component_id;}

private slots:
    void slot_change_analogAct_component(int entity, double value);
    void slot_change_analogSet_component(int entity, double value);

    void slot_change_digitalAct_component(int entity, bool value);
    void slot_change_digitalSet_component(int entity, bool value);

    void slot_change_script_file_component(int entity, QString value);

    void slot_change_data_aquistion_dll_component(int entity, QString value);

    void slot_change_timeout_component(int entity, int value);

    void slot_change_updaterate_component(int entity, int value);

    void slotChangeComponentDataForEntity(EntityManager::ComponentID component_id, EntityManager::EntityID entity_id,  const EntityManager::ComponentData &component_data);

    void slotEntityRemoved(EntityID id, QUuid serialVersionUid);

private:

    int current_identity_id;

    QMap<int, comonent_t> m_components;                       //component_id | official name | human-readable description | table-name
    QMap<int, entity_t> m_entities;                           //entity_id | human-readable label FOR DEBUGGING ONLY
    QMap<int, entity_components_t> m_entity_components;       //enity_component_hash | entity_id | component_id
    QMap<int, QVariant> m_component_data_table_N;             //enity_component_hash | [1..M columns, one column for each piece of data in your component]
    //QMap<EntityManager::ComponentDataID, EntityManager::ComponentData> m_component_data_table;
    QMap<int, QVector<double>*> m_componentDoubleVectorDataTable;
    //QMap<int, JrpcDatatypeList*> m_componentJrpcDatatypeListTable;
    QMap<int, EntityList*> m_componentEntityListDataTable;
    QMap<int, abstract_configuration_widget*> m_component_widget_table;
    QMap<int, QScriptEngine*> m_component_script_engine_table;
    QMap<int, abstract_scriptable_object*> m_component_scriptable_object_table;
    QPointer<EventManager> m_evt_mng;

    ComponentList m_exclusive_components;

private:
    //static QAtomicPointer<entity_manager> _instance;
    static EntityManager* _instance;
    static QMutex _mutex;

    //static const int P1=997;
    //static const int P2=1009;
};

} // namespace talorion

Q_DECLARE_METATYPE(talorion::EntityManager::EntityID)
Q_DECLARE_METATYPE(talorion::EntityManager::EntityList)

#endif // TALORION_ENTITY_MANAGER_HPP

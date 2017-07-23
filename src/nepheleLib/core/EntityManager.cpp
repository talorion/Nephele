#include "EntityManager.hpp"

#include <QAtomicPointer>
#include <QMutex>
#include <QDebug>
#include <QSignalMapper>
#include <QVariant>
#include <QSettings>

#include "core/EventManager.hpp"
#include "core/ManagerLocator.hpp"
#include "abstract_configuration_widget.hpp"
//#include "analogvalue.hpp"

//#include "version.hpp"

namespace talorion {

const EntityManager::ComponentData EntityManager::invalid_data= QVariant();//Constructs an invalid variant.
QVector<double> EntityManager::invalid_doubleVector_data = QVector<double>();
EntityManager::EntityList EntityManager::invalid_entityList_data = EntityManager::EntityList();
bool EntityManager::isValid(const EntityManager::ComponentData &component_data){return component_data.isValid();}


//QAtomicPointer<entity_manager> entity_manager::_instance;
EntityManager* EntityManager::_instance;
QMutex EntityManager::_mutex;

EntityManager::EntityManager(QObject *par) :
    QObject(par),
    current_identity_id(0),
    m_components(),
    m_entities(),
    m_entity_components(),
    m_component_data_table_N(),
    m_componentDoubleVectorDataTable(),
    m_componentEntityListDataTable(),
    m_component_widget_table(),
    m_component_script_engine_table(),
    m_component_scriptable_object_table(),
    m_evt_mng(EventManager::get_instance()),
    m_exclusive_components({CHILDREN_COMPONENT, PARENT_COMPONENT})
{

    qRegisterMetaType<EntityID>("EntityID");
    qRegisterMetaType<EntityList>("EntityList");

    auto evt_mg = EventManager::get_instance();
    auto xx  = this;
    current_identity_id = 0;

    QObject::connect(xx, SIGNAL(newSystem(int)), evt_mg, SIGNAL(newSystem(int)));
    connect(this, SIGNAL(newSystem(int)), EventManager::get_instance(), SIGNAL(newSystem(int)));

    connect(this,SIGNAL(connection_state_component_changed(int)),EventManager::get_instance(),SIGNAL(connection_state_component_changed(int)));

    connect(this, SIGNAL(user_data_path_changed(int)), EventManager::get_instance(), SIGNAL(user_data_path_component_changed(int)));
    connect(this, SIGNAL(analogAct_component_changed(int)), EventManager::get_instance(), SIGNAL(analogAct_component_changed(int)));
    connect(this, SIGNAL(analogSet_component_changed(int)), EventManager::get_instance(), SIGNAL(analogSet_component_changed(int)));
    connect(this, SIGNAL(digitalAct_component_changed(int)), EventManager::get_instance(), SIGNAL(digitalAct_component_changed(int)));
    connect(this, SIGNAL(digitalSet_component_changed(int)), EventManager::get_instance(), SIGNAL(digitalSet_component_changed(int)));
    connect(this,SIGNAL(name_component_changed(int)),EventManager::get_instance(),SIGNAL(name_component_changed(int)));
    connect(this,SIGNAL(script_file_component_changed(int)),EventManager::get_instance(),SIGNAL(script_file_component_changed(int)));
    connect(this, SIGNAL(data_aquistion_dll_component_changed(int)),EventManager::get_instance(),SIGNAL(data_aquistion_dll_component_changed(int)));
    connect(this, SIGNAL(timeout_component_changed(int)),EventManager::get_instance(),SIGNAL(timeout_component_changed(int)));
    connect(this, SIGNAL(updaterate_component_changed(int)),EventManager::get_instance(),SIGNAL(updaterate_component_changed(int)));
    //connect(this, SIGNAL(register_scritable_component(int)),event_manager::get_instance(),SIGNAL(register_scritable_component(int)));
    //connect(this, SIGNAL(unregister_scritable_component(int)),event_manager::get_instance(),SIGNAL(unregister_scritable_component(int)));
    connect(this, SIGNAL(newScriptableObject(int)),EventManager::get_instance(),SIGNAL(newScriptableObject(int)));

    connect(EventManager::get_instance(),SIGNAL(change_analogAct_component(int,double)),this,SLOT(slot_change_analogAct_component(int,double)),Qt::UniqueConnection);
    connect(EventManager::get_instance(),SIGNAL(change_analogSet_component(int,double)),this,SLOT(slot_change_analogSet_component(int,double)),Qt::UniqueConnection);
    connect(EventManager::get_instance(),SIGNAL(change_digitalAct_component(int,bool)),this,SLOT(slot_change_digitalAct_component(int,bool)),Qt::UniqueConnection);
    connect(EventManager::get_instance(),SIGNAL(change_digitalSet_component(int,bool)),this,SLOT(slot_change_digitalSet_component(int,bool)),Qt::UniqueConnection);
    connect(EventManager::get_instance(),SIGNAL(change_script_file_component(int,QString)),this,SLOT(slot_change_script_file_component(int,QString)));
    connect(EventManager::get_instance(),SIGNAL(change_data_aquistition_dll_component(int,QString)),this,SLOT(slot_change_data_aquistion_dll_component(int,QString)));
    connect(EventManager::get_instance(),SIGNAL(change_user_data_path_component(int,QString)),this,SLOT(slot_change_userDataPath_component(int,QString)));
    connect(EventManager::get_instance(),SIGNAL(change_timeout_component(int,int)),this,SLOT(slot_change_timeout_component(int,int)));
    connect(EventManager::get_instance(),SIGNAL(change_updaterate_component(int,int)),this,SLOT(slot_change_updaterate_component(int,int)));

    connect(this,SIGNAL(newAnalogInputValue(int)),EventManager::get_instance(),SIGNAL(newAnalogInputValue(int)));
    connect(this,SIGNAL(newAnalogOutputValue(int)),EventManager::get_instance(),SIGNAL(newAnalogOutputValue(int)));
    connect(this,SIGNAL(newAnalogValue(int)),EventManager::get_instance(),SIGNAL(newAnalogValue(int)));
    connect(this,SIGNAL(newDigitalInputValue(int)),EventManager::get_instance(),SIGNAL(newDigitalInputValue(int)));
    connect(this,SIGNAL(newDigitalOutputValue(int)),EventManager::get_instance(),SIGNAL(newDigitalOutputValue(int)));
    connect(this,SIGNAL(newDigitalValue(int)),EventManager::get_instance(),SIGNAL(newDigitalValue(int)));
    connect(this, SIGNAL(newTcpBox(int)),EventManager::get_instance(),SIGNAL(newTcpBox(int)));
    connect(this, SIGNAL(tcpBoxRemoved(int)),EventManager::get_instance(),SIGNAL(tcpBoxRemoved(int)));
    connect(this, SIGNAL(newQtScriptEngine(int)),EventManager::get_instance(),SIGNAL(newQtScriptEngine(int)));
    connect(this, SIGNAL(newTofDaqDll(int)),EventManager::get_instance(),SIGNAL(newTofDaqDll(int)));

    connect(this, SIGNAL(newQuickScript(int)),EventManager::get_instance(),SIGNAL(newQuickScript(int)));

    connect(this, SIGNAL(entityRemoved(EntityID,QUuid)), this , SLOT(slotEntityRemoved(EntityID,QUuid)));

    connect(this,SIGNAL(removedAnalogInputValue(int)),EventManager::get_instance(),SIGNAL(removedAnalogInputValue(int)));
    connect(this,SIGNAL(removedAnalogOutputValue(int)),EventManager::get_instance(),SIGNAL(removedAnalogOutputValue(int)));
    connect(this,SIGNAL(removedAnalogValue(int)),EventManager::get_instance(),SIGNAL(removedAnalogValue(int)));
    connect(this,SIGNAL(removedDigitalInputValue(int)),EventManager::get_instance(),SIGNAL(removedDigitalInputValue(int)));
    connect(this,SIGNAL(removedDigitalOutputValue(int)),EventManager::get_instance(),SIGNAL(removedDigitalOutputValue(int)));
    connect(this,SIGNAL(removedDigitalValue(int)),EventManager::get_instance(),SIGNAL(removedDigitalValue(int)));


}

EntityManager::~EntityManager()
{
    auto tmp = m_componentEntityListDataTable;
    foreach (auto lst, tmp) {
        delete lst;
    }
    m_componentEntityListDataTable.clear();
}

EntityManager* EntityManager::get_instance()
{
    if ( !_instance ){
        _mutex.lock();
        if (!_instance){
            //_instance = new EntityManager();
            _instance = &(ManagerLocator::entityManager());
        }
        _mutex.unlock();
    }
    return _instance;
}//end getInstance

void EntityManager::destroy()
{
    _mutex.lock();
    if ( _instance )
        delete _instance;
    _instance = 0;
    _mutex.unlock();
}

void EntityManager::initialize()
{
    int zero_entity = createNewEntity("", 0);
    createComponentAndAddTo(IP_ADDRESS_COMPONENT, zero_entity);
    createComponentAndAddTo(PORT_COMPONENT, zero_entity);
    createComponentAndAddTo( NAME_COMPONENT, zero_entity);
    createComponentAndAddTo(UNITS_COMPONENT, zero_entity);
    createComponentAndAddTo(SET_MIN_COMPONENT, zero_entity);
    createComponentAndAddTo(SET_MAX_COMPONENT, zero_entity);
    createComponentAndAddTo(ACT_MIN_COMPONENT, zero_entity);
    createComponentAndAddTo(ACT_MAX_COMPONENT, zero_entity);
    createComponentAndAddTo(ANALOG_SET_VALUE_COMPONENT, zero_entity);
    createComponentAndAddTo(ANALOG_ACT_VALUE_COMPONENT, zero_entity);
    createComponentAndAddTo(ID_COMPONENT, zero_entity);
    createComponentAndAddTo(BOX_ID_COMPONENT, zero_entity);
    createComponentAndAddTo(DIGITAL_SET_VALUE_COMPONENT, zero_entity);
    createComponentAndAddTo(DIGITAL_ACT_VALUE_COMPONENT, zero_entity);
    createComponentAndAddTo(SYSTEM_CONFIGURAION_WIDGET_COMPONENT, zero_entity);
    createComponentAndAddTo(IS_SYSTEM_COMPONENT, zero_entity);
    createComponentAndAddTo(CONNECTION_STATE_COMPONENT, zero_entity);
    createComponentAndAddTo(SERIAL_VERSION_UID_COMPONENT, zero_entity);
    createComponentAndAddTo(TCP_BOX_BACKEND_COMPONENT, zero_entity);

    QSettings settings;
    //QStringList groups = settings.childGroups();
    foreach (QString entity, settings.childGroups()) {
        settings.beginGroup(entity);
        int ent =  entity.toInt();
        ent = createNewEntity("", ent);
        foreach (QString component, settings.childKeys()) {
            int comp= component.toInt();
            QVariant value = settings.value(component);
            //qDebug()<<entity<<component<<value;
            //ent = createNewEntity("",ent);

            createComponentAndAddTo(comp, ent);
            setComponentDataForEntity(comp,ent,value);
            //current_identity_id = qMax(current_identity_id+1, ent+1);

        }
        settings.endGroup();
    }

}

void EntityManager::dispose()
{
    foreach (int id, get_all_Values()) {
        removeEntity(id);
    }

    foreach (int id, get_all_DValues()) {
        removeEntity(id);
    }

    //        foreach (int id, get_all_AnalogInputValues()) {
    //            delete_entity(id);
    //        }

    //        foreach (int id, get_all_AnalogOutputValues()) {
    //            delete_entity(id);
    //        }

    foreach (int id, get_all_Systems()) {
        removeEntity(id);
    }

    foreach (int id, get_all_ScriptableObjects()) {
        removeEntity(id);
    }

    foreach (auto id, get_all_JsonMethodDescriptors()) {
        removeEntity(id);
    }

    QString group;
    QString path;
    QVariant value;

    QSettings settings;
    qDebug() << settings.fileName();
    auto groups = settings.childGroups();
    foreach (auto grp, groups) {
        settings.remove(grp);
    }
    //settings.clear();
    foreach (int entity, allEntities()) {
        group =QString::number(entity);
        settings.beginGroup(group);
        foreach (int component, get_all_components_of_entity(entity)) {
            path    = QString::number(component);
            value   = componentDataForEntity(component,entity);
            //qDebug()<<group<<path<<value;
            if(value.isValid())
                settings.setValue(path,value);

        }
        settings.endGroup();
    }


}

EntityManager::ComponentID EntityManager::createNewComponent(ComponentID component_id, QString component_name){
    if(!isValid(component_id)){
        component_id = nextComponentId();
    }

    if(!component_name.isNull() && componentExists(component_name)){
        component_id= getComponentByName(component_name);
    }

    if(!componentExists(component_id)){
        addComponent(component_id, component_name);
    }

    return component_id;
}

EntityManager::ComponentID EntityManager::createNewComponent(const QString& component_name){
    return createNewComponent(invalid_id, component_name);
}

int EntityManager::createNewEntity(QString human_readable_label, int entity, bool isSystem)
{
    int offset = 255;

    if(isSystem)
        offset = 0;

    //Q_UNUSED(human_readable_label);
    int entity_id;
    if(entity<0){
        do{
            entity_id= offset + current_identity_id++;
        }while (m_entities.constFind(entity_id) != m_entities.constEnd());
    }
    else
        entity_id= entity;
    //entity_id= qMax(current_identity_id+1, entity+1);

    //current_identity_id = entity_id;

    entity_t et;
    et.entity_id= entity_id;
    et.human_readable_label =human_readable_label;
    m_entities.insert(entity_id, et);

    emit entityCreated(entity_id);

    return entity_id;
}

bool EntityManager::entityExists(EntityID entity_id) const
{
    return m_entities.contains(entity_id);
}

void EntityManager::removeEntity(int entity_id)
{
    //    m_entities.remove(entity_id);

    //    foreach (int component_data_id, get_all_components_of_entity(entity_id)) {
    //        m_entity_components.remove(component_data_id);
    //        m_component_data_table_N.remove(component_data_id);
    //    }

    removeParent(entity_id);

    if(entiyHasChildren(entity_id)){
        EntityList tmplist(getChildren(entity_id));
        foreach (auto child, tmplist) {
            removeEntity(child);
        }
    }

    if(entityExists(entity_id)){
        auto uid = get_serialVersionUID_component(entity_id);
        removeAllComponentsFromEntity(entity_id);
        m_entities.remove(entity_id);

        emit entityRemoved(entity_id, uid);
    }

}

void EntityManager::removeAllComponentsFromEntity(EntityID entity_id){
    auto component_ids = m_entity_components.values(entity_id);
    for(auto ent_comp : component_ids){
        removeComponentFromEntity(ent_comp.component_id, entity_id);
    }
}

EntityManager::ComponentID EntityManager::createComponentAndAddTo(int comp_id, int entity_id)
{
    return createComponentAndAddTo(comp_id, entity_id, false);
}

EntityManager::ComponentID EntityManager::createComponentAndAddTo(EntityManager::ComponentID comp_id, EntityManager::EntityID entity_id, bool ignore_exclusive)
{
    if(!ignore_exclusive && componentIsExclusive(comp_id))
        return invalid_id;

    if(!m_components.contains(comp_id)){
        comonent_t ct;
        ct.component_id = comp_id;
        ct.human_readable_description = QString();
        ct.official_name = QString();
        ct.table_name = QString();
        m_components.insert(comp_id, ct);
    }

    auto component_data_id = getcomponentDataId(comp_id, entity_id );

    entity_components_t ect;
    ect.component_data_id = component_data_id;
    ect.component_id = comp_id;
    ect.entity_id = entity_id;
    m_entity_components.insert(component_data_id, ect);

    return comp_id;

}

EntityManager::ComponentID EntityManager::getComponentByName(const QString& component_name)const{
    auto component_id =invalid_id;

    for(auto it=m_components.cbegin(); it!=m_components.cend();++it){
        if(QString::compare(it.value().official_name ,component_name, Qt::CaseInsensitive)==0){
            component_id = it.key();
            break;
        }
    }

    return component_id;
}

void EntityManager::removeComponentFrom(int comp_id, int entity_id)
{
    if(!hasComponent(comp_id,entity_id))
        return;

    auto component_data_id = getcomponentDataId(comp_id, entity_id );

    m_entity_components.remove(component_data_id);

}

void EntityManager::removeComponentFromEntity(ComponentID component_id, EntityID entity_id)
{
    removeComponentFrom(component_id, entity_id);
}

bool EntityManager::componentExists(int comp_id) const
{
    return (m_components.contains(comp_id));
}

bool EntityManager::componentExists(QString component_name) const
{
    auto component_id = getComponentByName(component_name);
    return componentExists(component_id);
}

bool EntityManager::entityHasComponent(EntityID entity_id, ComponentID component_id) const{
    return hasComponent(component_id, entity_id);
}

bool EntityManager::hasComponent(int comp_id, int entity_id) const
{
    if(!entityExists(entity_id))
        return false;

    if(!componentExists(comp_id))
        return false;

    auto component_data_id = getcomponentDataId(comp_id, entity_id );
    return m_entity_components.contains(component_data_id);

}


EntityManager::ComponentID EntityManager::createComponentAndAddTo(static_component_id comp_id, int entity_id)
{
    return createComponentAndAddTo((int)comp_id, entity_id);
}

QVariant EntityManager::componentDataForEntity(int component_id, int entity_id) const
{
    auto component_data_id = getcomponentDataId(component_id, entity_id );
    //QMap<int, QVariant>::ConstIterator av = m_component_data_table_N.constFind(component_data_id);
    auto av = m_component_data_table_N.constFind(component_data_id);
    if (av == m_component_data_table_N.constEnd()){
        return QVariant();
    }

    return av.value();

}

void EntityManager::setComponentDataForEntity(int component_id, int entity_id, const QVariant& component_data)
{
    return setComponentDataForEntity(component_id, entity_id, component_data, false);
}

void EntityManager::setComponentDataForEntity(EntityManager::ComponentID component_id, EntityManager::EntityID entity_id, const EntityManager::ComponentData &component_data, bool ignore_exclusive)
{
    if(!ignore_exclusive && componentIsExclusive(component_id))
        return;

    if(!component_data.isValid())
        return;

    QVariant tmp = componentDataForEntity(component_id, entity_id);

    if(tmp == component_data )
        return;

    auto component_data_id = getcomponentDataId(component_id, entity_id );
    m_component_data_table_N.insert(component_data_id, component_data);
    emit component_changed(entity_id, component_id);
}

void EntityManager::setComponentDataForEntity(ComponentID component_id, EntityID entity_id,  QVector<double> &component_data){
    if(componentIsExclusive(component_id))
        return;
    ComponentDataID component_data_id = getcomponentDataId(entity_id,component_id);
    setData(component_data_id, &component_data);
}

void EntityManager::setComponentDataForEntity(ComponentID component_id, EntityID entity_id, EntityList &component_data)
{
    setComponentDataForEntity(component_id, entity_id, component_data, false);
}

void EntityManager::setComponentDataForEntity(EntityManager::ComponentID component_id, EntityManager::EntityID entity_id, EntityManager::EntityList *component_data, bool ignore_exclusive)
{
    if(!ignore_exclusive && componentIsExclusive(component_id))
        return;
    ComponentDataID component_data_id = getcomponentDataId(entity_id,component_id);
    setData(component_data_id, component_data);
}

//void EntityManager::setComponentDataForEntity(EntityManager::ComponentID component_id, EntityManager::EntityID entity_id, JrpcDatatypeList &component_data)
//{
//    setComponentDataForEntity(component_id, entity_id, &component_data, false);
//}

//void EntityManager::setComponentDataForEntity(EntityManager::ComponentID component_id, EntityManager::EntityID entity_id, JrpcDatatypeList *component_data, bool ignore_exclusive)
//{
//    if(!ignore_exclusive && componentIsExclusive(component_id))
//        return;
//    ComponentDataID component_data_id = getcomponentDataId(entity_id,component_id);
//    setData(component_data_id, component_data);
//}

void EntityManager::setData(EntityManager::ComponentDataID component_data_id,  QVector<double>* component_data){
    if(component_data != Q_NULLPTR)
        m_componentDoubleVectorDataTable.insert(component_data_id, component_data);

}

void EntityManager::setData(EntityManager::ComponentDataID component_data_id, EntityList *component_data)
{
    if(component_data != Q_NULLPTR)
        m_componentEntityListDataTable.insert(component_data_id, component_data);
}

//void EntityManager::setData(EntityManager::ComponentDataID component_data_id, JrpcDatatypeList *component_data)
//{
//    if(component_data != Q_NULLPTR)
//        m_componentJrpcDatatypeListTable.insert(component_data_id, component_data);
//}

void EntityManager::addComponent(EntityManager::ComponentID component_id, QString official_name)
{
    comonent_t ct;
    ct.component_id = component_id;
    ct.human_readable_description = QString();
    ct.official_name = official_name;
    ct.table_name = QString();
    m_components.insert(component_id, ct);
}
bool EntityManager::componentIsExclusive(ComponentID component_id)const{
    return m_exclusive_components.contains(component_id);
}

bool EntityManager::isConnectedToEventManager()const{
    return !(m_evt_mng.isNull());
}

void EntityManager::connectToEventManager(EventManager* evt_mng){
    if(isConnectedToEventManager())
        return;

    m_evt_mng = evt_mng;

    connect(m_evt_mng,  SIGNAL(changeComponentDataForEntity(EntityManager::ComponentID,EntityManager::EntityID,EntityManager::ComponentData)),
            this,       SLOT(slotChangeComponentDataForEntity(  EntityManager::ComponentID,EntityManager::EntityID,EntityManager::ComponentData)));

}

EventManager* EntityManager::eventManager()const{
    if(!isConnectedToEventManager())
        return Q_NULLPTR;

    return m_evt_mng.data();
}

int EntityManager::createNewSystem(QUuid suid, QString nameVal, abstract_configuration_widget* sys_cfg_wdg, SystemViewsContainer &systemViews)
{
    QList<int> list =get_entity_by_systemVersionUID(suid);
    int new_id = -1;
    if(list.empty()){
        new_id = createNewEntity(nameVal,-1,true);

        createComponentAndAddTo(SYSTEM_VERSION_UID_COMPONENT, new_id);
        createComponentAndAddTo(NAME_COMPONENT, new_id);

        createComponentAndAddTo(IS_SYSTEM_COMPONENT, new_id);
        createComponentAndAddTo(SERIAL_VERSION_UID_COMPONENT, new_id);

        if(sys_cfg_wdg)
            createComponentAndAddTo(SYSTEM_CONFIGURAION_WIDGET_COMPONENT, new_id);

        if(systemViews.empty() == false )
            createComponentAndAddTo(SYSTEM_VIEW_WIDGET_COMPONENT, new_id);

    }else{
        new_id = list[0];
    }

    setComponentDataForEntity(SYSTEM_VERSION_UID_COMPONENT,               new_id, suid);
    setComponentDataForEntity(NAME_COMPONENT,               new_id, nameVal);
    if(sys_cfg_wdg)
        set_systemConfigurationWidget_component(new_id, sys_cfg_wdg);
    setComponentDataForEntity(IS_SYSTEM_COMPONENT,               new_id, true);
    setComponentDataForEntity(SERIAL_VERSION_UID_COMPONENT, new_id, get_System_uid());
    if(systemViews.empty() == false ){
        //abstract_system::SystemViewsContainer &systemViews;
        auto sysViews = QVariant::fromValue(systemViews);
        setComponentDataForEntity(SYSTEM_VIEW_WIDGET_COMPONENT,               new_id, sysViews);
    }
    emit newSystem(new_id);

    return new_id;

}

int EntityManager::createNewAnalogInputValue(QString nameVal, QString unitsVal, double amin, double amax, int id, int box_id, int entity)
{
    int new_id = entity;
    if(entity <0)
        new_id = createNewEntity();

    createComponentAndAddTo( NAME_COMPONENT, new_id );
    createComponentAndAddTo( UNITS_COMPONENT, new_id );
    //createComponentAndAddTo( SET_MIN_COMPONENT, new_id );
    //createComponentAndAddTo( SET_MAX_COMPONENT, new_id );
    createComponentAndAddTo( ACT_MIN_COMPONENT, new_id );
    createComponentAndAddTo( ACT_MAX_COMPONENT, new_id );
    //createComponentAndAddTo( ANALOG_SET_VALUE_COMPONENT, new_id );
    createComponentAndAddTo( ANALOG_ACT_VALUE_COMPONENT, new_id );
    createComponentAndAddTo( ID_COMPONENT, new_id );
    createComponentAndAddTo( BOX_ID_COMPONENT, new_id );
    createComponentAndAddTo(SERIAL_VERSION_UID_COMPONENT, new_id);
    createComponentAndAddTo(USER_DATA_COMPONENT, new_id);
    createComponentAndAddTo(METADATA_COMPONENT, new_id);

    setComponentDataForEntity(NAME_COMPONENT,               new_id, nameVal);
    setComponentDataForEntity(UNITS_COMPONENT,              new_id, unitsVal);
    //setComponentDataForEntity(SET_MIN_COMPONENT,            new_id, smin);
    //setComponentDataForEntity(SET_MAX_COMPONENT,            new_id, smax);
    setComponentDataForEntity(ACT_MIN_COMPONENT,            new_id, amin);
    setComponentDataForEntity(ACT_MAX_COMPONENT,            new_id, amax);
    //setComponentDataForEntity(ANALOG_SET_VALUE_COMPONENT,   new_id, setVal);
    setComponentDataForEntity(ANALOG_ACT_VALUE_COMPONENT,   new_id, 0);
    setComponentDataForEntity(ID_COMPONENT,                 new_id, id);
    setComponentDataForEntity(BOX_ID_COMPONENT,             new_id, box_id);
    setComponentDataForEntity(SERIAL_VERSION_UID_COMPONENT, new_id, get_AnalogInputValue_uid());
    setComponentDataForEntity(USER_DATA_COMPONENT, new_id, ANALOG_ACT_VALUE_COMPONENT);
    setComponentDataForEntity(METADATA_COMPONENT, new_id, QVariant());


    if(get_all_tcpBoxes().contains(box_id))
        addChild(box_id, new_id);
    //setParent(new_id, box_id);

    if(entity < 0)
        emit newAnalogInputValue(new_id);
    return new_id;

}

int EntityManager::createNewAnalogOutputValue(QString nameVal, QString unitsVal, double smin, double smax, double setVal, int id, int box_id, int entity)
{
    int new_id = entity;
    if(entity <0)
        new_id = createNewEntity();

    createComponentAndAddTo( NAME_COMPONENT, new_id );
    createComponentAndAddTo( UNITS_COMPONENT, new_id );
    createComponentAndAddTo( SET_MIN_COMPONENT, new_id );
    createComponentAndAddTo( SET_MAX_COMPONENT, new_id );
    //createComponentAndAddTo( ACT_MIN_COMPONENT, new_id );
    //createComponentAndAddTo( ACT_MAX_COMPONENT, new_id );
    createComponentAndAddTo( ANALOG_SET_VALUE_COMPONENT, new_id );
    //createComponentAndAddTo( ANALOG_ACT_VALUE_COMPONENT, new_id );
    createComponentAndAddTo( ID_COMPONENT, new_id );
    createComponentAndAddTo( BOX_ID_COMPONENT, new_id );
    createComponentAndAddTo(SERIAL_VERSION_UID_COMPONENT, new_id);
    createComponentAndAddTo(USER_DATA_COMPONENT, new_id);
    createComponentAndAddTo(METADATA_COMPONENT, new_id);

    setComponentDataForEntity(NAME_COMPONENT,               new_id, nameVal);
    setComponentDataForEntity(UNITS_COMPONENT,              new_id, unitsVal);
    setComponentDataForEntity(SET_MIN_COMPONENT,            new_id, smin);
    setComponentDataForEntity(SET_MAX_COMPONENT,            new_id, smax);
    //setComponentDataForEntity(ACT_MIN_COMPONENT,            new_id, amin);
    //setComponentDataForEntity(ACT_MAX_COMPONENT,            new_id, amax);
    setComponentDataForEntity(ANALOG_SET_VALUE_COMPONENT,   new_id, setVal);
    //setComponentDataForEntity(ANALOG_ACT_VALUE_COMPONENT,   new_id, setVal);
    setComponentDataForEntity(ID_COMPONENT,                 new_id, id);
    setComponentDataForEntity(BOX_ID_COMPONENT,             new_id, box_id);
    setComponentDataForEntity(SERIAL_VERSION_UID_COMPONENT, new_id, get_AnalogOutputValue_uid());
    setComponentDataForEntity(USER_DATA_COMPONENT, new_id, ANALOG_SET_VALUE_COMPONENT);
    setComponentDataForEntity(METADATA_COMPONENT,             new_id, QVariant());

    if(get_all_tcpBoxes().contains(box_id))
        addChild(box_id, new_id);
    //setParent(new_id, box_id);

    if(entity <0)
        emit newAnalogOutputValue(new_id);
    return new_id;
}

int EntityManager::createNewAnalogValue(QString nameVal, QString unitsVal, double smin, double smax, double amin, double amax, double setVal, int id, int box_id)
{
    //QUuid uid("{6ddc030e-2001-4a38-a8ce-57b309f902ff}");
    int new_id = createNewEntity();
    //USER_DATA_COMPONENT
    createComponentAndAddTo( NAME_COMPONENT, new_id );
    createComponentAndAddTo( UNITS_COMPONENT, new_id );
    createComponentAndAddTo( SET_MIN_COMPONENT, new_id );
    createComponentAndAddTo( SET_MAX_COMPONENT, new_id );
    createComponentAndAddTo( ACT_MIN_COMPONENT, new_id );
    createComponentAndAddTo( ACT_MAX_COMPONENT, new_id );
    createComponentAndAddTo( ANALOG_SET_VALUE_COMPONENT, new_id );
    createComponentAndAddTo( ANALOG_ACT_VALUE_COMPONENT, new_id );
    createComponentAndAddTo( ID_COMPONENT, new_id );
    createComponentAndAddTo( BOX_ID_COMPONENT, new_id );
    createComponentAndAddTo(SERIAL_VERSION_UID_COMPONENT, new_id);
    createComponentAndAddTo(USER_DATA_COMPONENT, new_id);
    createComponentAndAddTo(METADATA_COMPONENT, new_id);

    setComponentDataForEntity(NAME_COMPONENT,               new_id, nameVal);
    setComponentDataForEntity(UNITS_COMPONENT,              new_id, unitsVal);
    setComponentDataForEntity(SET_MIN_COMPONENT,            new_id, smin);
    setComponentDataForEntity(SET_MAX_COMPONENT,            new_id, smax);
    setComponentDataForEntity(ACT_MIN_COMPONENT,            new_id, amin);
    setComponentDataForEntity(ACT_MAX_COMPONENT,            new_id, amax);
    setComponentDataForEntity(ANALOG_SET_VALUE_COMPONENT,   new_id, setVal);
    setComponentDataForEntity(ANALOG_ACT_VALUE_COMPONENT,   new_id, setVal);
    setComponentDataForEntity(ID_COMPONENT,                 new_id, id);
    setComponentDataForEntity(BOX_ID_COMPONENT,             new_id, box_id);
    setComponentDataForEntity(SERIAL_VERSION_UID_COMPONENT, new_id, get_AnalogValue_uid());
    setComponentDataForEntity(USER_DATA_COMPONENT, new_id, ANALOG_ACT_VALUE_COMPONENT);
    setComponentDataForEntity(METADATA_COMPONENT,             new_id, QVariant());

    if(get_all_tcpBoxes().contains(box_id))
        addChild(box_id, new_id);
    //setParent(new_id, box_id);

    emit newAnalogValue(new_id);
    return new_id;
}

int EntityManager::createNewDigitalInputValue(QString nameVal, QString unitsVal, int id, int box_id, int entity)
{
    int new_id = entity;
    if(entity <0)
        new_id = createNewEntity();

    createComponentAndAddTo( NAME_COMPONENT, new_id );
    createComponentAndAddTo( UNITS_COMPONENT, new_id );
    //createComponentAndAddTo( DIGITAL_SET_VALUE_COMPONENT, new_id );
    createComponentAndAddTo( DIGITAL_ACT_VALUE_COMPONENT, new_id );
    createComponentAndAddTo( ID_COMPONENT, new_id );
    createComponentAndAddTo( BOX_ID_COMPONENT, new_id );
    createComponentAndAddTo(SERIAL_VERSION_UID_COMPONENT, new_id);
    createComponentAndAddTo(USER_DATA_COMPONENT, new_id);
    createComponentAndAddTo(METADATA_COMPONENT, new_id);

    setComponentDataForEntity(NAME_COMPONENT,               new_id, nameVal);
    setComponentDataForEntity(UNITS_COMPONENT,              new_id, unitsVal);
    //setComponentDataForEntity(DIGITAL_SET_VALUE_COMPONENT,   new_id, setVal);
    setComponentDataForEntity(DIGITAL_ACT_VALUE_COMPONENT,   new_id, 0);
    setComponentDataForEntity(ID_COMPONENT,                 new_id, id);
    setComponentDataForEntity(BOX_ID_COMPONENT,             new_id, box_id);
    setComponentDataForEntity(SERIAL_VERSION_UID_COMPONENT, new_id, get_DigitalInputValue_uid());
    setComponentDataForEntity(USER_DATA_COMPONENT, new_id, DIGITAL_ACT_VALUE_COMPONENT);
    setComponentDataForEntity(METADATA_COMPONENT,                 new_id, QVariant());

    if(get_all_tcpBoxes().contains(box_id))
        addChild(box_id, new_id);
    //setParent(new_id, box_id);

    if(entity <0)
        emit newDigitalInputValue(new_id);
    return new_id;
}

int EntityManager::createNewDigitalOutputValue(QString nameVal, bool setVal, int id, int box_id, int entity)
{
    int new_id = entity;
    if(entity <0)
        new_id = createNewEntity();

    createComponentAndAddTo( NAME_COMPONENT, new_id );
    createComponentAndAddTo( DIGITAL_SET_VALUE_COMPONENT, new_id );
    //createComponentAndAddTo( DIGITAL_ACT_VALUE_COMPONENT, new_id );
    createComponentAndAddTo( ID_COMPONENT, new_id );
    createComponentAndAddTo( BOX_ID_COMPONENT, new_id );
    createComponentAndAddTo(SERIAL_VERSION_UID_COMPONENT, new_id);
    createComponentAndAddTo(USER_DATA_COMPONENT, new_id);
    createComponentAndAddTo(METADATA_COMPONENT, new_id);

    setComponentDataForEntity(NAME_COMPONENT,               new_id, nameVal);
    setComponentDataForEntity(DIGITAL_SET_VALUE_COMPONENT,   new_id, setVal);
    //setComponentDataForEntity(DIGITAL_ACT_VALUE_COMPONENT,   new_id, setVal);
    setComponentDataForEntity(ID_COMPONENT,                 new_id, id);
    setComponentDataForEntity(BOX_ID_COMPONENT,             new_id, box_id);
    setComponentDataForEntity(SERIAL_VERSION_UID_COMPONENT, new_id, get_DigitalOutputValue_uid());
    setComponentDataForEntity(USER_DATA_COMPONENT, new_id, DIGITAL_SET_VALUE_COMPONENT);
    setComponentDataForEntity(METADATA_COMPONENT,                 new_id, QVariant());

    if(get_all_tcpBoxes().contains(box_id))
        addChild(box_id, new_id);
    //setParent(new_id, box_id);

    if(entity <0)
        emit newDigitalOutputValue(new_id);

    return new_id;
}

int EntityManager::createNewDigitalValue(QString nameVal, QString unitsVal, bool setVal, int id, int box_id)
{
    int new_id = createNewEntity();
    createComponentAndAddTo( NAME_COMPONENT, new_id );
    createComponentAndAddTo( UNITS_COMPONENT, new_id );
    createComponentAndAddTo( DIGITAL_SET_VALUE_COMPONENT, new_id );
    createComponentAndAddTo( DIGITAL_ACT_VALUE_COMPONENT, new_id );
    createComponentAndAddTo( ID_COMPONENT, new_id );
    createComponentAndAddTo( BOX_ID_COMPONENT, new_id );
    createComponentAndAddTo(SERIAL_VERSION_UID_COMPONENT, new_id);
    createComponentAndAddTo(USER_DATA_COMPONENT, new_id);
    createComponentAndAddTo(METADATA_COMPONENT, new_id);

    setComponentDataForEntity(NAME_COMPONENT,               new_id, nameVal);
    setComponentDataForEntity(UNITS_COMPONENT,              new_id, unitsVal);
    setComponentDataForEntity(DIGITAL_SET_VALUE_COMPONENT,   new_id, setVal);
    setComponentDataForEntity(DIGITAL_ACT_VALUE_COMPONENT,   new_id, setVal);
    setComponentDataForEntity(ID_COMPONENT,                 new_id, id);
    setComponentDataForEntity(BOX_ID_COMPONENT,             new_id, box_id);
    setComponentDataForEntity(SERIAL_VERSION_UID_COMPONENT, new_id, get_DigitalValue_uid());
    setComponentDataForEntity(USER_DATA_COMPONENT, new_id, DIGITAL_ACT_VALUE_COMPONENT);
    setComponentDataForEntity(METADATA_COMPONENT,                 new_id, QVariant());

    if(get_all_tcpBoxes().contains(box_id))
        addChild(box_id, new_id);
    //setParent(new_id, box_id);

    emit newDigitalValue(new_id);
    return new_id;
}

int EntityManager::createNewVersionInformation()
{
    int new_id = createNewEntity();

    return new_id;

}

EntityManager::ID EntityManager::nextIdFrom(const QList<ID> &container) const
{
    ID next_id = invalid_id;
    foreach (auto e, container){next_id = qMax(next_id, e);}
    next_id++;
    return next_id;
}

EntityManager::ComponentID EntityManager::nextComponentId() const
{
    ID next_id = nextIdFrom(m_components.keys());
    if(next_id>max_component_id)
        next_id = invalid_id;
    return next_id;
}

int EntityManager::createNewDigitalValue(QString nameVal, bool setVal, int id, int box_id)
{

    //QUuid uid("{837c326e-e5fb-4271-97e8-8a3161cfc02c}");
    int new_id = createNewEntity();
    createComponentAndAddTo( NAME_COMPONENT, new_id );
    createComponentAndAddTo( DIGITAL_ACT_VALUE_COMPONENT, new_id );
    createComponentAndAddTo( DIGITAL_SET_VALUE_COMPONENT, new_id );
    createComponentAndAddTo( ID_COMPONENT, new_id );
    createComponentAndAddTo( BOX_ID_COMPONENT, new_id );
    createComponentAndAddTo(SERIAL_VERSION_UID_COMPONENT, new_id);
    createComponentAndAddTo(USER_DATA_COMPONENT, new_id);
    createComponentAndAddTo(METADATA_COMPONENT, new_id);


    setComponentDataForEntity(NAME_COMPONENT,               new_id, nameVal);
    setComponentDataForEntity(DIGITAL_SET_VALUE_COMPONENT,  new_id, setVal);
    setComponentDataForEntity(DIGITAL_ACT_VALUE_COMPONENT,  new_id, setVal);
    setComponentDataForEntity(ID_COMPONENT,                 new_id, id);
    setComponentDataForEntity(BOX_ID_COMPONENT,             new_id, box_id);
    setComponentDataForEntity(SERIAL_VERSION_UID_COMPONENT, new_id, get_DigitalValue_uid());
    setComponentDataForEntity(USER_DATA_COMPONENT, new_id, DIGITAL_ACT_VALUE_COMPONENT);
    setComponentDataForEntity(METADATA_COMPONENT,                 new_id, QVariant());

    if(get_all_tcpBoxes().contains(box_id))
        addChild(box_id, new_id);
    //setParent(new_id, box_id);

    emit newDigitalValue(new_id);
    return new_id;
}

EntityManager::EntityID EntityManager::tcpBoxExists(QString serverName, quint16 serverPort)
{
    auto boxes = EntityManager::get_instance()->get_all_tcpBoxes();
    foreach (auto box, boxes) {
        auto host = EntityManager::get_instance()->componentDataForEntity(IP_ADDRESS_COMPONENT, box);
        auto port = EntityManager::get_instance()->componentDataForEntity(PORT_COMPONENT, box);

        if(serverName==host && serverPort == port)
            return box;
    }
    return EntityManager::invalid_id;
}

int EntityManager::createNewTcpBox(QString nameVal, QString ip, quint16 port)
{
    auto box = tcpBoxExists(ip, port);
    if(EntityManager::isValid(box))
        return box;

    //QUuid uid("{99060fb8-676f-47d8-b9f1-c9c492721009}");
    int new_id = createNewEntity();
    createComponentAndAddTo( NAME_COMPONENT, new_id );
    createComponentAndAddTo( BOX_ID_COMPONENT, new_id );
    createComponentAndAddTo( IP_ADDRESS_COMPONENT, new_id );
    createComponentAndAddTo( PORT_COMPONENT, new_id );
    createComponentAndAddTo( CONNECTION_STATE_COMPONENT, new_id );
    createComponentAndAddTo( SERIAL_VERSION_UID_COMPONENT, new_id);
    createComponentAndAddTo( TCP_BOX_BACKEND_COMPONENT, new_id);
    createComponentAndAddTo( AUTO_RECONNECT_COMPONENT, new_id);
    createComponentAndAddTo( METADATA_COMPONENT, new_id);

    setComponentDataForEntity(NAME_COMPONENT,               new_id, nameVal);
    setComponentDataForEntity(BOX_ID_COMPONENT,             new_id, new_id);
    setComponentDataForEntity(IP_ADDRESS_COMPONENT,         new_id, ip);
    setComponentDataForEntity(PORT_COMPONENT,               new_id, port);
    setComponentDataForEntity(CONNECTION_STATE_COMPONENT,   new_id, false);
    setComponentDataForEntity(SERIAL_VERSION_UID_COMPONENT, new_id, get_TcpBox_uid());
    setComponentDataForEntity(TCP_BOX_BACKEND_COMPONENT, new_id, 0);
    setComponentDataForEntity(AUTO_RECONNECT_COMPONENT, new_id, false);
    setComponentDataForEntity(METADATA_COMPONENT, new_id, QVariant());
    //setComponentDataForEntity(TCP_BOX_BACKEND_COMPONENT, new_id, -1);

    emit newTcpBox(new_id);
    return new_id;
}

int EntityManager::createQtScriptEngine(QString nameVal, QScriptEngine *engine)
{
    QList<int> list =get_all_Qt_Script_Engines(); // allow only one QtScriptEngine
    int new_id = -1;
    if(list.empty()){
        new_id = createNewEntity();
        createComponentAndAddTo( NAME_COMPONENT, new_id );
        createComponentAndAddTo(SERIAL_VERSION_UID_COMPONENT, new_id);
        if(engine)
            createComponentAndAddTo(QT_SCRIPT_ENGINE_COMPONENT, new_id);
        createComponentAndAddTo(SCRIPT_FILE_COMPONENT, new_id);

        //keep the FILE
        setComponentDataForEntity(SCRIPT_FILE_COMPONENT, new_id, QString(""));
    }else{
        new_id = list[0];
    }

    setComponentDataForEntity(NAME_COMPONENT,               new_id, nameVal);
    setComponentDataForEntity(SERIAL_VERSION_UID_COMPONENT, new_id, get_Qt_Script_Engine_uid());
    if(engine)
        set_qt_script_engine_component(new_id, engine);

    emit newQtScriptEngine(new_id);
    return new_id;
}

int EntityManager::createTofDaqDll(QString nameVal, QString pathVal, int timeout, int updaterate, QString user_data_path)
{
    int new_id = createNewEntity();
    createComponentAndAddTo( NAME_COMPONENT, new_id );
    createComponentAndAddTo( DATA_AQUISITION_DLL_COMPONENT, new_id );
    createComponentAndAddTo(SERIAL_VERSION_UID_COMPONENT, new_id);
    createComponentAndAddTo(TIMEOUT_COMPONENT, new_id);
    createComponentAndAddTo(UPDATERATE_COMPONENT, new_id);
    createComponentAndAddTo(USER_DATA_PATH_COMPONENT, new_id);

    setComponentDataForEntity(NAME_COMPONENT,               new_id, nameVal);
    setComponentDataForEntity(DATA_AQUISITION_DLL_COMPONENT,               new_id, pathVal);
    setComponentDataForEntity(SERIAL_VERSION_UID_COMPONENT, new_id, get_TofDaqDll_uid());
    setComponentDataForEntity(TIMEOUT_COMPONENT, new_id, timeout);
    setComponentDataForEntity(UPDATERATE_COMPONENT, new_id, updaterate);
    setComponentDataForEntity(USER_DATA_PATH_COMPONENT, new_id, user_data_path);

    emit newTofDaqDll(new_id);
    return new_id;
}

int EntityManager::create_scriptable_usr_data(QString nameVal, double setVal)
{
    int new_id = createNewEntity();
    createComponentAndAddTo( NAME_COMPONENT, new_id );
    createComponentAndAddTo(USER_DATA_COMPONENT, new_id);
    createComponentAndAddTo( ANALOG_ACT_VALUE_COMPONENT, new_id );
    createComponentAndAddTo(SERIAL_VERSION_UID_COMPONENT, new_id);

    setComponentDataForEntity(NAME_COMPONENT,               new_id, nameVal);
    setComponentDataForEntity(USER_DATA_COMPONENT, new_id, ANALOG_ACT_VALUE_COMPONENT);
    setComponentDataForEntity(ANALOG_ACT_VALUE_COMPONENT,  new_id, setVal);
    setComponentDataForEntity(SERIAL_VERSION_UID_COMPONENT, new_id, get_ScriptableUsrData_uid());

    return new_id;
}

int EntityManager::createScriptableObject(QString nameVal, abstract_scriptable_object *comp)
{
    int new_id = createNewEntity();
    createComponentAndAddTo( NAME_COMPONENT, new_id );
    createComponentAndAddTo( SCRIPTABLE_OBJECT_COMPONENT, new_id );
    createComponentAndAddTo(SERIAL_VERSION_UID_COMPONENT, new_id);

    //if(nameVal.isEmpty())
    //    nameVal = comp

    setComponentDataForEntity(NAME_COMPONENT,               new_id, nameVal);
    set_scriptable_object_component(new_id, comp);
    setComponentDataForEntity(SERIAL_VERSION_UID_COMPONENT, new_id, get_ScriptableObject_uid());

    emit newScriptableObject(new_id);
    return new_id;
}

int EntityManager::createQuickScript(QString nameVal, QString path)
{
    int new_id = createNewEntity();
    createComponentAndAddTo( NAME_COMPONENT, new_id );
    createComponentAndAddTo(USER_DATA_PATH_COMPONENT, new_id);
    createComponentAndAddTo(SERIAL_VERSION_UID_COMPONENT, new_id);

    setComponentDataForEntity(NAME_COMPONENT,               new_id, nameVal);
    setComponentDataForEntity(USER_DATA_PATH_COMPONENT, new_id, path);
    setComponentDataForEntity(SERIAL_VERSION_UID_COMPONENT, new_id, get_QuickScript_uid());

    emit newQuickScript(new_id);

    return new_id;
}

bool EntityManager::is_analog_value(int entity) const
{
    const QUuid uid=  get_serialVersionUID_component(entity);

    return ((uid == get_AnalogInputValue_uid()) || (uid == get_AnalogOutputValue_uid()) || (uid == get_AnalogValue_uid()) );
}

bool EntityManager::is_digital_value(int entity) const
{
    const QUuid uid=  get_serialVersionUID_component(entity);

    return ((uid == get_DigitalInputValue_uid()) || (uid == get_DigitalOutputValue_uid()) || (uid == get_DigitalValue_uid()) );
}

QList<int> EntityManager::get_all_Values() const
{
    QList<int> tmp= get_all_AnalogInputValues();
    tmp += get_all_AnalogOutputValues();
    tmp += get_all_AnalogValues();

    return tmp;
}

QList<int> EntityManager::get_all_DValues() const
{
    QList<int> tmp= get_all_DigitalInputValues();
    tmp += get_all_DigitalOutputValues();
    tmp += get_all_DigitalValues();

    return tmp;
}

//    int entity_manager::add_scriptable_component(int entity, abstract_scriptable_object *comp)
//    {
//        if(!comp)
//            return -1;

//        if(!entity_exists(entity))
//            return -1;

//        if(hasComponent(SCRIPTABLE_OBJECT_COMPONENT, entity))
//            return -1;

//        createComponentAndAddTo( SCRIPTABLE_OBJECT_COMPONENT, entity );
//        set_scriptable_object_component(entity, comp);
//        emit register_scritable_component(entity);
//        return 0;
//    }

//    int entity_manager::remove_scriptable_component(int entity)
//    {
//        if(!entity_exists(entity))
//            return -1;

//        if(!hasComponent(SCRIPTABLE_OBJECT_COMPONENT, entity))
//            return -1;

//        removeComponentFrom(SCRIPTABLE_OBJECT_COMPONENT, entity);
//        emit unregister_scritable_component(entity);
//        return 0;

//    }

QVector<double>& EntityManager::doubleVectorDataForEntity(ComponentID component_id, EntityID entity_id) const{
    ComponentDataID component_data_id = getcomponentDataId(entity_id,component_id);
    auto ret = m_componentDoubleVectorDataTable.value(component_data_id, Q_NULLPTR);
    if(ret == Q_NULLPTR)
        return invalid_doubleVector_data;
    return *ret;
}

EntityManager::EntityList& EntityManager::entityListDataForEntity(ComponentID component_id, EntityID entity_id) const{
    ComponentDataID component_data_id = getcomponentDataId(entity_id,component_id);
    auto ret = m_componentEntityListDataTable.value(component_data_id, Q_NULLPTR);
    if(ret == Q_NULLPTR)
        return invalid_entityList_data;
    return *ret;
}

abstract_configuration_widget *EntityManager::get_systemConfigurationWidget_component(int entity_id) const
{
    int component_id = SYSTEM_CONFIGURAION_WIDGET_COMPONENT;
    auto component_data_id = getcomponentDataId(component_id, entity_id );
    QMap<int, abstract_configuration_widget*>::ConstIterator av = m_component_widget_table.constFind(component_data_id);
    if (av == m_component_widget_table.constEnd()){
        return NULL;
    }

    return av.value();
}

QScriptEngine *EntityManager::get_qt_script_engine_component(int entity_id) const
{
    int component_id = QT_SCRIPT_ENGINE_COMPONENT;
    auto component_data_id = getcomponentDataId(component_id, entity_id );
    QMap<int, QScriptEngine*>::ConstIterator av = m_component_script_engine_table.constFind(component_data_id);
    if (av == m_component_script_engine_table.constEnd()){
        return NULL;
    }

    return av.value();
}

abstract_scriptable_object *EntityManager::get_scriptable_object_component(int entity_id) const
{
    int component_id = SCRIPTABLE_OBJECT_COMPONENT;
    auto component_data_id = getcomponentDataId(component_id, entity_id );
    QMap<int, abstract_scriptable_object*>::ConstIterator av = m_component_scriptable_object_table.constFind(component_data_id);
    if (av == m_component_scriptable_object_table.constEnd()){
        return NULL;
    }

    return av.value();
}

void EntityManager::set_systemConfigurationWidget_component(int entity_id, abstract_configuration_widget *wdgt)
{
    if(!wdgt)
        return;

    int component_id = SYSTEM_CONFIGURAION_WIDGET_COMPONENT;
    QWidget* tmp = get_systemConfigurationWidget_component(entity_id);

    if(wdgt == tmp)
        return;

    auto component_data_id = getcomponentDataId(component_id, entity_id );
    m_component_widget_table.insert(component_data_id, wdgt);
    emit component_changed(entity_id, component_id);
}

void EntityManager::set_qt_script_engine_component(int entity_id, QScriptEngine *engine)
{
    if(!engine)
        return;

    int component_id = QT_SCRIPT_ENGINE_COMPONENT;
    QScriptEngine* tmp = get_qt_script_engine_component(entity_id);

    if(engine == tmp)
        return;

    auto component_data_id = getcomponentDataId(component_id, entity_id );
    m_component_script_engine_table.insert(component_data_id, engine);
    emit component_changed(entity_id, component_id);
}

void EntityManager::set_scriptable_object_component(int entity_id, abstract_scriptable_object *engine)
{
    if(!engine)
        return;

    int component_id = SCRIPTABLE_OBJECT_COMPONENT;
    abstract_scriptable_object* tmp = get_scriptable_object_component(entity_id);

    if(engine == tmp)
        return;

    auto component_data_id = getcomponentDataId(component_id, entity_id );
    m_component_scriptable_object_table.insert(component_data_id, engine);
    emit component_changed(entity_id, component_id);
}



QList<int> EntityManager::get_entity_by_name(const QString &name) const
{
    QList<int> ecs;
    QMap<int, entity_t>::const_iterator it= m_entities.constBegin();
    for(it = m_entities.constBegin(); it !=m_entities.constEnd(); ++it){
        if(QString::compare(get_name_component(it.key()), name)==0){
            //return it.key();
            ecs.append(it.key());
        }
    }
    return ecs;

}

EntityManager::EntityList EntityManager::entitiesByComponentsValue(EntityManager::ComponentID component_id,  EntityManager::ComponentData component_data) const
{
    QList<EntityManager::EntityID> ents;
    auto tmp_cont=allEntities();
    foreach (EntityID id, tmp_cont) {
        if(entityHasComponent(id, component_id)){
            //auto data = componentDataForEntity(id,component_id);
            auto data = componentDataForEntity(component_id, id);
            if(data == component_data){
                ents<<id;
            }
        }
    }

    return ents;
}

QList<int> EntityManager::get_entity_by_serialVersionUID(const QUuid &uid) const
{
    QList<int> ecs;
    QMap<int, entity_t>::const_iterator it= m_entities.constBegin();
    for(it = m_entities.constBegin(); it !=m_entities.constEnd(); ++it){
        if(get_serialVersionUID_component(it.key()) == uid){
            ecs.append(it.key());
        }
    }
    return ecs;
}

QList<int> EntityManager::get_entity_by_systemVersionUID(const QUuid &uid) const
{
    QList<int> ecs;
    QMap<int, entity_t>::const_iterator it= m_entities.constBegin();
    for(it = m_entities.constBegin(); it !=m_entities.constEnd(); ++it){
        if(get_systemVersionUID_component(it.key()) == uid){
            ecs.append(it.key());
        }
    }
    return ecs;
}

QList<int> EntityManager::get_entity_by_component(int comp_id) const
{
    QList<int> ecs;
    QMap<int, entity_t>::const_iterator it= m_entities.constBegin();
    for(it = m_entities.constBegin(); it !=m_entities.constEnd(); ++it){
        if(hasComponent(comp_id,it.key())){
            ecs.append(it.key());
        }
    }
    return ecs;
}

QList<int> EntityManager::allEntities() const
{
    QList<int> ecs;
    QMap<int, entity_t>::const_iterator ecit;
    for(ecit=m_entities.constBegin(); ecit !=m_entities.constEnd(); ++ecit){
        ecs.append(ecit.key());
    }
    return ecs;
}
EntityManager::ComponentList EntityManager::allComponents() const{
    return m_components.keys();
}


QList<int> EntityManager::get_all_components_of_entity(int entity) const
{
    QList<int> ecs;
    QMap<int, entity_components_t>::const_iterator ecit;
    for(ecit=m_entity_components.constBegin(); ecit !=m_entity_components.constEnd(); ++ecit){
        if(ecit.value().entity_id == entity)
            ecs.append(ecit.value().component_id);
    }
    return ecs;
}

EntityManager::EntityList &EntityManager::getChildren(EntityManager::EntityID entity_id) const
{
    return entityListDataForEntity(CHILDREN_COMPONENT, entity_id);
}

bool EntityManager::entiyHasChildren(EntityManager::EntityID entity_id) const{
    return entityHasComponent(entity_id, CHILDREN_COMPONENT);
}

bool EntityManager::entiyHasParent(EntityManager::EntityID entity_id) const{
    return entityHasComponent(entity_id, PARENT_COMPONENT);
}

void EntityManager::addChildren(EntityManager::EntityID entity_id, EntityList &children)
{
    foreach (auto child, children) {
        addChild(entity_id, child);
    }
}

void EntityManager::addChild(EntityManager::EntityID entity_id, EntityManager::EntityID &child)
{
    if(!entiyHasChildren(entity_id)){
        EntityList* children = new EntityList();
        createComponentAndAddTo(CHILDREN_COMPONENT, entity_id, true);
        setComponentDataForEntity(CHILDREN_COMPONENT, entity_id, children, true);
    }

    EntityList& act_children = getChildren(entity_id);
    act_children.append(child);
    setComponentDataForEntity(CHILDREN_COMPONENT, entity_id, act_children);
    setParent(child, entity_id);

}

void EntityManager::removeChildren(EntityManager::EntityID entity_id, EntityList children)
{
    if(entiyHasChildren(entity_id)){
        EntityList& act_children = getChildren(entity_id);
        foreach (auto child, children) {
            removeParent(child);
            act_children.removeAll(child);
        }
        setComponentDataForEntity(CHILDREN_COMPONENT, entity_id, act_children);
    }
}

void EntityManager::removeChild(EntityManager::EntityID parent_entity_id, EntityManager::EntityID child_entity_id)
{
    if(entiyHasChildren(parent_entity_id)){
        EntityList& act_children = getChildren(parent_entity_id);
        if(act_children.removeAll(child_entity_id)>0)
            removeParent(child_entity_id);
        setComponentDataForEntity(CHILDREN_COMPONENT, parent_entity_id, act_children, true);
    }
}

void EntityManager::setParent(EntityManager::EntityID child_entity_id, EntityManager::EntityID parent_entity_id)
{
    if(entiyHasParent(child_entity_id)){
        removeChild(parent_entity_id, child_entity_id);
    }
    createComponentAndAddTo(PARENT_COMPONENT, child_entity_id, true);
    auto par=QVariant::fromValue(parent_entity_id);
    setComponentDataForEntity(PARENT_COMPONENT, child_entity_id, par, true);
}

void EntityManager::removeParent(EntityManager::EntityID child_entity_id)
{
    if(entiyHasParent(child_entity_id)){
        auto ent_parent= getParent(child_entity_id);
        removeComponentFromEntity(PARENT_COMPONENT, child_entity_id);
        removeChild(ent_parent, child_entity_id);
    }
}

EntityManager::EntityID EntityManager::getParent(EntityManager::EntityID child_entity_id)
{
    auto parvar= componentDataForEntity(PARENT_COMPONENT, child_entity_id);
    EntityManager::EntityID ret = invalid_id;
    if(parvar.canConvert<EntityID>())
        ret = parvar.value<EntityID>();
    return ret;
}

void EntityManager::set_analogActValue_component(int entity, double val){setComponentDataForEntity(ANALOG_ACT_VALUE_COMPONENT, entity, val);}

void EntityManager::set_analogSetValue_component(int entity, double val){setComponentDataForEntity(ANALOG_SET_VALUE_COMPONENT, entity, val);}

void EntityManager::set_digitalActValue_component(int entity, bool val){setComponentDataForEntity(DIGITAL_ACT_VALUE_COMPONENT, entity, val);}

void EntityManager::set_digitalSetValue_component(int entity, bool val){setComponentDataForEntity(DIGITAL_SET_VALUE_COMPONENT, entity, val);}

void EntityManager::set_setMin_component(int entity, double val){setComponentDataForEntity(SET_MIN_COMPONENT, entity, val);}

void EntityManager::set_setMax_component(int entity, double val){setComponentDataForEntity(SET_MAX_COMPONENT, entity, val);}

void EntityManager::set_actMin_component(int entity, double val){setComponentDataForEntity(ACT_MIN_COMPONENT, entity, val);}

void EntityManager::set_actMax_component(int entity, double val){setComponentDataForEntity(ACT_MAX_COMPONENT, entity, val);}

void EntityManager::set_name_component(int entity, QString val){setComponentDataForEntity(NAME_COMPONENT, entity, val);}

void EntityManager::set_units_component(int entity, QString val){setComponentDataForEntity(UNITS_COMPONENT, entity, val);}

void EntityManager::set_id_component(int entity, int val){setComponentDataForEntity(ID_COMPONENT, entity, val);}

void EntityManager::set_ip_address_component(int entity, QString val) {setComponentDataForEntity(IP_ADDRESS_COMPONENT, entity, val);}

void EntityManager::set_port_component(int entity, quint16 val){setComponentDataForEntity(PORT_COMPONENT, entity, val);}

void EntityManager::set_box_id_component(int entity, int val) {setComponentDataForEntity(BOX_ID_COMPONENT, entity, val);}

void EntityManager::set_connection_state_component(int entity, bool val){setComponentDataForEntity(CONNECTION_STATE_COMPONENT, entity, val);}

void EntityManager::set_serialVersionUID_component(int entity, QUuid val){setComponentDataForEntity(SERIAL_VERSION_UID_COMPONENT, entity, val);}

void EntityManager::set_tcp_box_backend_component(int entity, int val){setComponentDataForEntity(TCP_BOX_BACKEND_COMPONENT, entity, val);}

void EntityManager::set_script_file_component(int entity, QString val){setComponentDataForEntity(SCRIPT_FILE_COMPONENT, entity, val);}

void EntityManager::set_data_aquistion_dll_component(int entity, QString val){setComponentDataForEntity(DATA_AQUISITION_DLL_COMPONENT, entity, val);}

void EntityManager::set_timeout_component(int entity, int val){setComponentDataForEntity(TIMEOUT_COMPONENT, entity, val);}

void EntityManager::set_updaterate_component(int entity, int val){setComponentDataForEntity(UPDATERATE_COMPONENT, entity, val);}

void EntityManager::set_user_data_path_component(int entity, QString val){setComponentDataForEntity(USER_DATA_PATH_COMPONENT, entity, val);}

void EntityManager::setComponentDataForEntity(ComponentID component_id, EntityID entity_id,  EntityList &component_data, bool ignore_exclusive){
    if(!ignore_exclusive && componentIsExclusive(component_id))
        return;
    ComponentDataID component_data_id = getcomponentDataId(entity_id,component_id);
    setData(component_data_id, &component_data);
}

QString EntityManager::get_name_component(int entity) const{return componentDataForEntity(NAME_COMPONENT, entity).toString();}

QString EntityManager::get_units_component(int entity) const{return componentDataForEntity(UNITS_COMPONENT, entity).toString();}

QString EntityManager::get_ip_address_component(int entity) const{return componentDataForEntity(IP_ADDRESS_COMPONENT, entity).toString();}

quint16 EntityManager::get_port_component(int entity) const{return componentDataForEntity(PORT_COMPONENT, entity).toUInt();}

int EntityManager::get_box_id_component(int entity) const{return componentDataForEntity(BOX_ID_COMPONENT, entity).toInt();}

bool EntityManager::get_connection_state_component(int entity) const{return componentDataForEntity(CONNECTION_STATE_COMPONENT, entity).toBool();}

QUuid EntityManager::get_serialVersionUID_component(int entity) const{return componentDataForEntity(SERIAL_VERSION_UID_COMPONENT, entity).toUuid();}

QUuid EntityManager::get_systemVersionUID_component(int entity) const{return componentDataForEntity(SYSTEM_VERSION_UID_COMPONENT, entity).toUuid();}

int EntityManager::get_tcp_box_backend_component(int entity) const{return componentDataForEntity(TCP_BOX_BACKEND_COMPONENT, entity).toInt();}

QString EntityManager::get_script_file_component(int entity) const{return componentDataForEntity(SCRIPT_FILE_COMPONENT, entity).toString();}

QString EntityManager::get_data_aquistion_dll_component(int entity) const{return componentDataForEntity(DATA_AQUISITION_DLL_COMPONENT, entity).toString();}

int EntityManager::get_timeout_component(int entity) const{return componentDataForEntity(TIMEOUT_COMPONENT, entity).toInt();}

int EntityManager::get_updaterate_component(int entity) const{return componentDataForEntity(UPDATERATE_COMPONENT, entity).toInt();}

int EntityManager::get_userdata_component(int entity) const{return componentDataForEntity(USER_DATA_COMPONENT, entity).toInt();}

QString EntityManager::get_user_data_path_component(int entity) const{return componentDataForEntity(USER_DATA_PATH_COMPONENT, entity).toString();}

QVariant EntityManager::get_metadata_component(EntityManager::EntityID entity) const{return componentDataForEntity(METADATA_COMPONENT, entity);}

double EntityManager::get_analogActValue_component(int entity) const{return componentDataForEntity(ANALOG_ACT_VALUE_COMPONENT, entity).toDouble();}

double EntityManager::get_analogSetValue_component(int entity) const{return componentDataForEntity(ANALOG_SET_VALUE_COMPONENT, entity).toDouble();}

bool EntityManager::get_digitalActValue_component(int entity) const{return componentDataForEntity(DIGITAL_ACT_VALUE_COMPONENT, entity).toDouble();}

bool EntityManager::get_digitalSetValue_component(int entity) const{return componentDataForEntity(DIGITAL_SET_VALUE_COMPONENT, entity).toDouble();}

double EntityManager::get_setMin_component(int entity) const{return componentDataForEntity(SET_MIN_COMPONENT, entity).toDouble();}

double EntityManager::get_setMax_component(int entity) const{return componentDataForEntity(SET_MAX_COMPONENT, entity).toDouble();}

double EntityManager::get_actMin_component(int entity) const{return componentDataForEntity(ACT_MIN_COMPONENT, entity).toDouble();}

double EntityManager::get_actMax_component(int entity) const{return componentDataForEntity(ACT_MAX_COMPONENT, entity).toDouble();}

int EntityManager::get_id_component(int entity) const{return componentDataForEntity(ID_COMPONENT, entity).toInt();}

void EntityManager::slot_change_analogAct_component(int entity, double value)
{
    if( get_analogActValue_component(entity)!= value){
        set_analogActValue_component(entity, value);
        emit analogAct_component_changed(entity);
    }
}

void EntityManager::slot_change_analogSet_component(int entity, double value)
{
    //QObject* snd = sender();
    //qDebug()<<"entity_manager::slot_change_analogSet_component"<<snd<<value;
    if( get_analogSetValue_component(entity)!= value){

        set_analogSetValue_component(entity, value);
        emit analogSet_component_changed(entity);
    }
}

void EntityManager::slot_change_digitalAct_component(int entity, bool value)
{
    if( get_digitalActValue_component(entity)!= value){

        set_digitalActValue_component(entity, value);
        emit digitalAct_component_changed(entity);
    }
}

void EntityManager::slot_change_digitalSet_component(int entity, bool value)
{
    if( get_digitalSetValue_component(entity)!= value){
        set_digitalSetValue_component(entity, value);
        emit digitalSet_component_changed(entity);
    }
}

void EntityManager::slot_change_script_file_component(int entity, QString value)
{
    if( get_script_file_component(entity)!= value){
        set_script_file_component(entity, value);
        emit script_file_component_changed(entity);
    }
}

void EntityManager::slot_change_data_aquistion_dll_component(int entity, QString value)
{
    if( get_data_aquistion_dll_component(entity)!= value){
        set_data_aquistion_dll_component(entity, value);
        emit data_aquistion_dll_component_changed(entity);
    }
}

void EntityManager::slot_change_timeout_component(int entity, int value)
{
    if( get_timeout_component(entity)!= value){
        set_timeout_component(entity, value);
        emit timeout_component_changed(entity);
    }
}

void EntityManager::slot_change_updaterate_component(int entity, int value)
{
    if( get_updaterate_component(entity)!= value){
        set_updaterate_component(entity, value);
        emit updaterate_component_changed(entity);
    }
}
void EntityManager::slotChangeComponentDataForEntity(EntityManager::ComponentID component_id, EntityManager::EntityID entity_id,  const EntityManager::ComponentData &component_data){
    setComponentDataForEntity(component_id, entity_id, component_data);
}

void EntityManager::slotEntityRemoved(EntityManager::EntityID id, QUuid serialVersionUid)
{
    if(serialVersionUid == get_TcpBox_uid())
        emit tcpBoxRemoved(id);

    if(serialVersionUid == get_AnalogInputValue_uid())
        emit removedAnalogInputValue(id);

    if(serialVersionUid == get_AnalogOutputValue_uid())
        emit removedAnalogOutputValue(id);

    if(serialVersionUid == get_AnalogValue_uid())
        emit removedAnalogValue(id);

    if(serialVersionUid == get_DigitalInputValue_uid())
        emit removedDigitalInputValue(id);

    if(serialVersionUid == get_DigitalOutputValue_uid())
        emit removedDigitalOutputValue(id);

    if(serialVersionUid == get_DigitalValue_uid())
        emit removedDigitalValue(id);

}

void EntityManager::slot_change_userDataPath_component(int entity, QString value)
{
    if( get_user_data_path_component(entity)!= value){
        set_user_data_path_component(entity, value);
        emit user_data_path_changed(entity);
    }
}

void EntityManager::slot_change_metadata_component(EntityManager::EntityID entity, QVariant value)
{
    if(value.isNull())
        return;

    if(value.isValid() == false)
        return;

    if(get_metadata_component(entity) == value)
        return;

    setComponentDataForEntity(METADATA_COMPONENT, entity, value);
    emit ManagerLocator::eventManager().metadataChanged(entity);

}

void EntityManager::slot_change_name_component(int entity, QString value)
{
    if( get_name_component(entity)!= value){
        set_name_component(entity, value);
        emit name_component_changed(entity);
    }
}

void EntityManager::slot_change_ip_address_component(int entity, QString value)
{
    if( get_ip_address_component(entity)!= value){
        set_ip_address_component(entity, value);
        //emit analogAct_component_changed(entity);
    }
}

void EntityManager::slot_change_port_component(int entity, quint16 value)
{
    if( get_port_component(entity)!= value){
        set_port_component(entity, value);
        //emit analogAct_component_changed(entity);
    }
}

void EntityManager::slot_connection_state_component(int entity, bool value)
{
    if( get_connection_state_component(entity)!= value){
        set_connection_state_component(entity, value);
        emit connection_state_component_changed(entity);
    }
}

void EntityManager::slot_change_tcp_box_backend_component(int entity, int value)
{
    if( get_tcp_box_backend_component(entity)!= value){
        set_tcp_box_backend_component(entity, value);
        //emit connection_state_component_changed(entity);
    }
}






} // namespace talorion


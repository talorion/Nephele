#include "entity_manager.hpp"

#include <QAtomicPointer>
#include <QMutex>
#include <QDebug>
#include <QSignalMapper>
#include <QVariant>
#include <QSettings>

#include "core/event_manager.hpp"
#include "abstract_configuration_widget.hpp"
//#include "analogvalue.hpp"

#include "version.hpp"

namespace talorion {

    //QAtomicPointer<entity_manager> entity_manager::_instance;
    entity_manager* entity_manager::_instance;
    QMutex entity_manager::_mutex;

    entity_manager::entity_manager(QObject *par) :
        QObject(par),
        current_identity_id(0),
        components(),
        entities(),
        entity_components(),
        component_data_table_N(),
        component_widget_table(),
        component_script_engine_table(),
        component_scriptable_object_table()
    {
        connect(this,SIGNAL(newSystem(int)),event_manager::get_instance(),SIGNAL(newSystem(int)));

        connect(this,SIGNAL(connection_state_component_changed(int)),event_manager::get_instance(),SIGNAL(connection_state_component_changed(int)));

        connect(this, SIGNAL(user_data_path_changed(int)), event_manager::get_instance(), SIGNAL(user_data_path_component_changed(int)));
        connect(this, SIGNAL(analogAct_component_changed(int)), event_manager::get_instance(), SIGNAL(analogAct_component_changed(int)));
        connect(this, SIGNAL(analogSet_component_changed(int)), event_manager::get_instance(), SIGNAL(analogSet_component_changed(int)));
        connect(this, SIGNAL(digitalAct_component_changed(int)), event_manager::get_instance(), SIGNAL(digitalAct_component_changed(int)));
        connect(this, SIGNAL(digitalSet_component_changed(int)), event_manager::get_instance(), SIGNAL(digitalSet_component_changed(int)));
        connect(this,SIGNAL(name_component_changed(int)),event_manager::get_instance(),SIGNAL(name_component_changed(int)));
        connect(this,SIGNAL(script_file_component_changed(int)),event_manager::get_instance(),SIGNAL(script_file_component_changed(int)));
        connect(this, SIGNAL(data_aquistion_dll_component_changed(int)),event_manager::get_instance(),SIGNAL(data_aquistion_dll_component_changed(int)));
        connect(this, SIGNAL(timeout_component_changed(int)),event_manager::get_instance(),SIGNAL(timeout_component_changed(int)));
        connect(this, SIGNAL(updaterate_component_changed(int)),event_manager::get_instance(),SIGNAL(updaterate_component_changed(int)));
        //connect(this, SIGNAL(register_scritable_component(int)),event_manager::get_instance(),SIGNAL(register_scritable_component(int)));
        //connect(this, SIGNAL(unregister_scritable_component(int)),event_manager::get_instance(),SIGNAL(unregister_scritable_component(int)));
        connect(this, SIGNAL(newScriptableObject(int)),event_manager::get_instance(),SIGNAL(newScriptableObject(int)));

        connect(event_manager::get_instance(),SIGNAL(change_analogAct_component(int,double)),this,SLOT(slot_change_analogAct_component(int,double)),Qt::UniqueConnection);
        connect(event_manager::get_instance(),SIGNAL(change_analogSet_component(int,double)),this,SLOT(slot_change_analogSet_component(int,double)),Qt::UniqueConnection);
        connect(event_manager::get_instance(),SIGNAL(change_digitalAct_component(int,bool)),this,SLOT(slot_change_digitalAct_component(int,bool)),Qt::UniqueConnection);
        connect(event_manager::get_instance(),SIGNAL(change_digitalSet_component(int,bool)),this,SLOT(slot_change_digitalSet_component(int,bool)),Qt::UniqueConnection);
        connect(event_manager::get_instance(),SIGNAL(change_script_file_component(int,QString)),this,SLOT(slot_change_script_file_component(int,QString)));
        connect(event_manager::get_instance(),SIGNAL(change_data_aquistition_dll_component(int,QString)),this,SLOT(slot_change_data_aquistion_dll_component(int,QString)));
        connect(event_manager::get_instance(),SIGNAL(change_user_data_path_component(int,QString)),this,SLOT(slot_change_userDataPath_component(int,QString)));
        connect(event_manager::get_instance(),SIGNAL(change_timeout_component(int,int)),this,SLOT(slot_change_timeout_component(int,int)));
        connect(event_manager::get_instance(),SIGNAL(change_updaterate_component(int,int)),this,SLOT(slot_change_updaterate_component(int,int)));

        connect(this,SIGNAL(newAnalogInputValue(int)),event_manager::get_instance(),SIGNAL(newAnalogInputValue(int)));
        connect(this,SIGNAL(newAnalogOutputValue(int)),event_manager::get_instance(),SIGNAL(newAnalogOutputValue(int)));
        connect(this,SIGNAL(newAnalogValue(int)),event_manager::get_instance(),SIGNAL(newAnalogValue(int)));
        connect(this,SIGNAL(newDigitalInputValue(int)),event_manager::get_instance(),SIGNAL(newDigitalInputValue(int)));
        connect(this,SIGNAL(newDigitalOutputValue(int)),event_manager::get_instance(),SIGNAL(newDigitalOutputValue(int)));
        connect(this,SIGNAL(newDigitalValue(int)),event_manager::get_instance(),SIGNAL(newDigitalValue(int)));
        connect(this, SIGNAL(newTcpBox(int)),event_manager::get_instance(),SIGNAL(newTcpBox(int)));
        connect(this, SIGNAL(newQtScriptEngine(int)),event_manager::get_instance(),SIGNAL(newQtScriptEngine(int)));
        connect(this, SIGNAL(newTofDaqDll(int)),event_manager::get_instance(),SIGNAL(newTofDaqDll(int)));

        connect(this, SIGNAL(newQuickScript(int)),event_manager::get_instance(),SIGNAL(newQuickScript(int)));
    }

    entity_manager::~entity_manager()
    {
    }

    entity_manager* entity_manager::get_instance()
    {
        if ( !_instance ){
            _mutex.lock();
            if (!_instance)
                _instance = new entity_manager();
            _mutex.unlock();
        }
        return _instance;
    }//end getInstance

    void entity_manager::destroy()
    {
        _mutex.lock();
        if ( _instance )
            delete _instance;
        _instance = 0;
        _mutex.unlock();
    }

    void entity_manager::initialize()
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

    void entity_manager::dispose()
    {
        foreach (int id, get_all_Values()) {
            delete_entity(id);
        }

        foreach (int id, get_all_DValues()) {
            delete_entity(id);
        }

        //        foreach (int id, get_all_AnalogInputValues()) {
        //            delete_entity(id);
        //        }

        //        foreach (int id, get_all_AnalogOutputValues()) {
        //            delete_entity(id);
        //        }

        foreach (int id, get_all_Systems()) {
            delete_entity(id);
        }

        foreach (int id, get_all_ScriptableObjects()) {
            delete_entity(id);
        }

        QString group;
        QString path;
        QVariant value;

        QSettings settings;
        qDebug() << settings.fileName();
        foreach (int entity, get_all_entities()) {
            group =QString::number(entity);
            settings.beginGroup(group);
            foreach (int component, get_all_components_of_entity(entity)) {
                path    = QString::number(component);
                value   = getComponentDataForEntity(component,entity);
                //qDebug()<<group<<path<<value;
                if(value.isValid())
                    settings.setValue(path,value);
            }
            settings.endGroup();
        }


    }

    int entity_manager::createNewEntity(QString human_readable_label, int entity, bool isSystem)
    {
        int offset = 255;

        if(isSystem)
            offset = 0;

        //Q_UNUSED(human_readable_label);
        int entity_id;
        if(entity<0){
            do{
                entity_id= offset + current_identity_id++;
            }while (entities.constFind(entity_id) != entities.constEnd());
        }
        else
            entity_id= entity;
        //entity_id= qMax(current_identity_id+1, entity+1);

        //current_identity_id = entity_id;

        entity_t et;
        et.entity_id= entity_id;
        et.human_readable_label =human_readable_label;
        entities.insert(entity_id, et);
        return entity_id;
    }

    bool entity_manager::entity_exists(int entity_id) const
    {
        return entities.contains(entity_id);
    }

    void entity_manager::delete_entity(int entity_id)
    {
        entities.remove(entity_id);

        foreach (int component_data_id, get_all_components_of_entity(entity_id)) {
            entity_components.remove(component_data_id);
            component_data_table_N.remove(component_data_id);
        }

    }

    void entity_manager::createComponentAndAddTo(int comp_id, int entity_id)
    {
        if(!components.contains(comp_id)){
            comonent_t ct;
            ct.component_id = comp_id;
            ct.human_readable_description = QString();
            ct.official_name = QString();
            ct.table_name = QString();
            components.insert(comp_id, ct);
        }

        int component_data_id = calc_enity_component_hash(comp_id, entity_id );

        entity_components_t ect;
        ect.component_data_id = component_data_id;
        ect.component_id = comp_id;
        ect.entity_id = entity_id;
        entity_components.insert(component_data_id, ect);

    }

    void entity_manager::removeComponentFrom(int comp_id, int entity_id)
    {
        if(!hasComponent(comp_id,entity_id))
            return;

        int component_data_id = calc_enity_component_hash(comp_id, entity_id );

        entity_components.remove(component_data_id);

    }

    bool entity_manager::component_exists(int comp_id) const
    {
        return (components.contains(comp_id));
    }

    bool entity_manager::hasComponent(int comp_id, int entity_id) const
    {
        if(!entity_exists(entity_id))
            return false;

        if(!component_exists(comp_id))
            return false;

        int component_data_id = calc_enity_component_hash(comp_id, entity_id );
        return entity_components.contains(component_data_id);

    }


    void entity_manager::createComponentAndAddTo(static_component_id comp_id, int entity_id)
    {
        createComponentAndAddTo((int)comp_id, entity_id);
    }

    QVariant entity_manager::getComponentDataForEntity(int component_id, int entity_id) const
    {
        int component_data_id = calc_enity_component_hash(component_id, entity_id );
        QMap<int, QVariant>::ConstIterator av = component_data_table_N.constFind(component_data_id);
        if (av == component_data_table_N.constEnd()){
            return QVariant();
        }

        return av.value();

    }

    void entity_manager::setComponentDataForEntity(int component_id, int entity_id, const QVariant& component_data)
    {
        if(!component_data.isValid())
            return;

        QVariant tmp = getComponentDataForEntity(component_id, entity_id);

        if(tmp == component_data )
            return;

        int component_data_id = calc_enity_component_hash(component_id, entity_id );
        component_data_table_N.insert(component_data_id, component_data);
        emit component_changed(entity_id, component_id);

    }

    int entity_manager::createNewSystem(QUuid suid, QString nameVal, abstract_configuration_widget* sys_cfg_wdg)
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

        }else{
            new_id = list[0];
        }

        setComponentDataForEntity(SYSTEM_VERSION_UID_COMPONENT,               new_id, suid);
        setComponentDataForEntity(NAME_COMPONENT,               new_id, nameVal);
        if(sys_cfg_wdg)
            set_systemConfigurationWidget_component(new_id, sys_cfg_wdg);
        setComponentDataForEntity(IS_SYSTEM_COMPONENT,               new_id, true);
        setComponentDataForEntity(SERIAL_VERSION_UID_COMPONENT, new_id, get_System_uid());

        emit newSystem(new_id);

        return new_id;

    }

    int entity_manager::createNewAnalogInputValue(QString nameVal, QString unitsVal, double amin, double amax, int id, int box_id, int entity)
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

        if(entity <0)
            emit newAnalogInputValue(new_id);
        return new_id;

    }

    int entity_manager::createNewAnalogOutputValue(QString nameVal, QString unitsVal, double smin, double smax, double setVal, int id, int box_id, int entity)
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

        if(entity <0)
            emit newAnalogOutputValue(new_id);
        return new_id;
    }

    int entity_manager::createNewAnalogValue(QString nameVal, QString unitsVal, double smin, double smax, double amin, double amax, double setVal, int id, int box_id)
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

        emit newAnalogValue(new_id);
        return new_id;
    }

    int entity_manager::createNewDigitalInputValue(QString nameVal, QString unitsVal, int id, int box_id, int entity)
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

        setComponentDataForEntity(NAME_COMPONENT,               new_id, nameVal);
        setComponentDataForEntity(UNITS_COMPONENT,              new_id, unitsVal);
        //setComponentDataForEntity(DIGITAL_SET_VALUE_COMPONENT,   new_id, setVal);
        setComponentDataForEntity(DIGITAL_ACT_VALUE_COMPONENT,   new_id, 0);
        setComponentDataForEntity(ID_COMPONENT,                 new_id, id);
        setComponentDataForEntity(BOX_ID_COMPONENT,             new_id, box_id);
        setComponentDataForEntity(SERIAL_VERSION_UID_COMPONENT, new_id, get_DigitalInputValue_uid());
        setComponentDataForEntity(USER_DATA_COMPONENT, new_id, DIGITAL_ACT_VALUE_COMPONENT);

        if(entity <0)
            emit newDigitalInputValue(new_id);
        return new_id;
    }

    int entity_manager::createNewDigitalOutputValue(QString nameVal, bool setVal, int id, int box_id, int entity)
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

        setComponentDataForEntity(NAME_COMPONENT,               new_id, nameVal);
        setComponentDataForEntity(DIGITAL_SET_VALUE_COMPONENT,   new_id, setVal);
        //setComponentDataForEntity(DIGITAL_ACT_VALUE_COMPONENT,   new_id, setVal);
        setComponentDataForEntity(ID_COMPONENT,                 new_id, id);
        setComponentDataForEntity(BOX_ID_COMPONENT,             new_id, box_id);
        setComponentDataForEntity(SERIAL_VERSION_UID_COMPONENT, new_id, get_DigitalOutputValue_uid());
        setComponentDataForEntity(USER_DATA_COMPONENT, new_id, DIGITAL_SET_VALUE_COMPONENT);

        if(entity <0)
            emit newDigitalOutputValue(new_id);
        return new_id;
    }

    int entity_manager::createNewDigitalValue(QString nameVal, QString unitsVal, bool setVal, int id, int box_id)
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

        setComponentDataForEntity(NAME_COMPONENT,               new_id, nameVal);
        setComponentDataForEntity(UNITS_COMPONENT,              new_id, unitsVal);
        setComponentDataForEntity(DIGITAL_SET_VALUE_COMPONENT,   new_id, setVal);
        setComponentDataForEntity(DIGITAL_ACT_VALUE_COMPONENT,   new_id, setVal);
        setComponentDataForEntity(ID_COMPONENT,                 new_id, id);
        setComponentDataForEntity(BOX_ID_COMPONENT,             new_id, box_id);
        setComponentDataForEntity(SERIAL_VERSION_UID_COMPONENT, new_id, get_DigitalValue_uid());
        setComponentDataForEntity(USER_DATA_COMPONENT, new_id, DIGITAL_ACT_VALUE_COMPONENT);

        emit newDigitalValue(new_id);
        return new_id;
    }

    int entity_manager::createNewVersionInformation()
    {
        int new_id = createNewEntity();

        return new_id;

    }

    int entity_manager::createNewDigitalValue(QString nameVal, bool setVal, int id, int box_id)
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


        setComponentDataForEntity(NAME_COMPONENT,               new_id, nameVal);
        setComponentDataForEntity(DIGITAL_SET_VALUE_COMPONENT,  new_id, setVal);
        setComponentDataForEntity(DIGITAL_ACT_VALUE_COMPONENT,  new_id, setVal);
        setComponentDataForEntity(ID_COMPONENT,                 new_id, id);
        setComponentDataForEntity(BOX_ID_COMPONENT,             new_id, box_id);
        setComponentDataForEntity(SERIAL_VERSION_UID_COMPONENT, new_id, get_DigitalValue_uid());
        setComponentDataForEntity(USER_DATA_COMPONENT, new_id, DIGITAL_ACT_VALUE_COMPONENT);

        emit newDigitalValue(new_id);
        return new_id;
    }

    int entity_manager::createNewTcpBox(QString nameVal, QString ip, quint16 port)
    {
        //QUuid uid("{99060fb8-676f-47d8-b9f1-c9c492721009}");
        int new_id = createNewEntity();
        createComponentAndAddTo( NAME_COMPONENT, new_id );
        createComponentAndAddTo( BOX_ID_COMPONENT, new_id );
        createComponentAndAddTo( IP_ADDRESS_COMPONENT, new_id );
        createComponentAndAddTo( PORT_COMPONENT, new_id );
        createComponentAndAddTo( CONNECTION_STATE_COMPONENT, new_id );
        createComponentAndAddTo(SERIAL_VERSION_UID_COMPONENT, new_id);
        createComponentAndAddTo(TCP_BOX_BACKEND_COMPONENT, new_id);

        setComponentDataForEntity(NAME_COMPONENT,               new_id, nameVal);
        setComponentDataForEntity(BOX_ID_COMPONENT,             new_id, new_id);
        setComponentDataForEntity(IP_ADDRESS_COMPONENT,         new_id, ip);
        setComponentDataForEntity(PORT_COMPONENT,               new_id, port);
        setComponentDataForEntity(CONNECTION_STATE_COMPONENT,   new_id, false);
        setComponentDataForEntity(SERIAL_VERSION_UID_COMPONENT, new_id, get_TcpBox_uid());
        setComponentDataForEntity(TCP_BOX_BACKEND_COMPONENT, new_id, 0);

        emit newTcpBox(new_id);
        return new_id;
    }

    int entity_manager::createQtScriptEngine(QString nameVal, QScriptEngine *engine)
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

    int entity_manager::createTofDaqDll(QString nameVal, QString pathVal, int timeout, int updaterate, QString user_data_path)
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

    int entity_manager::create_scriptable_usr_data(QString nameVal, double setVal)
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

    int entity_manager::createScriptableObject(QString nameVal, abstract_scriptable_object *comp)
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

    int entity_manager::createQuickScript(QString nameVal, QString path)
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

    bool entity_manager::is_analog_value(int entity) const
    {
        const QUuid uid=  get_serialVersionUID_component(entity);

        return ((uid == get_AnalogInputValue_uid()) || (uid == get_AnalogOutputValue_uid()) || (uid == get_AnalogValue_uid()) );
    }

    bool entity_manager::is_digital_value(int entity) const
    {
        const QUuid uid=  get_serialVersionUID_component(entity);

        return ((uid == get_DigitalInputValue_uid()) || (uid == get_DigitalOutputValue_uid()) || (uid == get_DigitalValue_uid()) );
    }

    QList<int> entity_manager::get_all_Values() const
    {
        QList<int> tmp= get_all_AnalogInputValues();
        tmp += get_all_AnalogOutputValues();
        tmp += get_all_AnalogValues();

        return tmp;
    }

    QList<int> entity_manager::get_all_DValues() const
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


    abstract_configuration_widget *entity_manager::get_systemConfigurationWidget_component(int entity_id) const
    {
        int component_id = SYSTEM_CONFIGURAION_WIDGET_COMPONENT;
        int component_data_id = calc_enity_component_hash(component_id, entity_id );
        QMap<int, abstract_configuration_widget*>::ConstIterator av = component_widget_table.constFind(component_data_id);
        if (av == component_widget_table.constEnd()){
            return NULL;
        }

        return av.value();
    }

    QScriptEngine *entity_manager::get_qt_script_engine_component(int entity_id) const
    {
        int component_id = QT_SCRIPT_ENGINE_COMPONENT;
        int component_data_id = calc_enity_component_hash(component_id, entity_id );
        QMap<int, QScriptEngine*>::ConstIterator av = component_script_engine_table.constFind(component_data_id);
        if (av == component_script_engine_table.constEnd()){
            return NULL;
        }

        return av.value();
    }

    abstract_scriptable_object *entity_manager::get_scriptable_object_component(int entity_id) const
    {
        int component_id = SCRIPTABLE_OBJECT_COMPONENT;
        int component_data_id = calc_enity_component_hash(component_id, entity_id );
        QMap<int, abstract_scriptable_object*>::ConstIterator av = component_scriptable_object_table.constFind(component_data_id);
        if (av == component_scriptable_object_table.constEnd()){
            return NULL;
        }

        return av.value();
    }

    void entity_manager::set_systemConfigurationWidget_component(int entity_id, abstract_configuration_widget *wdgt)
    {
        if(!wdgt)
            return;

        int component_id = SYSTEM_CONFIGURAION_WIDGET_COMPONENT;
        QWidget* tmp = get_systemConfigurationWidget_component(entity_id);

        if(wdgt == tmp)
            return;

        int component_data_id = calc_enity_component_hash(component_id, entity_id );
        component_widget_table.insert(component_data_id, wdgt);
        emit component_changed(entity_id, component_id);
    }

    void entity_manager::set_qt_script_engine_component(int entity_id, QScriptEngine *engine)
    {
        if(!engine)
            return;

        int component_id = QT_SCRIPT_ENGINE_COMPONENT;
        QScriptEngine* tmp = get_qt_script_engine_component(entity_id);

        if(engine == tmp)
            return;

        int component_data_id = calc_enity_component_hash(component_id, entity_id );
        component_script_engine_table.insert(component_data_id, engine);
        emit component_changed(entity_id, component_id);
    }

    void entity_manager::set_scriptable_object_component(int entity_id, abstract_scriptable_object *engine)
    {
        if(!engine)
            return;

        int component_id = SCRIPTABLE_OBJECT_COMPONENT;
        abstract_scriptable_object* tmp = get_scriptable_object_component(entity_id);

        if(engine == tmp)
            return;

        int component_data_id = calc_enity_component_hash(component_id, entity_id );
        component_scriptable_object_table.insert(component_data_id, engine);
        emit component_changed(entity_id, component_id);
    }



    QList<int> entity_manager::get_entity_by_name(const QString &name) const
    {
        QList<int> ecs;
        QMap<int, entity_t>::const_iterator it= entities.constBegin();
        for(it = entities.constBegin(); it !=entities.constEnd(); ++it){
            if(QString::compare(get_name_component(it.key()), name)==0){
                //return it.key();
                ecs.append(it.key());
            }
        }
        return ecs;

    }

    QList<int> entity_manager::get_entity_by_serialVersionUID(const QUuid &uid) const
    {
        QList<int> ecs;
        QMap<int, entity_t>::const_iterator it= entities.constBegin();
        for(it = entities.constBegin(); it !=entities.constEnd(); ++it){
            if(get_serialVersionUID_component(it.key()) == uid){
                ecs.append(it.key());
            }
        }
        return ecs;
    }

    QList<int> entity_manager::get_entity_by_systemVersionUID(const QUuid &uid) const
    {
        QList<int> ecs;
        QMap<int, entity_t>::const_iterator it= entities.constBegin();
        for(it = entities.constBegin(); it !=entities.constEnd(); ++it){
            if(get_systemVersionUID_component(it.key()) == uid){
                ecs.append(it.key());
            }
        }
        return ecs;
    }

    QList<int> entity_manager::get_entity_by_component(int comp_id) const
    {
        QList<int> ecs;
        QMap<int, entity_t>::const_iterator it= entities.constBegin();
        for(it = entities.constBegin(); it !=entities.constEnd(); ++it){
            if(hasComponent(comp_id,it.key())){
                ecs.append(it.key());
            }
        }
        return ecs;
    }

    QList<int> entity_manager::get_all_entities() const
    {
        QList<int> ecs;
        QMap<int, entity_t>::const_iterator ecit;
        for(ecit=entities.constBegin(); ecit !=entities.constEnd(); ++ecit){
            ecs.append(ecit.key());
        }
        return ecs;
    }

    QList<int> entity_manager::get_all_components_of_entity(int entity) const
    {
        QList<int> ecs;
        QMap<int, entity_components_t>::const_iterator ecit;
        for(ecit=entity_components.constBegin(); ecit !=entity_components.constEnd(); ++ecit){
            if(ecit.value().entity_id == entity)
                ecs.append(ecit.value().component_id);
        }
        return ecs;
    }


    void entity_manager::set_analogActValue_component(int entity, double val){setComponentDataForEntity(ANALOG_ACT_VALUE_COMPONENT, entity, val);}

    void entity_manager::set_analogSetValue_component(int entity, double val){setComponentDataForEntity(ANALOG_SET_VALUE_COMPONENT, entity, val);}

    void entity_manager::set_digitalActValue_component(int entity, bool val){setComponentDataForEntity(DIGITAL_ACT_VALUE_COMPONENT, entity, val);}

    void entity_manager::set_digitalSetValue_component(int entity, bool val){setComponentDataForEntity(DIGITAL_SET_VALUE_COMPONENT, entity, val);}

    void entity_manager::set_setMin_component(int entity, double val){setComponentDataForEntity(SET_MIN_COMPONENT, entity, val);}

    void entity_manager::set_setMax_component(int entity, double val){setComponentDataForEntity(SET_MAX_COMPONENT, entity, val);}

    void entity_manager::set_actMin_component(int entity, double val){setComponentDataForEntity(ACT_MIN_COMPONENT, entity, val);}

    void entity_manager::set_actMax_component(int entity, double val){setComponentDataForEntity(ACT_MAX_COMPONENT, entity, val);}

    void entity_manager::set_name_component(int entity, QString val){setComponentDataForEntity(NAME_COMPONENT, entity, val);}

    void entity_manager::set_units_component(int entity, QString val){setComponentDataForEntity(UNITS_COMPONENT, entity, val);}

    void entity_manager::set_id_component(int entity, int val){setComponentDataForEntity(ID_COMPONENT, entity, val);}

    void entity_manager::set_ip_address_component(int entity, QString val) {setComponentDataForEntity(IP_ADDRESS_COMPONENT, entity, val);}

    void entity_manager::set_port_component(int entity, quint16 val){setComponentDataForEntity(PORT_COMPONENT, entity, val);}

    void entity_manager::set_box_id_component(int entity, int val) {setComponentDataForEntity(BOX_ID_COMPONENT, entity, val);}

    void entity_manager::set_connection_state_component(int entity, bool val){setComponentDataForEntity(CONNECTION_STATE_COMPONENT, entity, val);}

    void entity_manager::set_serialVersionUID_component(int entity, QUuid val){setComponentDataForEntity(SERIAL_VERSION_UID_COMPONENT, entity, val);}

    void entity_manager::set_tcp_box_backend_component(int entity, int val){setComponentDataForEntity(TCP_BOX_BACKEND_COMPONENT, entity, val);}

    void entity_manager::set_script_file_component(int entity, QString val){setComponentDataForEntity(SCRIPT_FILE_COMPONENT, entity, val);}

    void entity_manager::set_data_aquistion_dll_component(int entity, QString val){setComponentDataForEntity(DATA_AQUISITION_DLL_COMPONENT, entity, val);}

    void entity_manager::set_timeout_component(int entity, int val){setComponentDataForEntity(TIMEOUT_COMPONENT, entity, val);}

    void entity_manager::set_updaterate_component(int entity, int val){setComponentDataForEntity(UPDATERATE_COMPONENT, entity, val);}

    void entity_manager::set_user_data_path_component(int entity, QString val){setComponentDataForEntity(USER_DATA_PATH_COMPONENT, entity, val);}

    QString entity_manager::get_name_component(int entity) const{return getComponentDataForEntity(NAME_COMPONENT, entity).toString();}

    QString entity_manager::get_units_component(int entity) const{return getComponentDataForEntity(UNITS_COMPONENT, entity).toString();}

    QString entity_manager::get_ip_address_component(int entity) const{return getComponentDataForEntity(IP_ADDRESS_COMPONENT, entity).toString();}

    quint16 entity_manager::get_port_component(int entity) const{return getComponentDataForEntity(PORT_COMPONENT, entity).toUInt();}

    int entity_manager::get_box_id_component(int entity) const{return getComponentDataForEntity(BOX_ID_COMPONENT, entity).toInt();}

    bool entity_manager::get_connection_state_component(int entity) const{return getComponentDataForEntity(CONNECTION_STATE_COMPONENT, entity).toBool();}

    QUuid entity_manager::get_serialVersionUID_component(int entity) const{return getComponentDataForEntity(SERIAL_VERSION_UID_COMPONENT, entity).toUuid();}

    QUuid entity_manager::get_systemVersionUID_component(int entity) const{return getComponentDataForEntity(SYSTEM_VERSION_UID_COMPONENT, entity).toUuid();}

    int entity_manager::get_tcp_box_backend_component(int entity) const{return getComponentDataForEntity(TCP_BOX_BACKEND_COMPONENT, entity).toInt();}

    QString entity_manager::get_script_file_component(int entity) const{return getComponentDataForEntity(SCRIPT_FILE_COMPONENT, entity).toString();}

    QString entity_manager::get_data_aquistion_dll_component(int entity) const{return getComponentDataForEntity(DATA_AQUISITION_DLL_COMPONENT, entity).toString();}

    int entity_manager::get_timeout_component(int entity) const{return getComponentDataForEntity(TIMEOUT_COMPONENT, entity).toInt();}

    int entity_manager::get_updaterate_component(int entity) const{return getComponentDataForEntity(UPDATERATE_COMPONENT, entity).toInt();}

    int entity_manager::get_userdata_component(int entity) const{return getComponentDataForEntity(USER_DATA_COMPONENT, entity).toInt();}

    QString entity_manager::get_user_data_path_component(int entity) const{return getComponentDataForEntity(USER_DATA_PATH_COMPONENT, entity).toString();}

    double entity_manager::get_analogActValue_component(int entity) const{return getComponentDataForEntity(ANALOG_ACT_VALUE_COMPONENT, entity).toDouble();}

    double entity_manager::get_analogSetValue_component(int entity) const{return getComponentDataForEntity(ANALOG_SET_VALUE_COMPONENT, entity).toDouble();}

    bool entity_manager::get_digitalActValue_component(int entity) const{return getComponentDataForEntity(DIGITAL_ACT_VALUE_COMPONENT, entity).toDouble();}

    bool entity_manager::get_digitalSetValue_component(int entity) const{return getComponentDataForEntity(DIGITAL_SET_VALUE_COMPONENT, entity).toDouble();}

    double entity_manager::get_setMin_component(int entity) const{return getComponentDataForEntity(SET_MIN_COMPONENT, entity).toDouble();}

    double entity_manager::get_setMax_component(int entity) const{return getComponentDataForEntity(SET_MAX_COMPONENT, entity).toDouble();}

    double entity_manager::get_actMin_component(int entity) const{return getComponentDataForEntity(ACT_MIN_COMPONENT, entity).toDouble();}

    double entity_manager::get_actMax_component(int entity) const{return getComponentDataForEntity(ACT_MAX_COMPONENT, entity).toDouble();}

    int entity_manager::get_id_component(int entity) const{return getComponentDataForEntity(ID_COMPONENT, entity).toInt();}

    void entity_manager::slot_change_analogAct_component(int entity, double value)
    {
        if( get_analogActValue_component(entity)!= value){
            set_analogActValue_component(entity, value);
            emit analogAct_component_changed(entity);
        }
    }

    void entity_manager::slot_change_analogSet_component(int entity, double value)
    {
        //QObject* snd = sender();
        //qDebug()<<"entity_manager::slot_change_analogSet_component"<<snd<<value;
        if( get_analogSetValue_component(entity)!= value){

            set_analogSetValue_component(entity, value);
            emit analogSet_component_changed(entity);
        }
    }

    void entity_manager::slot_change_digitalAct_component(int entity, bool value)
    {
        if( get_digitalActValue_component(entity)!= value){

            set_digitalActValue_component(entity, value);
            emit digitalAct_component_changed(entity);
        }
    }

    void entity_manager::slot_change_digitalSet_component(int entity, bool value)
    {
        if( get_digitalSetValue_component(entity)!= value){
            set_digitalSetValue_component(entity, value);
            emit digitalSet_component_changed(entity);
        }
    }

    void entity_manager::slot_change_script_file_component(int entity, QString value)
    {
        if( get_script_file_component(entity)!= value){
            set_script_file_component(entity, value);
            emit script_file_component_changed(entity);
        }
    }

    void entity_manager::slot_change_data_aquistion_dll_component(int entity, QString value)
    {
        if( get_data_aquistion_dll_component(entity)!= value){
            set_data_aquistion_dll_component(entity, value);
            emit data_aquistion_dll_component_changed(entity);
        }
    }

    void entity_manager::slot_change_timeout_component(int entity, int value)
    {
        if( get_timeout_component(entity)!= value){
            set_timeout_component(entity, value);
            emit timeout_component_changed(entity);
        }
    }

    void entity_manager::slot_change_updaterate_component(int entity, int value)
    {
        if( get_updaterate_component(entity)!= value){
            set_updaterate_component(entity, value);
            emit updaterate_component_changed(entity);
        }
    }

    void entity_manager::slot_change_userDataPath_component(int entity, QString value)
    {
        if( get_user_data_path_component(entity)!= value){
            set_user_data_path_component(entity, value);
            emit user_data_path_changed(entity);
        }
    }

    void entity_manager::slot_change_name_component(int entity, QString value)
    {
        if( get_name_component(entity)!= value){
            set_name_component(entity, value);
            emit name_component_changed(entity);
        }
    }

    void entity_manager::slot_change_ip_address_component(int entity, QString value)
    {
        if( get_ip_address_component(entity)!= value){
            set_ip_address_component(entity, value);
            //emit analogAct_component_changed(entity);
        }
    }

    void entity_manager::slot_change_port_component(int entity, quint16 value)
    {
        if( get_port_component(entity)!= value){
            set_port_component(entity, value);
            //emit analogAct_component_changed(entity);
        }
    }

    void entity_manager::slot_connection_state_component(int entity, bool value)
    {
        if( get_connection_state_component(entity)!= value){
            set_connection_state_component(entity, value);
            emit connection_state_component_changed(entity);
        }
    }

    void entity_manager::slot_change_tcp_box_backend_component(int entity, int value)
    {
        if( get_tcp_box_backend_component(entity)!= value){
            set_tcp_box_backend_component(entity, value);
            //emit connection_state_component_changed(entity);
        }
    }






} // namespace talorion


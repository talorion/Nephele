#include "entity_manager.hpp"

#include <QAtomicPointer>
#include <QMutex>
#include <QDebug>
#include <QSignalMapper>
#include <QVariant>

#include "core/event_manager.hpp"
//#include "analogvalue.hpp"

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
        component_widget_table()
    {
        connect(this,SIGNAL(newSystem(int)),event_manager::get_instance(),SIGNAL(newSystem(int)));

        connect(this, SIGNAL(analogAct_component_changed(int)), event_manager::get_instance(), SIGNAL(analogAct_component_changed(int)));
        connect(this, SIGNAL(analogSet_component_changed(int)), event_manager::get_instance(), SIGNAL(analogSet_component_changed(int)));
        connect(this, SIGNAL(digitalAct_component_changed(int)), event_manager::get_instance(), SIGNAL(digitalAct_component_changed(int)));
        connect(this, SIGNAL(digitalSet_component_changed(int)), event_manager::get_instance(), SIGNAL(digitalSet_component_changed(int)));

        connect(event_manager::get_instance(),SIGNAL(change_analogAct_component(int,double)),this,SLOT(slot_change_analogAct_component(int,double)));
        connect(event_manager::get_instance(),SIGNAL(change_analogSet_component(int,double)),this,SLOT(slot_change_analogSet_component(int,double)));
        connect(event_manager::get_instance(),SIGNAL(change_digitalAct_component(int,bool)),this,SLOT(slot_change_digitalAct_component(int,bool)));
        connect(event_manager::get_instance(),SIGNAL(change_digitalSet_component(int,bool)),this,SLOT(slot_change_digitalSet_component(int,bool)));

        connect(this,SIGNAL(newAnalogValue(int)),event_manager::get_instance(),SIGNAL(newAnalogValue(int)));
        connect(this,SIGNAL(newDigitalValue(int)),event_manager::get_instance(),SIGNAL(newDigitalValue(int)));

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

    int entity_manager::createNewEntity(QString human_readable_label)
    {
        Q_UNUSED(human_readable_label);
        int entity_id= current_identity_id++;
        entity_t et;
        et.entity_id= entity_id;
        et.human_readable_label =human_readable_label;
        entities.insert(entity_id, et);
        return entity_id;
    }

    void entity_manager::delete_entity(int entity_id)
    {
        entities.remove(entity_id);

        foreach (int component_data_id, get_all_components_of_entity(entity_id)) {
            entity_components.remove(component_data_id);
            component_data_table_N.remove(component_data_id);
        }

    }

    void entity_manager::createComponentAndAddTo(static_component_id comp_id, int entity_id)
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

    int entity_manager::createNewSystem(QString nameVal, QWidget *sys_cfg_wdg)
    {
        int entity = createNewEntity();

       createComponentAndAddTo(NAME_COMPONENT, entity);
        if(sys_cfg_wdg)
            createComponentAndAddTo(SYSTEM_CONFIGURAION_WIDGET_COMPONENT, entity);

        set_name_component(entity,nameVal);
        if(sys_cfg_wdg)
            set_systemConfigurationWidget_component(entity, sys_cfg_wdg);

        emit newSystem(entity);
        return entity;

    }

    int entity_manager::createNewAnalogValue(QString nameVal, QString unitsVal, double smin, double smax, double amin, double amax, double setVal, int id, int box_id)
    {
        int new_id = createNewEntity();

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

        emit newAnalogValue(new_id);
        return new_id;
    }

    int entity_manager::createNewDigitalValue(QString nameVal, bool setVal, int id, int box_id)
    {
        int new_id = createNewEntity();
        createComponentAndAddTo( NAME_COMPONENT, new_id );
        createComponentAndAddTo( DIGITAL_ACT_VALUE_COMPONENT, new_id );
        createComponentAndAddTo( DIGITAL_SET_VALUE_COMPONENT, new_id );
        createComponentAndAddTo( ID_COMPONENT, new_id );
        createComponentAndAddTo( BOX_ID_COMPONENT, new_id );

        setComponentDataForEntity(NAME_COMPONENT,               new_id, nameVal);
        setComponentDataForEntity(DIGITAL_SET_VALUE_COMPONENT,  new_id, setVal);
        setComponentDataForEntity(DIGITAL_ACT_VALUE_COMPONENT,  new_id, setVal);
        setComponentDataForEntity(ID_COMPONENT,                 new_id, id);
        setComponentDataForEntity(BOX_ID_COMPONENT,             new_id, box_id);

        emit newDigitalValue(new_id);
        return new_id;
    }


    QWidget *entity_manager::get_systemConfigurationWidget_component(int entity_id) const
    {
        int component_id = SYSTEM_CONFIGURAION_WIDGET_COMPONENT;
        int component_data_id = calc_enity_component_hash(component_id, entity_id );
        QMap<int, QWidget*>::ConstIterator av = component_widget_table.constFind(component_data_id);
        if (av == component_widget_table.constEnd()){
            return NULL;
        }

        return av.value();
    }

    void entity_manager::set_systemConfigurationWidget_component(int entity_id, QWidget *wdgt)
    {
        if(!wdgt)
            return;

        int component_id = SYSTEM_CONFIGURAION_WIDGET_COMPONENT;
        QWidget* tmp = get_systemConfigurationWidget_component(entity_id);

        if(tmp == wdgt )
            return;

        int component_data_id = calc_enity_component_hash(component_id, entity_id );
        component_widget_table.insert(component_data_id, wdgt);
        emit component_changed(entity_id, component_id);
    }


    //        int entity_manager::create_new_fc_box_connection(QString nameVal, int box_id, QString ip_address, quint16 port)
    //        {
    //            int new_id = createNewEntity();

    //            createComponentAndAddTo( IP_ADDRESS_COMPONENT, new_id );
    //            createComponentAndAddTo( PORT_COMPONENT, new_id );
    //            createComponentAndAddTo( BOX_ID_COMPONENT, new_id );
    //            createComponentAndAddTo( NAME_COMPONENT, new_id );

    //            setComponentDataForEntity(IP_ADDRESS_COMPONENT, new_id, ip_address);
    //            setComponentDataForEntity(PORT_COMPONENT, new_id, port);
    //            setComponentDataForEntity(BOX_ID_COMPONENT, new_id, box_id);
    //            setComponentDataForEntity(NAME_COMPONENT, new_id, nameVal);

    //            return new_id;
    //        }



    int entity_manager::get_entity_by_name(const QString &name) const
    {
        QMap<int, entity_t>::const_iterator it= entities.constBegin();
        for(it = entities.constBegin(); it !=entities.constEnd(); ++it){
            if(QString::compare(get_name_component(it.key()), name)){
                return it.key();
            }
        }
        return -1;

    }

    QList<int> entity_manager::get_all_components_of_entity(int entity) const
    {
        QList<int> ecs;
        QMap<int, entity_components_t>::const_iterator ecit;
        for(ecit=entity_components.constBegin(); ecit !=entity_components.constEnd(); ++ecit){
            if(ecit.value().entity_id == entity)
                ecs.append(ecit.value().entity_id);
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

    QString entity_manager::get_name_component(int entity) const{return getComponentDataForEntity(NAME_COMPONENT, entity).toString();}

    QString entity_manager::get_units_component(int entity) const{return getComponentDataForEntity(UNITS_COMPONENT, entity).toString();}

    double entity_manager::get_analogActValue_component(int entity) const{return getComponentDataForEntity(ANALOG_ACT_VALUE_COMPONENT, entity).toDouble();}

    double entity_manager::get_analogSetValue_component(int entity) const{return getComponentDataForEntity(ANALOG_SET_VALUE_COMPONENT, entity).toDouble();}

    bool entity_manager::get_digitalActValue_component(int entity) const{return getComponentDataForEntity(DIGITAL_ACT_VALUE_COMPONENT, entity).toDouble();}

    bool entity_manager::get_digitalSetValue_component(int entity) const{return getComponentDataForEntity(DIGITAL_SET_VALUE_COMPONENT, entity).toDouble();}

    double entity_manager::get_setMin_component(int entity) const{return getComponentDataForEntity(SET_MIN_COMPONENT, entity).toDouble();}

    double entity_manager::get_setMax_component(int entity) const{return getComponentDataForEntity(SET_MAX_COMPONENT, entity).toDouble();}

    double entity_manager::get_actMin_component(int entity) const{return getComponentDataForEntity(ACT_MIN_COMPONENT, entity).toDouble();}

    double entity_manager::get_actMax_component(int entity) const{return getComponentDataForEntity(ACT_MAX_COMPONENT, entity).toDouble();}

    int entity_manager::get_id_component(int entity) const{return getComponentDataForEntity(ID_COMPONENT, entity).toDouble();}

    void entity_manager::slot_change_analogAct_component(int entity, double value)
    {
        if( get_analogActValue_component(entity)!= value){

            set_analogActValue_component(entity, value);
            emit analogAct_component_changed(entity);
        }
    }

    void entity_manager::slot_change_analogSet_component(int entity, double value)
    {
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


} // namespace talorion


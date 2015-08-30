#include "entity_manager.hpp"

#include <QAtomicPointer>
#include <QMutex>
#include <QDebug>
#include <QSignalMapper>
#include <QVariant>

#include "core/event_manager.hpp"
#include "analogvalue.hpp"

namespace talorion {

    QAtomicPointer<entity_manager> entity_manager::_instance;
    QMutex entity_manager::_mutex;

    entity_manager::entity_manager(QObject *par) :
        QObject(par),
        analog_values(),
        act_value_signalMapper(NULL),
        set_value_signalMapper(NULL),
        current_identity_id(0)
    {
        act_value_signalMapper = new QSignalMapper(this);
        set_value_signalMapper = new QSignalMapper(this);
        //connect(act_value_signalMapper, SIGNAL(mapped(int)), event_manager::get_instance(), SIGNAL(act_value_changed(int)));
        //connect(set_value_signalMapper, SIGNAL(mapped(int)), event_manager::get_instance(), SIGNAL(set_value_changed(int)));

        connect(this, SIGNAL(act_value_changed(int)), event_manager::get_instance(), SIGNAL(act_value_changed(int)));
        connect(this, SIGNAL(set_value_changed(int)), event_manager::get_instance(), SIGNAL(set_value_changed(int)));

        connect(this,SIGNAL(newAnalogValue(int)),event_manager::get_instance(),SIGNAL(newAnalogValue(int)));


        connect(event_manager::get_instance(),SIGNAL(change_act_value(int,double)),this,SLOT(slot_change_act_value(int,double)));
        connect(event_manager::get_instance(),SIGNAL(change_set_value(int,double)),this,SLOT(slot_change_set_value(int,double)));

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
        return current_identity_id++;
    }

    //analogValue* entity_manager::createNewAnalogValue(QString nameVal, QString unitsVal, double smin, double smax, double amin, double amax, double setVal, int id, int box_id)
    int entity_manager::createNewAnalogValue(QString nameVal, QString unitsVal, double smin, double smax, double amin, double amax, double setVal, int id, int box_id)
    {
        int entity = createNewEntity(QString("box:")+QString::number(box_id)+QString(" value:")+nameVal);
        analogValue* fc =get_analogValue(entity);
        if (!fc){
            fc = new analogValue(nameVal,
                                 unitsVal,
                                 smin,
                                 smax,
                                 amin,
                                 amax,
                                 setVal,
                                 id,
                                 box_id,
                                 entity
                                 );
            analog_values.insert(entity,fc);
            //connect(fc, SIGNAL(act_value_changed()), act_value_signalMapper, SLOT(map()));
            //act_value_signalMapper->setMapping(fc, entity);
            //connect(fc, SIGNAL(set_value_changed()), set_value_signalMapper, SLOT(map()));
            //set_value_signalMapper->setMapping(fc, entity);
            emit newAnalogValue(entity);
        }

        //return fc;
        return entity;
    }

    QString entity_manager::get_name_component(int entity) const
    {
        analogValue* fc =get_analogValue(entity);
        if (!fc){
            return "";
        }
        return fc->getName();
    }

    QString entity_manager::get_units_component(int entity) const
    {
        analogValue* fc =get_analogValue(entity);
        if (!fc){
            return "";
        }
        return fc->getUnits();
    }

    int entity_manager::get_entity_by_name(const QString &name) const
    {
        QMap<int, analogValue*>::ConstIterator it = analog_values.constBegin();
        for(it=analog_values.constBegin(); it !=analog_values.constEnd(); ++it){
            //int entity = it.value()->getEntity();
            if(QString::compare(name,it.value()->getName(), Qt::CaseInsensitive) == 0)
                return it.value()->getEntity();;
        }
        return -1;
    }

    void entity_manager::set_actValue_component(int entity, double val)
    {
        analogValue* fc =get_analogValue(entity);
        if (fc){
            fc->setAct(val);
        }
    }

    void entity_manager::set_setValue_component(int entity, double val)
    {
        analogValue* fc =get_analogValue(entity);
        if (fc){
            fc->setSet(val);
        }
    }

    void entity_manager::set_setMin_component(int entity, double val)
    {
        analogValue* fc =get_analogValue(entity);
        if (fc){
            Q_UNUSED(val);
        }
    }

    void entity_manager::set_setMax_component(int entity, double val)
    {
        analogValue* fc =get_analogValue(entity);
        if (fc){
            Q_UNUSED(val);
        }
    }

    void entity_manager::set_actMin_component(int entity, double val)
    {
        analogValue* fc =get_analogValue(entity);
        if (fc){
            Q_UNUSED(val);
        }
    }

    void entity_manager::set_actMax_component(int entity, double val)
    {
        analogValue* fc =get_analogValue(entity);
        if (fc){
            Q_UNUSED(val);
        }
    }

    void entity_manager::set_name_component(int entity, QString val)
    {
        analogValue* fc =get_analogValue(entity);
        if (fc){
            Q_UNUSED(val);
        }
    }

    void entity_manager::set_units_component(int entity, QString val)
    {
        analogValue* fc =get_analogValue(entity);
        if (fc){
            Q_UNUSED(val);
        }
    }

    double entity_manager::get_actValue_component(int entity) const
    {
        analogValue* fc =get_analogValue(entity);
        if (!fc){
            return -1;
        }
        return fc->getAct();
    }

    double entity_manager::get_setValue_component(int entity) const
    {
        analogValue* fc =get_analogValue(entity);
        if (!fc){
            return -1;
        }
        return fc->getSet();
    }

    double entity_manager::get_setMin_component(int entity) const
    {
        analogValue* fc =get_analogValue(entity);
        if (!fc){
            return -1;
        }
        return fc->getSmin();
    }

    double entity_manager::get_setMax_component(int entity) const
    {
        analogValue* fc =get_analogValue(entity);
        if (!fc){
            return -1;
        }
        return fc->getSmax();
    }

    double entity_manager::get_actMin_component(int entity) const
    {
        analogValue* fc =get_analogValue(entity);
        if (!fc){
            return -1;
        }
        return fc->getAmin();
    }

    double entity_manager::get_actMax_component(int entity) const
    {
        analogValue* fc =get_analogValue(entity);
        if (!fc){
            return -1;
        }
        return fc->getAmax();
    }

    int entity_manager::get_id_component(int entity) const
    {
        analogValue* fc =get_analogValue(entity);
        if (!fc){
            return -1;
        }
        return fc->getId();
    }

    analogValue *entity_manager::get_analogValue(int entity) const
    {
        QMap<int, analogValue*>::ConstIterator av = analog_values.constFind(entity);
        if (av == analog_values.constEnd()){
            return NULL;
        }
        return av.value();
    }

    void entity_manager::slot_change_act_value(int entity, double value)
    {
        if( get_actValue_component(entity)!= value){

            set_actValue_component(entity, value);
            emit act_value_changed(entity);
        }
    }

    void entity_manager::slot_change_set_value(int entity, double value)
    {
        if( get_setValue_component(entity)!= value){
            set_setValue_component(entity, value);
            emit set_value_changed(entity);
        }
    }


} // namespace talorion


#include "script_set_handler.hpp"

#include <QCoreApplication>
#include <QThread>
#include <QTime>
#include <QTimer>
#include <QDebug>

#include <QScriptValue>

#include "core/event_manager.hpp"
#include "core/entity_manager.hpp"

namespace talorion {

    script_set_handler::script_set_handler(QObject *par) :
        abstract_scriptable_object("set", par)
    {
        connect(this,SIGNAL(set_value_changed(int,double)),event_manager::get_instance(),SIGNAL(change_analogSet_component(int,double)),Qt::UniqueConnection);
        connect(event_manager::get_instance(),SIGNAL(analogSet_component_changed(int)),this,SLOT(slot_set_value_changed(int)));
        connect(event_manager::get_instance(),SIGNAL(newAnalogValue(int)),this,SLOT(slot_newAnalogValue(int)));

    }

    script_set_handler::~script_set_handler()
    {

    }

    void script_set_handler::val(QString name, double value)
    {
        int entity=entity_manager::get_instance()->get_entity_by_name(name);
        if(entity >= 0){
            //qDebug()<<"script_set_handler::val";
            emit set_value_changed(entity, value);
        }
    }

    void script_set_handler::slot_set_value_changed(int entity)
    {
        QString nme = entity_manager::get_instance()->get_name_component(entity);
        double valu = entity_manager::get_instance()->get_analogSetValue_component(entity);
        if(getScrip_value() != NULL)
            getScrip_value()->setProperty(nme, valu, QScriptValue::ReadOnly);
    }

    void script_set_handler::slot_newAnalogValue(int entity)
    {
        //slot_act_value_changed(entity);
        slot_set_value_changed(entity);
    }
}


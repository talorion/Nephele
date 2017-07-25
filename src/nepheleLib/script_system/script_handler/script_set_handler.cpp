#include "script_set_handler.hpp"


#include <QCoreApplication>
#include <QThread>
#include <QTime>
#include <QTimer>
#include <QDebug>

#include <QScriptValue>

#include "core/EventManager.hpp"
#include "core/EntityManager.hpp"

namespace talorion {

    script_set_handler::script_set_handler(QObject *par) :
        abstract_scriptable_object("set", par)
    {
        connect(this,SIGNAL(set_value_changed(int,double)),EventManager::get_instance(),SIGNAL(change_analogSet_component(int,double)),Qt::UniqueConnection);
        connect(this,SIGNAL(set_dig_value_changed(int,bool)),EventManager::get_instance(),SIGNAL(change_digitalSet_component(int,bool)),Qt::UniqueConnection);
        connect(EventManager::get_instance(),SIGNAL(analogSet_component_changed(int)),this,SLOT(slot_set_value_changed(int)));
        connect(EventManager::get_instance(),SIGNAL(digitalSet_component_changed(int)),this,SLOT(slot_digital_set_value_changed(int)));
        connect(EventManager::get_instance(),SIGNAL(newAnalogValue(int)),this,SLOT(slot_newAnalogValue(int)));
        connect(EventManager::get_instance(),SIGNAL(newAnalogOutputValue(int)),this,SLOT(slot_newAnalogValue(int)));
        connect(EventManager::get_instance(),SIGNAL(newDigitalValue(int)),this,SLOT(slot_newDigitalValue(int)));
        connect(EventManager::get_instance(),SIGNAL(newDigitalOutputValue(int)),this,SLOT(slot_newDigitalValue(int)));
    }

    script_set_handler::~script_set_handler()
    {

    }

    void script_set_handler::val(QString name, double value)
    {
        QList<int> entities=EntityManager::get_instance()->get_entity_by_name(name);

        if(!entities.isEmpty()){
            int entity = entities.at(0);
            //qDebug()<<"script_set_handler::val";
            if(EntityManager::get_instance()->is_analog_value(entity))
                emit set_value_changed(entity, value);
            if(EntityManager::get_instance()->is_digital_value(entity))
                val(name, value != 0);
        }
    }

    void script_set_handler::val(QString name, bool value)
    {
        QList<int> entities=EntityManager::get_instance()->get_entity_by_name(name);
        if(!entities.isEmpty()){
            int entity = entities.at(0);
            //qDebug()<<"script_set_handler::val";
            emit set_dig_value_changed(entity, value);
        }

    }

    void script_set_handler::slot_set_value_changed(int entity)
    {
        QString nme = EntityManager::get_instance()->get_name_component(entity);
        double valu = EntityManager::get_instance()->get_analogSetValue_component(entity);
        if(getScrip_value() != NULL)
            getScrip_value()->setProperty(nme, valu, QScriptValue::ReadOnly);
    }

    void script_set_handler::slot_digital_set_value_changed(int entity)
    {
        QString nme = EntityManager::get_instance()->get_name_component(entity);
        bool valu = EntityManager::get_instance()->get_digitalSetValue_component(entity);
        if(getScrip_value() != NULL)
            getScrip_value()->setProperty(nme, valu, QScriptValue::ReadOnly);
    }

    void script_set_handler::slot_newAnalogValue(int entity)
    {
        //slot_act_value_changed(entity);
        slot_set_value_changed(entity);
    }

    void script_set_handler::slot_newDigitalValue(int entity)
    {
        //slot_act_value_changed(entity);
        slot_digital_set_value_changed(entity);
    }
}


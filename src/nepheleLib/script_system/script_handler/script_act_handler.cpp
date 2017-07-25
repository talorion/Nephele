#include "script_act_handler.hpp"

#include <QScriptValue>

#include "core/EventManager.hpp"
#include "core/EntityManager.hpp"

namespace talorion {

    script_act_handler::script_act_handler(QObject *par) :
        abstract_scriptable_object("act", par)
    {
        connect(EventManager::get_instance(),SIGNAL(analogAct_component_changed(int)),this,SLOT(slot_act_value_changed(int)));
        connect(EventManager::get_instance(),SIGNAL(digitalAct_component_changed(int)),this,SLOT(slot_dig_act_value_changed(int)));
        connect(EventManager::get_instance(),SIGNAL(newAnalogValue(int)),this,SLOT(slot_newAnalogValue(int)));

    }

    script_act_handler::~script_act_handler()
    {

    }

    void script_act_handler::slot_act_value_changed(int entity)
    {
        QString nme = EntityManager::get_instance()->get_name_component(entity);
        double val = EntityManager::get_instance()->get_analogActValue_component(entity);
        QScriptValue* tmp= getScrip_value();
        if( tmp != NULL)
            tmp->setProperty(nme, val, QScriptValue::ReadOnly);
    }

    void script_act_handler::slot_dig_act_value_changed(int entity)
    {
        QString nme = EntityManager::get_instance()->get_name_component(entity);
        bool val = EntityManager::get_instance()->get_digitalActValue_component(entity);
        QScriptValue* tmp= getScrip_value();
        if( tmp != NULL)
            tmp->setProperty(nme, val, QScriptValue::ReadOnly);
    }

    void script_act_handler::slot_newAnalogValue(int entity)
    {
        slot_act_value_changed(entity);
        //slot_set_value_changed(entity);
    }
}



#include "script_act_handler.hpp"

#include <QScriptValue>

#include "core/event_manager.hpp"
#include "core/entity_manager.hpp"

namespace talorion {

    script_act_handler::script_act_handler(QObject *par) :
        abstract_scriptable_object("act", par)
    {
        connect(event_manager::get_instance(),SIGNAL(analogAct_component_changed(int)),this,SLOT(slot_act_value_changed(int)));
        connect(event_manager::get_instance(),SIGNAL(digitalAct_component_changed(int)),this,SLOT(slot_dig_act_value_changed(int)));
        connect(event_manager::get_instance(),SIGNAL(newAnalogValue(int)),this,SLOT(slot_newAnalogValue(int)));

    }

    script_act_handler::~script_act_handler()
    {

    }

    void script_act_handler::slot_act_value_changed(int entity)
    {
        QString nme = entity_manager::get_instance()->get_name_component(entity);
        double val = entity_manager::get_instance()->get_analogActValue_component(entity);
        QScriptValue* tmp= getScrip_value();
        if( tmp != NULL)
            tmp->setProperty(nme, val, QScriptValue::ReadOnly);
    }

    void script_act_handler::slot_dig_act_value_changed(int entity)
    {
        QString nme = entity_manager::get_instance()->get_name_component(entity);
        bool val = entity_manager::get_instance()->get_digitalActValue_component(entity);
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



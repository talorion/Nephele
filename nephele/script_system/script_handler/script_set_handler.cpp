#include "script_set_handler.hpp"

#include <QCoreApplication>
#include <QThread>
#include <QTime>
#include <QTimer>

#include "core/event_manager.hpp"
#include "core/entity_manager.hpp"

namespace talorion {

    script_set_handler::script_set_handler(QObject *par) :
        QObject(par)
    {
        connect(this,SIGNAL(set_value_changed(int,double)),event_manager::get_instance(),SIGNAL(change_analogSet_component(int,double)));

    }

    script_set_handler::~script_set_handler()
    {

    }

    void script_set_handler::val(QString name, double value)
    {
        int entity=entity_manager::get_instance()->get_entity_by_name(name);
        if(entity >= 0)
            emit set_value_changed(entity, value);
    }
}


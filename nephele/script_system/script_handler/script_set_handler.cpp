#include "script_set_handler.hpp"

#include <QCoreApplication>
#include <QThread>
#include <QTime>
#include <QTimer>

#include "core/event_manager.hpp"

namespace talorion {

    script_set_handler::script_set_handler(QObject *par) :
        QObject(par)
    {
        //connect(this,SIGNAL(val(QString,double)),event_manager::get_instance(),SIGNAL(set_value_changed(QString,double)));

    }

    script_set_handler::~script_set_handler()
    {

    }
}


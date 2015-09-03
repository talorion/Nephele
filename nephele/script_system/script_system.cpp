#include "script_system.hpp"

#include <QDebug>
#include "scripting_worker.hpp"
#include "core/event_manager.hpp"

namespace talorion {

    script_system::script_system(QObject *par) :
        QThread(par),
        abstract_system("{8c2f892c-e2e2-417b-a3b1-96600e7822d3}")
    {
        connect(event_manager::get_instance(),SIGNAL(application_aboutToQuit()),this,SLOT(quit()));
    }

    script_system::~script_system()
    {

    }


    void script_system::run()
    {
        scripting_worker worker;
        worker.initialize();

        exec();
    }

    void script_system::do_start_system()
    {
        this->start();
    }
}

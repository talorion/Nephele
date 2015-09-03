#include "power_supply_dll_system.hpp"

#include "core/event_manager.hpp"

namespace talorion{

    power_supply_dll_system::power_supply_dll_system(QObject *par):
        QThread(par),
        abstract_system("{1a08d877-a8cd-4b03-9780-171e02fb77d8}")
    {
        connect(event_manager::get_instance(),SIGNAL(application_aboutToQuit()),this,SLOT(quit()));
    }

    power_supply_dll_system::~power_supply_dll_system()
    {

    }

    void power_supply_dll_system::run()
    {
        exec();
    }

    void power_supply_dll_system::do_start_system()
    {
        this->start();
    }

}


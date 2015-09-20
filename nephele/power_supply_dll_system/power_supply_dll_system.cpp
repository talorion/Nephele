#include "power_supply_dll_system.hpp"

#include "power_supply_dll_wrapper.h"

#include "psd_config_widget/psd_config_widget.hpp"

#include "core/event_manager.hpp"

namespace talorion{

    power_supply_dll_system::power_supply_dll_system(QObject *par):
        QThread(par),
        abstract_system("{1a08d877-a8cd-4b03-9780-171e02fb77d8}"),
        config_wdg(NULL)
    {
        config_wdg =  new psd_config_widget();
        connect(event_manager::get_instance(),SIGNAL(application_aboutToQuit()),this,SLOT(quit()));
    }

    power_supply_dll_system::~power_supply_dll_system()
    {
        delete config_wdg;
    }

    void power_supply_dll_system::run()
    {
        //power_supply_dll_wrapper* wrk

        exec();
    }

    void power_supply_dll_system::do_start_system()
    {
        this->start();
    }

    abstract_configuration_widget *power_supply_dll_system::do_get_configuration_widget()
    {
        return config_wdg;
    }

}


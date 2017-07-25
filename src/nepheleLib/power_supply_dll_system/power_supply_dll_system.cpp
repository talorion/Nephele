#include "power_supply_dll_system.hpp"


#include "power_supply_dll_wrapper.h"

#include "psd_config_widget/psd_config_widget.hpp"

#include "core/EventManager.hpp"

#include <core/ManagerLocator.hpp>

namespace talorion{

    power_supply_dll_system::power_supply_dll_system(QObject *par):
        QThread(par),
        abstract_system("{1a08d877-a8cd-4b03-9780-171e02fb77d8}"),
        config_wdg(NULL)
    {
        config_wdg =  new psd_config_widget();
        QObject::connect(EventManager::get_instance(),SIGNAL(application_aboutToQuit()),(QThread*)this,SLOT(quit()));
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

    int power_supply_dll_system::doInitialize()
    {
        return 0;
    }

    int power_supply_dll_system::doDispose()
    {
        return 0;
    }

    int power_supply_dll_system::do_start_system()
    {
        this->start();
        return 0;
    }

    abstract_configuration_widget *power_supply_dll_system::do_get_configuration_widget()
    {
        return config_wdg;
    }

    void power_supply_dll_system::doProvideEntityManager(EntityManager *service)
    {
        ManagerLocator::provideEntityManager(service);
    }

    void power_supply_dll_system::doProvideEventManager(EventManager *service)
    {
        ManagerLocator::provideEventManager(service);
    }

    void power_supply_dll_system::doProvideSystemManager(SystemManager *service)
    {
        ManagerLocator::provideSystemManager(service);
    }

    QCoreApplication *power_supply_dll_system::doMyQApp() const
    {
        return qApp;
    }

}


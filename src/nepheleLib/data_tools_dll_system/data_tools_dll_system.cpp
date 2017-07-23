#include "data_tools_dll_system.hpp"


#include "dtd_config_widget/dtd_config_widget.hpp"

#include "core/EventManager.hpp"

#include <core/ManagerLocator.hpp>

namespace talorion {

data_tools_dll_system::data_tools_dll_system(QObject *par) :
    QThread(par),
    abstract_system("{41dc1dd5-2b1f-4424-8203-7b887c78e437}"),
    config_wdg(NULL)
{
    config_wdg =  new dtd_config_widget();

    QObject::connect(EventManager::get_instance(),SIGNAL(application_aboutToQuit()),(QThread*)this,SLOT(quit()));
}

data_tools_dll_system::~data_tools_dll_system()
{
    delete config_wdg;
}

void data_tools_dll_system::run()
{
    exec();
}

int data_tools_dll_system::doInitialize()
{
    return 0;
}

int data_tools_dll_system::doDispose()
{
    return 0;
}

int data_tools_dll_system::do_start_system()
{
    this->start();
    return 0;
}

abstract_configuration_widget *data_tools_dll_system::do_get_configuration_widget()
{
    return config_wdg;
}

void data_tools_dll_system::doProvideEntityManager(EntityManager *service)
{
    ManagerLocator::provideEntityManager(service);
}

void data_tools_dll_system::doProvideEventManager(EventManager *service)
{
    ManagerLocator::provideEventManager(service);
}

void data_tools_dll_system::doProvideSystemManager(SystemManager *service)
{
    ManagerLocator::provideSystemManager(service);
}

QCoreApplication *data_tools_dll_system::doMyQApp() const
{
    return qApp;
}

} // namespace talorion


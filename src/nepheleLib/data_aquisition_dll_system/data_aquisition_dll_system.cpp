#include "data_aquisition_dll_system.hpp"

#include "dad_config_widget/dad_config_widget.hpp"
#include "data_aquisition_dll_wrapper.hpp"
#include "core/EventManager.hpp"

#include "data_aquisition_dll_worker.hpp"

#include <core/ManagerLocator.hpp>

namespace talorion {

data_aquisition_dll_system::data_aquisition_dll_system(QObject *par) :
    QThread(par),
    abstract_system("{e6ec66b4-85e5-44bd-b9c5-73fea98413a8}"),
    config_wdg(NULL)
{
    config_wdg =  new dad_config_widget();
    QObject::connect(EventManager::get_instance(),SIGNAL(application_aboutToQuit()),(QThread*)this,SLOT(quit()));
}

data_aquisition_dll_system::~data_aquisition_dll_system()
{
    delete config_wdg;
}

void data_aquisition_dll_system::run()
{
    data_aquisition_dll_worker* wrk = new data_aquisition_dll_worker();

    exec();

    delete wrk;
}

int data_aquisition_dll_system::doInitialize()
{
    return 0;
}

int data_aquisition_dll_system::doDispose()
{
    return 0;
}

int data_aquisition_dll_system::do_start_system()
{
    this->start();
    return 0;
}

abstract_configuration_widget *data_aquisition_dll_system::do_get_configuration_widget()
{
    return config_wdg;
}

void data_aquisition_dll_system::doProvideEntityManager(EntityManager *service)
{
    ManagerLocator::provideEntityManager(service);
}

void data_aquisition_dll_system::doProvideEventManager(EventManager *service)
{
    ManagerLocator::provideEventManager(service);
}

void data_aquisition_dll_system::doProvideSystemManager(SystemManager *service)
{
    ManagerLocator::provideSystemManager(service);
}

QCoreApplication *data_aquisition_dll_system::doMyQApp() const
{
    return qApp;
}


} // namespace talorion


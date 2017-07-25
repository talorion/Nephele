#include "script_system.hpp"


#include <QDebug>
#include "scripting_worker.hpp"
#include "core/EventManager.hpp"

#include "script_config_widget/script_config_widget.hpp"

#include <core/ManagerLocator.hpp>

#include <script_system/Scriptables/TofDaqScriptable.hpp>

namespace talorion {

script_system::script_system(QObject *par) :
    QThread(par),
    abstract_system("{8c2f892c-e2e2-417b-a3b1-96600e7822d3}"),
    config_wdg(NULL),
    m_direct_daq(Q_NULLPTR)
{
    config_wdg = new script_config_widget();

    QObject::connect(EventManager::get_instance(),SIGNAL(application_aboutToQuit()),(QThread*)this,SLOT(quit()));
}

script_system::~script_system()
{
    if(config_wdg)
        delete config_wdg;
}


void script_system::run()
{
    scripting_worker* wrk = new scripting_worker();
    wrk->initialize();

    exec();

    delete wrk;
}

int script_system::doInitialize()
{
    m_direct_daq = new TofDaqScriptable();
    EntityManager::get_instance()->createScriptableObject(m_direct_daq->script_name(), m_direct_daq);
    return 0;
}

int script_system::doDispose()
{
    return 0;
}

int script_system::do_start_system()
{
    this->start();
    return 0;
}

abstract_configuration_widget *script_system::do_get_configuration_widget()
{
    return config_wdg;
}

void script_system::doProvideEntityManager(EntityManager *service)
{
    ManagerLocator::provideEntityManager(service);
}

void script_system::doProvideEventManager(EventManager *service)
{
    ManagerLocator::provideEventManager(service);
}

void script_system::doProvideSystemManager(SystemManager *service)
{
    ManagerLocator::provideSystemManager(service);
}

QCoreApplication *script_system::doMyQApp() const
{
    return qApp;
}


}

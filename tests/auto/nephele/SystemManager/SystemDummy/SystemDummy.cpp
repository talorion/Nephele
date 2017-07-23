#include "DummyView.hpp"
#include "SystemDummy.hpp"

#include <core/ManagerLocator.hpp>

namespace talorion{

SystemDummy::SystemDummy() :
    abstract_system("{bb802ebc-4467-49e3-8c07-c3288a875233}"),
    m_sysemViews()
{
    m_sysemViews.append(new DummyView());
}

int SystemDummy::doInitialize()
{
    return 0;
}

int SystemDummy::doDispose()
{
    return 0;
}

int SystemDummy::do_start_system()
{
    return 0;
}

QString SystemDummy::do_get_system_name()
{
    return QString("System Dummy");
}

abstract_configuration_widget *SystemDummy::do_get_configuration_widget()
{
    return Q_NULLPTR;
}

void SystemDummy::doProvideEntityManager(EntityManager *service)
{
    ManagerLocator::provideEntityManager(service);
}

void SystemDummy::doProvideEventManager(EventManager *service)
{
    ManagerLocator::provideEventManager(service);
}

void SystemDummy::doProvideSystemManager(SystemManager *service)
{
    ManagerLocator::provideSystemManager(service);
}

QCoreApplication *SystemDummy::doMyQApp() const
{
    return qApp;
}

SystemViewsContainer SystemDummy::doSystemViews() const
{
    return m_sysemViews;
}

//  abstract_configuration_widget *SystemDummy::do_get_configuration_widget()
//  {
//      return Q_NULLPTR;
//  }

//  void SystemDummy::doProvideEntityManager(EntityManager *service)
//  {
//      ManagerLocator::provideEntityManager(service);
//  }

//  void SystemDummy::doProvideEventManager(EventManager *service)
//  {
//      ManagerLocator::provideEventManager(service);
//  }

//  void SystemDummy::doProvideSystemManager(SystemManager *service)
//  {
//      ManagerLocator::provideSystemManager(service);
//  }
}




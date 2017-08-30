#include "SerialBoxSystem.hpp"
#include "SerialBoxesThread.hpp"

#include <core/ManagerLocator.hpp>

#include <SerialBoxSystem/sbs_config_widget/sbs_config_widget.hpp>

namespace talorion {

SerialBoxSystem::SerialBoxSystem(QObject *par) :
    QObject(par),
    abstract_system("{1995e0a2-5470-4830-8f8f-65c72bbb6605}"),
    m_thread(Q_NULLPTR),
    config_wdg(new sbs_config_widget())
{

}

int SerialBoxSystem::doInitialize()
{
    EventManager &evtMng = ManagerLocator::eventManager();
    //auto sources = RegisteredDataEntityFactory::getAllRegisteredSourceEntities();
    //foreach (auto source, sources) {
    //    newRegisteredSourceEntity(source);
    //  }
    //connect(&evtMng, SIGNAL(newRegisteredSourceEntity(EntityManager::EntityID)), this, SLOT(newRegisteredSourceEntity(EntityManager::EntityID)));

    return 0;
}

int SerialBoxSystem::doDispose()
{
    if(m_thread == Q_NULLPTR){
        qInfo()<<"DataAquisitionSystem disposed while not started";
        return 0;
      }

    if(m_thread->isRunning()){
        m_thread->quit();
        m_thread->wait(1000);
      }

    delete m_thread;
    m_thread =Q_NULLPTR;

    //foreach(auto data, RegisteredDataEntityFactory::getAllRegisteredDataEntites()){
    //    ManagerLocator::entityManager().removeEntity(data);
    //  }

    //foreach(auto sources, RegisteredDataEntityFactory::getAllRegisteredSourceEntities()){
    //    ManagerLocator::entityManager().removeEntity(sources);
    //  }

    return 0;
}

int SerialBoxSystem::do_start_system()
{
    m_thread = new SerialBoxesThread();

    Q_ASSERT(m_thread != Q_NULLPTR);
    QEventLoop tmp_evt_loop(this);

    connect(m_thread ,SIGNAL(started()), &tmp_evt_loop, SLOT(quit()));

    QTimer::singleShot(1000 ,&tmp_evt_loop,SLOT(quit()));
    m_thread->start();

    tmp_evt_loop.exec();

    return 0;
}

QString SerialBoxSystem::do_get_system_name()
{
    return QString("SerialBoxSystem");
}

QCoreApplication *SerialBoxSystem::doMyQApp() const
{
    return qApp;
}

abstract_configuration_widget *SerialBoxSystem::do_get_configuration_widget()
{
    return config_wdg;
}

SystemViewsContainer SerialBoxSystem::doSystemViews() const
{
    return m_systemViews;
}

void SerialBoxSystem::doProvideEntityManager(EntityManager *service)
{
    ManagerLocator::provideEntityManager(service);
}

void SerialBoxSystem::doProvideEventManager(EventManager *service)
{
    ManagerLocator::provideEventManager(service);
}

void SerialBoxSystem::doProvideSystemManager(SystemManager *service)
{
    ManagerLocator::provideSystemManager(service);
}

} // namespace talorion

#include "AbstractSystem.hpp"

#include "ConfigurationWidget/AbstractConfigurationWidget.hpp"
#include "SystemView/AbstractSystemView.hpp"

#include <ManagerLocator.hpp>

namespace talorion {

  AbstractSystem::AbstractSystem(QObject *par) :
    QObject(par),
    m_state(SystemState::UNKNOWN),
    m_entity_id(EntityManager::invalid_id)
  {
  }

  void AbstractSystem::initialize(){

    foreach(QString comp_nme, defaultComponents()){
        ManagerLocator::entityManager().createNewComponent(comp_nme);
      }

    auto ret =doInitialize();
    if(ret==0)
      setState(SystemState::INITIALIZED);
    else
      setState(SystemState::NOT_INITIALIZED);
  }

  void AbstractSystem::dispose()
  {
    auto ret =doDispose();
    if(ret==0)
      setState(SystemState::DISPOSED);
    else
      setState(SystemState::NOT_DISPOSED);
  }

  void AbstractSystem::start()
  {
    auto ret =doStart();
    if(ret==0)
      setState(SystemState::STARTED);
    else
      setState(SystemState::NOT_STARTED);
  }

  EntityManager &AbstractSystem::entityManager() const
  {
    return ManagerLocator::entityManager();
  }

  QStringList AbstractSystem::defaultComponents() const
  {
    QStringList tmp{""};

    tmp.append(doGetDefaultComponents());

    return tmp;
  }

  AbstractConfigurationWidget* AbstractSystem::doConfigurationWidget() const{return Q_NULLPTR;}

  AbstractSystemView* AbstractSystem::doSystemView() const{return Q_NULLPTR;}

  void AbstractSystem::setState(SystemState st)
  {
    m_state =st;
  }

  void AbstractSystem::doProvideSystemManager(SystemManager *service)
  {
    ManagerLocator::provideSystemManager(service);
  }

  void AbstractSystem::doProvideEventManager(EventManager *service)
  {
    ManagerLocator::provideEventManager(service);
  }

  void AbstractSystem::doProvideEntityManager(EntityManager *service)
  {
    ManagerLocator::provideEntityManager(service);
  }

  SystemState AbstractSystem::state() const
  {
    return m_state;
  }

}

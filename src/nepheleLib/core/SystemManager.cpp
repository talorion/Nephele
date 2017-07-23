#include "SystemManager.hpp"

#include "EntityManager.hpp"
#include "ManagerLocator.hpp"

namespace talorion {

  SystemManager* SystemManager::_instance;
  QMutex SystemManager::_mutex;

  SystemManager::SystemManager(QObject *par) :
    QObject(par),
    m_systems()
  {

  }

  SystemManager::~SystemManager()
  {
    //    foreach (abstract_system* sys, m_systems) {
    //        delete sys;
    //        sys = NULL;
    //    }
    //    m_systems.clear();
  }

  SystemManager* SystemManager::get_instance()
  {
    if ( !_instance ){
        _mutex.lock();
        if (!_instance){
            //_instance = new SystemManager();
            _instance = &(ManagerLocator::systemManager());
          }
        _mutex.unlock();
      }
    return _instance;
  }//end getInstance

  void SystemManager::destroy()
  {
    _mutex.lock();
    if ( _instance )
      delete _instance;
    _instance = 0;
    _mutex.unlock();
  }

  QList<int> SystemManager::allSystems() const
  {
    QList<int> tmp;
    //QSet<abstract_system*> m_systems;
    foreach (abstract_system* sys, m_systems) {
        if(sys)
          tmp.append(sys->getEntity_id());
      }
    return tmp;
  }

  SystemManager::SystemContainer::size_type SystemManager::size() const
  {
    return allSystems().size();
  }

  void SystemManager::load_system(abstract_system *sys)
  {
    if(!sys)
      return;

    qInfo()<<"loading system: "<<sys->get_system_name();

    EntityManager& entity_mng_ext = ManagerLocator::entityManager();
    sys->provideEntityManager(&entity_mng_ext);
    EventManager& event_mng_ext = ManagerLocator::eventManager();
    sys->provideEventManager(&event_mng_ext);
    SystemManager& system_mng_ext = ManagerLocator::systemManager();
    sys->provideSystemManager(&system_mng_ext);

    auto tmp = sys->get_configuration_widget();
    auto sys_v = sys->systemViews();
    auto entity = EntityManager::get_instance()->createNewSystem(sys->getSerialVersionUID(),sys->get_system_name(), tmp, sys_v);
    //auto entity = entity_mng_ext.createNewSystem(sys->getSerialVersionUID(),sys->get_system_name(), tmp);
    sys->setEntity_id(entity);



    qInfo()<<"init system: "<<sys->get_system_name();
    m_systems.append(sys);
    sys->initialize();

    //sys->start_system();
  }

  void SystemManager::startAllSystems()
  {
    foreach (auto sys, m_systems) {
        qInfo()<<"starting system: "<<sys->get_system_name();
        sys->start_system();
      }
    EventManager& event_mng_ext = ManagerLocator::eventManager();
    emit event_mng_ext.allSystemsStarted();
  }

  void SystemManager::disposeAllSystems()
  {
    foreach (auto sys, m_systems) {
        qInfo()<<"disposing system: "<<sys->get_system_name();
        sys->dispose();
      }
  }

  void SystemManager::deleteAllSystems()
  {
    foreach (auto sys, m_systems) {
        qInfo()<<"deleting system: "<<sys->get_system_name();
        //sys.clear();
      }
    m_systems.clear();
  }


} // namespace talorion


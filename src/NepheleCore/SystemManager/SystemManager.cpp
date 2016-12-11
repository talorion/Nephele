#include "SystemManager.hpp"
#include "SystemEntityFactory.hpp"
#include "AbstractSystem.hpp"

#include "ManagerLocator.hpp"

#include <QDebug>
#include <QDir>
#include <QPluginLoader>

#ifndef NEPHELE_PLUGINS_DIR
#define NEPHELE_PLUGINS_DIR "plugins"
#endif

namespace talorion {

  //entity_manager system_manager::s_ent_mng(Q_NULLPTR);

  SystemManager::SystemManager(QObject *par) :
    QObject(par),
    m_all_systems()
  {
    //if(!m_ent_mng.is_connected_to_event_manager())
    //  m_ent_mng.connect_to_event_manager(&s_evt_mng);
  }

//  SystemManager::~SystemManager()
//  {

//  }

  SystemManager::SystemContainer SystemManager::allSystems() const
  {
    return m_all_systems;
  }

  SystemManager::SystemContainer::size_type SystemManager::size() const
  {
    return m_all_systems.size();
  }

  void SystemManager::startAllSystems()
  {
    foreach (auto sys, m_all_systems) {
        qDebug()<<"starting system: "<<sys->systemName();
        sys->start();
      }
    EventManager& event_mng_ext = ManagerLocator::eventManager();
    emit event_mng_ext.allSystemsStarted();
  }

  void SystemManager::disposeAllSystems()
  {
    foreach (auto sys, m_all_systems) {
        qDebug()<<"disposing system: "<<sys->systemName();
        sys->dispose();
      }
  }

  void SystemManager::deleteAllSystems()
  {
    foreach (auto sys, m_all_systems) {
        qDebug()<<"disposing system: "<<sys->systemName();
        sys.clear();
      }
    m_all_systems.clear();
  }

  void SystemManager::loadPluginSystems()
  {
    QDir plugDir = pluginsDir();
    qDebug()<<"loading systems from "<<plugDir.absolutePath();
    foreach (QString fileName, plugDir.entryList(QDir::Files)) {
        qDebug()<<"fileName "<<plugDir.absoluteFilePath(fileName);

        QPluginLoader loader(plugDir.absoluteFilePath(fileName));

        QObject *plugin = loader.instance();
        if(plugin != Q_NULLPTR){
            AbstractSystemPtr sys(qobject_cast<AbstractSystem *>(plugin));
            loadSystem(sys);
          }else{
            qDebug()<<loader.errorString();
          }

      }
  }

  QDir SystemManager::pluginsDir()
  {
    QDir pluginsDir = QDir::current();

    pluginsDir.cd(NEPHELE_PLUGINS_DIR);

    return pluginsDir;
  }

  void SystemManager::loadSystem(AbstractSystemPtr sys)
  {
    qDebug()<<"loading system: "<<sys->systemName();
    EntityManager& entity_mng_ext = ManagerLocator::entityManager();
    sys->provideEntityManager(&entity_mng_ext);
    EventManager& event_mng_ext = ManagerLocator::eventManager();
    sys->provideEventManager(&event_mng_ext);
    SystemManager& system_mng_ext = ManagerLocator::systemManager();
    sys->provideSystemManager(&system_mng_ext);

    createSystemEntity(sys);
    qDebug()<<"init system: "<<sys->systemName();
    m_all_systems.append(sys);
    sys->initialize();
  }

  void SystemManager::createSystemEntity(AbstractSystemPtr sys)
  {
    SystemEntityFactory& fact = SystemEntityFactory::instance();
    auto new_id = fact.createNewSystemEntity(sys);
    sys->setEntityId(new_id);
  }

} // namespace talorion

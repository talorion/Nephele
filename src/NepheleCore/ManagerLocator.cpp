#include "ManagerLocator.hpp"

namespace talorion {

  EntityManager* ManagerLocator::s_ent_mng  = Q_NULLPTR;
  EventManager*  ManagerLocator::s_evt_mng  = Q_NULLPTR;
  SystemManager* ManagerLocator::s_sys_mng  = Q_NULLPTR;

  EntityManager* ManagerLocator::s_null_ent_mng = new EntityManager(Q_NULLPTR);
  EventManager*  ManagerLocator::s_null_evt_mng = new EventManager(Q_NULLPTR);
  SystemManager* ManagerLocator::s_null_sys_mng = new SystemManager(Q_NULLPTR);

  bool ManagerLocator::s_instantiated =false;

  ManagerLocator::ManagerLocator(QObject *par):
    QObject(par)
  {
  }

  ManagerLocator::~ManagerLocator()
  {

  }


  void ManagerLocator::initialize() {
    if (s_ent_mng == Q_NULLPTR){s_ent_mng = s_null_ent_mng;}
    if (s_evt_mng == Q_NULLPTR){s_evt_mng = s_null_evt_mng;}
    if (s_sys_mng == Q_NULLPTR){s_sys_mng = s_null_sys_mng;}

    s_instantiated =true;
  }

  void ManagerLocator::dispose()
  {
    provideEntityManager(Q_NULLPTR);
    provideEventManager(Q_NULLPTR);
    provideSystemManager(Q_NULLPTR);

    s_instantiated = false;

  }

  EntityManager &ManagerLocator::entityManager() {
    if(!s_instantiated){initialize();}
    return *s_ent_mng;
  }

  void ManagerLocator::provideEntityManager(EntityManager *service){

    if(s_ent_mng == service)
      return;

    if(s_ent_mng != s_null_ent_mng && s_ent_mng != Q_NULLPTR)
      delete s_ent_mng;

    if (service == Q_NULLPTR){
        // Revert to null service.
        s_ent_mng = s_null_ent_mng;
      }else{
        s_ent_mng = service;
      }
  }

  EventManager &ManagerLocator::eventManager() {
    if(!s_instantiated){initialize();}
    return *s_evt_mng;
  }

  void ManagerLocator::provideEventManager(EventManager *service){

    if(s_evt_mng == service)
      return;

    if(s_evt_mng != s_null_evt_mng && s_evt_mng != Q_NULLPTR)
      delete s_evt_mng;

    if (service == Q_NULLPTR){
        // Revert to null service.
        s_evt_mng = s_null_evt_mng;
      }else{
        s_evt_mng = service;
      }
  }

  SystemManager &ManagerLocator::systemManager() {
    if(!s_instantiated){initialize();}
    return *s_sys_mng;
  }

  void ManagerLocator::provideSystemManager(SystemManager *service){

    if(s_sys_mng == service)
      return;

    if(s_sys_mng != s_null_sys_mng && s_sys_mng != Q_NULLPTR)
      delete s_sys_mng;

    if (service == Q_NULLPTR){
        // Revert to null service.
        s_sys_mng = s_null_sys_mng;
      }else{
        s_sys_mng = service;
      }
  }

} // namespace talorion

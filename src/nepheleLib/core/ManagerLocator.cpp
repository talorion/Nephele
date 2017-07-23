#include "ManagerLocator.hpp"


namespace talorion {

  EntityManager* ManagerLocator::s_ent_mng  = Q_NULLPTR;
  EventManager*  ManagerLocator::s_evt_mng  = Q_NULLPTR;
  SystemManager* ManagerLocator::s_sys_mng  = Q_NULLPTR;

  //EventManager*  ManagerLocator::s_null_evt_mng = new EventManager();
  //EntityManager* ManagerLocator::s_null_ent_mng = new EntityManager();
  //SystemManager* ManagerLocator::s_null_sys_mng = new SystemManager();
  EventManager*  ManagerLocator::s_null_evt_mng = Q_NULLPTR;
  EntityManager* ManagerLocator::s_null_ent_mng = Q_NULLPTR;
  SystemManager* ManagerLocator::s_null_sys_mng = Q_NULLPTR;

  bool ManagerLocator::s_instantiated =false;

  ManagerLocator::ManagerLocator(QObject *par):
    QObject(par)
  {
  }

  ManagerLocator::~ManagerLocator()
  {

  }


  void ManagerLocator::initialize() {
    //if (s_ent_mng == Q_NULLPTR){s_ent_mng = nullEntityManager();}
    //if (s_evt_mng == Q_NULLPTR){s_evt_mng = nullEventManager();}
    //if (s_sys_mng == Q_NULLPTR){s_sys_mng = nullSystemManager();}

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
    //if(!s_instantiated){initialize();}
    if (s_ent_mng == Q_NULLPTR){s_ent_mng = nullEntityManager();}
    return *s_ent_mng;
  }

  void ManagerLocator::provideEntityManager(EntityManager *service){

    if(s_ent_mng == service)
      return;

    if(s_ent_mng != nullEntityManager() && s_ent_mng != Q_NULLPTR)
      delete s_ent_mng;

    if (service == Q_NULLPTR){
        // Revert to null service.
        s_ent_mng = nullEntityManager();
      }else{
        s_ent_mng = service;
      }
  }

  EventManager &ManagerLocator::eventManager() {
    //if(!s_instantiated){initialize();}
    if (s_evt_mng == Q_NULLPTR){s_evt_mng = nullEventManager();}
    return *s_evt_mng;
  }

  void ManagerLocator::provideEventManager(EventManager *service){

    if(s_evt_mng == service)
      return;

    if(s_evt_mng != nullEventManager() && s_evt_mng != Q_NULLPTR)
      delete s_evt_mng;

    if (service == Q_NULLPTR){
        // Revert to null service.
        s_evt_mng = nullEventManager();
      }else{
        s_evt_mng = service;
      }
  }

  SystemManager &ManagerLocator::systemManager() {
    //if(!s_instantiated){initialize();}
    if (s_sys_mng == Q_NULLPTR){s_sys_mng = nullSystemManager();}
    return *s_sys_mng;
  }

  void ManagerLocator::provideSystemManager(SystemManager *service){

    if(s_sys_mng == service)
      return;

    if(s_sys_mng != nullSystemManager() && s_sys_mng != Q_NULLPTR)
      delete s_sys_mng;

    if (service == Q_NULLPTR){
        // Revert to null service.
        s_sys_mng = nullSystemManager();
      }else{
        s_sys_mng = service;
    }
  }

  EntityManager *ManagerLocator::nullEntityManager()
  {
      if(s_null_ent_mng == Q_NULLPTR)
          s_null_ent_mng = new EntityManager();
      return s_null_ent_mng;
  }

  EventManager *ManagerLocator::nullEventManager()
  {
      if(s_null_evt_mng == Q_NULLPTR)
          s_null_evt_mng = new EventManager();
      return s_null_evt_mng;
  }

  SystemManager *ManagerLocator::nullSystemManager()
  {
      if(s_null_sys_mng == Q_NULLPTR)
          s_null_sys_mng = new SystemManager();
      return s_null_sys_mng;
  }

} // namespace talorion

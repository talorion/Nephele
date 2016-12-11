#ifndef TALORION_MANAGER_LOCATOR_HPP
#define TALORION_MANAGER_LOCATOR_HPP

#include <QObject>

#include "EntityManager/EntityManager.hpp"
#include "EventManager/EventManager.hpp"
#include "SystemManager/SystemManager.hpp"

namespace talorion {

  class ManagerLocator : public QObject
  {
    Q_OBJECT
  public:
    explicit ManagerLocator(QObject *par = Q_NULLPTR);
    virtual ~ManagerLocator();

    static void initialize();
    static void dispose();

    static EntityManager& entityManager();
    static void provideEntityManager(EntityManager* service);

    static EventManager& eventManager();
    static void provideEventManager(EventManager* service);

    static SystemManager& systemManager();
    static void provideSystemManager(SystemManager* service);

  private:
    static EntityManager* s_ent_mng;
    static EventManager* s_evt_mng;
    static SystemManager* s_sys_mng;

    static EntityManager* s_null_ent_mng;
    static EventManager*  s_null_evt_mng;
    static SystemManager* s_null_sys_mng;

    static bool s_instantiated;

  };

} // namespace talorion

#endif // TALORION_MANAGER_LOCATOR_HPP

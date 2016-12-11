#include "SystemDummy.hpp"

#include <ManagerLocator.hpp>

namespace talorion{

  SystemDummy::SystemDummy(QObject *parent) : AbstractSystem(parent)
  {

  }

  SystemDummy::StateTransRet talorion::SystemDummy::doInitialize()
  {
    return 0;
  }

  SystemDummy::StateTransRet talorion::SystemDummy::doStart()
  {
    return 0;
  }

  SystemDummy::StateTransRet talorion::SystemDummy::doDispose()
  {
    return 0;
  }

  QString SystemDummy::doSystemName() const
  {
    return QString("System Dummy");
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
}

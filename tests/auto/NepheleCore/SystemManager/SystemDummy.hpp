#ifndef SYSTEMDUMMY_HPP
#define SYSTEMDUMMY_HPP

#include "SystemManager/AbstractSystem.hpp"

namespace talorion{
  class SystemDummy : public AbstractSystem
  {
    Q_OBJECT
  public:
    explicit SystemDummy(QObject *parent = 0);

  private:
    virtual StateTransRet doInitialize() Q_DECL_OVERRIDE;
    virtual StateTransRet doStart() Q_DECL_OVERRIDE;
    virtual StateTransRet doDispose() Q_DECL_OVERRIDE;

    // AbstractSystem interface
  private:
    virtual QString doSystemName() const Q_DECL_OVERRIDE;

  protected:
    virtual void doProvideEntityManager(EntityManager *service) Q_DECL_OVERRIDE;
    virtual void doProvideEventManager(EventManager *service) Q_DECL_OVERRIDE;
    virtual void doProvideSystemManager(SystemManager *service) Q_DECL_OVERRIDE;
  };
}

#endif // SYSTEMDUMMY_HPP

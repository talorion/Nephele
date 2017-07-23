#ifndef SYSTEMDUMMY_HPP
#define SYSTEMDUMMY_HPP

#include "core/abstract_system.hpp"

namespace talorion{
class SystemDummy : public abstract_system
{
public:
    SystemDummy();
    virtual ~SystemDummy() = default;

    // AbstractSystem interface
private:
    virtual int doInitialize() Q_DECL_OVERRIDE;
    virtual int doDispose() Q_DECL_OVERRIDE;
    virtual int do_start_system()Q_DECL_OVERRIDE;
    virtual QString do_get_system_name()Q_DECL_OVERRIDE;
    virtual abstract_configuration_widget* do_get_configuration_widget()Q_DECL_OVERRIDE;

protected:
    virtual void doProvideEntityManager(EntityManager* service) Q_DECL_OVERRIDE;
    virtual void doProvideEventManager(EventManager* service) Q_DECL_OVERRIDE;
    virtual void doProvideSystemManager(SystemManager* service) Q_DECL_OVERRIDE;

    QCoreApplication *doMyQApp() const Q_DECL_OVERRIDE;

private:
    SystemViewsContainer m_sysemViews;

    // AbstractSystem interface
private:
    virtual SystemViewsContainer doSystemViews() const Q_DECL_OVERRIDE;
};
}

#endif // SYSTEMDUMMY_HPP

#ifndef SERIALBOXSYSTEM_HPP
#define SERIALBOXSYSTEM_HPP

#include <core/abstract_system.hpp>

namespace talorion {

class SerialBoxesThread;
class sbs_config_widget;

class SerialBoxSystem :public QObject,  public abstract_system
{
    Q_OBJECT
public:
    explicit SerialBoxSystem(QObject *par = nullptr);
    virtual ~SerialBoxSystem() = default;
    Q_DISABLE_COPY(SerialBoxSystem)

signals:
    void newSystemView(int);

    // abstract_system interface
private:
    virtual int doInitialize() Q_DECL_OVERRIDE;
    virtual int doDispose() Q_DECL_OVERRIDE;
    virtual int do_start_system() Q_DECL_OVERRIDE;
    virtual QString do_get_system_name() Q_DECL_OVERRIDE;
    virtual QCoreApplication *doMyQApp() const Q_DECL_OVERRIDE;
    virtual abstract_configuration_widget *do_get_configuration_widget() Q_DECL_OVERRIDE;
    virtual SystemViewsContainer doSystemViews() const Q_DECL_OVERRIDE;

protected:
    virtual void doProvideEntityManager(EntityManager *service) Q_DECL_OVERRIDE;
    virtual void doProvideEventManager(EventManager *service) Q_DECL_OVERRIDE;
    virtual void doProvideSystemManager(SystemManager *service) Q_DECL_OVERRIDE;

private:
    SystemViewsContainer m_systemViews;
    SerialBoxesThread* m_thread;

    sbs_config_widget* config_wdg;
};

} // namespace talorion

#endif // SERIALBOXSYSTEM_HPP

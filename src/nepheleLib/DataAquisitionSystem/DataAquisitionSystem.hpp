#ifndef TALORION_DATAAQUISITIONSYSTEM_HPP
#define TALORION_DATAAQUISITIONSYSTEM_HPP

#include "core/abstract_system.hpp"

#include <core/EntityManager.hpp>
//#include "core/abstract_configuration_widget.hpp"

namespace talorion {

class DataAquisitionThread;
class DataAquisitionConfigurationWidget;

class DataAquisitionSystem : public QObject,  public abstract_system
{
    Q_OBJECT
public:
    explicit DataAquisitionSystem(QObject *par = Q_NULLPTR);
    virtual ~DataAquisitionSystem();

private:
   Q_DISABLE_COPY(DataAquisitionSystem)

public:
    Qt::HANDLE threadId();

signals:
  void newSystemView(int);

  // AbstractSystem interface
private:
    virtual int doInitialize() Q_DECL_OVERRIDE;
    virtual int doDispose() Q_DECL_OVERRIDE;
    virtual int do_start_system() Q_DECL_OVERRIDE;
    virtual QString do_get_system_name() Q_DECL_OVERRIDE;
    virtual QCoreApplication *doMyQApp() const Q_DECL_OVERRIDE;

protected:
    virtual void doProvideEntityManager(EntityManager *service) Q_DECL_OVERRIDE;
    virtual void doProvideEventManager(EventManager *service) Q_DECL_OVERRIDE;
    virtual void doProvideSystemManager(SystemManager *service) Q_DECL_OVERRIDE;

    virtual void doSetSystemViews(SystemViewsContainer views)Q_DECL_OVERRIDE{m_systemViews = views;}
    // AbstractSystem interface
private:
    virtual ActionsContainer doActions() const Q_DECL_OVERRIDE;
    virtual ToolbarsContainer doToolbars() const Q_DECL_OVERRIDE;
    virtual SystemViewsContainer doSystemViews()const Q_DECL_OVERRIDE;

private slots:
    void newRegisteredSourceEntity(EntityManager::EntityID entity);

private:
    ActionsContainer m_actions;
    ToolbarsContainer m_toolbars;
    DataAquisitionThread* m_thread;

    DataAquisitionConfigurationWidget* m_configurationWidget;

    SystemViewsContainer m_systemViews;

    // AbstractSystem interface
private:
    virtual abstract_configuration_widget *do_get_configuration_widget() Q_DECL_OVERRIDE;
};

} // namespace talorion

#endif // TALORION_DATAAQUISITIONSYSTEM_HPP

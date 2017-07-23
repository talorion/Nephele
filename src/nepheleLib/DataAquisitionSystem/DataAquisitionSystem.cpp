#include "DataAquisitionSystem.hpp"

#include "core/ManagerLocator.hpp"

#include "DataAquisitionThread.hpp"
#include <QDebug>
#include <QTimer>

#include "ConfigurationWidget/DataAquisitionConfigurationWidget.hpp"
#include <DataAquisitionSystem/RegisteredDataEntity/registereddataentityfactory.hpp>
#include <DataAquisitionSystem/SystemView/registereddataview.hpp>

namespace talorion {

  DataAquisitionSystem::DataAquisitionSystem(QObject *par) :
    QObject(par),
    abstract_system("{e6ec66b4-85e5-44bd-b9c5-73fea98413a8}"),
    m_actions(),
    m_toolbars(),
    m_thread(Q_NULLPTR),
    m_configurationWidget(Q_NULLPTR),
    m_systemViews()
  {
    Q_UNUSED(par);

    m_configurationWidget = new DataAquisitionConfigurationWidget();

    QToolBar* m_aquisitionToolBar;
    AbstractAction* m_startAquisitonAction;
    AbstractAction* m_stopAquisitonAction;

    QString aquisitionMenu = tr("&Aquisition");
    m_aquisitionToolBar = new QToolBar(tr("Aquisition"));

    m_startAquisitonAction = new AbstractAction(tr("Start"), Q_NULLPTR);
    m_startAquisitonAction->setIconText(QChar(0x23F5));
    m_startAquisitonAction->setStatusTip(tr("Start the Aqcuisition"));
    m_startAquisitonAction->setPreferedMenu(aquisitionMenu);
    m_aquisitionToolBar->addAction(m_startAquisitonAction);
    //connect(m_startAquisitonAction, SIGNAL(triggered(bool)), m_dataServer,SLOT(startAcquisition()));

    m_stopAquisitonAction = new AbstractAction(tr("Stop"), Q_NULLPTR);
    //m_stopAquisitonAction->setIconText(QChar(0x23F9));
    m_stopAquisitonAction->setIconText(QChar(0x2BC0));
    m_stopAquisitonAction->setStatusTip(tr("Stop the Aqcuisition"));
    m_stopAquisitonAction->setPreferedMenu(aquisitionMenu);
    m_aquisitionToolBar->addAction( m_stopAquisitonAction);
    //connect(m_stopAquisitonAction, SIGNAL(triggered(bool)), m_dataServer,SLOT(stopAcquisition()));

    m_toolbars<<m_aquisitionToolBar;
    m_actions<<m_startAquisitonAction<<m_stopAquisitonAction;
  }

  DataAquisitionSystem::~DataAquisitionSystem()
  {

  }

  Qt::HANDLE DataAquisitionSystem::threadId()
  {
    if(m_thread == Q_NULLPTR){
        return QThread::currentThreadId();
      }
    return m_thread->threadId();
  }

  int DataAquisitionSystem::doInitialize()
  {
    EventManager &evtMng = ManagerLocator::eventManager();
    auto sources = RegisteredDataEntityFactory::getAllRegisteredSourceEntities();
    foreach (auto source, sources) {
        newRegisteredSourceEntity(source);
      }
    connect(&evtMng, SIGNAL(newRegisteredSourceEntity(EntityManager::EntityID)), this, SLOT(newRegisteredSourceEntity(EntityManager::EntityID)));

    //connect(this, SIGNAL(newSystemView(int)), &evtMng, SIGNAL(newSystemView(int)));
    return 0;
  }

  int DataAquisitionSystem::doDispose()
  {
    if(m_thread == Q_NULLPTR){
        qInfo()<<"DataAquisitionSystem disposed while not started";
        return 0;
      }

    if(m_thread->isRunning()){
        m_thread->quit();
        m_thread->wait(1000);
      }

    delete m_thread;
    m_thread =Q_NULLPTR;

    foreach(auto data, RegisteredDataEntityFactory::getAllRegisteredDataEntites()){
        ManagerLocator::entityManager().removeEntity(data);
      }

    foreach(auto sources, RegisteredDataEntityFactory::getAllRegisteredSourceEntities()){
        ManagerLocator::entityManager().removeEntity(sources);
      }

    return 0;
  }

  int DataAquisitionSystem::do_start_system()
  {
    m_thread = new DataAquisitionThread();

    Q_ASSERT(m_thread != Q_NULLPTR);
    QEventLoop tmp_evt_loop(this);

    connect(m_thread ,SIGNAL(started()), &tmp_evt_loop, SLOT(quit()));

    QTimer::singleShot(1000 ,&tmp_evt_loop,SLOT(quit()));
    m_thread->start();

    tmp_evt_loop.exec();

    return 0;
  }

  QString DataAquisitionSystem::do_get_system_name()
  {
    return QString("DataAquisitionSystem");
  }

  QCoreApplication *DataAquisitionSystem::doMyQApp() const
  {
    return qApp;
  }

  void DataAquisitionSystem::doProvideEntityManager(EntityManager *service)
  {
    ManagerLocator::provideEntityManager(service);
  }

  void DataAquisitionSystem::doProvideEventManager(EventManager *service)
  {
    ManagerLocator::provideEventManager(service);
  }

  void DataAquisitionSystem::doProvideSystemManager(SystemManager *service)
  {
    ManagerLocator::provideSystemManager(service);
  }

  abstract_system::ActionsContainer DataAquisitionSystem::doActions() const
  {
    return m_actions;
  }

  abstract_system::ToolbarsContainer DataAquisitionSystem::doToolbars() const
  {
    return m_toolbars;
  }

  SystemViewsContainer DataAquisitionSystem::doSystemViews() const
  {
    return m_systemViews;
  }

  void DataAquisitionSystem::newRegisteredSourceEntity(EntityManager::EntityID entity)
  {
    RegisteredDataView* rdview = Q_NULLPTR;
    foreach (auto view, m_systemViews) {
        rdview = dynamic_cast<RegisteredDataView*>(view);
        if(rdview == Q_NULLPTR)
          continue;
        if(rdview->entity() == entity)
          return;

        rdview = Q_NULLPTR;
      }
    if(rdview != Q_NULLPTR)
      return;

    rdview = new RegisteredDataView(entity);
    addSystemView(rdview);

    EventManager &evtMng = ManagerLocator::eventManager();
    emit evtMng.newSystemView(getEntity_id());
  }

  abstract_configuration_widget *DataAquisitionSystem::do_get_configuration_widget()
  {
    return m_configurationWidget;
  }

} // namespace talorion






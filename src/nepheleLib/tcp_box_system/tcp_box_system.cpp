#include "tcp_box_system.hpp"


#include "core/EventManager.hpp"
#include "core/EntityManager.hpp"

#include "tcpdriver.hpp"
#include "qvmbackend.hpp"
#include "flowcontrollerbackend.h"

#include "tbs_config_widget/tbs_config_widget.hpp"

#include "tcp_box_worker.hpp"

#include <core/ManagerLocator.hpp>

#include <tcp_box_system/SystemView/tcpboxview.hpp>

namespace talorion {

  tcp_box_system::tcp_box_system(QObject *par):
    QThread(par),
    abstract_system("{bde1f255-8271-4e2c-bb5f-24e3a2ddffed}"),
    m_actions(),
    m_toolbars(),
    m_systemViews(),
    m_thread_id(),
    config_wdg(NULL)
  {
    config_wdg = new tbs_config_widget();

    qRegisterMetaType<tcpDriverDataTypes::dataType>("tcpDriverDataTypes::dataType");

    QObject::connect(EventManager::get_instance(),SIGNAL(application_aboutToQuit()),(QThread*)this,SLOT(quit()), Qt::QueuedConnection);

  }

  tcp_box_system::~tcp_box_system()
  {
    dispose();

    if(config_wdg)
      delete config_wdg;
  }

  Qt::HANDLE tcp_box_system::threadId()
  {
    if(isRunning())
      return m_thread_id;

    return QThread::currentThreadId();
  }

  void tcp_box_system::run()
  {
    auto old_thread_id = m_thread_id;
    m_thread_id = QThread::currentThreadId();
    tcp_box_worker* wrk = new tcp_box_worker();

    exec();

    delete wrk;
    m_thread_id = old_thread_id;
  }

  int tcp_box_system::doInitialize()
  {
    EventManager &evtMng = ManagerLocator::eventManager();
    auto boxes = ManagerLocator::entityManager().get_all_tcpBoxes();
    foreach (auto box, boxes) {
        newTcpBox(box);
      }
    connect(&evtMng, SIGNAL(newTcpBox(int)), this, SLOT(newTcpBox(EntityManager::EntityID)));
    //connect(&evtMng, SIGNAL(tcpBoxRemoved(int)), this, SLOT(tcpBoxRemoved(EntityManager::EntityID)));

    return 0;
  }

  int tcp_box_system::doDispose()
  {
    if(isRunning()){
        quit();
        wait(100);
      }
    return 0;
  }

  int tcp_box_system::do_start_system()
  {
    this->start();
    return 0;
  }

  abstract_configuration_widget *tcp_box_system::do_get_configuration_widget()
  {
    return config_wdg;
  }

  abstract_system::ActionsContainer tcp_box_system::doActions() const
  {
    return m_actions;
  }

  abstract_system::ToolbarsContainer tcp_box_system::doToolbars() const
  {
    return m_toolbars;
  }

  SystemViewsContainer tcp_box_system::doSystemViews() const
  {
    return m_systemViews;
  }

  void tcp_box_system::doProvideEntityManager(EntityManager *service)
  {
    ManagerLocator::provideEntityManager(service);
  }

  void tcp_box_system::doProvideEventManager(EventManager *service)
  {
    ManagerLocator::provideEventManager(service);
  }

  void tcp_box_system::doProvideSystemManager(SystemManager *service)
  {
    ManagerLocator::provideSystemManager(service);
  }

  QCoreApplication *tcp_box_system::doMyQApp() const
  {
    return qApp;
  }

  void tcp_box_system::newTcpBox(EntityManager::EntityID entity)
  {
    TcpBoxView* tcpboxview = Q_NULLPTR;
    foreach (auto view, m_systemViews) {
        tcpboxview = dynamic_cast<TcpBoxView*>(view);
        if(tcpboxview == Q_NULLPTR)
          continue;
        if(tcpboxview->entity() == entity)
          return;

        tcpboxview = Q_NULLPTR;
      }
    if(tcpboxview != Q_NULLPTR)
      return;

    tcpboxview = new TcpBoxView(entity);
    addSystemView(tcpboxview);

    EventManager &evtMng = ManagerLocator::eventManager();
    emit evtMng.newSystemView(getEntity_id());
  }

  //    int tcp_box_system::new_box_id()
  //    {
  //        return curr_box_id++;
  //    }

  //    //void tcp_box_system::connect_to_fc_box(QString ip, quint16 port)
  //    void tcp_box_system::connect_to_fc_box(int box_id)
  //    {
  //        qDebug()<<"creating fc box";

  //        flowControllerBackend* back = new flowControllerBackend();

  //        //int box_id = entity_manager::get_instance()->createNewTcpBox("new TCP Box", ip, port);
  //        QString ip = entity_manager::get_instance()->get_ip_address_component(box_id);
  //        quint16 port = entity_manager::get_instance()->get_port_component(box_id);

  //        tcpDriver* dev1;
  //        dev1 = new tcpDriver(box_id, "uibkafc getAll","uibkafc getAll", back); // for AFC Board
  //        bool co = dev1->connectDevice(ip, port);


  //        entity_manager::get_instance()->slot_connection_state_component(box_id, co);

  //        if(co)
  //            boxes.insert(box_id,dev1);
  //        else
  //            delete dev1;
  //    }

  //void tcp_box_system::connect_to_av_box(QString ip, quint16 port)
  //    void tcp_box_system::connect_to_av_box(int box_id)
  //    {
  //        qvmbackend* back = new qvmbackend();
  //        qDebug()<<"creating av box";

  //        //int box_id = entity_manager::get_instance()->createNewTcpBox("new TCP Box", ip, port);
  //        QString ip = entity_manager::get_instance()->get_ip_address_component(box_id);
  //        quint16 port = entity_manager::get_instance()->get_port_component(box_id);

  //        tcpDriver* dev1;
  //        dev1 = new tcpDriver(new_box_id(), "uibkav getAll","uibkav getAll", back); // for AFC Board
  //        bool co = dev1->connectDevice(ip, port);

  //        entity_manager::get_instance()->slot_connection_state_component(box_id, co);

  //        if(co)
  //            boxes.insert(box_id,dev1);
  //        else
  //            delete dev1;
  //    }

} // namespace talorion


#include "TcpboxSystem.hpp"

#include <QDateTime>
#include <QTimer>
#include <QEventLoop>
#include <ManagerLocator.hpp>
#include "EventManager/EventManager.hpp"


namespace talorion {

  TcpboxSystem::TcpboxSystem(QObject *par) :
    AbstractSystem(par),
    m_thread(new TcpboxSystemThread(*this))
  {

  }

  TcpboxSystem::~TcpboxSystem()
  {
    if(!m_thread.isNull()){
        if(m_thread->isRunning()){
            QTimer::singleShot(100 ,m_thread.data(), SLOT(quit()));
            m_thread->wait(1000);
          }
      }
  }


  bool TcpboxSystem::containsTcpbox(const tcpbox_container::value_type &tcpbox) const
  {
    return getConfiguredBoxes().contains(tcpbox);
  }

  void TcpboxSystem::deleteBox(const tcpbox_container::value_type &tcpbox)
  {
    entityManager().removeEntity(tcpbox);
    emit deleteTcpbox(tcpbox);
  }

  void TcpboxSystem::deleteAllBoxes()
  {
    auto tmp_con= getConfiguredBoxes();
    foreach (auto tcpbox,  tmp_con){
        deleteBox(tcpbox);
      }
  }

  Qt::HANDLE TcpboxSystem::threadId()
  {
    if(m_thread.isNull()){
        return QThread::currentThreadId();
      }
    return m_thread->threadId();
  }

  TcpboxSystem::tcpbox_container TcpboxSystem::getConfiguredBoxes() const
  {
    return TcpboxFactory::getInstance().getAllTcpboxes(*this);
  }

  //  tcpbox_system::tcpbox_command_container tcpbox_system::get_configured_box_commands(tcpbox_t tcpbox) const
  //  {
  //    return tcpbox_factory::get_instance().get_all_tcpboxes(*this);
  //  }

  AbstractSystem::StateTransRet TcpboxSystem::doInitialize()
  {
    EventManager& evt_mng      = ManagerLocator::eventManager();
    EventManager* evt_mng_ptr  = &evt_mng;

    QObject::connect(this, SIGNAL(newTcpbox(EntityManager::EntityID)), evt_mng_ptr, SIGNAL(newTcpbox(EntityManager::EntityID)),Qt::AutoConnection);
    QObject::connect(this, SIGNAL(deleteTcpbox(EntityManager::EntityID)), evt_mng_ptr, SIGNAL(deleteTcpbox(EntityManager::EntityID)),Qt::AutoConnection);

    return 0;
  }

  AbstractSystem::StateTransRet TcpboxSystem::doDispose()
  {
    EventManager& evt_mng      = ManagerLocator::eventManager();
    EventManager* evt_mng_ptr  = &evt_mng;

    QObject::disconnect(this, SIGNAL(newTcpbox(EntityManager::EntityID)), evt_mng_ptr, SIGNAL(newTcpbox(EntityManager::EntityID)));
    QObject::disconnect(this, SIGNAL(deleteTcpbox(EntityManager::EntityID)), evt_mng_ptr, SIGNAL(deleteTcpbox(EntityManager::EntityID)));

    if(!m_thread.isNull()){
        if(m_thread->isRunning()){
            //QTimer::singleShot(100 ,m_thread.data(), SLOT(quit()));
            m_thread->quit();
            m_thread->wait(1000);
          }
        //        if(m_thread->isRunning()){
        //            m_thread->terminate();
        //          }
      }

    return 0;
  }

  AbstractSystem::StateTransRet TcpboxSystem::doStart()
  {

    QEventLoop tmp_evt_loop(this);

    connect(m_thread.data() ,SIGNAL(started()), &tmp_evt_loop, SLOT(quit()));

    //QTimer::singleShot(100 ,m_thread.data(), SLOT(start()));
    QTimer::singleShot(1000 ,&tmp_evt_loop,SLOT(quit()));
    m_thread->start();

    tmp_evt_loop.exec();


    //    QTime myTimer;
    //    myTimer.start();
    //    do{
    //        QThread::currentThread()->msleep(100);
    //      }while(!(m_thread->isRunning()) || myTimer.elapsed()>1000 ) ;
    return 0;
  }

  QStringList TcpboxSystem::doGetDefaultComponents() const
  {
    QStringList tmp;

    tmp.append("NAME_COMPONENT");
    tmp.append("HOST_NAME_COMPONENT");
    tmp.append("PORT_COMPONENT");
    tmp.append("BOX_ID_COMPONENT");

    return tmp;

  }

  void TcpboxSystem::addBox(const tcpbox_container::value_type &tcpbox)
  {
    if(!(entityManager().isValid(tcpbox)))
      return;

    if(containsTcpbox(tcpbox))
      return;

    emit newTcpbox(tcpbox);

  }

  QString TcpboxSystem::doSystemName() const
  {
    return QString("TcpBoxSystem");
  }

  void TcpboxSystem::doProvideEntityManager(EntityManager *service)
  {
    ManagerLocator::provideEntityManager(service);
  }

  void TcpboxSystem::doProvideEventManager(EventManager *service)
  {
    ManagerLocator::provideEventManager(service);
  }

  void TcpboxSystem::doProvideSystemManager(SystemManager *service)
  {
    ManagerLocator::provideSystemManager(service);
  }


} // namespace talorion

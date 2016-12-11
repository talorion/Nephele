#include "TcpboxSystemThread.hpp"

#include "TcpboxSystem.hpp"
#include "EventManager/EventManager.hpp"

#include <ManagerLocator.hpp>


namespace talorion {

  TcpboxSystemThread::TcpboxSystemThread(const TcpboxSystem &sys, QObject *par) :
    QThread(par),
    m_sys(sys),
    m_thread_id(Q_NULLPTR),
    m_mutex()
  {

  }

  TcpboxSystemThread::~TcpboxSystemThread()
  {

  }

  Qt::HANDLE TcpboxSystemThread::threadId()
  {
    QMutexLocker locker(&m_mutex);

    if(m_thread_id == Q_NULLPTR){
        return QThread::currentThreadId();
      }
    return m_thread_id;
  }

  void TcpboxSystemThread::run()
  {
    m_mutex.lock();
    auto old_thread_id = m_thread_id;
    m_thread_id = QThread::currentThreadId();

    EventManager& evt_mng      = ManagerLocator::eventManager();
    EventManager* evt_mng_ptr  = &evt_mng;

    QScopedPointer<EcmdConnectionManager> m_con_mng(new EcmdConnectionManager(m_sys));
    connect(evt_mng_ptr, SIGNAL(newTcpbox(EntityManager::EntityID)),m_con_mng.data(), SLOT(slotNewTcpbox(EntityManager::EntityID)));
    connect(evt_mng_ptr, SIGNAL(deleteTcpbox(EntityManager::EntityID)),m_con_mng.data(), SLOT(slotDeleteTcpbox(EntityManager::EntityID)));
    connect(evt_mng_ptr, SIGNAL(connectBox(EntityManager::EntityID)), m_con_mng.data(), SLOT(slotConnectBox(EntityManager::EntityID)), Qt::QueuedConnection);
    connect(evt_mng_ptr, SIGNAL(disconnectBox(EntityManager::EntityID)), m_con_mng.data(), SLOT(slotDisconnectBox(EntityManager::EntityID)), Qt::QueuedConnection);
    connect(evt_mng_ptr, SIGNAL(sendCommandToBox(EntityManager::EntityID,QString)), m_con_mng.data(), SLOT(slotSendCommandToBox(EntityManager::EntityID,QString)), Qt::QueuedConnection);

    m_mutex.unlock();

    exec();

    m_mutex.lock();
    m_thread_id = old_thread_id;
    m_mutex.unlock();

  }

} // namespace talorion

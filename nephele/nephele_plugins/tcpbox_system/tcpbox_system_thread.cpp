#include "tcpbox_system_thread.hpp"

#include "tcpbox_system.hpp"
#include "event_manager/event_manager.hpp"


namespace talorion {

  tcpbox_system_thread::tcpbox_system_thread(const tcpbox_system& sys,QObject *par) :
    QThread(par),
    m_sys(sys),
    //m_tcpbox_clients(),
    m_thread_id(Q_NULLPTR),
    m_mutex(),
    m_connections()
  {

  }

  Qt::HANDLE tcpbox_system_thread::thread_id()
  {
    QMutexLocker locker(&m_mutex);

    if(m_thread_id == Q_NULLPTR){
        return QThread::currentThreadId();
      }
    return m_thread_id;
  }

  void tcpbox_system_thread::slot_connect_box(entity_manager::entity_id_t tcpbox_id)
  {
    QMutexLocker locker(&m_mutex);

    ecmd_connection* tmp_con=new ecmd_connection();
    tmp_con->moveToThread(this);

  }

  void tcpbox_system_thread::run()
  {
    m_mutex.lock();
    m_thread_id = QThread::currentThreadId();
    connect(&(m_sys.get_event_manager()), SIGNAL(connect_box(entity_manager::entity_id_t)),this,SLOT(slot_connect_box(entity_manager::entity_id_t)));

    m_mutex.unlock();

    exec();
  }

} // namespace talorion

#include "tcpbox_system_thread.hpp"

#include "tcpbox_system.hpp"
#include "event_manager/event_manager.hpp"


namespace talorion {

  tcpbox_system_thread::tcpbox_system_thread(const tcpbox_system &sys, QObject *par) :
    QThread(par),
    m_sys(sys),
    m_thread_id(Q_NULLPTR),
    m_mutex()
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

  void tcpbox_system_thread::run()
  {
    m_mutex.lock();
    auto old_thread_id = m_thread_id;
    m_thread_id = QThread::currentThreadId();

    QScopedPointer<ecmd_connection_manager> m_con_mng(new ecmd_connection_manager(m_sys));
    connect(&(m_sys.get_event_manager()), SIGNAL(new_tcpbox(entity_manager::entity_id_t)),m_con_mng.data(), SLOT(slot_new_tcpbox(entity_manager::entity_id_t)));
    connect(&(m_sys.get_event_manager()), SIGNAL(delete_tcpbox(entity_manager::entity_id_t)),m_con_mng.data(), SLOT(slot_delete_tcpbox(entity_manager::entity_id_t)));
    connect(&(m_sys.get_event_manager()), SIGNAL(connect_box(entity_manager::entity_id_t)), m_con_mng.data(), SLOT(slot_connect_box(entity_manager::entity_id_t)), Qt::QueuedConnection);
    connect(&(m_sys.get_event_manager()), SIGNAL(disconnect_box(entity_manager::entity_id_t)), m_con_mng.data(), SLOT(slot_disconnect_box(entity_manager::entity_id_t)), Qt::QueuedConnection);
    connect(&(m_sys.get_event_manager()), SIGNAL(send_command_to_box(entity_manager::entity_id_t,QString)), m_con_mng.data(), SLOT(slot_send_command_to_box(entity_manager::entity_id_t,QString)), Qt::QueuedConnection);

    m_mutex.unlock();

    exec();

    m_mutex.lock();
    m_thread_id = old_thread_id;
    m_mutex.unlock();

  }

} // namespace talorion

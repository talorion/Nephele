#include "tcpbox_system.hpp"

#include <QDateTime>



namespace talorion {

  tcpbox_system::tcpbox_system(QObject *par) :
    abstract_system(par),
    m_tcpbox_clients(),
    m_thread(new tcpbox_system_thread(*this))
  {

  }

  tcpbox_system::~tcpbox_system()
  {
    if(!m_thread.isNull()){
        m_thread->quit();
      }
  }


  bool tcpbox_system::contains_tcpbox(const tcpbox_container::value_type &tcpbox) const
  {
    return m_tcpbox_clients.contains(tcpbox);
    //return m_thread->contains_tcpbox(tcpbox);
  }

  void tcpbox_system::delete_box(const tcpbox_container::value_type &tcpbox)
  {
    //m_thread->delete_box(tcpbox);
    get_entity_manager().remove_entity(tcpbox);
    m_tcpbox_clients.removeAll(tcpbox);
  }

  void tcpbox_system::delete_all_boxes()
  {
    m_tcpbox_clients.clear();
    //m_thread->delete_all_boxes();
  }

  Qt::HANDLE tcpbox_system::thread_id()
  {
    if(m_thread.isNull()){
        return QThread::currentThreadId();
      }
    return m_thread->thread_id();
  }

  tcpbox_system::tcpbox_container tcpbox_system::get_configured_boxes() const
  {
    return m_tcpbox_clients;
  }

  abstract_system::state_trans_ret_t tcpbox_system::do_initialize()
  {
    return 0;
  }

  abstract_system::state_trans_ret_t tcpbox_system::do_dispose()
  {
    m_thread->quit();
    m_thread->wait();
    delete m_thread;

    return 0;
  }

  abstract_system::state_trans_ret_t tcpbox_system::do_start()
  {
    m_thread->start();



    QTime myTimer;
    myTimer.start();
    do{
        QThread::currentThread()->msleep(100);
      }while(!(m_thread->isRunning()) || myTimer.elapsed()>1000 ) ;
    return 0;
  }

  QStringList tcpbox_system::do_get_default_components() const
  {
    QStringList tmp;

    tmp.append("NAME_COMPONENT");
    tmp.append("HOST_NAME_COMPONENT");
    tmp.append("PORT_COMPONENT");
    tmp.append("BOX_ID_COMPONENT");

    return tmp;

  }

  void tcpbox_system::add_box(const tcpbox_container::value_type &tcpbox)
  {
    if(!(get_entity_manager().is_valid(tcpbox)))
      return;

    if(contains_tcpbox(tcpbox))
      return;

    m_tcpbox_clients.append(tcpbox);

    //return m_thread->add_box(tcpbox);
  }


} // namespace talorion

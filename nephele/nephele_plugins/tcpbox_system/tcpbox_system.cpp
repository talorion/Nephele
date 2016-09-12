#include "tcpbox_system.hpp"

#include <QDateTime>
#include <QTimer>
#include <QEventLoop>
#include "event_manager/event_manager.hpp"


namespace talorion {

  tcpbox_system::tcpbox_system(QObject *par) :
    abstract_system(par),
    m_thread(new tcpbox_system_thread(*this))
  {

  }

  tcpbox_system::~tcpbox_system()
  {
    if(!m_thread.isNull()){
        if(m_thread->isRunning()){
            QTimer::singleShot(100 ,m_thread.data(), SLOT(quit()));
            m_thread->wait(1000);
          }
      }
  }


  bool tcpbox_system::contains_tcpbox(const tcpbox_container::value_type &tcpbox) const
  {
    return get_configured_boxes().contains(tcpbox);
  }

  void tcpbox_system::delete_box(const tcpbox_container::value_type &tcpbox)
  {
    get_entity_manager().remove_entity(tcpbox);
    emit delete_tcpbox(tcpbox);
  }

  void tcpbox_system::delete_all_boxes()
  {
    auto tmp_con= get_configured_boxes();
    foreach (auto tcpbox,  tmp_con){
        delete_box(tcpbox);
      }
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
    return tcpbox_factory::get_instance().get_all_tcpboxes(*this);
  }

  tcpbox_system::tcpbox_command_container tcpbox_system::get_configured_box_commands(tcpbox_t tcpbox) const
  {
return tcpbox_factory::get_instance().get_all_tcpboxes(*this);
  }

  abstract_system::state_trans_ret_t tcpbox_system::do_initialize()
  {
    QObject::connect(this, SIGNAL(new_tcpbox(entity_manager::entity_id_t)), &(this->get_event_manager()), SIGNAL(new_tcpbox(entity_manager::entity_id_t)),Qt::AutoConnection);
    QObject::connect(this, SIGNAL(delete_tcpbox(entity_manager::entity_id_t)), &(this->get_event_manager()), SIGNAL(delete_tcpbox(entity_manager::entity_id_t)),Qt::AutoConnection);

    return 0;
  }

  abstract_system::state_trans_ret_t tcpbox_system::do_dispose()
  {

    QObject::disconnect(this, SIGNAL(new_tcpbox(entity_manager::entity_id_t)), &(this->get_event_manager()), SIGNAL(new_tcpbox(entity_manager::entity_id_t)));
    QObject::disconnect(this, SIGNAL(delete_tcpbox(entity_manager::entity_id_t)), &(this->get_event_manager()), SIGNAL(delete_tcpbox(entity_manager::entity_id_t)));

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

  abstract_system::state_trans_ret_t tcpbox_system::do_start()
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

    emit new_tcpbox(tcpbox);

  }


} // namespace talorion

#include "tcp_box_worker.hpp"


#include "core/EventManager.hpp"
#include "core/EntityManager.hpp"

#include "tcpdriver.hpp"
#include "qvmbackend.hpp"
#include "flowcontrollerbackend.h"
#include "rf_backend.hpp"

#include "backend_type.hpp"
#include "jsonrpcdriver.hpp"

namespace talorion {

  tcp_box_worker::tcp_box_worker(QObject *par) :
    QObject(par),
    curr_box_id(0),
    boxes(),
    bkends(),
    reconnectTimer(Q_NULLPTR)
  {
    connect(EventManager::get_instance(),SIGNAL(connect_tcp_box(int)),this,SLOT(slot_connect_tcp_box(int)), Qt::QueuedConnection);
    connect(EventManager::get_instance(),SIGNAL(disconnect_tcp_box(int)),this,SLOT(slot_disconnect_tcp_box(int)), Qt::QueuedConnection);
    connect(EventManager::get_instance(),SIGNAL(tcpBoxRemoved(int)), this, SLOT(tcpBoxRemoved(int)));

    connect(this,SIGNAL(tcp_box_disconnected(int)),EventManager::get_instance(),SIGNAL(disconnect_tcp_box(int)), Qt::QueuedConnection);

    foreach (int box, EntityManager::get_instance()->get_all_tcpBoxes()) {
        EntityManager::get_instance()->setComponentDataForEntity(AUTO_RECONNECT_COMPONENT, box, QVariant::fromValue(true));
        slot_connect_tcp_box(box);
      }

    reconnectTimer = new QTimer();
    reconnectTimer->setSingleShot(false);
    reconnectTimer->setInterval(10000);
    connect(reconnectTimer,SIGNAL(timeout()),this,SLOT(reconnect_all_boxes()));
    reconnectTimer->start();
  }

  tcp_box_worker::~tcp_box_worker()
  {
    reconnectTimer->stop();
    delete reconnectTimer;

    //QMap<int, tcpDriver*>::iterator it;
    for(auto it=boxes.begin();it !=boxes.end();it++){
        auto tmp= it.value();
        if(tmp)
          delete tmp;
      }
    boxes.clear();

    foreach (abstract_backend* var, bkends) {
        if(var)
          delete var;
      }
    bkends.clear();
  }

  int tcp_box_worker::count() const
  {
    return boxes.size();
  }

  void tcp_box_worker::connectToTcpBox(int entity) const
  {
    auto ent_mng = EntityManager::get_instance();
    if(ent_mng->isValid(entity) == false)
      return;

    auto all_boxes = ent_mng->get_all_tcpBoxes();
    if(all_boxes.contains(entity) == false)
      return;

    auto evt_mng =  EventManager::get_instance();
    emit evt_mng->connect_tcp_box(entity);

  }

  void tcp_box_worker::slot_connect_tcp_box(int entity)
  {
    int mode = EntityManager::get_instance()->get_tcp_box_backend_component(entity);

    auto it = boxes.find(entity);
    if(it == boxes.end()){
        switch(mode){
          case 0:{connect_to_fc_box(entity);emit tcp_box_connected(entity);break;}
          case 1:{connect_to_av_box(entity);emit tcp_box_connected(entity);break;}
          case 2:{connect_to_rf_box(entity);emit tcp_box_connected(entity);break;}
          case 3:{connect_to_jrpc_box(entity);break;}
          }
      }else{
        switch(mode){
          case 0:{reconnect_tcp_box(entity);break;}
          case 1:{break;}
          case 2:{break;}
          case 3:{break;}
          }
      }

  }

  void tcp_box_worker::slot_disconnect_tcp_box(int entity)
  {
    auto it = boxes.find(entity);
    if(it == boxes.end())
      return;

    auto dev1 = it.value();
    if(!dev1)
      return;

    dev1->disconectDevice();
    boxes.remove(entity);
    delete dev1;
  }

  void tcp_box_worker::slot_tcp_box_disconnected(int entity)
  {
    Q_UNUSED(entity);
  }

  void tcp_box_worker::reconnect_all_boxes()
  {

  }

  void tcp_box_worker::tcpBoxRemoved(int boxId)
  {
    auto it = boxes.find(boxId);
    if(it == boxes.end())
      return;

    auto drv = it.value();
    boxes.remove(boxId);
    delete drv;

  }

  int tcp_box_worker::new_box_id()
  {
    return curr_box_id++;
  }

  void tcp_box_worker::connect_to_fc_box(int box_id)
  {
    qDebug()<<"creating fc box";

    flowControllerBackend* back = new flowControllerBackend();

    QString ip = EntityManager::get_instance()->get_ip_address_component(box_id);
    quint16 port = EntityManager::get_instance()->get_port_component(box_id);

    tcpDriver* dev1;
    dev1 = new tcpDriver(box_id, "uibkafc getAll","uibkafc getAll", back); // for AFC Board
    connect(dev1, SIGNAL(disconnected(int)),this,SLOT(slot_tcp_box_disconnected(int)));
    bool co = dev1->connectDevice(ip, port, 1000);

    if(co)
      boxes.insert(box_id,dev1);
    else
      delete dev1;
  }

  void tcp_box_worker::connect_to_av_box(int box_id)
  {
    qvmbackend* back = new qvmbackend();
    qDebug()<<"creating av box";

    //int box_id = entity_manager::get_instance()->createNewTcpBox("new TCP Box", ip, port);
    QString ip = EntityManager::get_instance()->get_ip_address_component(box_id);
    quint16 port = EntityManager::get_instance()->get_port_component(box_id);

    tcpDriver* dev1;
    dev1 = new tcpDriver(box_id, "uibkav getAll","uibkav getAll", back); // for AFC Board
    bool co = dev1->connectDevice(ip, port, 1000);

    //entity_manager::get_instance()->slot_connection_state_component(box_id, co);

    if(co)
      boxes.insert(box_id,dev1);
    else
      delete dev1;
  }

  void tcp_box_worker::connect_to_rf_box(int box_id)
  {
    rf_backend* back = new rf_backend();
    qDebug()<<"creating rf box";

    //int box_id = entity_manager::get_instance()->createNewTcpBox("new TCP Box", ip, port);
    QString ip = EntityManager::get_instance()->get_ip_address_component(box_id);
    quint16 port = EntityManager::get_instance()->get_port_component(box_id);

    tcpDriver* dev1;
    dev1 = new tcpDriver(box_id, "uibk getAll","uibk getAll", back); // for AFC Board
    bool co = dev1->connectDevice(ip, port, 1000);

    //entity_manager::get_instance()->slot_connection_state_component(box_id, co);

    if(co)
      boxes.insert(box_id,dev1);
    else
      delete dev1;
  }

  void tcp_box_worker::connect_to_jrpc_box(int box_id)
  {
    AbstractTcpDriver* dev1;
    auto it = boxes.find(box_id);
    if(it == boxes.end())
      dev1 = new JsonRpcDriver(box_id);
    else
      dev1 = it.value();

    dev1->connectDevice();
    boxes.insert(box_id,dev1);
  }

  void tcp_box_worker::reconnect_tcp_box(int box_id)
  {
    Q_UNUSED(box_id)
  }

} // namespace talorion


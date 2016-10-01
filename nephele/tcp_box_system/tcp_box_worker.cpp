#include "tcp_box_worker.hpp"

#include "core/event_manager.hpp"
#include "core/entity_manager.hpp"

#include "tcpdriver.hpp"
#include "qvmbackend.hpp"
#include "flowcontrollerbackend.h"
#include "rf_backend.hpp"

namespace talorion {

    tcp_box_worker::tcp_box_worker(QObject *par) :
        QObject(par),
        curr_box_id(0),
        boxes(),
        bkends(),
        reconnectTimer(Q_NULLPTR)
    {
        connect(event_manager::get_instance(),SIGNAL(connect_tcp_box(int)),this,SLOT(slot_connect_tcp_box(int)), Qt::QueuedConnection);
        connect(event_manager::get_instance(),SIGNAL(disconnect_tcp_box(int)),this,SLOT(slot_disconnect_tcp_box(int)), Qt::QueuedConnection);

        connect(this,SIGNAL(tcp_box_disconnected(int)),event_manager::get_instance(),SIGNAL(disconnect_tcp_box(int)), Qt::QueuedConnection);

        foreach (int box, entity_manager::get_instance()->get_all_tcpBoxes()) {
            slot_connect_tcp_box(box);
        }

        reconnectTimer = new QTimer();
        reconnectTimer->setInterval(10000);
        connect(reconnectTimer,SIGNAL(timeout()),this,SLOT(reconnect_all_boxes()));
        reconnectTimer->start();
    }

    tcp_box_worker::~tcp_box_worker()
    {
        reconnectTimer->stop();

        QMap<int, tcpDriver*>::iterator it;
        for(it=boxes.begin();it !=boxes.end();it++){
            tcpDriver* tmp= it.value();
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

    void tcp_box_worker::slot_connect_tcp_box(int entity)
    {
        int mode = entity_manager::get_instance()->get_tcp_box_backend_component(entity);

        QMap<int, tcpDriver*>::iterator it = boxes.find(entity);
        if(it == boxes.end()){

            switch(mode){

            case 0:{connect_to_fc_box(entity);break;}
            case 1:{connect_to_av_box(entity);break;}
            case 2:{connect_to_rf_box(entity);break;}
            }

            //            if(mode == 0)
            //                connect_to_fc_box(entity);
            //            else
            //                connect_to_av_box(entity);
        }else{
            reconnect_tcp_box(entity);
          }
        //        else{
        //            tcpDriver* dev1 = it.value();
        //            if(!dev1)
        //                return;
        //            dev1->connect()
        //        }
    }

    void tcp_box_worker::slot_disconnect_tcp_box(int entity)
    {
        QMap<int, tcpDriver*>::iterator it = boxes.find(entity);
        if(it == boxes.end())
            return;

        tcpDriver* dev1 = it.value();
        if(!dev1)
            return;

        dev1->disconect();
        boxes.remove(entity);
        delete dev1;
    }

    void tcp_box_worker::slot_tcp_box_disconnected(int entity)
    {
//      foreach(int val, entity_manager::get_instance()->get_all_DValues()){
//          int box_id= entity_manager::get_instance()->get_box_id_component(val);
//          if(box_id == entity)
//            entity_manager::get_instance()->delete_entity(val);
//        }

//      foreach(int val, entity_manager::get_instance()->get_all_Values()){
//          int box_id= entity_manager::get_instance()->get_box_id_component(val);
//          if(box_id == entity)
//            entity_manager::get_instance()->delete_entity(val);
//        }


//      entity_manager::get_instance()->slot_connection_state_component(entity, false);
//      emit tcp_box_disconnected(entity);
    }

    void tcp_box_worker::reconnect_all_boxes()
    {
      foreach (int box, entity_manager::get_instance()->get_all_tcpBoxes()) {
          bool is_box_connected = entity_manager::get_instance()->get_connection_state_component(box);
          if(!is_box_connected)
            slot_connect_tcp_box(box);
      }
    }

    int tcp_box_worker::new_box_id()
    {
        return curr_box_id++;
    }

    void tcp_box_worker::connect_to_fc_box(int box_id)
    {
        qDebug()<<"creating fc box";

        flowControllerBackend* back = new flowControllerBackend();

        //int box_id = entity_manager::get_instance()->createNewTcpBox("new TCP Box", ip, port);
        QString ip = entity_manager::get_instance()->get_ip_address_component(box_id);
        quint16 port = entity_manager::get_instance()->get_port_component(box_id);

        tcpDriver* dev1;
        dev1 = new tcpDriver(box_id, "uibkafc getAll","uibkafc getAll", back); // for AFC Board
        connect(dev1, SIGNAL(disconnected(int)),this,SLOT(slot_tcp_box_disconnected(int)));
        bool co = dev1->connectDevice(ip, port, 1000);

        //entity_manager::get_instance()->slot_connection_state_component(box_id, co);

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
        QString ip = entity_manager::get_instance()->get_ip_address_component(box_id);
        quint16 port = entity_manager::get_instance()->get_port_component(box_id);

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
        QString ip = entity_manager::get_instance()->get_ip_address_component(box_id);
        quint16 port = entity_manager::get_instance()->get_port_component(box_id);

        tcpDriver* dev1;
        dev1 = new tcpDriver(box_id, "uibk getAll","uibk getAll", back); // for AFC Board
        bool co = dev1->connectDevice(ip, port, 1000);

        //entity_manager::get_instance()->slot_connection_state_component(box_id, co);

        if(co)
            boxes.insert(box_id,dev1);
        else
          delete dev1;
    }

    void tcp_box_worker::reconnect_tcp_box(int box_id)
    {
      QMap<int, tcpDriver*>::iterator it = boxes.find(box_id);
      if(it == boxes.end())
          return;

      tcpDriver* dev1 = it.value();
      if(!dev1)
          return;

      QString ip = entity_manager::get_instance()->get_ip_address_component(box_id);
      quint16 port = entity_manager::get_instance()->get_port_component(box_id);

      entity_manager::get_instance()->slot_connection_state_component(box_id, false);
      bool co = dev1->connectDevice(ip, port);
      //entity_manager::get_instance()->slot_connection_state_component(box_id, co);

    }

} // namespace talorion


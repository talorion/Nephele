#include "tcp_box_system.hpp"

#include "core/event_manager.hpp"
#include "core/entity_manager.hpp"

#include "tcpdriver.hpp"
#include "qvmbackend.hpp"
#include "flowcontrollerbackend.h"

#include "tbs_config_widget/tbs_config_widget.hpp"

namespace talorion {

    tcp_box_system::tcp_box_system(QObject *par):
        QThread(par),
        curr_box_id(0),
        boxes(),
        config_wdg(NULL)
    {
        config_wdg = new tbs_config_widget();

        qRegisterMetaType<tcpDriverDataTypes::dataType>("tcpDriverDataTypes::dataType");

        connect(event_manager::get_instance(),SIGNAL(application_aboutToQuit()),this,SLOT(quit()), Qt::QueuedConnection);

    }

    tcp_box_system::~tcp_box_system()
    {

    }

    void tcp_box_system::run()
    {
        connect(event_manager::get_instance(),SIGNAL(connect_tcp_box(int,int)),this,SLOT(slot_connect_tcp_box(int,int)), Qt::QueuedConnection);
        connect(event_manager::get_instance(),SIGNAL(disconnect_tcp_box(int)),this,SLOT(slot_disconnect_tcp_box(int)), Qt::QueuedConnection);


        //int box_id = entity_manager::get_instance()->createNewTcpBox("new TCP Box", "10.0.1.23", 2701);
        //connect_to_fc_box(box_id);

        exec();

        //        foreach (tcpDriver* drv, drivers) {
        //            delete drv;
        //        }
        //        drivers.clear();
    }

    void tcp_box_system::slot_connect_tcp_box(int entity, int mode)
    {

        QMap<int, tcpDriver*>::iterator it = boxes.find(entity);
        if(it == boxes.end()){
            if(mode== 0)
                connect_to_fc_box(entity);
            else
                connect_to_av_box(entity);
        }
        //        else{
        //            tcpDriver* dev1 = it.value();
        //            if(!dev1)
        //                return;
        //            dev1->connect()
        //        }

    }

    void tcp_box_system::slot_disconnect_tcp_box(int entity)
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

    void tcp_box_system::do_start_system()
    {
        this->start();
    }

    abstract_configuration_widget *tcp_box_system::do_get_configuration_widget()
    {
        return config_wdg;
    }

    int tcp_box_system::new_box_id()
    {
        return curr_box_id++;
    }

    //void tcp_box_system::connect_to_fc_box(QString ip, quint16 port)
    void tcp_box_system::connect_to_fc_box(int box_id)
    {
        flowControllerBackend* back = new flowControllerBackend();

        //int box_id = entity_manager::get_instance()->createNewTcpBox("new TCP Box", ip, port);
        QString ip = entity_manager::get_instance()->get_ip_address_component(box_id);
        quint16 port = entity_manager::get_instance()->get_port_component(box_id);

        tcpDriver* dev1;
        dev1 = new tcpDriver(box_id, "uibkafc getAll","uibkafc getAll", back); // for AFC Board
        bool co = dev1->connectDevice(ip, port);


        entity_manager::get_instance()->slot_connection_state_component(box_id, co);

        if(co)
            boxes.insert(box_id,dev1);
        else
            delete dev1;
    }

    //void tcp_box_system::connect_to_av_box(QString ip, quint16 port)
    void tcp_box_system::connect_to_av_box(int box_id)
    {
        qvmbackend* back = new qvmbackend();

        //int box_id = entity_manager::get_instance()->createNewTcpBox("new TCP Box", ip, port);
        QString ip = entity_manager::get_instance()->get_ip_address_component(box_id);
        quint16 port = entity_manager::get_instance()->get_port_component(box_id);

        tcpDriver* dev1;
        dev1 = new tcpDriver(new_box_id(), "uibkafc getAll","uibkafc getAll", back); // for AFC Board
        bool co = dev1->connectDevice(ip, port);

        entity_manager::get_instance()->slot_connection_state_component(box_id, co);

        if(co)
            boxes.insert(box_id,dev1);
        else
            delete dev1;
    }

} // namespace talorion


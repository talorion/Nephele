#include "tcp_box_system.hpp"

#include "event_manager/event_manager_locator.hpp"
#include "entity_manager/entity_manager.hpp"

#include "tcpdriver.hpp"
#include "qvmbackend.hpp"
#include "flowcontrollerbackend.h"

#include "tbs_config_widget/tbs_config_widget.hpp"

#include "tcp_box_worker.hpp"

#include <QTimer>

namespace talorion {

    tcp_box_system::tcp_box_system(QObject *par):
        QThread(par),
        abstract_system( "{bde1f255-8271-4e2c-bb5f-24e3a2ddffed}"),
        //        curr_box_id(0),
        //        boxes(),
        config_wdg(NULL)
    {
        //abstract_entity_manager* ent= entity_manager_locator::get_instance();
        //config_wdg = new tbs_config_widget();

        qRegisterMetaType<tcpDriverDataTypes::dataType>("tcpDriverDataTypes::dataType");

        //connect(event_manager_locator::get_instance(),SIGNAL(application_aboutToQuit()),this,SLOT(quit()), Qt::QueuedConnection);

    }

    tcp_box_system::~tcp_box_system()
    {
//        if(config_wdg)
//            delete config_wdg;

        exit(0);
        //QTimer::singleShot(0,this,SLOT(quit()));

        //wait();

    }

    void tcp_box_system::do_init_system()
    {
       //abstract_entity_manager* ent= entity_manager_locator::get_instance();

        config_wdg = new tbs_config_widget();

        connect(event_manager_locator::get_instance(),SIGNAL(application_aboutToQuit()),this,SLOT(quit()), Qt::QueuedConnection);
    }

    void tcp_box_system::do_dispose_system()
    {
        //QTimer::singleShot(0,this,SLOT(quit()));
        //quit();
        //wait();
        exit(0);
    }

    void tcp_box_system::run()
    {
        //        connect(event_manager::get_instance(),SIGNAL(connect_tcp_box(int)),this,SLOT(slot_connect_tcp_box(int)), Qt::QueuedConnection);
        //        connect(event_manager::get_instance(),SIGNAL(disconnect_tcp_box(int)),this,SLOT(slot_disconnect_tcp_box(int)), Qt::QueuedConnection);

        //        foreach (int box, entity_manager::get_instance()->get_all_tcpBoxes()) {
        //            slot_connect_tcp_box(box);
        //        }


        //int box_id = entity_manager::get_instance()->createNewTcpBox("new TCP Box", "10.0.1.23", 2701);
        //connect_to_fc_box(box_id);

        tcp_box_worker* wrk = new tcp_box_worker();
        connect(event_manager_locator::get_instance(),SIGNAL(application_aboutToQuit()),wrk,SLOT(deleteLater()), Qt::QueuedConnection);

        exec();

        //        foreach (tcpDriver* drv, drivers) {
        //            delete drv;
        //        }
        //        drivers.clear();

        //delete wrk;


    }

    //    void tcp_box_system::slot_connect_tcp_box(int entity)
    //    {

    //        int mode = entity_manager::get_instance()->get_tcp_box_backend_component(entity);

    //        QMap<int, tcpDriver*>::iterator it = boxes.find(entity);
    //        if(it == boxes.end()){
    //            if(mode == 0)
    //                connect_to_fc_box(entity);
    //            else
    //                connect_to_av_box(entity);
    //        }
    //        //        else{
    //        //            tcpDriver* dev1 = it.value();
    //        //            if(!dev1)
    //        //                return;
    //        //            dev1->connect()
    //        //        }

    //    }

    //    void tcp_box_system::slot_disconnect_tcp_box(int entity)
    //    {
    //        QMap<int, tcpDriver*>::iterator it = boxes.find(entity);
    //        if(it == boxes.end())
    //            return;

    //        tcpDriver* dev1 = it.value();
    //        if(!dev1)
    //            return;

    //        dev1->disconect();
    //        boxes.remove(entity);
    //        delete dev1;

    //    }

    void tcp_box_system::do_start_system()
    {
        //config_wdg = new tbs_config_widget();

        this->start();
    }

    abstract_configuration_widget *tcp_box_system::do_get_configuration_widget()
    {
        return config_wdg;
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


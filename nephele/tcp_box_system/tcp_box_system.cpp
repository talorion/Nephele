#include "tcp_box_system.hpp"

#include "core/event_manager.hpp"

#include "tcpdriver.hpp"
#include "qvmbackend.hpp"
#include "flowcontrollerbackend.h"

namespace talorion {

    tcp_box_system::tcp_box_system(QObject *par):
        QThread(par),
        curr_box_id(0)
    {
        connect(event_manager::get_instance(),SIGNAL(application_aboutToQuit()),this,SLOT(quit()));
    }

    tcp_box_system::~tcp_box_system()
    {

    }

    void tcp_box_system::run()
    {
        tcpDriver* dev1;
        //qvmbackend* dcs;
        flowControllerBackend* dcs;

        dcs = new flowControllerBackend();

        dev1 = new tcpDriver(new_box_id(), "uibkafc getAll","uibkafc getActSet"); // for AFC Board
        //dev1->connectDevice("192.168.0.90");
        dev1->connectDevice("10.0.1.23");

        //sudo ifconfig eth0:0 192.168.168.90
        //sudo ifconfig eth0:0 down
        //tcpDriver* dev2;
        //dev2 = new tcpDriver(new_box_id(), "uibkafc getAll","uibkafc getActSet"); // for AFC Board
        //dev2->connectDevice("192.168.168.90");

        exec();

        delete dcs;
        delete dev1;
    }

    void tcp_box_system::do_start_system()
    {
        this->start();
    }

    int tcp_box_system::new_box_id()
    {
        return curr_box_id++;
    }

} // namespace talorion


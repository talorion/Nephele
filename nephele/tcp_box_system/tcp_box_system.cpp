#include "tcp_box_system.hpp"

#include "core/event_manager.hpp"

#include "tcpdriver.hpp"
#include "qvmbackend.hpp"
#include "flowcontrollerbackend.h"

namespace talorion {

    tcp_box_system::tcp_box_system(QObject *par):
        QThread(par),
        curr_box_id(0),
        drivers()
    {
        qRegisterMetaType<tcpDriverDataTypes::dataType>("tcpDriverDataTypes::dataType");

        connect(event_manager::get_instance(),SIGNAL(application_aboutToQuit()),this,SLOT(quit()));
    }

    tcp_box_system::~tcp_box_system()
    {

    }

    void tcp_box_system::run()
    {


        //flowControllerBackend* dcs;
        //dcs = new flowControllerBackend();

        //tcpDriver* dev1;
        //dev1 = new tcpDriver(0, "uibkafc getAll","uibkafc getActSet"); // for AFC Board
        //dev1->connectDevice("192.168.0.90");
        //dev1->connectDevice("10.0.1.23");
        connect_to_fc_box("10.0.1.23");

        //sudo ifconfig eth0:0 192.168.168.90
        //sudo ifconfig eth0:0 down
        //tcpDriver* dev2;
        //dev2 = new tcpDriver(1, "uibkafc getAll","uibkafc getActSet"); // for AFC Board
        //dev2->connectDevice("10.0.1.23",4414);
        //dev2->connectDevice("localhost");
        connect_to_fc_box("10.0.1.23",4414);

        exec();


        foreach (tcpDriver* drv, drivers) {
            delete drv;
        }
        drivers.clear();
    }

    void tcp_box_system::do_start_system()
    {
        this->start();
    }

    int tcp_box_system::new_box_id()
    {
        return curr_box_id++;
    }

    void tcp_box_system::connect_to_fc_box(QString ip, quint16 port)
    {
        tcpDriver* dev1;
        dev1 = new tcpDriver(new_box_id(), "uibkafc getAll","uibkafc getActSet"); // for AFC Board
        dev1->connectDevice(ip, port);

        drivers.append(dev1);
    }

} // namespace talorion


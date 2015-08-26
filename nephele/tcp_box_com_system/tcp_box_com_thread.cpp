#include "tcp_box_com_thread.hpp"

#include "core/event_manager.hpp"

#include "tcpdriver.hpp"
#include "qvmbackend.hpp"
#include "flowcontrollerbackend.h"

namespace talorion {

    tcp_box_com_thread::tcp_box_com_thread(QObject *par):
        QThread(par)
    {
        connect(event_manager::get_instance(),SIGNAL(application_aboutToQuit()),this,SLOT(quit()));
    }

    void tcp_box_com_thread::run()
    {
        tcpDriver* dev1;
        //qvmbackend* dcs;
        flowControllerBackend* dcs;

        dcs = new flowControllerBackend();

        //dev1 = new tcpDriver("uibkav getAll","uibkav getAll"); // for DC Board
        dev1 = new tcpDriver("uibkafc getAll","uibkafc getActSet"); // for AFC Board

        //dev1->connectDevice("192.168.0.90");
        dev1->connectDevice("10.0.1.23");
        //connect(dev1, SIGNAL(receivedCustomData(QByteArray)),this,SLOT(displayCustomResponse(QByteArray)));

        exec();

        delete dcs;
        delete dev1;
    }

} // namespace talorion


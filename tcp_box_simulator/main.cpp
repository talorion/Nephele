
#include <QApplication>
#include <QTime>
#include <QtCore>

#include "tcp_box_simulator.hpp"

#include "simulated_av_box.hpp"
#include "simulated_fc_box.h"
#include "simulated_ao_box.hpp"

int main(int argc, char *argv[])
{
    QTime time = QTime::currentTime();
    qsrand((uint)time.msec());

    QApplication a(argc, argv);

    simulated_fc_box* fc_box = new simulated_fc_box();
    tcp_box_simulator w(fc_box);
    w.show();

    simulated_av_box* av_box = new simulated_av_box();
    tcp_box_simulator t(av_box, 4414);
    t.show();

    simulated_ao_box* ao_box = new simulated_ao_box();
    tcp_box_simulator d(ao_box, 4223);
    d.show();

    int ret = a.exec();

    delete fc_box;
    delete av_box;

    return ret;
}

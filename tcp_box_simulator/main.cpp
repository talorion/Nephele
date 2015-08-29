
#include <QApplication>
#include <QTime>
#include <QtCore>

#include "tcp_box_simulator.hpp"

int main(int argc, char *argv[])
{
    QTime time = QTime::currentTime();
    qsrand((uint)time.msec());

    QApplication a(argc, argv);
    tcp_box_simulator w;
    w.show();

    tcp_box_simulator t;
    t.show();

    return a.exec();
}

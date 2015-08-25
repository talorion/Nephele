#include "tcp_box_simulator.hpp"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    tcp_box_simulator w;
    w.show();

    return a.exec();
}

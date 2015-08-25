#include "gui_system/nephele_main_window.hpp"
#include <QApplication>


using namespace talorion;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    nephele_main_window w;
    w.show();

    return a.exec();
}


#include <QApplication>


#include "core/event_manager.hpp"

#include "gui_system/gui_system.hpp"
#include "script_system/script_system.hpp"
#include "tcp_box_system/tcp_box_system.hpp"



using namespace talorion;

int main(int argc, char *argv[])
{
    //Q_INIT_RESOURCE(application);


    QApplication a(argc, argv);

    QObject::connect(&a,SIGNAL(aboutToQuit()), event_manager::get_instance(), SIGNAL(application_aboutToQuit()));


    gui_system* gui_s = new gui_system();
    gui_s->start_system();

    script_system *script_s = new script_system();
    script_s->start_system();

    tcp_box_system *tcp_box_s = new tcp_box_system();
    tcp_box_s->start_system();

    int ret = a.exec();

    event_manager::destroy();

    return ret;
}

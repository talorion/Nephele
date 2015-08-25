
#include <QApplication>

#include "core/event_manager.hpp"

#include "gui_system/nephele_main_window.hpp"
#include "script_system/scripting_thread.hpp"
#include "tcp_box_com_system/tcp_box_com_thread.hpp"


using namespace talorion;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QObject::connect(&a,SIGNAL(aboutToQuit()), event_manager::get_instance(), SIGNAL(application_aboutToQuit()));

    //start the gui
    nephele_main_window w;
    w.show();

    //start the systems;
    scripting_thread *scrpt_thrd = new scripting_thread(&a);
    scrpt_thrd->start();

    tcp_box_com_thread *com_thrd = new tcp_box_com_thread(&a);
    com_thrd->start();

    int ret = a.exec();

    event_manager::destroy();

    return ret;
}

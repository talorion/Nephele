#ifndef BUILD
#include "build_number.h"
#endif

#include <QApplication>
#include <QDebug>

#include "core/entity_manager.hpp"
#include "core/event_manager.hpp"
#include "core/system_manager.hpp"

#include "data_aquisition_dll_system/data_aquisition_dll_system.hpp"
#include "gui_system/gui_system.hpp"
#include "script_system/script_system.hpp"
#include "tcp_box_system/tcp_box_system.hpp"
#include "data_tools_dll_system/data_tools_dll_system.hpp"
#include "power_supply_dll_system/power_supply_dll_system.hpp"


using namespace talorion;

int main(int argc, char *argv[])
{
    int ret = 0;
    QString v = "0.1.0."+QString::number(BUILD);
    qDebug()<<v;

    QApplication a(argc, argv);
    QCoreApplication::setOrganizationName("Nephele");
    QCoreApplication::setOrganizationDomain("github.com/talorion");
    QCoreApplication::setApplicationName("Nephele");
    QCoreApplication::setApplicationVersion(v);

    entity_manager::get_instance()->initialize();

    QObject::connect(&a,SIGNAL(aboutToQuit()), event_manager::get_instance(), SIGNAL(application_aboutToQuit()));

    system_manager::get_instance()->register_new_system<gui_system>();

    system_manager::get_instance()->register_new_system<script_system>();

    system_manager::get_instance()->register_new_system<tcp_box_system>();

    system_manager::get_instance()->register_new_system<data_aquisition_dll_system>();

    system_manager::get_instance()->register_new_system<data_tools_dll_system>();

    system_manager::get_instance()->register_new_system<power_supply_dll_system>();

    ret = a.exec();

    event_manager::destroy();

    entity_manager::get_instance()->dispose();
    entity_manager::destroy();

    return ret;
}

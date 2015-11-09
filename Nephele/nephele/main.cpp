#ifndef BUILD
#include "build_number.h"
#endif

#include <QApplication>
#include <QDebug>

#include "entity_manager/entity_manager_locator.hpp"
#include "entity_manager/entity_manager.hpp"
#include "event_manager/event_manager_locator.hpp"
#include "event_manager/event_manager.hpp"
#include "system_manager.hpp"

//#include "data_aquisition_dll_system.hpp"
#include "gui_system/gui_system.hpp"
#include "script_system/script_system.hpp"
//#include "tcp_box_system.hpp"
//#include "data_tools_dll_system/data_tools_dll_system.hpp"
//#include "power_supply_dll_system/power_supply_dll_system.hpp"


using namespace talorion;

int main(int argc, char *argv[])
{

    int ret = -1;

    try{
        QString v = "0.1.0."+QString::number(BUILD);
        qDebug()<<v;

        QApplication a(argc, argv);
        QCoreApplication::setOrganizationName("Nephele");
        QCoreApplication::setOrganizationDomain("github.com/talorion");
        QCoreApplication::setApplicationName("Nephele");
        QCoreApplication::setApplicationVersion(v);

        event_manager* evt_mng = new event_manager();
        event_manager_locator::provide(evt_mng);

        entity_manager* ent_mng = new entity_manager();
        entity_manager_locator::provide(ent_mng);


        entity_manager_locator::get_instance()->initialize();
        //event_manager_locator::get_instance()->initialize();


        QObject::connect(&a,SIGNAL(aboutToQuit()), event_manager_locator::get_instance(), SIGNAL(application_aboutToQuit()));

        //==== core systems==
        system_manager::get_instance()->register_new_system<gui_system>();

        system_manager::get_instance()->register_new_system<script_system>();
        //===================

        //==== plugin systems==
        //system_manager::get_instance()->register_new_system<tcp_box_system>();
        system_manager::get_instance()->load_plugin_systems();

        //system_manager::get_instance()->register_new_system<data_aquisition_dll_system>();

        //system_manager::get_instance()->register_new_system<data_tools_dll_system>();

        //system_manager::get_instance()->register_new_system<power_supply_dll_system>();
        //===================

        ret = a.exec();

        //event_manager_locator::destroy();

        //entity_manager_locator::get_instance()->dispose();
        //entity_manager_locator::destroy();

        system_manager::get_instance()->destroy();

        delete evt_mng;
        delete ent_mng;

    } catch(const std::runtime_error &ex){
        qDebug()<<ex.what();
        ret = -2;
    } catch (const std::exception& ex) {
        qDebug()<<ex.what();
        ret = -3;
//    } catch (const std::string& ex) {
//        qDebug()<<ex.what();
//        ret = -4;
    } catch (...) {
        qDebug()<<"OMG! an unexpected exception has been caught";
        ret = -5;
    }
    return ret;
}
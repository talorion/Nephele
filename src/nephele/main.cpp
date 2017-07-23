#ifndef BUILD
#include "build_number.h"
#endif

//#include <vld.h>

#include <QApplication>
#include <QDebug>
#include <QFontDatabase>

#include "core/EntityManager.hpp"
#include "core/EventManager.hpp"
#include "core/SystemManager.hpp"

#include "core/ManagerLocator.hpp"

//#include "data_aquisition_dll_system/data_aquisition_dll_system.hpp"
#include "gui_system/gui_system.hpp"
#include "script_system/script_system.hpp"
#include "tcp_box_system/tcp_box_system.hpp"
//#include "data_tools_dll_system/data_tools_dll_system.hpp"
//#include "power_supply_dll_system/power_supply_dll_system.hpp"
#include "DataAquisitionSystem/DataAquisitionSystem.hpp"


using namespace talorion;

int main(int argc, char *argv[])
{

    int ret = -1;


    qDebug()<<QUuid::createUuid();

    try{
        QString v = "0.1.0."+QString::number(BUILD);
        qDebug()<<v;

        QApplication a(argc, argv);
        QCoreApplication::setOrganizationName("Nephele");
        QCoreApplication::setOrganizationDomain("github.com/talorion");
        QCoreApplication::setApplicationName("Nephele");
        QCoreApplication::setApplicationVersion(v);
        //int id = QFontDatabase::addApplicationFont(":/fonts/Cantarell-Regular.ttf");

        EntityManager::get_instance()->initialize();

        QObject::connect(&a,SIGNAL(aboutToQuit()), EventManager::get_instance(), SIGNAL(application_aboutToQuit()));

        //SystemManager::get_instance()->register_new_system<gui_system>();
        ManagerLocator::systemManager().register_new_system<gui_system>();
        //SystemManager::get_instance()->register_new_system<script_system>();
        ManagerLocator::systemManager().register_new_system<script_system>();
        //SystemManager::get_instance()->register_new_system<tcp_box_system>();
        ManagerLocator::systemManager().register_new_system<tcp_box_system>();
        //SystemManager::get_instance()->register_new_system<data_aquisition_dll_system>();
        //ManagerLocator::systemManager().register_new_system<data_aquisition_dll_system>();
        //SystemManager::get_instance()->register_new_system<data_tools_dll_system>();
        //ManagerLocator::systemManager().register_new_system<data_tools_dll_system>();
        //SystemManager::get_instance()->register_new_system<power_supply_dll_system>();
        //ManagerLocator::systemManager().register_new_system<power_supply_dll_system>();
        //SystemManager::get_instance()->startAllSystems();
        ManagerLocator::systemManager().register_new_system<DataAquisitionSystem>();
        ManagerLocator::systemManager().startAllSystems();

        ret = a.exec();

        EventManager::destroy();

        SystemManager::get_instance()->disposeAllSystems();

        EntityManager::get_instance()->dispose();
        EntityManager::destroy();

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

#include "data_tools_dll_system.hpp"

#include "dtd_config_widget/dtd_config_widget.hpp"

#include "event_manager/event_manager_locator.hpp"

#include <QTimer>

namespace talorion {

    data_tools_dll_system::data_tools_dll_system( QObject *par) :
        QThread(par),
        abstract_system("{41dc1dd5-2b1f-4424-8203-7b887c78e437}"),
        config_wdg(NULL)
    {
//        config_wdg =  new dtd_config_widget();

//        connect(event_manager_locator::get_instance(),SIGNAL(application_aboutToQuit()),this,SLOT(quit()));
    }

    data_tools_dll_system::~data_tools_dll_system()
    {
        //delete config_wdg;
        exit(0);
    }

    void data_tools_dll_system::do_init_system()
    {
        config_wdg =  new dtd_config_widget();

        connect(event_manager_locator::get_instance(),SIGNAL(application_aboutToQuit()),this,SLOT(quit()));
    }

    void data_tools_dll_system::do_dispose_system()
    {
//        QTimer::singleShot(0,this,SLOT(quit()));

//        wait();
        exit(0);
    }

    void data_tools_dll_system::run()
    {
        exec();
    }

    void data_tools_dll_system::do_start_system()
    {
        this->start();
    }

    abstract_configuration_widget *data_tools_dll_system::do_get_configuration_widget()
    {
        return config_wdg;
    }

} // namespace talorion


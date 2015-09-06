#include "data_aquisition_dll_system.hpp"

#include "dad_config_widget/dad_config_widget.hpp"
#include "data_aquisition_dll_wrapper.hpp"
#include "core/event_manager.hpp"

namespace talorion {

    data_aquisition_dll_system::data_aquisition_dll_system(QObject *par) :
        QThread(par),
        abstract_system("{e6ec66b4-85e5-44bd-b9c5-73fea98413a8}"),
        config_wdg(NULL)
    {
        config_wdg =  new dad_config_widget();
        connect(event_manager::get_instance(),SIGNAL(application_aboutToQuit()),this,SLOT(quit()));
    }

    data_aquisition_dll_system::~data_aquisition_dll_system()
    {

    }

    void data_aquisition_dll_system::run()
    {
        data_aquisition_dll_wrapper* wrap= new data_aquisition_dll_wrapper();
        //wrap->init();

        exec();

        wrap->dispose();
        delete wrap;
    }

    void data_aquisition_dll_system::do_start_system()
    {
        this->start();
    }

    abstract_configuration_widget *data_aquisition_dll_system::do_get_configuration_widget()
    {
        return config_wdg;
    }


} // namespace talorion


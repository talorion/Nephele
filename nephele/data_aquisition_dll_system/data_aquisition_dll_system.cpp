#include "data_aquisition_dll_system.hpp"

#include "data_aquisition_dll_wrapper.hpp"
#include "core/event_manager.hpp"

namespace talorion {

    data_aquisition_dll_system::data_aquisition_dll_system(QObject *par) :
        QThread(par),
        abstract_system()
    {
        connect(event_manager::get_instance(),SIGNAL(application_aboutToQuit()),this,SLOT(quit()));
    }

    data_aquisition_dll_system::~data_aquisition_dll_system()
    {

    }

    void data_aquisition_dll_system::run()
    {
        data_aquisition_dll_wrapper* wrap= new data_aquisition_dll_wrapper();
        wrap->init();

        exec();

        wrap->dispose();
        delete wrap;
    }

    void data_aquisition_dll_system::do_start_system()
    {
        this->start();
    }


} // namespace talorion


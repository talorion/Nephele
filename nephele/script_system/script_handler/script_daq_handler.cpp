#include "script_daq_handler.hpp"

#include "core/event_manager.hpp"

namespace talorion{

    script_daq_handler::script_daq_handler(QObject *par) : QObject(par)
    {
        connect(this,SIGNAL(start_aq()),event_manager::get_instance(),SIGNAL(start_aquisition()));
        connect(this,SIGNAL(stop_aq()),event_manager::get_instance(),SIGNAL(stop_aquisition()));
    }

    script_daq_handler::~script_daq_handler()
    {

    }
}


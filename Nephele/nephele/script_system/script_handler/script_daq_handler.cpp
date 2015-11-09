#include "script_daq_handler.hpp"

#include "event_manager/event_manager_locator.hpp"

namespace talorion{

    script_daq_handler::script_daq_handler(QObject *par) :
        abstract_scriptable_object("daq", par)
    {
        connect(this,SIGNAL(start_aq()),event_manager_locator::get_instance(),SIGNAL(start_aquisition()));
        connect(this,SIGNAL(stop_aq()),event_manager_locator::get_instance(),SIGNAL(stop_aquisition()));
    }

    script_daq_handler::~script_daq_handler()
    {

    }
}


#include "script_daq_handler.hpp"


#include "core/EventManager.hpp"

namespace talorion{

    script_daq_handler::script_daq_handler(QObject *par) :
        abstract_scriptable_object("daq", par)
    {
        connect(this,SIGNAL(start_aq()),EventManager::get_instance(),SIGNAL(start_aquisition()));
        connect(this,SIGNAL(stop_aq()),EventManager::get_instance(),SIGNAL(stop_aquisition()));
    }

    script_daq_handler::~script_daq_handler()
    {

    }
}


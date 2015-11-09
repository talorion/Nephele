#include "event_manager_locator.hpp"


namespace talorion {

    null_event_manager event_manager_locator::null_manager_;
    abstract_event_manager* event_manager_locator::manager_ = &event_manager_locator::null_manager_;


    event_manager_locator::event_manager_locator(QObject *par) :
        QObject(par)
    {

    }

    event_manager_locator::~event_manager_locator()
    {

    }

    void event_manager_locator::provide(abstract_event_manager *evt_manager)
    {
        if(evt_manager == NULL){
            //Revert to null service
            manager_ = &null_manager_;
        }else{
            manager_ = evt_manager;
        }
    }
}

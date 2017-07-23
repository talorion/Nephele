#include "data_aquisition_dll_worker.hpp"

#include "core/EventManager.hpp"
#include "core/EntityManager.hpp"

#include "data_aquisition_dll_wrapper.hpp"
#include "tof_daq_specific/tof_daq_dll_worker.hpp"

namespace talorion {

    data_aquisition_dll_worker::data_aquisition_dll_worker(QObject *par) :
        QObject( par),
        dlls(),
        worker()
    {
        connect(EventManager::get_instance(),SIGNAL(data_aquistion_dll_component_changed(int)),this,SLOT(slot_data_aquistion_dll_component_changed(int)),Qt::QueuedConnection);

        foreach (int dll, EntityManager::get_instance()->get_all_TofDaqDlls()) {
            slot_data_aquistion_dll_component_changed(dll);
        }
    }

    data_aquisition_dll_worker::~data_aquisition_dll_worker()
    {
        QMap<int, data_aquisition_dll_wrapper*>::iterator it;
        for(it = dlls.begin(); it != dlls.end(); it++){
            data_aquisition_dll_wrapper* tmp = it.value();
            if(tmp)
                delete tmp;
            it.value() = NULL;
        }
        dlls.clear();
    }



    void data_aquisition_dll_worker::slot_data_aquistion_dll_component_changed(int entity)
    {
        qDebug()<< "slot_data_aquistion_dll_component_changed";

        QString dynll=EntityManager::get_instance()->get_data_aquistion_dll_component(entity);

        if(dynll.isNull() || dynll.isEmpty())
            return;

        data_aquisition_dll_wrapper* wr;

        QMap<int, data_aquisition_dll_wrapper*>::const_iterator it= dlls.constFind(entity);
        if ( it== dlls.constEnd()){
            wr = new data_aquisition_dll_wrapper();
            dlls.insert(entity, wr);
        }else{
            wr = it.value();
            if(wr){
                wr->dispose();
            }
        }

        if(wr){
            if(wr->init(dynll) == 0){
                EntityManager::get_instance()->createScriptableObject(wr->script_name(), wr);

                tof_daq_dll_worker* wrk =  new tof_daq_dll_worker(wr, entity);
                worker.insert(entity, wrk);
            }

        }

    }

    data_aquisition_dll_wrapper* data_aquisition_dll_worker::get_wrapper(int entity) const
    {
        QMap<int, data_aquisition_dll_wrapper*>::const_iterator cit =  dlls.constFind(entity);
        if(cit == dlls.constEnd())
            return NULL;

        return cit.value();
    }



} // namespace talorion


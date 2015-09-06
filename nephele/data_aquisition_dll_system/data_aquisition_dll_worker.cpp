#include "data_aquisition_dll_worker.hpp"

#include "core/event_manager.hpp"
#include "core/entity_manager.hpp"

#include "data_aquisition_dll_wrapper.hpp"

namespace talorion {

    data_aquisition_dll_worker::data_aquisition_dll_worker(QObject *par) :
        QObject(par),
        dlls()
    {
        connect(event_manager::get_instance(),SIGNAL(data_aquistion_dll_component_changed(int)),this,SLOT(slot_data_aquistion_dll_component_changed(int)),Qt::QueuedConnection);

        foreach (int box, entity_manager::get_instance()->get_all_TofDaqDlls()) {
            slot_data_aquistion_dll_component_changed(box);
        }
    }

    data_aquisition_dll_worker::~data_aquisition_dll_worker()
    {

    }

    void data_aquisition_dll_worker::slot_data_aquistion_dll_component_changed(int entity)
    {
        QString dynll=entity_manager::get_instance()->get_data_aquistion_dll_component(entity);

        if(dynll.isNull() || dynll.isEmpty())
            return;

        data_aquisition_dll_wrapper* wr;

        QMap<int, data_aquisition_dll_wrapper*>::const_iterator it= dlls.constFind(entity);
        if ( it== dlls.constEnd()){
            wr = new data_aquisition_dll_wrapper();
            dlls.insert(entity, wr);
        }else{
            wr = it.value();
            if(wr)
                wr->dispose();
        }

        if(wr)
            wr->init(dynll);

    }

} // namespace talorion


#include "tof_daq_dll_worker.hpp"

#include <QDebug>
#include <QStateMachine>

#include "core/event_manager.hpp"
#include "core/entity_manager.hpp"

#include "../data_aquisition_dll_wrapper.hpp"

#include "shared_memory_descriptor.h"
#include "shared_memory_pointer.h"

namespace talorion{

    tof_daq_dll_worker::tof_daq_dll_worker(data_aquisition_dll_wrapper* dll, int ent, QObject *par) :
        QObject(par),
        m_shmdesc(NULL),
        m_shmptr(NULL),
        m_dll(dll),
        m_entity(ent),
        timer(NULL),
        registered_values(),
        values_registered(false),
        fatal_emited(false)
    {
        timer = new QTimer(this);
        connect(timer, SIGNAL(timeout()), this, SLOT(update()));
        int rate =entity_manager::get_instance()->get_updaterate_component(m_entity);
        timer->start(rate);

        connect(event_manager::get_instance(),SIGNAL(updaterate_component_changed(int)),this,SLOT(updaterate_component_changed(int)));
        connect(this,SIGNAL(fatal(QString)),event_manager::get_instance(),SIGNAL(fatal(QString)));

        //        QStateMachine machine;
        //        QState *s1 = new QState();
        //        QState *s11 = new QState(s1);
        //        s1->setInitialState(s11);

        //        s1->addTransition(quitButton, SIGNAL(clicked()), s2);
        //        QFinalState *s2 = new QFinalState();
        //        machine.setInitialState(s1);

        //        machine.start();
    }

    tof_daq_dll_worker::~tof_daq_dll_worker()
    {
        timer->stop();
        QString path= entity_manager::get_instance()->get_user_data_path_component(m_entity);
        //        QString name;
        //        QStringList value_names;
        //        foreach (int var, registered_values) {
        //            //int component_id = entity_manager::get_instance()->get_userdata_component(var);
        //            //tmp = entity_manager::get_instance()->getComponentDataForEntity(component_id,var);
        //            name =entity_manager::get_instance()->get_name_component(var);
        //            value_names.append(name);
        //        }


        if(m_dll)
            m_dll->unregister_user_data(path);

        if(m_shmdesc)
            delete m_shmdesc;

        if(m_shmptr)
            delete m_shmptr;

        if(m_dll)
            delete m_dll;

        if(timer)
            delete timer;
    }

    int tof_daq_dll_worker::wait_for_new_data(int Timeout, bool WaitForEventReset)
    {
        if(Timeout <0)
            Timeout = 5000; //TODO: calculate timeoutepending on measuretime

        int ret = m_dll->wait_for_new_data(Timeout, *m_shmdesc, *m_shmptr, WaitForEventReset);
        if(ret == 4){
            m_shmdesc->swap();
            m_shmptr->swap();
        }

        emit data_aquisition_dll_answer(m_entity, ret);
        return ret;
    }

    void tof_daq_dll_worker::update()
    {
        //m_dll->UpdateUserData();
        register_user_data();
        update_user_data();
        //qDebug()<<"tick";
    }

    void tof_daq_dll_worker::updaterate_component_changed(int ent)
    {
        if(m_entity !=ent)
            return;
        int rate =entity_manager::get_instance()->get_updaterate_component(m_entity);
        timer->setInterval(rate);
    }

    bool tof_daq_dll_worker::aquisition_active() const
    {
        return m_dll->aquisition_active();
    }

    void tof_daq_dll_worker::register_user_data( int cmp_lvl)
    {
        QVariant tmp;
        QString name;
        bool ok;

        if(m_entity<0)
            return;
        QString path= entity_manager::get_instance()->get_user_data_path_component(m_entity);

        QStringList value_names;
        foreach (int var, entity_manager::get_instance()->get_entities_with_userdata_components()) {
            if(!registered_values.contains(var)){
                int component_id = entity_manager::get_instance()->get_userdata_component(var);
                tmp = entity_manager::get_instance()->getComponentDataForEntity(component_id,var);
                name =entity_manager::get_instance()->get_name_component(var);
                if(name.isEmpty())
                    continue;
                if(tmp.isValid()){
                    tmp.toDouble(&ok);
                    if(ok){
                        value_names.append(name);
                        registered_values.append(var);
                    }
                }
            }
        }
        if(value_names.isEmpty())
            return;

        if(path.isEmpty()){
            qDebug()<<"path is empty";
            return;
        }

        qDebug()<<"registering user data: "<<value_names.length()<<"at"<<path;

        //values_registered_old = values_registered;

        if(m_dll->register_user_data(value_names, path, cmp_lvl) != 4){
            qDebug()<<"unknown error at register_user_data";
            registered_values.clear();
            values_registered = false;
        }else{
            values_registered = true;
            fatal_emited = false;
        }

        if(!values_registered){
            if(!fatal_emited){
                emit fatal("VALUES NOT REGISTERD");
                fatal_emited = true;
            }
        }

    }

    void tof_daq_dll_worker::update_user_data()
    {
        QVariant tmp;
        QString name;
        bool ok;
        double tmp_dbl;

        if(m_entity<0)
            return;
        QString path= entity_manager::get_instance()->get_user_data_path_component(m_entity);

        QVector<double> Data;

        foreach (int var, registered_values) {
            int component_id = entity_manager::get_instance()->get_userdata_component(var);
            tmp = entity_manager::get_instance()->getComponentDataForEntity(component_id,var);
            name =entity_manager::get_instance()->get_name_component(var);
            if(tmp.isValid()){
                tmp_dbl = tmp.toDouble(&ok);
                if(ok){
                    Data.append(tmp_dbl);
                }
            }
        }

        if(m_dll->UpdateUserData(Data, path)<0)
            qDebug()<<"unknown error at UpdateUserData";
    }

    void tof_daq_dll_worker::prepare_buffers()
    {
//        int NbrSamples    = m_dll->read_int_parameter("NbrSamples");
//        int NbrPeaks      = m_dll->read_int_parameter("NbrPeaks");
//        int NbrSegments   = m_dll->read_int_parameter("NbrSegments");
//        int NbrBufs       = m_dll->read_int_parameter("NbrBufs");
//        int NbrWrites     = m_dll->read_int_parameter("NbrWrites");

//        if(!m_shmdesc)
//            m_shmdesc = new shared_memory_descriptor();

        //if(!m_shmptr)
            //m_shmptr = new shared_memory_pointer(NbrSamples, NbrPeaks, NbrSegments, NbrBufs, NbrWrites);

    }

}


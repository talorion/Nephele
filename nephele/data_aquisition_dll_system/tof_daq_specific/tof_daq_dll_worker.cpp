#include "tof_daq_dll_worker.hpp"

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
        m_entity(ent)
    {

    }

    tof_daq_dll_worker::~tof_daq_dll_worker()
    {
        if(m_shmdesc)
            delete m_shmdesc;
        if(m_shmptr)
            delete m_shmptr;

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

    void tof_daq_dll_worker::prepare_buffers()
    {
        int NbrSamples    = m_dll->read_int_parameter("NbrSamples");
        int NbrPeaks      = m_dll->read_int_parameter("NbrPeaks");
        int NbrSegments   = m_dll->read_int_parameter("NbrSegments");
        int NbrBufs       = m_dll->read_int_parameter("NbrBufs");
        int NbrWrites     = m_dll->read_int_parameter("NbrWrites");

        if(!m_shmdesc)
            m_shmdesc = new shared_memory_descriptor();

        if(!m_shmptr)
            m_shmptr = new shared_memory_pointer(NbrSamples, NbrPeaks, NbrSegments, NbrBufs, NbrWrites);

    }

}


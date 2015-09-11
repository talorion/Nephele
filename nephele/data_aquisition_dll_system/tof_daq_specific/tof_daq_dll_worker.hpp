#ifndef TOF_DAQ_DLL_WORKER_HPP
#define TOF_DAQ_DLL_WORKER_HPP

#include <QObject>

namespace talorion{

    class shared_memory_descriptor;
    class shared_memory_pointer;
    class data_aquisition_dll_wrapper;

    class tof_daq_dll_worker : public QObject
    {
        Q_OBJECT
    public:
        explicit tof_daq_dll_worker(data_aquisition_dll_wrapper* dll, int ent, QObject *par = 0);
        ~tof_daq_dll_worker();
        Q_DISABLE_COPY(tof_daq_dll_worker)

    signals:
        void data_aquisition_dll_answer(int, int);
        void data_aquisition_dll_answer(int, bool);
        void data_aquisition_dll_answer(int, const QString&);
        void data_aquisition_dll_answer(int, qint64);
        void data_aquisition_dll_answer(int, float);
        void data_aquisition_dll_answer(int, double);

    public slots:
        int wait_for_new_data(int Timeout =-1, bool WaitForEventReset=false);

    private:
        void prepare_buffers();
        //        void swap_shmdesc();
        //        void swap_shmptr();

    private:
        shared_memory_descriptor* m_shmdesc;
        //        shared_memory_descriptor* current_shmdesc;
        //        shared_memory_descriptor* next_shmdesc;

        shared_memory_pointer* m_shmptr;
        //        shared_memory_pointer* current_shmptr;
        //        shared_memory_pointer* next_shmptr;

        data_aquisition_dll_wrapper* m_dll;

        int m_entity;

    };
}

#endif // TOF_DAQ_DLL_WORKER_HPP

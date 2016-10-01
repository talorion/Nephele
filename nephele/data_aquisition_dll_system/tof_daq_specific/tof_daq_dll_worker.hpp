#ifndef TOF_DAQ_DLL_WORKER_HPP
#define TOF_DAQ_DLL_WORKER_HPP

#include <QObject>
#include <QTimer>
#include <QList>

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

        void fatal(QString);

    public slots:
        int wait_for_new_data(int Timeout =-1, bool WaitForEventReset=false);

    private slots:
        void update();

        void updaterate_component_changed(int ent);
        void register_user_data(int cmp_lvl =0);
        void update_user_data();

        bool aquisition_active() const;

    private:
        void prepare_buffers();
        //        void swap_shmdesc();
        //        void swap_shmptr();

        int get_tw_index(const QString& previous_name, const QString& name, const QString& next_name, int starting_index=0);

    private:
        shared_memory_descriptor* m_shmdesc;
        //        shared_memory_descriptor* current_shmdesc;
        //        shared_memory_descriptor* next_shmdesc;

        shared_memory_pointer* m_shmptr;
        //        shared_memory_pointer* current_shmptr;
        //        shared_memory_pointer* next_shmptr;

        data_aquisition_dll_wrapper* m_dll;

        int m_entity;

        QTimer *timer;

        QList<int> registered_values;
        QStringList tw_registered_values;

        bool values_registered;
        bool fatal_emited;

    };

}

#endif // TOF_DAQ_DLL_WORKER_HPP

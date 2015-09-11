#ifndef TALORION_DATA_AQUISITION_DLL_WORKER_HPP
#define TALORION_DATA_AQUISITION_DLL_WORKER_HPP

#include <QObject>
#include <QMap>

namespace talorion {

    class data_aquisition_dll_wrapper;
    class tof_daq_dll_worker;

    class data_aquisition_dll_worker : public QObject
    {
        Q_OBJECT
    public:
        explicit data_aquisition_dll_worker(QObject *par = 0);
        ~data_aquisition_dll_worker();
        Q_DISABLE_COPY(data_aquisition_dll_worker)

    private:
        data_aquisition_dll_wrapper *get_wrapper(int entity) const;

    private slots:
        void slot_data_aquistion_dll_component_changed(int entity);

    private:
        QMap<int, data_aquisition_dll_wrapper*> dlls;
        QMap<int, tof_daq_dll_worker*> worker;

    };

} // namespace talorion

#endif // TALORION_DATA_AQUISITION_DLL_WORKER_HPP

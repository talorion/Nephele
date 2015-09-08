#ifndef TALORION_DATA_AQUISITION_DLL_WORKER_HPP
#define TALORION_DATA_AQUISITION_DLL_WORKER_HPP

#include <QObject>
#include <QMap>

namespace talorion {

    class data_aquisition_dll_wrapper;

    class data_aquisition_dll_worker : public QObject
    {
        Q_OBJECT
    public:
        explicit data_aquisition_dll_worker(QObject *par = 0);
        ~data_aquisition_dll_worker();
        Q_DISABLE_COPY(data_aquisition_dll_worker)

    signals:
        void data_aquisition_dll_answer(int, int);
        void data_aquisition_dll_answer(int, bool);
        void data_aquisition_dll_answer(int, const QString&);
        void data_aquisition_dll_answer(int, qint64);
        void data_aquisition_dll_answer(int, float);
        void data_aquisition_dll_answer(int, double);

    public slots:

    private slots:
        void slot_data_aquistion_dll_component_changed(int entity);

    private:
        QMap<int, data_aquisition_dll_wrapper*> dlls;
    };

} // namespace talorion

#endif // TALORION_DATA_AQUISITION_DLL_WORKER_HPP

#ifndef TOF_DAQ_DLL_WRAPPER_H
#define TOF_DAQ_DLL_WRAPPER_H

#include <QLibrary>
#include <QObject>
#include <QDebug>

namespace talorion{
    typedef int (*InitializeDll_prototype)();
    typedef void (*CleanupDll_prototype)();
    typedef int (*StartAcquisition_prototype)();
    typedef int (*StopAcquisition_prototype)();
    typedef bool (*DaqActive_prototype)();
    typedef int (*RegisterUserData_prototype)(char* Location, int NbrElements, char* ElementDescription, int CompressionLevel);
    typedef int (*UnregisterUserData_prototype)(char* Location);

    class data_aquisition_dll_wrapper: public QObject
    {
        Q_OBJECT
    public:
        data_aquisition_dll_wrapper(QObject *par = 0);
        ~data_aquisition_dll_wrapper();
        Q_DISABLE_COPY(data_aquisition_dll_wrapper)

        void init();
        void dispose();

    private slots:
        void initialize_dll();
        void cleanup_dll();

        void unregister_all_user_data();

    public slots:
        void start_aquisition();
        void stop_aquisition();
        bool aquisition_active();
        void register_user_data(const QStringList& value_names, const QString& path = "/USER_DATA", int cmp_lvl =0);
        void unregister_user_data(const QString& path);

    private:
        template<typename T>
        T resolve_method(const QString& method_name);

        bool success(int error)const {return error == m_success_return;}

    private:
        InitializeDll_prototype m_InitializeDll;
        CleanupDll_prototype m_CleanupDll;
        StartAcquisition_prototype m_StartAcquisition;
        StopAcquisition_prototype m_StopAcquisition;
        DaqActive_prototype m_AquisitionActive;
        RegisterUserData_prototype m_RegisterUserDat;
        UnregisterUserData_prototype m_UnregisterUserData;

        QLibrary* m_data_aquisition_dll;
        int m_success_return;
        QStringList registered_user_data;

    };

    template<typename T>
    T data_aquisition_dll_wrapper::resolve_method(const QString& method_name)
    {
        if(!m_data_aquisition_dll)
            return NULL;

        if(!m_data_aquisition_dll->isLoaded())
            return NULL;

        if(method_name.trimmed().isEmpty())
            return NULL;

        QString decorated_method_name = "_"+method_name;

        T tmp = (T) m_data_aquisition_dll->resolve(decorated_method_name.toLocal8Bit());
        if (!tmp){
            qDebug()<<"Could not resolve "+method_name<<m_data_aquisition_dll->errorString();
            return NULL;
        }
        return tmp;
    }
}


#endif // TOF_DAQ_DLL_WRAPPER_H

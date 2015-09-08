#ifndef TOF_DAQ_DLL_WRAPPER_H
#define TOF_DAQ_DLL_WRAPPER_H

#include "core/abstract_scriptable_object.hpp"

#include <QLibrary>
#include <QObject>
#include <QVector>
#include <QDebug>

namespace talorion {

    class shared_memory_descriptor;
    class shared_memory_pointer;

    typedef int     (*InitializeDll_prototype)();
    typedef void    (*CleanupDll_prototype)();
    typedef int     (*StartAcquisition_prototype)();
    typedef int     (*StopAcquisition_prototype)();
    typedef bool    (*DaqActive_prototype)();
    typedef int     (*RegisterUserData_prototype)(char* , int , char* , int );
    typedef int     (*UnregisterUserData_prototype)(char* );
    typedef int     (*GetSpectrum_prototype)(float* , int , int , int , bool );
    typedef int     (*GetAverageSpectrum_prototype)(double* , bool );
    typedef int     (*GetTraces_prototype)(float* , float* , int , int , int );
    typedef char*   (*GetParameter_prototype)(char* );
    typedef int     (*GetParameterInt_prototype)(char* );
    typedef bool    (*GetParameterBool_prototype)(char* );
    typedef float   (*GetParameterFloat_prototype)(char* );
    typedef qint64  (*GetParameterInt64_prototype)(char* );
    typedef double  (*GetParameterDouble_prototype)(char* );
    typedef int     (*WaitForNewData_prototype)(int , void* , void* , bool );
    typedef int     (*UpdateUserData_prototype)(char* , int , double* );
    typedef int     (*SetParameter_prototyper)(char* Parameter, char* ValueString);
    typedef int     (*SetParameterInt_prototype)(char* Parameter, int Value);
    typedef int     (*SetParameterBool_prototype)(char* Parameter, bool Value);
    typedef int     (*SetParameterFloat_prototype)(char* Parameter, float Value);
    typedef int     (*SetParameterInt64_prototype)(char* Parameter, qint64 Value);
    typedef int     (*SetParameterDouble_prototype)(char* Parameter, double Value);

    class data_aquisition_dll_wrapper: public abstract_scriptable_object
    {
        Q_OBJECT
    public:
        data_aquisition_dll_wrapper(QObject *par = 0);
        ~data_aquisition_dll_wrapper();
        Q_DISABLE_COPY(data_aquisition_dll_wrapper)

        int init(QString dll_name = "C:\\Tofwerk\\TofDaq_1.97_noHW\\TofDaqDll.dll");
        void dispose();

    public slots:
        int initialize_dll() const;
        void cleanup_dll() const;

        int start_aquisition() const;
        int stop_aquisition() const;
        bool aquisition_active() const;
        int register_user_data(const QStringList& value_names, const QString& path = "/USER_DATA", int cmp_lvl =0) const;
        int UpdateUserData(QVector<double>& Data, const QString& path = "/USER_DATA") const;
        int unregister_user_data(const QString& path) const;

        int read_spectrum(QVector<float>& buffer_Spectrum, int BufIndex, int SegmentIndex = -1, int SegmentEndIndex = -1,bool Normalize = false) const;
        int read_average_spectrum(QVector<double>& buffer_avg_spectrum, bool Normalize = false) const;
        int read_traces( QVector<float>& buffer_Spectrum, QVector<float> buffer_Masses, int BufIndex, int SegmentIndex = -1, int SegmentEndIndex = -1) const;

        //int wait_for_new_data(int Timeout,shared_memory_descriptor& pBufDesc,shared_memory_pointer& pShMem,bool WaitForEventReset);
        QString read_parameter(const QString& para) const;
        int read_int_parameter(const QString& para) const;
        bool read_bool_parameter(const QString& para) const;
        float read_float_parameter(const QString& para) const;
        qint64 read_int64_parameter(const QString& para) const;
        double read_double_parameter(const QString& para) const;

        int write_parameter(const QString& para, QString value) const;
        int write_int_parameter(const QString& para, int value) const;
        int write_bool_parameter(const QString& para, bool value) const;
        int write_float_parameter(const QString& para, float value) const;
        int write_int64_parameter(const QString& para, qint64 value) const;
        int write_double_parameter(const QString& para, double value) const;


    private:
        template<typename T>
        T resolve_method(const QString& method_name);

        void update_spectrum(QVector<float>& newData);

    private:
        InitializeDll_prototype m_InitializeDll;
        CleanupDll_prototype m_CleanupDll;
        StartAcquisition_prototype m_StartAcquisition;
        StopAcquisition_prototype m_StopAcquisition;
        DaqActive_prototype m_AquisitionActive;
        RegisterUserData_prototype m_RegisterUserDat;
        UnregisterUserData_prototype m_UnregisterUserData;
        GetSpectrum_prototype m_GetSpectrum;
        GetAverageSpectrum_prototype m_GetAverageSpectrum;
        GetTraces_prototype m_GetTraces;
        GetParameter_prototype m_GetParameter;
        GetParameterInt_prototype m_GetParameterInt;
        GetParameterBool_prototype m_GetParameterBool;
        GetParameterFloat_prototype m_GetParameterFloat;
        GetParameterInt64_prototype m_GetParameterInt64;
        GetParameterDouble_prototype m_GetParameterDouble;
        WaitForNewData_prototype m_WaitForNewData;
        UpdateUserData_prototype m_UpdateUserData;
        SetParameter_prototyper m_SetParameter;
        SetParameterInt_prototype m_SetParameterInt;
        SetParameterBool_prototype m_SetParameterBool;
        SetParameterFloat_prototype m_SetParameterFloat;
        SetParameterInt64_prototype m_SetParameterInt64;
        SetParameterDouble_prototype m_SetParameterDouble;

        QLibrary* m_data_aquisition_dll;
        QString m_dll_name;
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

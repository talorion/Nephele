#ifndef TOFDAQDLL_HPP
#define TOFDAQDLL_HPP

#include <QObject>
#include <QLibrary>
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

    typedef int     (*TwInitializeDll_prototype)(void);
    typedef void    (*TwCleanupDll_prototype)(void);
    typedef int     (*TwTpsConnect_prototype)(void);
    typedef int     (*TwTpsConnect2_prototype)(char* , int );
    typedef int     (*TwTpsDisconnect_prototype)(void);
    typedef int     (*TwTpsGetMonitorValue_prototype)(int , double* );
    typedef int     (*TwTpsGetTargetValue_prototype)(int , double* );
    typedef int     (*TwTpsGetLastSetValue_prototype)(int , double* );
    typedef int     (*TwTpsSetTargetValue_prototype)(int , double );
    typedef int     (*TwTpsGetNbrModules_prototype)(int* );
    typedef int     (*TwTpsGetModuleCodes_prototype)(int* , int );
    typedef int     (*TwTpsInitialize_prototype)(void);
    typedef int     (*TwTpsSetAllVoltages_prototype)(void);
    typedef int     (*TwTpsShutdown_prototype)(void);
    typedef int     (*TwTpsGetStatus_prototype)(int* );
    typedef int     (*TwTpsLoadSetFile_prototype)(char* );
    typedef int     (*TwTpsSaveSetFile_prototype)(char* );
    typedef int     (*TwTpsGetActiveFilament_prototype)(int* );
    typedef int     (*TwTpsSetActiveFilament_prototype)(int );
    typedef int     (*TwTpsGetModuleLimits_prototype)(int , double* , double* );

    class TofDaqDll : public QObject
    {
        Q_OBJECT
    public:
        explicit TofDaqDll(QObject *par = 0);
        ~TofDaqDll();
        Q_DISABLE_COPY(TofDaqDll)

        int init(QString dll_name = "C:\\Tofwerk\\TofDaq_1.97_noHW\\TofDaqDll.dll");
        void dispose();

    public:
        Q_INVOKABLE int initialize_dll() const;
        Q_INVOKABLE void cleanup_dll() const;

        Q_INVOKABLE int start_aquisition() const;
        Q_INVOKABLE int stop_aquisition() const;
        Q_INVOKABLE bool aquisition_active() const;
        Q_INVOKABLE int register_user_data(const QStringList& value_names, const QString& path = "/USER_DATA", int cmp_lvl =0) const;
        Q_INVOKABLE int UpdateUserData(QVector<double>& Data, const QString& path = "/USER_DATA") const;
        Q_INVOKABLE int unregister_user_data(const QString& path) const;

        Q_INVOKABLE int read_spectrum(QVector<float>& buffer_Spectrum, int BufIndex, int SegmentIndex = -1, int SegmentEndIndex = -1,bool Normalize = false) const;
        Q_INVOKABLE int read_average_spectrum(QVector<double>& buffer_avg_spectrum, bool Normalize = false) const;
        Q_INVOKABLE int read_traces( QVector<float>& buffer_Spectrum, QVector<float> buffer_Masses, int BufIndex, int SegmentIndex = -1, int SegmentEndIndex = -1) const;

        Q_INVOKABLE int wait_for_new_data(int Timeout,shared_memory_descriptor& pBufDesc,shared_memory_pointer& pShMem,bool WaitForEventReset);
        Q_INVOKABLE QString read_parameter(const QString& para) const;
        Q_INVOKABLE int read_int_parameter(const QString& para) const;
        Q_INVOKABLE bool read_bool_parameter(const QString& para) const;
        Q_INVOKABLE float read_float_parameter(const QString& para) const;
        Q_INVOKABLE qint64 read_int64_parameter(const QString& para) const;
        Q_INVOKABLE double read_double_parameter(const QString& para) const;

        Q_INVOKABLE int write_parameter(const QString& para, QString value) const;
        Q_INVOKABLE int write_int_parameter(const QString& para, int value) const;
        Q_INVOKABLE int write_bool_parameter(const QString& para, bool value) const;
        Q_INVOKABLE int write_float_parameter(const QString& para, float value) const;
        Q_INVOKABLE int write_int64_parameter(const QString& para, qint64 value) const;
        Q_INVOKABLE int write_double_parameter(const QString& para, double value) const;

        Q_INVOKABLE int tps_connect(void);
        Q_INVOKABLE int tps_connect_2(QString ip, int type);
        Q_INVOKABLE int tps_disconnect(void);
        Q_INVOKABLE int tps_get_monitor_value(int moduleCode, double &value);
        Q_INVOKABLE int tps_get_target_value(int moduleCode, double &value);
        Q_INVOKABLE int tps_get_last_set_value(int moduleCode, double &value);
        Q_INVOKABLE int tps_set_target_value(int moduleCode, double value);
        Q_INVOKABLE int tps_get_nbr_modules(int &nbrModules);
        Q_INVOKABLE int tps_get_module_codes(QVector<int> &moduleCodeBuffer);
        Q_INVOKABLE int tps_initialize(void);
        Q_INVOKABLE int tps_set_all_voltages(void);
        Q_INVOKABLE int tps_shutdown(void);
        Q_INVOKABLE int tps_get_status(int &status);
        Q_INVOKABLE int tps_load_set_file (QString setFile);
        Q_INVOKABLE int tps_save_set_file (QString setFile);
        Q_INVOKABLE int tps_get_active_filament(int &activeFilament);
        Q_INVOKABLE int tps_set_active_filament(int activeFilament);
        Q_INVOKABLE int tps_get_module_limits(int moduleCode, double &minLimit, double &maxLimit);


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

        TwTpsConnect_prototype	m_TpsConnect;
        TwTpsConnect2_prototype	m_TpsConnect2;
        TwTpsDisconnect_prototype	m_TpsDisconnect;
        TwTpsGetMonitorValue_prototype	m_TpsGetMonitorValue;
        TwTpsGetTargetValue_prototype	m_TpsGetTargetValue;
        TwTpsGetLastSetValue_prototype	m_TpsGetLastSetValue;
        TwTpsSetTargetValue_prototype	m_TpsSetTargetValue;
        TwTpsGetNbrModules_prototype	m_TpsGetNbrModules;
        TwTpsGetModuleCodes_prototype	m_TpsGetModuleCodes;
        TwTpsInitialize_prototype	m_TpsInitialize;
        TwTpsSetAllVoltages_prototype	m_TpsSetAllVoltages;
        TwTpsShutdown_prototype	m_TpsShutdown;
        TwTpsGetStatus_prototype	m_TpsGetStatus;
        TwTpsLoadSetFile_prototype	m_TpsLoadSetFile;
        TwTpsSaveSetFile_prototype	m_TpsSaveSetFile;
        TwTpsGetActiveFilament_prototype	m_TpsGetActiveFilament;
        TwTpsSetActiveFilament_prototype	m_TpsSetActiveFilament;
        TwTpsGetModuleLimits_prototype	m_TpsGetModuleLimits;

        QLibrary* m_data_aquisition_dll;
        QString m_dll_name;
    };

    template<typename T>
    T TofDaqDll::resolve_method(const QString& method_name)
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

#endif // TOFDAQDLL_HPP

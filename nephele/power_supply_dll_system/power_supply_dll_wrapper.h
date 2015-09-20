#ifndef POWER_SUPPLY_DLL_WRAPPER_H
#define POWER_SUPPLY_DLL_WRAPPER_H

#include <QObject>
#include <QLibrary>
#include <QDebug>

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

namespace talorion{
    class power_supply_dll_wrapper : public QObject
    {
        Q_OBJECT
    public:
        explicit power_supply_dll_wrapper(QObject *par = 0);
        ~power_supply_dll_wrapper();
        Q_DISABLE_COPY(power_supply_dll_wrapper)

        int init(QString dll_name = "C:\\Tofwerk\\TofDaq_1.97_noHW\\TofDaqDll.dll");
        void dispose();

    public slots:
        int initializeDll(void);
        void cleanupDll(void);
        int tps_connect(void);
        int tps_connect_2(QString ip, int type);
        int tps_disconnect(void);
        int tps_get_monitor_value(int moduleCode, double &value);
        int tps_get_target_value(int moduleCode, double &value);
        int tps_get_last_set_value(int moduleCode, double &value);
        int tps_set_target_value(int moduleCode, double value);
        int tps_get_nbr_modules(int &nbrModules);
        int tps_get_module_codes(QVector<int> &moduleCodeBuffer);
        int tps_initialize(void);
        int tps_set_all_voltages(void);
        int tps_shutdown(void);
        int tps_get_status(int &status);
        int tps_load_set_file (QString setFile);
        int tps_save_set_file (QString setFile);
        int tps_get_active_filament(int &activeFilament);
        int tps_set_active_filament(int activeFilament);
        int tps_get_module_limits(int moduleCode, double &minLimit, double &maxLimit);


    private:
        template<typename T>
        T resolve_method(const QString& method_name);

    private:
        QLibrary* m_data_aquisition_dll;
        QString m_dll_name;

        TwInitializeDll_prototype	m_InitializeDll;
        TwCleanupDll_prototype	m_CleanupDll;
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

    };

    template<typename T>
    T power_supply_dll_wrapper::resolve_method(const QString& method_name)
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

#endif // POWER_SUPPLY_DLL_WRAPPER_H

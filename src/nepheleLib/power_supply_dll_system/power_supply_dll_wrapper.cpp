#include "power_supply_dll_wrapper.h"


#include <QFileInfo>
#include <QDir>

#if defined( Q_OS_WIN )
#include <Windows.h>
#endif

namespace talorion{
    power_supply_dll_wrapper::power_supply_dll_wrapper(QObject *par) :
        QObject(par),
        m_data_aquisition_dll(NULL),
        m_dll_name(),
        m_InitializeDll(NULL),
        m_CleanupDll(NULL),
        m_TpsConnect(NULL),
        m_TpsConnect2(NULL),
        m_TpsDisconnect(NULL),
        m_TpsGetMonitorValue(NULL),
        m_TpsGetTargetValue(NULL),
        m_TpsGetLastSetValue(NULL),
        m_TpsSetTargetValue(NULL),
        m_TpsGetNbrModules(NULL),
        m_TpsGetModuleCodes(NULL),
        m_TpsInitialize(NULL),
        m_TpsSetAllVoltages(NULL),
        m_TpsShutdown(NULL),
        m_TpsGetStatus(NULL),
        m_TpsLoadSetFile(NULL),
        m_TpsSaveSetFile(NULL),
        m_TpsGetActiveFilament(NULL),
        m_TpsSetActiveFilament(NULL),
        m_TpsGetModuleLimits(NULL)
    {

    }

    power_supply_dll_wrapper::~power_supply_dll_wrapper()
    {

    }

    int power_supply_dll_wrapper::init(QString dll_name)
    {
        QFileInfo f(dll_name);
        QString absolutePath = f.absoluteDir().absolutePath();
        QString name = f.fileName();

#if defined( Q_OS_WIN )
        LPCWSTR str = (const wchar_t*)absolutePath.utf16();
        SetDllDirectory( str );
        qDebug()<<"add Library path: "<<absolutePath;
#endif

        m_data_aquisition_dll = new QLibrary(name);
        if (!m_data_aquisition_dll->load()){
            qDebug()<<m_data_aquisition_dll->errorString();
            return -1;
        }
        m_dll_name = dll_name;

        QString meth = "TwInitializeDll";
        m_InitializeDll= resolve_method<TwInitializeDll_prototype>(meth);
        meth = "TwCleanupDll";
        m_CleanupDll= resolve_method<TwCleanupDll_prototype>(meth);
        meth = "TwTpsConnect";
        m_TpsConnect= resolve_method<TwTpsConnect_prototype>(meth);
        meth = "TwTpsConnect2";
        m_TpsConnect2= resolve_method<TwTpsConnect2_prototype>(meth);
        meth = "TwTpsDisconnect";
        m_TpsDisconnect= resolve_method<TwTpsDisconnect_prototype>(meth);
        meth = "TwTpsGetMonitorValue";
        m_TpsGetMonitorValue= resolve_method<TwTpsGetMonitorValue_prototype>(meth);
        meth = "TwTpsGetTargetValue";
        m_TpsGetTargetValue= resolve_method<TwTpsGetTargetValue_prototype>(meth);
        meth = "TwTpsGetLastSetValue";
        m_TpsGetLastSetValue= resolve_method<TwTpsGetLastSetValue_prototype>(meth);
        meth = "TwTpsSetTargetValue";
        m_TpsSetTargetValue= resolve_method<TwTpsSetTargetValue_prototype>(meth);
        meth = "TwTpsGetNbrModules";
        m_TpsGetNbrModules= resolve_method<TwTpsGetNbrModules_prototype>(meth);
        meth = "TwTpsGetModuleCodes";
        m_TpsGetModuleCodes= resolve_method<TwTpsGetModuleCodes_prototype>(meth);
        meth = "TwTpsInitialize";
        m_TpsInitialize= resolve_method<TwTpsInitialize_prototype>(meth);
        meth = "TwTpsSetAllVoltages";
        m_TpsSetAllVoltages= resolve_method<TwTpsSetAllVoltages_prototype>(meth);
        meth = "TwTpsShutdown";
        m_TpsShutdown= resolve_method<TwTpsShutdown_prototype>(meth);
        meth = "TwTpsGetStatus";
        m_TpsGetStatus= resolve_method<TwTpsGetStatus_prototype>(meth);
        meth = "TwTpsLoadSetFile";
        m_TpsLoadSetFile= resolve_method<TwTpsLoadSetFile_prototype>(meth);
        meth = "TwTpsSaveSetFile";
        m_TpsSaveSetFile= resolve_method<TwTpsSaveSetFile_prototype>(meth);
        meth = "TwTpsGetActiveFilament";
        m_TpsGetActiveFilament= resolve_method<TwTpsGetActiveFilament_prototype>(meth);
        meth = "TwTpsSetActiveFilament";
        m_TpsSetActiveFilament= resolve_method<TwTpsSetActiveFilament_prototype>(meth);
        meth = "TwTpsGetModuleLimits";
        m_TpsGetModuleLimits= resolve_method<TwTpsGetModuleLimits_prototype>(meth);

        return 0;
    }

    void power_supply_dll_wrapper::dispose()
    {

    }

    int power_supply_dll_wrapper::initializeDll()
    {
        if(m_InitializeDll){
            return (m_InitializeDll());
        }
        return -1;
    }

    void power_supply_dll_wrapper::cleanupDll()
    {
        if(m_CleanupDll){
            return (m_CleanupDll());
        }
    }

    int power_supply_dll_wrapper::tps_connect()
    {
        if(m_TpsConnect){
            return (m_TpsConnect());
        }
        return -1;
    }

    int power_supply_dll_wrapper::tps_connect_2(QString ip, int type)
    {
        if(m_TpsConnect2){
            QByteArray ip_ba = ip.toLocal8Bit();
            return (m_TpsConnect2(ip_ba.data(), type));
        }
        return -1;
    }

    int power_supply_dll_wrapper::tps_disconnect()
    {
        if(m_TpsDisconnect){
            return (m_TpsDisconnect());
        }
        return -1;
    }

    int power_supply_dll_wrapper::tps_get_monitor_value(int moduleCode, double &value)
    {
        if(m_TpsGetMonitorValue){
            return (m_TpsGetMonitorValue(moduleCode, &value));
        }
        return -1;
    }

    int power_supply_dll_wrapper::tps_get_target_value(int moduleCode, double &value)
    {
        if(m_TpsGetTargetValue){
            return (m_TpsGetTargetValue(moduleCode, &value));
        }
        return -1;
    }

    int power_supply_dll_wrapper::tps_get_last_set_value(int moduleCode, double &value)
    {
        if(m_TpsGetLastSetValue){
            return (m_TpsGetLastSetValue(moduleCode, &value));
        }
        return -1;
    }

    int power_supply_dll_wrapper::tps_set_target_value(int moduleCode, double value)
    {
        if(m_TpsSetTargetValue){
            return (m_TpsSetTargetValue(moduleCode, value));
        }
        return -1;
    }

    int power_supply_dll_wrapper::tps_get_nbr_modules(int &nbrModules)
    {
        if(m_TpsGetNbrModules){
            return (m_TpsGetNbrModules(&nbrModules));
        }
        return -1;
    }

    int power_supply_dll_wrapper::tps_get_module_codes(QVector<int> &moduleCodeBuffer)
    {
        if(m_TpsGetModuleCodes){
            int bufferLength =moduleCodeBuffer.length();
            return (m_TpsGetModuleCodes(moduleCodeBuffer.data(), bufferLength));
        }
        return -1;
    }

    int power_supply_dll_wrapper::tps_initialize()
    {
        if(m_TpsInitialize){
            return (m_TpsInitialize());
        }
        return -1;
    }

    int power_supply_dll_wrapper::tps_set_all_voltages()
    {
        if(m_TpsSetAllVoltages){
            return (m_TpsSetAllVoltages());
        }
        return -1;
    }

    int power_supply_dll_wrapper::tps_shutdown()
    {
        if(m_TpsShutdown){
            return (m_TpsShutdown());
        }
        return -1;
    }

    int power_supply_dll_wrapper::tps_get_status(int &status)
    {
        if(m_TpsGetStatus){
            return (m_TpsGetStatus(&status));
        }
        return -1;
    }

    int power_supply_dll_wrapper::tps_load_set_file(QString setFile)
    {
        if(m_TpsLoadSetFile){
            QByteArray setFile_ba = setFile.toLocal8Bit();
            return (m_TpsLoadSetFile(setFile_ba.data()));
        }
        return -1;
    }

    int power_supply_dll_wrapper::tps_save_set_file(QString setFile)
    {
        if(m_TpsSaveSetFile){
            QByteArray setFile_ba = setFile.toLocal8Bit();
            return (m_TpsSaveSetFile(setFile_ba.data()));
        }
        return -1;
    }

    int power_supply_dll_wrapper::tps_get_active_filament(int &activeFilament)
    {
        if(m_TpsGetActiveFilament){
            return (m_TpsGetActiveFilament(&activeFilament));
        }
        return -1;
    }

    int power_supply_dll_wrapper::tps_set_active_filament(int activeFilament)
    {
        if(m_TpsSetActiveFilament){
            return (m_TpsSetActiveFilament(activeFilament));
        }
        return -1;
    }

    int power_supply_dll_wrapper::tps_get_module_limits(int moduleCode, double &minLimit, double &maxLimit)
    {
        if(m_TpsGetModuleLimits){
            return (m_TpsGetModuleLimits(moduleCode, &minLimit, &maxLimit));
        }
        return -1;
    }
}

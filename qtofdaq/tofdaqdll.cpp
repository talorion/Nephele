#include "tofdaqdll.hpp"



#include "twutils.hpp"
#include "shared_memory_descriptor.hpp"
#include "shared_memory_pointer.hpp"

#if defined( Q_OS_WIN )
#include <Windows.h>
#endif

#include <QDebug>
#include <QDir>
#include <QtGlobal>

namespace talorion{


    TofDaqDll::TofDaqDll(QObject *par) :
        QObject(par),
        m_InitializeDll(NULL),
        m_CleanupDll(NULL),
        m_StartAcquisition(NULL),
        m_StopAcquisition(NULL),
        m_AquisitionActive(NULL),
        m_RegisterUserDat(NULL),
        m_UnregisterUserData(NULL),
        m_GetSpectrum(NULL),
        m_GetAverageSpectrum(NULL),
        m_GetTraces(NULL),
        m_GetParameter(NULL),
        m_GetParameterInt(NULL),
        m_GetParameterBool(NULL),
        m_GetParameterFloat(NULL),
        m_GetParameterInt64(NULL),
        m_GetParameterDouble(NULL),
        m_WaitForNewData(NULL),
        m_UpdateUserData(NULL),
        m_SetParameter(NULL),
        m_SetParameterInt(NULL),
        m_SetParameterBool(NULL),
        m_SetParameterFloat(NULL),
        m_SetParameterInt64(NULL),
        m_SetParameterDouble(NULL),
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
        m_TpsGetModuleLimits(NULL),
        m_data_aquisition_dll(NULL),
        m_dll_name()
    {

    }

    TofDaqDll::~TofDaqDll()
    {
        if(m_data_aquisition_dll)
            delete m_data_aquisition_dll;
    }

    int TofDaqDll::init(QString dll_name)
    {
        QFileInfo f(dll_name);
        QString name = f.fileName();

#if defined( Q_OS_WIN )
        QString absolutePath = f.absoluteDir().absolutePath();

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
        m_InitializeDll = resolve_method<InitializeDll_prototype>(meth);

        meth = "TwCleanupDll";
        m_CleanupDll= resolve_method<CleanupDll_prototype>(meth);

        meth = "TwStartAcquisition";
        m_StartAcquisition= resolve_method<StartAcquisition_prototype>(meth);

        meth = "TwStopAcquisition";
        m_StopAcquisition= resolve_method<StopAcquisition_prototype>(meth);

        meth = "TwDaqActive";
        m_AquisitionActive= resolve_method<DaqActive_prototype>(meth);

        meth = "TwRegisterUserDataBuf";
        m_RegisterUserDat= resolve_method<RegisterUserData_prototype>(meth);

        meth = "TwUnregisterUserData";
        m_UnregisterUserData= resolve_method<UnregisterUserData_prototype>(meth);

        meth = "TwGetTofSpectrumFromShMem";
        m_GetSpectrum= resolve_method<GetSpectrum_prototype>(meth);

        meth = "TwGetSumSpectrumFromShMem";
        m_GetAverageSpectrum= resolve_method<GetAverageSpectrum_prototype>(meth);

        meth = "TwGetStickSpectrumFromShMem";
        m_GetTraces= resolve_method<GetTraces_prototype>(meth);

        meth = "TwGetDaqParameter";
        m_GetParameter= resolve_method<GetParameter_prototype>(meth);

        meth = "TwGetDaqParameterInt";
        m_GetParameterInt= resolve_method<GetParameterInt_prototype>(meth);

        meth = "TwGetDaqParameterBool";
        m_GetParameterBool= resolve_method<GetParameterBool_prototype>(meth);

        meth = "TwGetDaqParameterFloat";
        m_GetParameterFloat= resolve_method<GetParameterFloat_prototype>(meth);

        meth = "TwGetDaqParameterInt64";
        m_GetParameterInt64= resolve_method<GetParameterInt64_prototype>(meth);

        meth = "TwGetDaqParameterDouble";
        m_GetParameterDouble= resolve_method<GetParameterDouble_prototype>(meth);

        meth = "TwWaitForNewData";
        m_WaitForNewData= resolve_method<WaitForNewData_prototype>(meth);

        meth = "TwUpdateUserData";
        m_UpdateUserData = resolve_method<UpdateUserData_prototype>(meth);

        meth = "TwSetDaqParameter";
        m_SetParameter = resolve_method<SetParameter_prototyper>(meth);

        meth = "TwSetDaqParameterInt";
        m_SetParameterInt = resolve_method<SetParameterInt_prototype>(meth);

        meth = "TwSetDaqParameterBool";
        m_SetParameterBool = resolve_method<SetParameterBool_prototype>(meth);

        meth = "TwSetDaqParameterFloat";
        m_SetParameterFloat = resolve_method<SetParameterFloat_prototype>(meth);

        meth = "TwSetDaqParameterInt64";
        m_SetParameterInt64 = resolve_method<SetParameterInt64_prototype>(meth);

        meth = "TwSetDaqParameterDouble";
        m_SetParameterDouble = resolve_method<SetParameterDouble_prototype>(meth);

        meth = "TwInitializeDll";
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

        int in = initialize_dll();
        if( in == 4)
            return 0;
        else
            return -1;

    }

    void TofDaqDll::dispose()
    {
        cleanup_dll();

        m_InitializeDll = NULL;
        m_CleanupDll = NULL;
        m_StartAcquisition = NULL;
        m_StopAcquisition = NULL;
        m_AquisitionActive = NULL;
        m_RegisterUserDat = NULL;
        m_UnregisterUserData = NULL;
        m_GetSpectrum = NULL;
        m_GetAverageSpectrum = NULL;
        m_GetTraces = NULL;
        m_GetParameter = NULL;
        m_GetParameterInt = NULL;
        m_GetParameterBool = NULL;
        m_GetParameterFloat = NULL;
        m_GetParameterInt64 = NULL;
        m_GetParameterDouble = NULL;
        m_WaitForNewData = NULL;
        m_UpdateUserData = NULL;
        m_SetParameter = NULL;
        m_SetParameterInt = NULL;
        m_SetParameterBool = NULL;
        m_SetParameterFloat = NULL;
        m_SetParameterInt64 = NULL;
        m_SetParameterDouble = NULL;

        if(m_data_aquisition_dll){

            if(m_data_aquisition_dll->isLoaded())
                m_data_aquisition_dll->unload();

            delete m_data_aquisition_dll;
            m_data_aquisition_dll = NULL;
        }


    }

    int TofDaqDll::initialize_dll() const
    {
        if(m_InitializeDll){
            return twErrChk(m_InitializeDll());
        }
        return -1;
    }

    void TofDaqDll::cleanup_dll() const
    {
        if(m_CleanupDll){
            m_CleanupDll();
        }
    }

    int TofDaqDll::start_aquisition() const
    {
        qDebug()<<"start_aquisition";
        if(m_StartAcquisition){
            return twErrChk(m_StartAcquisition());
        }
        return -1;
    }

    int TofDaqDll::stop_aquisition() const
    {
        if(m_StopAcquisition){
            return twErrChk(m_StopAcquisition());
        }
        return -1;
    }

    bool TofDaqDll::aquisition_active() const
    {
        if(m_AquisitionActive){
            return m_AquisitionActive();
        }
        return false;
    }

    int  TofDaqDll::register_user_data(const QStringList& value_names, const QString& path , int cmp_lvl) const
    {
        if(m_RegisterUserDat){
            int NbrElements = value_names.length();
            QByteArray path_ba = path.toLocal8Bit();
            path_ba.append('\0');
            QByteArray desc_ba;
            QByteArray desc_elem_ba;
            foreach (QString des_elem, value_names) {
                desc_elem_ba = des_elem.toLocal8Bit();
                desc_elem_ba.resize(256);
                desc_ba.append(desc_elem_ba);
                desc_elem_ba.clear();
            }

            int ret = twErrChk(m_RegisterUserDat(path_ba.data(), NbrElements, desc_ba.data(), cmp_lvl));
            //int ret = twErrChk(m_RegisterUserDat(path_ba.data(), NbrElements, NULL, cmp_lvl));

            return ret;
        }
        return -1;
    }

    int TofDaqDll::UpdateUserData( QVector<double> &Data, const QString &path) const
    {
        if(m_UpdateUserData){
            QByteArray path_ba = path.toLocal8Bit();
            path_ba.append('\0');
            int NbrElements = Data.length();

            return twErrChk(m_UpdateUserData(path_ba.data(),NbrElements, Data.data()));
        }
        return -1;
    }

    int TofDaqDll::unregister_user_data(const QString& path) const
    {
        QByteArray path_ba = path.toLocal8Bit();

        if(m_UnregisterUserData){
            int ret = twErrChk(m_UnregisterUserData(path_ba.data()));
            return ret;
        }
        return -1;
    }

    int TofDaqDll::read_spectrum(QVector<float>& buffer_Spectrum, int BufIndex , int SegmentIndex, int SegmentEndIndex, bool Normalize) const
    {
        if(m_GetSpectrum){
            return twErrChk(m_GetSpectrum(buffer_Spectrum.data(), SegmentIndex, SegmentEndIndex, BufIndex, Normalize));
        }
        return -1;
    }

    int TofDaqDll::read_average_spectrum(QVector<double> &buffer_avg_spectrum, bool Normalize) const
    {
        if(m_GetAverageSpectrum){
            return twErrChk(m_GetAverageSpectrum(buffer_avg_spectrum.data(), Normalize));
        }
        return -1;
    }

    int TofDaqDll::read_traces(QVector<float>& buffer_Spectrum, QVector<float> buffer_Masses, int BufIndex, int SegmentIndex, int SegmentEndIndex) const
    {
        if(m_GetTraces){
            return twErrChk(m_GetTraces(buffer_Spectrum.data(), buffer_Masses.data(), SegmentIndex, SegmentEndIndex, BufIndex));
        }
        return -1;
    }

    int TofDaqDll::wait_for_new_data(int Timeout, shared_memory_descriptor &pBufDesc, shared_memory_pointer &pShMem, bool WaitForEventReset)
    {
        if(m_WaitForNewData){
            return twErrChk(m_WaitForNewData(Timeout, pBufDesc.next_data(), pShMem.next_data(), WaitForEventReset));
        }
        return -1;
    }

    QString TofDaqDll::read_parameter(const QString& para) const
    {
        if(m_GetParameter){
            QByteArray para_ba = para.toLocal8Bit();

            char* tmp = (m_GetParameter(para_ba.data()));
            return QString(tmp);
        }
        return "";
    }

    int TofDaqDll::read_int_parameter(const QString& para) const
    {
        if(m_GetParameterInt){
            QByteArray para_ba = para.toLocal8Bit();

            return (m_GetParameterInt(para_ba.data()));
        }
        return -1;
    }

    bool TofDaqDll::read_bool_parameter(const QString& para) const
    {
        if(m_GetParameterBool){
            QByteArray para_ba = para.toLocal8Bit();

            return (m_GetParameterBool(para_ba.data()));
        }
        return false;
    }

    float TofDaqDll::read_float_parameter(const QString& para) const
    {
        if(m_GetParameterFloat){
            QByteArray para_ba = para.toLocal8Bit();

            return (m_GetParameterFloat(para_ba.data()));
        }
        return -1;
    }

    qint64 TofDaqDll::read_int64_parameter(const QString& para) const
    {
        if(m_GetParameterInt64){
            QByteArray para_ba = para.toLocal8Bit();

            return (m_GetParameterInt64(para_ba.data()));
        }
        return -1;
    }

    double TofDaqDll::read_double_parameter(const QString& para) const
    {
        if(m_GetParameterDouble){
            QByteArray para_ba = para.toLocal8Bit();

            return (m_GetParameterDouble(para_ba.data()));
        }
        return -1;
    }

    int TofDaqDll::write_parameter(const QString &para, QString value) const
    {
        if(m_SetParameter){
            QByteArray para_ba = para.toLocal8Bit();
            QByteArray value_ba = value.toLocal8Bit();

            return twErrChk(m_SetParameter(para_ba.data(),value_ba.data()));
        }
        return -1;
    }

    int TofDaqDll::write_int_parameter(const QString &para, int value) const
    {
        if(m_SetParameterInt){
            QByteArray para_ba = para.toLocal8Bit();

            return twErrChk(m_SetParameterInt(para_ba.data(),value));
        }
        return -1;
    }

    int TofDaqDll::write_bool_parameter(const QString &para, bool value) const
    {
        if(m_SetParameterBool){
            QByteArray para_ba = para.toLocal8Bit();

            return twErrChk(m_SetParameterBool(para_ba.data(),value));
        }
        return -1;
    }

    int TofDaqDll::write_float_parameter(const QString &para, float value) const
    {
        if(m_SetParameterFloat){
            QByteArray para_ba = para.toLocal8Bit();

            return twErrChk(m_SetParameterFloat(para_ba.data(),value));
        }
        return -1;
    }

    int TofDaqDll::write_int64_parameter(const QString &para, qint64 value) const
    {
        if(m_SetParameterInt64){
            QByteArray para_ba = para.toLocal8Bit();

            return twErrChk(m_SetParameterInt64(para_ba.data(),value));
        }
        return -1;
    }

    int TofDaqDll::write_double_parameter(const QString &para, double value) const
    {
        if(m_SetParameterDouble){
            QByteArray para_ba = para.toLocal8Bit();

            return twErrChk(m_SetParameterDouble(para_ba.data(),value));
        }
        return -1;
    }

    int TofDaqDll::tps_connect()
    {
        if(m_TpsConnect){
            return (m_TpsConnect());
        }
        return -1;
    }

    int TofDaqDll::tps_connect_2(QString ip, int type)
    {
        if(m_TpsConnect2){
            QByteArray ip_ba = ip.toLocal8Bit();
            return (m_TpsConnect2(ip_ba.data(), type));
        }
        return -1;
    }

    int TofDaqDll::tps_disconnect()
    {
        if(m_TpsDisconnect){
            return (m_TpsDisconnect());
        }
        return -1;
    }

    int TofDaqDll::tps_get_monitor_value(int moduleCode, double &value)
    {
        if(m_TpsGetMonitorValue){
            return (m_TpsGetMonitorValue(moduleCode, &value));
        }
        return -1;
    }

    int TofDaqDll::tps_get_target_value(int moduleCode, double &value)
    {
        if(m_TpsGetTargetValue){
            return (m_TpsGetTargetValue(moduleCode, &value));
        }
        return -1;
    }

    int TofDaqDll::tps_get_last_set_value(int moduleCode, double &value)
    {
        if(m_TpsGetLastSetValue){
            return (m_TpsGetLastSetValue(moduleCode, &value));
        }
        return -1;
    }

    int TofDaqDll::tps_set_target_value(int moduleCode, double value)
    {
        if(m_TpsSetTargetValue){
            return (m_TpsSetTargetValue(moduleCode, value));
        }
        return -1;
    }

    int TofDaqDll::tps_get_nbr_modules(int &nbrModules)
    {
        if(m_TpsGetNbrModules){
            return (m_TpsGetNbrModules(&nbrModules));
        }
        return -1;
    }

    int TofDaqDll::tps_get_module_codes(QVector<int> &moduleCodeBuffer)
    {
        if(m_TpsGetModuleCodes){
            int bufferLength =moduleCodeBuffer.length();
            return (m_TpsGetModuleCodes(moduleCodeBuffer.data(), bufferLength));
        }
        return -1;
    }

    int TofDaqDll::tps_initialize()
    {
        if(m_TpsInitialize){
            return (m_TpsInitialize());
        }
        return -1;
    }

    int TofDaqDll::tps_set_all_voltages()
    {
        if(m_TpsSetAllVoltages){
            return (m_TpsSetAllVoltages());
        }
        return -1;
    }

    int TofDaqDll::tps_shutdown()
    {
        if(m_TpsShutdown){
            return (m_TpsShutdown());
        }
        return -1;
    }

    int TofDaqDll::tps_get_status(int &status)
    {
        if(m_TpsGetStatus){
            return (m_TpsGetStatus(&status));
        }
        return -1;
    }

    int TofDaqDll::tps_load_set_file(QString setFile)
    {
        if(m_TpsLoadSetFile){
            QByteArray setFile_ba = setFile.toLocal8Bit();
            return (m_TpsLoadSetFile(setFile_ba.data()));
        }
        return -1;
    }

    int TofDaqDll::tps_save_set_file(QString setFile)
    {
        if(m_TpsSaveSetFile){
            QByteArray setFile_ba = setFile.toLocal8Bit();
            return (m_TpsSaveSetFile(setFile_ba.data()));
        }
        return -1;
    }

    int TofDaqDll::tps_get_active_filament(int &activeFilament)
    {
        if(m_TpsGetActiveFilament){
            return (m_TpsGetActiveFilament(&activeFilament));
        }
        return -1;
    }

    int TofDaqDll::tps_set_active_filament(int activeFilament)
    {
        if(m_TpsSetActiveFilament){
            return (m_TpsSetActiveFilament(activeFilament));
        }
        return -1;
    }

    int TofDaqDll::tps_get_module_limits(int moduleCode, double &minLimit, double &maxLimit)
    {
        if(m_TpsGetModuleLimits){
            return (m_TpsGetModuleLimits(moduleCode, &minLimit, &maxLimit));
        }
        return -1;
    }

}


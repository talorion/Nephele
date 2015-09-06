#include "data_aquisition_dll_wrapper.hpp"

#include <QDebug>
#include <QtGlobal>

#include "tof_daq_specific/tof_daq_dll_tools.hpp"

#include "core/event_manager.hpp"


namespace talorion{
    data_aquisition_dll_wrapper::data_aquisition_dll_wrapper(QObject *par) :
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
        m_data_aquisition_dll(NULL)
    {
        connect(event_manager::get_instance(),SIGNAL(start_aquisition()),this, SLOT(start_aquisition()));
        connect(event_manager::get_instance(),SIGNAL(stop_aquisition()),this, SLOT(stop_aquisition()));

    }

    data_aquisition_dll_wrapper::~data_aquisition_dll_wrapper()
    {

    }

    void data_aquisition_dll_wrapper::init(QString dll_name)
    {
        m_data_aquisition_dll = new QLibrary(dll_name);
        if (!m_data_aquisition_dll->load()){
            qDebug()<<m_data_aquisition_dll->errorString();
            return;
        }

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

        initialize_dll();

    }

    void data_aquisition_dll_wrapper::dispose()
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

    int data_aquisition_dll_wrapper::initialize_dll()
    {
        if(m_InitializeDll){
            return twErrChk(m_InitializeDll());
        }
        return -1;
    }

    void data_aquisition_dll_wrapper::cleanup_dll()
    {
        if(m_CleanupDll){
            m_CleanupDll();
        }
    }

    int data_aquisition_dll_wrapper::start_aquisition()
    {
        if(m_StartAcquisition){
            return twErrChk(m_StartAcquisition());
        }
        return -1;
    }

    int data_aquisition_dll_wrapper::stop_aquisition()
    {
        if(m_StopAcquisition){
            return twErrChk(m_StopAcquisition());
        }
        return -1;
    }

    bool data_aquisition_dll_wrapper::aquisition_active()
    {
        if(m_AquisitionActive){
            return m_AquisitionActive();
        }
        return false;
    }

    int  data_aquisition_dll_wrapper::register_user_data(const QStringList& value_names, const QString& path , int cmp_lvl)
    {
        if(m_RegisterUserDat){
            int NbrElements = value_names.size();
            QByteArray path_ba = path.toLocal8Bit();
            QByteArray desc_ba;
            QByteArray desc_elem_ba;
            foreach (QString des_elem, value_names) {
                desc_elem_ba = des_elem.toLocal8Bit();
                desc_elem_ba.resize(256);
                desc_ba.append(desc_elem_ba);
                desc_elem_ba.clear();
            }

            int ret = twErrChk(m_RegisterUserDat(path_ba.data(), NbrElements, desc_ba.data(), cmp_lvl));

            return ret;
        }
        return -1;
    }

    int data_aquisition_dll_wrapper::UpdateUserData( QVector<double> &Data, const QString &path)
    {
        if(m_UpdateUserData){
            QByteArray path_ba = path.toLocal8Bit();
            int NbrElements = Data.size();

            return twErrChk(m_UpdateUserData(path_ba.data(),NbrElements, Data.data()));
        }
        return -1;
    }

    int data_aquisition_dll_wrapper::unregister_user_data(const QString& path)
    {
        QByteArray path_ba = path.toLocal8Bit();

        if(m_UnregisterUserData){
            int ret = twErrChk(m_UnregisterUserData(path_ba.data()));
            return ret;
        }
        return -1;
    }

    int data_aquisition_dll_wrapper::read_spectrum(QVector<float>& buffer_Spectrum, int BufIndex , int SegmentIndex, int SegmentEndIndex, bool Normalize)
    {
        if(m_GetSpectrum){
            return twErrChk(m_GetSpectrum(buffer_Spectrum.data(), SegmentIndex, SegmentEndIndex, BufIndex, Normalize));
        }
        return -1;
    }

    int data_aquisition_dll_wrapper::read_average_spectrum(QVector<double> &buffer_avg_spectrum, bool Normalize)
    {
        if(m_GetAverageSpectrum){
            return twErrChk(m_GetAverageSpectrum(buffer_avg_spectrum.data(), Normalize));
        }
        return -1;
    }

    int data_aquisition_dll_wrapper::read_traces(QVector<float>& buffer_Spectrum, QVector<float> buffer_Masses, int BufIndex, int SegmentIndex, int SegmentEndIndex)
    {
        if(m_GetTraces){
            return twErrChk(m_GetTraces(buffer_Spectrum.data(), buffer_Masses.data(), SegmentIndex, SegmentEndIndex, BufIndex));
        }
        return -1;
    }

    //    int data_aquisition_dll_wrapper::wait_for_new_data(int Timeout, shared_memory_descriptor &pBufDesc, shared_memory_pointer &pShMem, bool WaitForEventReset)
    //    {
    //        if(m_WaitForNewData){
    //            return twErrChk(m_WaitForNewData(Timeout, pBufDesc.data(), pShMem.data(), WaitForEventReset));
    //        }
    //        return -1;
    //    }

    QString data_aquisition_dll_wrapper::read_parameter(const QString& para)
    {
        if(m_GetParameter){
            QByteArray para_ba = para.toLocal8Bit();

            char* tmp = (m_GetParameter(para_ba.data()));
            return QString(tmp);
        }
        return "";
    }

    int data_aquisition_dll_wrapper::read_int_parameter(const QString& para)
    {
        if(m_GetParameterInt){
            QByteArray para_ba = para.toLocal8Bit();

            return (m_GetParameterInt(para_ba.data()));
        }
        return -1;
    }

    bool data_aquisition_dll_wrapper::read_bool_parameter(const QString& para)
    {
        if(m_GetParameterBool){
            QByteArray para_ba = para.toLocal8Bit();

            return (m_GetParameterBool(para_ba.data()));
        }
        return false;
    }

    float data_aquisition_dll_wrapper::read_float_parameter(const QString& para)
    {
        if(m_GetParameterFloat){
            QByteArray para_ba = para.toLocal8Bit();

            return (m_GetParameterFloat(para_ba.data()));
        }
        return -1;
    }

    qint64 data_aquisition_dll_wrapper::read_int64_parameter(const QString& para)
    {
        if(m_GetParameterInt64){
            QByteArray para_ba = para.toLocal8Bit();

            return (m_GetParameterInt64(para_ba.data()));
        }
        return -1;
    }

    double data_aquisition_dll_wrapper::read_double_parameter(const QString& para)
    {
        if(m_GetParameterDouble){
            QByteArray para_ba = para.toLocal8Bit();

            return (m_GetParameterDouble(para_ba.data()));
        }
        return -1;
    }

    int data_aquisition_dll_wrapper::write_parameter(const QString &para, QString value)
    {
        if(m_SetParameter){
            QByteArray para_ba = para.toLocal8Bit();
            QByteArray value_ba = value.toLocal8Bit();

            return twErrChk(m_SetParameter(para_ba.data(),value_ba.data()));
        }
        return -1;
    }

    int data_aquisition_dll_wrapper::write_int_parameter(const QString &para, int value)
    {
        if(m_SetParameterInt){
            QByteArray para_ba = para.toLocal8Bit();

            return twErrChk(m_SetParameterInt(para_ba.data(),value));
        }
        return -1;
    }

    int data_aquisition_dll_wrapper::write_bool_parameter(const QString &para, bool value)
    {
        if(m_SetParameterBool){
            QByteArray para_ba = para.toLocal8Bit();

            return twErrChk(m_SetParameterBool(para_ba.data(),value));
        }
        return -1;
    }

    int data_aquisition_dll_wrapper::write_float_parameter(const QString &para, float value)
    {
        if(m_SetParameterFloat){
            QByteArray para_ba = para.toLocal8Bit();

            return twErrChk(m_SetParameterFloat(para_ba.data(),value));
        }
        return -1;
    }

    int data_aquisition_dll_wrapper::write_int64_parameter(const QString &para, qint64 value)
    {
        if(m_SetParameterInt64){
            QByteArray para_ba = para.toLocal8Bit();

            return twErrChk(m_SetParameterInt64(para_ba.data(),value));
        }
        return -1;
    }

    int data_aquisition_dll_wrapper::write_double_parameter(const QString &para, double value)
    {
        if(m_SetParameterDouble){
            QByteArray para_ba = para.toLocal8Bit();

            return twErrChk(m_SetParameterDouble(para_ba.data(),value));
        }
        return -1;
    }
}


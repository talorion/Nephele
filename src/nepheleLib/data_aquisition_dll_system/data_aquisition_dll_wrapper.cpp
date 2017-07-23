#include "data_aquisition_dll_wrapper.hpp"

#if defined( Q_OS_WIN )
#include <Windows.h>
#endif

#include <QApplication>
#include <QDebug>
#include <QDir>
#include <QtGlobal>

#include "tof_daq_specific/tof_daq_dll_tools.hpp"
#include "tof_daq_specific/shared_memory_descriptor.h"
#include "tof_daq_specific/shared_memory_pointer.h"

#include "core/EventManager.hpp"


namespace talorion{
data_aquisition_dll_wrapper::data_aquisition_dll_wrapper(QObject *par) :
    abstract_scriptable_object("direct_daq", par),
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
    m_GetRegUserDataSources(NULL),
    m_TofDaqRunning(NULL),
    m_GetRegUserDataDesc(NULL),
    m_ReadRegUserData(NULL),
    m_data_aquisition_dll(NULL),
    m_dll_name()
{


}

data_aquisition_dll_wrapper::~data_aquisition_dll_wrapper()
{

    if(m_data_aquisition_dll)
        delete m_data_aquisition_dll;
}

int data_aquisition_dll_wrapper::init(QString dll_name)
{
    QFileInfo f(dll_name);
    QString absolutePath = f.absoluteDir().absolutePath();
    QString name = f.fileName();


#if defined( Q_OS_WIN )
    //LPCSTR lstr = absolutePath.toStdString().c_str();
    //LPCWSTR lstr = absolutePath.toUtf8();
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

    meth = "TwGetRegUserDataSources";
    m_GetRegUserDataSources = resolve_method<GetRegUserDataSources_prototype>(meth);

    meth = "TwTofDaqRunning";
    m_TofDaqRunning = resolve_method<TofDaqRunning_prototype>(meth);

    meth = "TwGetRegUserDataDesc";
    m_GetRegUserDataDesc = resolve_method<GetRegUserDataDesc_prototype>(meth);

    meth = "TwReadRegUserData";
    m_ReadRegUserData = resolve_method<ReadRegUserData_prototype>(meth);


    int in = initialize_dll();
    if( in == 4)
        return 0;
    else
        return -1;

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
    m_GetRegUserDataSources =NULL;
    m_TofDaqRunning =NULL;
    m_GetRegUserDataDesc =NULL;

    if(m_data_aquisition_dll){

        if(m_data_aquisition_dll->isLoaded())
            m_data_aquisition_dll->unload();

        delete m_data_aquisition_dll;
        m_data_aquisition_dll = NULL;
    }
}

int data_aquisition_dll_wrapper::initialize_dll() const
{
    if(m_InitializeDll){
        return twErrChk(m_InitializeDll());
    }
    return -1;
}

void data_aquisition_dll_wrapper::cleanup_dll() const
{
    if(m_CleanupDll){
        m_CleanupDll();
    }
}

int data_aquisition_dll_wrapper::start_aquisition() const
{
    qDebug()<<"start_aquisition";
    if(m_StartAcquisition){
        return twErrChk(m_StartAcquisition());
    }
    return -1;
}

int data_aquisition_dll_wrapper::stop_aquisition() const
{
    if(m_StopAcquisition){
        return twErrChk(m_StopAcquisition());
    }
    return -1;
}

bool data_aquisition_dll_wrapper::aquisition_active() const
{
    if(m_AquisitionActive){
        return m_AquisitionActive();
    }
    return false;
}

bool data_aquisition_dll_wrapper::tof_daq_running() const
{
    if(m_TofDaqRunning){
        return m_TofDaqRunning();
    }
    return false;
}

int  data_aquisition_dll_wrapper::register_user_data(const QStringList& value_names, const QString& path , int cmp_lvl) const
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

int data_aquisition_dll_wrapper::UpdateUserData( QVector<double> &Data, const QString &path) const
{
    if(m_UpdateUserData){
        QByteArray path_ba = path.toLocal8Bit();
        path_ba.append('\0');
        int NbrElements = Data.length();

        return twErrChk(m_UpdateUserData(path_ba.data(),NbrElements, Data.data()));
    }
    return -1;
}

int data_aquisition_dll_wrapper::unregister_user_data(const QString& path) const
{
    QByteArray path_ba = path.toLocal8Bit();

    if(m_UnregisterUserData){
        int ret = twErrChk(m_UnregisterUserData(path_ba.data()));
        return ret;
    }
    return -1;
}

int data_aquisition_dll_wrapper::get_reg_user_data_sources(QStringList& locations, QVector<int>& nbrElements, QVector<int>& type)
{
    if(m_GetRegUserDataSources){
        int arrayLength =0;
        //Can be used to query the number of data sources by setting *arrayLength to 0 and passing NULL for location, nbrElements and type.
        int ret = m_GetRegUserDataSources(&arrayLength, NULL, NULL, NULL);

        if(ret== 9){
            ret = 4;
        }

        if(ret == 4  && arrayLength>0){
            char *location = new char[arrayLength*256];

            nbrElements.resize(arrayLength);
            type.resize(arrayLength);

            ret = twErrChk(m_GetRegUserDataSources(&arrayLength, location, nbrElements.data(), type.data()));

            QString tmp;
            char *location_tmp;
            for(int i=0; i<arrayLength; ++i){
                location_tmp = location+(i*256);
                //QByteArray arr(location[i*256], 256);
                tmp = QString::fromLocal8Bit(location_tmp, 256);
                locations.append(tmp);
            }

            delete[] location;

        }

        return ret;
    }
    return -1;
}

int data_aquisition_dll_wrapper::get_reg_user_data_desc(const QString &path, QStringList &descriptions)
{
    if(m_GetRegUserDataSources){
        int arrayLength =0;

        QByteArray path_ba = path.toLocal8Bit();
        path_ba.append('\0');

        int ret =m_GetRegUserDataDesc(path_ba.data(), &arrayLength, NULL);

        if((ret == 4 || ret == 9) && arrayLength>0){
            descriptions.clear();
            char *elementDescription = new char[arrayLength*256];
            ret = twErrChk(m_GetRegUserDataDesc(path_ba.data(), &arrayLength, elementDescription));
            if(ret == 4  && arrayLength>0){
                QString tmp;
                char *elementDescription_tmp;
                for(int i=0; i<arrayLength; ++i){
                    elementDescription_tmp = elementDescription+(i*256);
                    //QByteArray arr(location[i*256], 256);
                    //tmp = QString::fromLocal8Bit(elementDescription_tmp, 256);
                    tmp = QString::fromLocal8Bit(elementDescription_tmp);
                    tmp = tmp.trimmed();
                    descriptions.append(tmp);
                }
            }
            delete[] elementDescription;

        }
        return ret;
    }
    return -1;
}

int data_aquisition_dll_wrapper::read_reg_user_data(const QString &path, QVector<double> &Data)
{
    if(m_ReadRegUserData){
        int NbrElements= Data.length();
        if(NbrElements<1)
            return -1;

        QByteArray path_ba = path.toLocal8Bit();
        path_ba.append('\0');

        return m_ReadRegUserData(path_ba.data(), NbrElements, Data.data());

    }
    return -1;
}

//    int data_aquisition_dll_wrapper::read_reg_user_date(QStringList &value_names, QVector<double> &Data)
//    {
//      if(m_ReadRegUserData){
//          int ret = twErrChk(m_ReadRegUserData());
//          return ret;
//        }
//      return -1;
//    }

int data_aquisition_dll_wrapper::read_spectrum(QVector<float>& buffer_Spectrum, int BufIndex , int SegmentIndex, int SegmentEndIndex, bool Normalize) const
{
    if(m_GetSpectrum){
        return twErrChk(m_GetSpectrum(buffer_Spectrum.data(), SegmentIndex, SegmentEndIndex, BufIndex, Normalize));
    }
    return -1;
}

int data_aquisition_dll_wrapper::read_average_spectrum(QVector<double> &buffer_avg_spectrum, bool Normalize) const
{
    if(m_GetAverageSpectrum){
        return twErrChk(m_GetAverageSpectrum(buffer_avg_spectrum.data(), Normalize));
    }
    return -1;
}

int data_aquisition_dll_wrapper::read_traces(QVector<float>& buffer_Spectrum, QVector<float> buffer_Masses, int BufIndex, int SegmentIndex, int SegmentEndIndex) const
{
    if(m_GetTraces){
        return twErrChk(m_GetTraces(buffer_Spectrum.data(), buffer_Masses.data(), SegmentIndex, SegmentEndIndex, BufIndex));
    }
    return -1;
}

int data_aquisition_dll_wrapper::wait_for_new_data(int Timeout, shared_memory_descriptor &pBufDesc, shared_memory_pointer &pShMem, bool WaitForEventReset)
{
    if(m_WaitForNewData){
        return twErrChk(m_WaitForNewData(Timeout, pBufDesc.next_data(), pShMem.next_data(), WaitForEventReset));
    }
    return -1;
}

QString data_aquisition_dll_wrapper::read_parameter(const QString& para) const
{
    if(m_GetParameter){
        QByteArray para_ba = para.toLocal8Bit();

        char* tmp = (m_GetParameter(para_ba.data()));
        return QString(tmp);
    }
    return "";
}

int data_aquisition_dll_wrapper::read_int_parameter(const QString& para) const
{
    if(m_GetParameterInt){
        QByteArray para_ba = para.toLocal8Bit();

        return (m_GetParameterInt(para_ba.data()));
    }
    return -1;
}

bool data_aquisition_dll_wrapper::read_bool_parameter(const QString& para) const
{
    if(m_GetParameterBool){
        QByteArray para_ba = para.toLocal8Bit();

        return (m_GetParameterBool(para_ba.data()));
    }
    return false;
}

float data_aquisition_dll_wrapper::read_float_parameter(const QString& para) const
{
    if(m_GetParameterFloat){
        QByteArray para_ba = para.toLocal8Bit();

        return (m_GetParameterFloat(para_ba.data()));
    }
    return -1;
}

qint64 data_aquisition_dll_wrapper::read_int64_parameter(const QString& para) const
{
    if(m_GetParameterInt64){
        QByteArray para_ba = para.toLocal8Bit();

        return (m_GetParameterInt64(para_ba.data()));
    }
    return -1;
}

double data_aquisition_dll_wrapper::read_double_parameter(const QString& para) const
{
    if(m_GetParameterDouble){
        QByteArray para_ba = para.toLocal8Bit();

        return (m_GetParameterDouble(para_ba.data()));
    }
    return -1;
}

int data_aquisition_dll_wrapper::write_parameter(const QString &para, QString value) const
{
    if(m_SetParameter){
        QByteArray para_ba = para.toLocal8Bit();
        QByteArray value_ba = value.toLocal8Bit();

        return twErrChk(m_SetParameter(para_ba.data(),value_ba.data()));
    }
    return -1;
}

int data_aquisition_dll_wrapper::write_int_parameter(const QString &para, int value) const
{
    if(m_SetParameterInt){
        QByteArray para_ba = para.toLocal8Bit();

        return twErrChk(m_SetParameterInt(para_ba.data(),value));
    }
    return -1;
}

int data_aquisition_dll_wrapper::write_bool_parameter(const QString &para, bool value) const
{
    if(m_SetParameterBool){
        QByteArray para_ba = para.toLocal8Bit();

        return twErrChk(m_SetParameterBool(para_ba.data(),value));
    }
    return -1;
}

int data_aquisition_dll_wrapper::write_float_parameter(const QString &para, float value) const
{
    if(m_SetParameterFloat){
        QByteArray para_ba = para.toLocal8Bit();

        return twErrChk(m_SetParameterFloat(para_ba.data(),value));
    }
    return -1;
}

int data_aquisition_dll_wrapper::write_int64_parameter(const QString &para, qint64 value) const
{
    if(m_SetParameterInt64){
        QByteArray para_ba = para.toLocal8Bit();

        return twErrChk(m_SetParameterInt64(para_ba.data(),value));
    }
    return -1;
}

int data_aquisition_dll_wrapper::write_double_parameter(const QString &para, double value) const
{
    if(m_SetParameterDouble){
        QByteArray para_ba = para.toLocal8Bit();

        return twErrChk(m_SetParameterDouble(para_ba.data(),value));
    }
    return -1;
}
}


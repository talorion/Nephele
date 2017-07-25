#include "QTofDaqDll.hpp"

#include <QFileInfo>

#include <QFileInfo>
#include <QDebug>
#include <QDir>
#include <QtGlobal>
#include <QThread>
#include <QTime>

#if defined( Q_OS_WIN )
#include <WTypes.h>
#else
#define __int64 long long
#define __stdcall
#define __declspec( dllexport )
#endif

#include "TofDaqDll.h"
#include "TofIpcStrucs.h"


namespace talorion {

QTofDaqDll::QTofDaqDll(QObject *par):
    QObject(par),
    m_absolutePath(),
    m_dll_name(),
    m_data_aquisition_dll(Q_NULLPTR),
    m_TwCleanupDll(Q_NULLPTR),
    m_TwGetDllVersion(Q_NULLPTR),
    m_TwDaqActive(Q_NULLPTR),
    m_TwGetTimeout(Q_NULLPTR),
    m_TwSetTimeout(Q_NULLPTR),
    m_TwTofDaqRunning(Q_NULLPTR),
    m_TwCloseTofDaqRec(Q_NULLPTR),
    m_TwGetDaqParameter(Q_NULLPTR),
    m_TwGetDaqParameterInt(Q_NULLPTR),
    m_TwGetDaqParameterBool(Q_NULLPTR),
    m_TwGetDaqParameterFloat(Q_NULLPTR),
    m_TwGetDaqParameterInt64(Q_NULLPTR),
    m_TwGetDaqParameterDouble(Q_NULLPTR),
    m_TwSetDaqParameter(Q_NULLPTR),
    m_TwSetDaqParameterInt(Q_NULLPTR),
    m_TwSetDaqParameterBool(Q_NULLPTR),
    m_TwSetDaqParameterFloat(Q_NULLPTR),
    m_TwSetDaqParameterInt64(Q_NULLPTR),
    m_TwSetDaqParameterDouble(Q_NULLPTR),
    m_TwStartAcquisition(Q_NULLPTR),
    m_TwStopAcquisition(Q_NULLPTR),
    m_TwContinueAcquisition(Q_NULLPTR),
    m_TwGetDescriptor(Q_NULLPTR),
    m_TwGetPeakParameters(Q_NULLPTR),
    m_TwGetMassCalib(Q_NULLPTR),
    m_TwGetSpecXaxisFromShMem(Q_NULLPTR),
    m_TwGetTofSpectrumFromShMem(Q_NULLPTR),
    m_TwGetBufTimeFromShMem(Q_NULLPTR),
    m_TwGetStickSpectrumFromShMem(Q_NULLPTR),
    m_TwGetRegUserDataSources(Q_NULLPTR),
    m_TwRegisterUserDataBuf(Q_NULLPTR),
    m_TwGetRegUserDataDesc(Q_NULLPTR),
    m_TwReadRegUserData(Q_NULLPTR),
    m_TwUnregisterUserData(Q_NULLPTR),
    m_TwUpdateUserData(Q_NULLPTR),
    m_TwGetSumSpectrumFromShMem(Q_NULLPTR),
    m_shared_memory_descriptor(new TSharedMemoryDesc)
{

}

QTofDaqDll::~QTofDaqDll()
{
    dispose();
    delete m_shared_memory_descriptor;
}

int QTofDaqDll::init(QString dll_name, int timeout)
{
    if(dll_name.isEmpty()){
        qWarning()<<"given QTofDaqDll dll_name is empty";
        return -3;
    }

    if(isInitialized())
        dispose();

    QFileInfo f(dll_name);
    m_absolutePath = f.absoluteDir().absolutePath();
    QString name = f.fileName();

#if defined( Q_OS_WIN )
    LPCWSTR str = (const wchar_t*)m_absolutePath.utf16();
    //SetDllDirectory( str );
    m_cookie =AddDllDirectory( str );
    SetDefaultDllDirectories(LOAD_LIBRARY_SEARCH_USER_DIRS);
#else
    name = dll_name;
#endif

    m_data_aquisition_dll = new QLibrary(name);
    if (!m_data_aquisition_dll->load()){
        QString err(m_data_aquisition_dll->errorString());
        qWarning()<<err;
        return -1;
    }
    m_dll_name = dll_name;

    QString meth = "TwCleanupDll";
    m_TwCleanupDll = resolveMethod<TwCleanupDllPtr>(meth);

    meth = "TwGetDllVersion";
    m_TwGetDllVersion = resolveMethod<TwGetDllVersionPtr>(meth);

    meth = "TwDaqActive";
    m_TwDaqActive = resolveMethod<TwDaqActivePtr>(meth);

    meth = "TwSetTimeout";
    m_TwSetTimeout = resolveMethod<TwSetTimeoutPtr>(meth);

    meth = "TwGetTimeout";
    m_TwGetTimeout = resolveMethod<TwGetTimeoutPtr>(meth);

    meth = "TwTofDaqRunning";
    m_TwTofDaqRunning = resolveMethod<TwTofDaqRunningPtr>(meth);

    meth = "TwCloseTofDaqRec";
    m_TwCloseTofDaqRec = resolveMethod<TwCloseTofDaqRecPtr>(meth);

    meth = "TwGetDaqParameter";
    m_TwGetDaqParameter = resolveMethod<TwGetDaqParameterPtr>(meth);

    meth = "TwGetDaqParameterInt";
    m_TwGetDaqParameterInt = resolveMethod<TwGetDaqParameterIntPtr>(meth);

    meth = "TwGetDaqParameterBool";
    m_TwGetDaqParameterBool = resolveMethod<TwGetDaqParameterBoolPtr>(meth);

    meth = "TwGetDaqParameterFloat";
    m_TwGetDaqParameterFloat = resolveMethod<TwGetDaqParameterFloatPtr>(meth);

    meth = "TwGetDaqParameterInt64";
    m_TwGetDaqParameterInt64 = resolveMethod<TwGetDaqParameterInt64Ptr>(meth);

    meth = "TwGetDaqParameterDouble";
    m_TwGetDaqParameterDouble = resolveMethod<TwGetDaqParameterDoublePtr>(meth);

    meth = "TwSetDaqParameter";
    m_TwSetDaqParameter = resolveMethod<TwSetDaqParameterPtr>(meth);

    meth = "TwSetDaqParameterInt";
    m_TwSetDaqParameterInt = resolveMethod<TwSetDaqParameterIntPtr>(meth);

    meth = "TwSetDaqParameterBool";
    m_TwSetDaqParameterBool = resolveMethod<TwSetDaqParameterBoolPtr>(meth);

    meth = "TwSetDaqParameterFloat";
    m_TwSetDaqParameterFloat = resolveMethod<TwSetDaqParameterFloatPtr>(meth);

    meth = "TwSetDaqParameterInt64";
    m_TwSetDaqParameterInt64 = resolveMethod<TwSetDaqParameterInt64Ptr>(meth);

    meth = "TwSetDaqParameterDouble";
    m_TwSetDaqParameterDouble = resolveMethod<TwSetDaqParameterDoublePtr>(meth);

    meth = "TwStartAcquisition";
    m_TwStartAcquisition = resolveMethod<TwStartAcquisitionPtr>(meth);

    meth = "TwStopAcquisition";
    m_TwStopAcquisition = resolveMethod<TwStopAcquisitionPtr>(meth);

    meth = "TwContinueAcquisition";
    m_TwContinueAcquisition = resolveMethod<TwContinueAcquisitionPtr>(meth);

    meth = "TwGetDescriptor";
    m_TwGetDescriptor = resolveMethod<TwGetDescriptorPtr>(meth);

    meth = "TwGetPeakParameters";
    m_TwGetPeakParameters = resolveMethod<TwGetPeakParametersPtr>(meth);

    meth = "TwGetMassCalib";
    m_TwGetMassCalib = resolveMethod<TwGetMassCalibPtr>(meth);

    meth = "TwGetSpecXaxisFromShMem";
    m_TwGetSpecXaxisFromShMem = resolveMethod<TwGetSpecXaxisFromShMemPtr>(meth);

    meth = "TwGetTofSpectrumFromShMem";
    m_TwGetTofSpectrumFromShMem = resolveMethod<TwGetTofSpectrumFromShMemPtr>(meth);

    meth = "TwGetBufTimeFromShMem";
    m_TwGetBufTimeFromShMem = resolveMethod<TwGetBufTimeFromShMemPtr>(meth);

    meth = "TwGetStickSpectrumFromShMem";
    m_TwGetStickSpectrumFromShMem = resolveMethod<TwGetStickSpectrumFromShMemPtr>(meth);

    meth = "TwGetRegUserDataSources";
    m_TwGetRegUserDataSources = resolveMethod<TwGetRegUserDataSourcesPtr>(meth);

    meth = "TwRegisterUserDataBuf";
    m_TwRegisterUserDataBuf = resolveMethod<TwRegisterUserDataBufPtr>(meth);

    meth = "TwGetRegUserDataDesc";
    m_TwGetRegUserDataDesc = resolveMethod<TwGetRegUserDataDescPtr>(meth);

    meth = "TwReadRegUserData";
    m_TwReadRegUserData = resolveMethod<TwReadRegUserDataPtr>(meth);

    meth = "TwUnregisterUserData";
    m_TwUnregisterUserData = resolveMethod<TwUnregisterUserDataPtr>(meth);

    meth = "TwUpdateUserData";
    m_TwUpdateUserData = resolveMethod<TwUpdateUserDataPtr>(meth);

    meth = "TwGetSumSpectrumFromShMem";
    m_TwGetSumSpectrumFromShMem = resolveMethod<TwGetSumSpectrumFromShMemPtr>(meth);

    setTimeout(timeout);

    if(isInitialized()){
        emit initialized();
        return 0;
    }

    return 5;//TwError
}

void QTofDaqDll::dispose()
{
    if(!isInitialized()){
        if(m_data_aquisition_dll != Q_NULLPTR){
            delete m_data_aquisition_dll;
            m_data_aquisition_dll = Q_NULLPTR;
        }
        return;
    }

    emit disposed();

    cleanupDll();

    m_data_aquisition_dll->unload();
    //delete m_data_aquisition_dll.data();
    //m_data_aquisition_dll.clear();
    delete m_data_aquisition_dll;
    m_data_aquisition_dll = Q_NULLPTR;

#if defined( Q_OS_WIN )
    if(RemoveDllDirectory( m_cookie ) ==false){
        qDebug()<<"RemoveDllDirectory";
    }
    //SetDefaultDllDirectories(LOAD_LIBRARY_SEARCH_DEFAULT_DIRS);
#endif
    m_absolutePath= QString();

    m_dll_name = QString();

    m_TwCleanupDll    = Q_NULLPTR;
    m_TwGetDllVersion = Q_NULLPTR;
    m_TwDaqActive     = Q_NULLPTR;
    m_TwGetTimeout    = Q_NULLPTR;
    m_TwSetTimeout    = Q_NULLPTR;
    m_TwTofDaqRunning = Q_NULLPTR;
    m_TwGetDaqParameter = Q_NULLPTR;
    m_TwGetDaqParameterInt = Q_NULLPTR;
    m_TwGetDaqParameterBool = Q_NULLPTR;
    m_TwGetDaqParameterFloat = Q_NULLPTR;
    m_TwGetDaqParameterInt64 = Q_NULLPTR;
    m_TwGetDaqParameterDouble = Q_NULLPTR;
    m_TwSetDaqParameter = Q_NULLPTR;
    m_TwSetDaqParameterInt = Q_NULLPTR;
    m_TwSetDaqParameterBool = Q_NULLPTR;
    m_TwSetDaqParameterFloat = Q_NULLPTR;
    m_TwSetDaqParameterInt64 = Q_NULLPTR;
    m_TwSetDaqParameterDouble = Q_NULLPTR;
    m_TwStartAcquisition = Q_NULLPTR;
    m_TwStopAcquisition = Q_NULLPTR;
    m_TwContinueAcquisition= Q_NULLPTR;
    m_TwGetPeakParameters = Q_NULLPTR;
    m_TwGetMassCalib = Q_NULLPTR;
    m_TwGetSpecXaxisFromShMem = Q_NULLPTR;
    m_TwGetTofSpectrumFromShMem = Q_NULLPTR;
    m_TwGetBufTimeFromShMem = Q_NULLPTR;
    m_TwGetStickSpectrumFromShMem = Q_NULLPTR;

}

void QTofDaqDll::setDllName(QString dll_name)
{
    dispose();
    init(dll_name);
}

int QTofDaqDll::twErrCheck(const int error) const
{
    if (error != 4){
        qDebug()<<"TW ERROR:"<< getTwErrorDescription(error);
    }
    return error;
}

Qt::HANDLE QTofDaqDll::threadId() const
{
    return QThread::currentThreadId();
}

QString QTofDaqDll::tofDaqRecName() const
{
    if(!isInitialized())
        return QString();

    if(m_absolutePath.isEmpty())
        return QString();

    QFileInfo exe(m_absolutePath, "TofDaqRec.exe");
    return QString(exe.absoluteFilePath());
}

void QTofDaqDll::cleanupDll()
{
    if(m_TwCleanupDll){
        m_TwCleanupDll();
    }
}

double QTofDaqDll::getDllVersion() const
{
    if(m_TwGetDllVersion){
        return m_TwGetDllVersion();
    }
    return 0.0;
}

bool QTofDaqDll::daqActive() const
{
    if(m_TwDaqActive){
        return m_TwDaqActive();
    }
    return false;
}

bool QTofDaqDll::tofDaqRunning() const
{
    if(m_TwTofDaqRunning){
        return m_TwTofDaqRunning();
    }
    return false;
}

void QTofDaqDll::setTimeout(int timeout) const
{
    if(m_TwSetTimeout){
        m_TwSetTimeout(timeout);
    }
}

int QTofDaqDll::getTimeout()
{
    if(m_TwGetTimeout){
        return m_TwGetTimeout();
    }
    return 0;
}

QString QTofDaqDll::getDaqParameter(const QString &para) const
{
    if(m_TwGetDaqParameter){
        QByteArray para_ba = para.toLocal8Bit();
        char* tmp = (m_TwGetDaqParameter(para_ba.data()));
        return QString(tmp);
    }
    return QString();
}

int QTofDaqDll::getDaqParameterInt(const QString &para) const
{
    if(m_TwGetDaqParameterInt){
        QByteArray para_ba = para.toLocal8Bit();
        return(m_TwGetDaqParameterInt(para_ba.data()));
    }
    return -1;
}

bool QTofDaqDll::getDaqParameterBool(const QString &para) const
{
    if(m_TwGetDaqParameterBool){
        QByteArray para_ba = para.toLocal8Bit();
        return(m_TwGetDaqParameterBool(para_ba.data()));
    }
    return false;
}

float QTofDaqDll::getDaqParameterFloat(const QString &para) const
{
    if(m_TwGetDaqParameterFloat){
        QByteArray para_ba = para.toLocal8Bit();
        return(m_TwGetDaqParameterFloat(para_ba.data()));
    }
    return -1;
}

qint64 QTofDaqDll::getDaqParameterInt64(const QString &para) const
{
    if(m_TwGetDaqParameterInt64){
        QByteArray para_ba = para.toLocal8Bit();
        return(m_TwGetDaqParameterInt64(para_ba.data()));
    }
    return -1;
}

double QTofDaqDll::getDaqParameterDouble(const QString &para) const
{
    if(m_TwGetDaqParameterDouble){
        QByteArray para_ba = para.toLocal8Bit();
        return(m_TwGetDaqParameterDouble(para_ba.data()));
    }
    return -1;
}

int QTofDaqDll::closeTofDaqRec() const
{
    if(m_TwCloseTofDaqRec){
        return(m_TwCloseTofDaqRec());
    }
    return -1;
}

int QTofDaqDll::setDaqParameter(const QString &para, QString value) const
{
    if(m_TwSetDaqParameter){
        QByteArray para_ba = para.toLocal8Bit();
        QByteArray value_ba = value.toLocal8Bit();
        return m_TwSetDaqParameter(para_ba.data(),value_ba.data());
    }
    return -1;
}

int QTofDaqDll::setDaqParameterInt(const QString &para, int value) const
{
    if(m_TwSetDaqParameterInt){
        QByteArray para_ba = para.toLocal8Bit();
        return (m_TwSetDaqParameterInt(para_ba.data(),value));
    }
    return -1;
}

int QTofDaqDll::setDaqParameterBool(const QString &para, bool value) const
{
    if(m_TwSetDaqParameterBool){
        QByteArray para_ba = para.toLocal8Bit();
        return (m_TwSetDaqParameterBool(para_ba.data(),value));
    }
    return -1;
}

int QTofDaqDll::setDaqParameterFloat(const QString &para, float value) const
{
    if(m_TwSetDaqParameterFloat){
        QByteArray para_ba = para.toLocal8Bit();
        return (m_TwSetDaqParameterFloat(para_ba.data(),value));
    }
    return -1;
}

int QTofDaqDll::setDaqParameterInt64(const QString &para, qint64 value) const
{
    if(m_TwSetDaqParameterInt64){
        QByteArray para_ba = para.toLocal8Bit();
        return (m_TwSetDaqParameterInt64(para_ba.data(),value));
    }
    return -1;
}

int QTofDaqDll::setDaqParameterDouble(const QString &para, double value) const
{
    if(m_TwSetDaqParameterDouble){
        QByteArray para_ba = para.toLocal8Bit();
        return (m_TwSetDaqParameterDouble(para_ba.data(),value));
    }
    return -1;
}

int QTofDaqDll::startAcquisition()
{
    if(m_TwStartAcquisition){
        auto ret = (m_TwStartAcquisition());
        //QThread::msleep(100);
        return ret;
    }
    return -1;
}

int QTofDaqDll::stopAcquisition()
{
    if(m_TwStopAcquisition){
        return (m_TwStopAcquisition());
    }
    return -1;
}

int QTofDaqDll::continueAcquisition()
{
    if(m_TwContinueAcquisition){
        return (m_TwContinueAcquisition());
    }
    return -1;
}

int QTofDaqDll::waitForNewData(int Timeout, TSharedMemoryDesc *pBufDesc, TSharedMemoryPointer *pShMem, bool WaitForEventReset)
{
    Q_UNUSED(pShMem);
    Q_UNUSED(WaitForEventReset);

    if(pBufDesc == Q_NULLPTR)
        return 5; // TwError

    auto ret = getDescriptor(pBufDesc);
    if(ret != 4)
        return ret;

    auto TotalBufsProcessed = pBufDesc->TotalBufsProcessed;
    //auto TotalBufsProcessed_new = TotalBufsProcessed;
    int32_t TotalBufsProcessed_new;
    auto dieTime= QTime::currentTime().addMSecs(Timeout);
    bool timout_occured = false;
    do{
        QThread::msleep(10);
        ret = getDescriptor(pBufDesc);
        if(ret != 4)
            return ret;
        TotalBufsProcessed_new = pBufDesc->TotalBufsProcessed;

        timout_occured = QTime::currentTime() > dieTime;
    }while((TotalBufsProcessed == TotalBufsProcessed_new) && (timout_occured == false));

    if(timout_occured)
        return 8; //TwTimeout
    else
        return 4; //TwSuccess
}

int QTofDaqDll::getDescriptor(TSharedMemoryDesc *pBufDesc)
{
    if(m_TwGetDescriptor){
        return (m_TwGetDescriptor(pBufDesc));
    }
    return -1;
}

int QTofDaqDll::getPeakParameters(TPeakPar *PeakPar, int PeakIndex)
{
    if(m_TwGetPeakParameters){
        return (m_TwGetPeakParameters(PeakPar, PeakIndex));
    }
    return -1;
}

int QTofDaqDll::getAllPeakParameters(QStringList &mass_labels, QVector<float> &masses, QVector<float> &masses_hi, QVector<float> &masses_lo)
{
    int NbrPeaks = 0;
    if(m_shared_memory_descriptor != Q_NULLPTR){
        getDescriptor(m_shared_memory_descriptor);
        NbrPeaks = m_shared_memory_descriptor->NbrPeaks;
    }
    if(NbrPeaks > 0){
        mass_labels.clear();
        masses.clear();
        masses_hi.clear();
        masses_lo.clear();

        int ret =0;
        TPeakPar *PeakPar = new TPeakPar();
        for(int i=0;i<NbrPeaks; i++){
            ret = getPeakParameters(PeakPar, i);
            if(ret==4){
                mass_labels.push_back(QString(PeakPar->label));
                masses.push_back(PeakPar->mass);
                masses_hi.push_back(PeakPar->hiMass);
                masses_lo.push_back(PeakPar->loMass);
            }else{
                mass_labels.clear();
                masses.clear();
                masses_hi.clear();
                masses_lo.clear();
                break;
            }
        }
        delete PeakPar;
        return ret;
    }
    return -1;
}

int QTofDaqDll::getMassCalib_unsave(int &mode, int &nbrParams, QVector<double>& p, int &nbrPoints, QVector<double>& mass, QVector<double>& tof, QVector<double>& weight){
    if(m_TwGetMassCalib){
        if(p.size()==0 && mass.size()==0 && tof.size()==0 && weight.size()==0){
            return m_TwGetMassCalib(&mode, &nbrParams, 0, &nbrPoints, 0, 0, 0);
        }else{
            return m_TwGetMassCalib(&mode, &nbrParams, p.data(), &nbrPoints, mass.data(), tof.data(), weight.data());
        }
    }
    return -1;
}


int QTofDaqDll::getMassCalib(int &mode, int &nbrParams, QVector<double>& p, int &nbrPoints, QVector<double>& mass, QVector<double>& tof, QVector<double>& weight)
{
    QVector<double> tmp;
    int ret = getMassCalib_unsave(mode, nbrParams, tmp, nbrPoints, tmp, tmp, tmp);
    if(((ret == 4) || (ret == 9)) && nbrParams > 0){
        p.resize(nbrParams);
        p.fill(0.0);

        mass.resize(nbrParams);
        mass.fill(0.0);

        tof.resize(nbrParams);
        tof.fill(0.0);

        weight.resize(nbrParams);
        weight.fill(0.0);

        ret = getMassCalib_unsave(mode, nbrParams, p, nbrPoints, mass, tof, weight);
    }
    return ret;

}
int QTofDaqDll::getSpecXaxisFromShMem_unsave(QVector<double>& SpecAxis, int Type, double maxMass)
{
    if(m_TwGetSpecXaxisFromShMem){
        return m_TwGetSpecXaxisFromShMem(SpecAxis.data(), Type, 0, maxMass);
    }
    return -1;
}

int QTofDaqDll::getSpecXaxisFromShMem(QVector<double>& SpecAxis, int Type, double maxMass)
{
    auto NbrSamples = getDaqParameterInt("NbrSamples");
    if(NbrSamples > 0){
        SpecAxis.resize(NbrSamples);
        SpecAxis.fill(0.0);
        return getSpecXaxisFromShMem_unsave(SpecAxis, Type, maxMass);
    }
    return -1;
}

int QTofDaqDll::getTofSpectrumFromShMem_unsave(QVector<float>& Spectrum, int SegmentIndex, int SegmentEndIndex, int BufIndex, bool Normalize){
    if(m_TwGetTofSpectrumFromShMem){
        return m_TwGetTofSpectrumFromShMem(Spectrum.data(), SegmentIndex, SegmentEndIndex, BufIndex, Normalize);
    }
    return -1;
}

//  int QTofDaqDll::GetTofSpectrumFromShMem_unsave(QVector<double> &Spectrum, int SegmentIndex, int SegmentEndIndex, int BufIndex, bool Normalize)
//  {
//    if(m_TwGetTofSpectrumFromShMem){
//        //QVector<float>tmp_Spectrum(Spectrum.size());
//        GetTofSpectrumFromShMem_unsave(tmp_Spectrum, SegmentIndex, SegmentEndIndex, BufIndex, Normalize);

//      }
//    return -1;
//  }

int QTofDaqDll::getTofSpectrumFromShMem(QVector<float>& Spectrum, int SegmentIndex, int SegmentEndIndex, int BufIndex, bool Normalize)
{
    auto NbrSamples = getDaqParameterInt("NbrSamples");
    if(NbrSamples > 0){
        Spectrum.resize(NbrSamples);
        Spectrum.fill(0.0);
        return getTofSpectrumFromShMem_unsave(Spectrum, SegmentIndex, SegmentEndIndex, BufIndex, Normalize);
    }
    return -1;
}

int QTofDaqDll::getTofSpectrumFromShMem(QVector<float> &Spectrum, int BufIndex)
{
    return getTofSpectrumFromShMem(Spectrum, -1, -1, BufIndex, false);
}

int QTofDaqDll::getBufTimeFromShMem(double &BufTime, int BufIndex, int WriteIndex)
{
    if(m_TwGetBufTimeFromShMem){
        return m_TwGetBufTimeFromShMem(&BufTime, BufIndex, WriteIndex);
    }
    return -1;
}

int QTofDaqDll::getStickSpectrumFromShMem_unsave(QVector<float>& Spectrum, QVector<float>&Masses, int SegmentIndex, int SegmentEndIndex, int BufIndex)
{
    if(m_TwGetStickSpectrumFromShMem){
        return m_TwGetStickSpectrumFromShMem(Spectrum.data(), Masses.data(), SegmentIndex, SegmentEndIndex, BufIndex);
    }
    return -1;
}

int QTofDaqDll::getStickSpectrumFromShMem(QVector<float>& Spectrum, QVector<float>&Masses, int SegmentIndex, int SegmentEndIndex, int BufIndex)
{
    int NbrPeaks = 0;
    if(m_shared_memory_descriptor != Q_NULLPTR){
        getDescriptor(m_shared_memory_descriptor);
        NbrPeaks = m_shared_memory_descriptor->NbrPeaks;
    }
    if(NbrPeaks > 0){
        Spectrum.resize(NbrPeaks);
        Spectrum.fill(0.0);

        Masses.resize(NbrPeaks);
        Masses.fill(0.0);

        return getStickSpectrumFromShMem_unsave(Spectrum, Masses, SegmentIndex, SegmentEndIndex, BufIndex);
    }
    return -1;
}

int QTofDaqDll::queryNumberOfDataSources(int &arrayLength)
{
    if(m_TwGetRegUserDataSources){
        return m_TwGetRegUserDataSources(&arrayLength, NULL, NULL, NULL);
    }
    return -1;
}

int QTofDaqDll::getRegUserDataSources(QStringList &location, QVector<int> &nbrElements, QVector<int> &type){
    int arrayLength = 0;
    return getRegUserDataSources(arrayLength, location, nbrElements, type);
}


int QTofDaqDll::getRegUserDataSources(int &arrayLength, QStringList &location, QVector<int> &nbrElements, QVector<int> &type)
{
    int ret = -1;
    if(m_TwGetRegUserDataSources){
        if(arrayLength == 0){
            ret = m_TwGetRegUserDataSources(&arrayLength, NULL, NULL, NULL);
        }
        if(ret != 9){
            return ret;
        }
        //location.resize(arrayLength);
        char* location_raw = new char[arrayLength*256];
        memset(location_raw, 0, arrayLength*256);
        nbrElements.resize(arrayLength);
        type.resize(arrayLength);
        ret = m_TwGetRegUserDataSources(&arrayLength, location_raw, nbrElements.data(), type.data());

        int j =0;
        location.clear();
        for(int i = 0; i < arrayLength; i++){
            const char * tmp = location_raw+(i*256);
            QString str(tmp);
            location << str.trimmed();
            j+=256;
        }

        delete[] location_raw;
        return ret;
    }
    return ret;
}

int QTofDaqDll::registerUserDataBuf(QString Location, int NbrElements, int CompressionLevel)
{
    int ret= -1;
    if(m_TwRegisterUserDataBuf ){
        auto LocationBa   = Location.toLocal8Bit();
        ret = m_TwRegisterUserDataBuf(LocationBa.data(), NbrElements, 0, CompressionLevel);
    }
    return ret;
}

int QTofDaqDll::getRegUserDataDesc(QString location, int NbrElements, QStringList &elementDescription)
{
    int ret= -1;
    if(m_TwGetRegUserDataDesc ){
        auto LocationBa   = location.toLocal8Bit();

        char* elementDescriptionRaw = new char[NbrElements*256];
        memset(elementDescriptionRaw, 0, NbrElements*256);

        ret = m_TwGetRegUserDataDesc(LocationBa.data(), &NbrElements, elementDescriptionRaw);

        elementDescription.clear();
        for(int i=0; i<NbrElements; i++){
            const char* tmp = elementDescriptionRaw+(i*256);
            QString str(tmp);
            elementDescription<<str.trimmed();
        }

        delete[] elementDescriptionRaw;
        return ret;
    }
    return ret;
}

int QTofDaqDll::getRegUserDataDesc(QString location, QStringList &elementDescription)
{
    int ret= -1;
    if(m_TwGetRegUserDataDesc ){
        auto LocationBa   = location.toLocal8Bit();
        int nbrElements = 0;
        ret = m_TwGetRegUserDataDesc(LocationBa.data(), &nbrElements , NULL);
        if(ret != 9){
            return ret;
        }

        ret = getRegUserDataDesc(LocationBa.data(), nbrElements, elementDescription);
        //        char* elementDescriptionRaw = new char[nbrElements*256];
        //        memset(elementDescriptionRaw, 0, nbrElements*256);

        //        ret = m_TwGetRegUserDataDesc(LocationBa.data(), &nbrElements, elementDescriptionRaw);

        //        elementDescription.clear();
        //        for(int i=0; i<nbrElements; i++){
        //            const char* tmp = elementDescriptionRaw+(i*256);
        //            QString str(tmp);
        //            elementDescription<<str.trimmed();
        //          }

        //        delete[] elementDescriptionRaw;
        return ret;
    }
    return ret;
}

int QTofDaqDll::readRegUserData(QString location, QVector<double> &Data)
{
    int ret= -1;
    if(m_TwReadRegUserData ){
        auto LocationBa   = location.toLocal8Bit();
        int NbrElements   = Data.size();
        ret = m_TwReadRegUserData(LocationBa.data(), NbrElements, Data.data());

    }
    return ret;
}

int QTofDaqDll::unregisterUserData(QString location)
{
    int ret= -1;
    if(m_TwUnregisterUserData ){
        auto LocationBa   = location.toLocal8Bit();
        ret = m_TwUnregisterUserData(LocationBa.data());
        //give it time to really remove it
        QThread::currentThread()->msleep(5);
    }
    return ret;
}

int QTofDaqDll::updateUserData(QString location, QVector<double> &Data)
{
    int ret= -1;
    if(m_TwUpdateUserData ){
        auto LocationBa   = location.toLocal8Bit();
        int NbrElements   = Data.size();
        ret = m_TwUpdateUserData(LocationBa.data(), NbrElements, Data.data());
    }
    return ret;
}

int QTofDaqDll::updateUserData(UserDataSource source)
{
    auto data = source.data();
    return updateUserData(source.location(), data);
}

int QTofDaqDll::unregisterAllUserData()
{
    QStringList locations;
    QVector<int> nbrElements;
    QVector<int> type;
    auto ret = getRegUserDataSources(locations, nbrElements, type);
    if(((ret != 4) && (ret != 9)))
        return ret;
    foreach (auto location, locations) {
        //qDebug()<<Q_FUNC_INFO<<"unregistering"<<location;
        auto tret = unregisterUserData(location);
        if(tret != 4)
            ret = tret;
    }
    return ret;
}

int QTofDaqDll::readRegUserData(UserDataSourceList &usrDtaSrc)
{
    QStringList locations;
    QVector<int> nbrElements;
    QVector<int> type;
    auto ret = getRegUserDataSources(locations, nbrElements, type);
    if(((ret != 4) && (ret != 9)))
        return ret;

    QStringList elementDescription;
    int nbrElems = 0;
    QVector<double> data;
    int i= 0;
    //auto tmpret = ret;
    usrDtaSrc.clear();
    foreach (auto location, locations) {
        nbrElems = nbrElements.at(i);

        if(nbrElems == 0 )
            continue;

        data.clear();
        data.resize(nbrElems);
        auto tmpret = readRegUserData(location, data);
        if(tmpret != 4)
            continue;

        elementDescription.clear();
        tmpret = getRegUserDataDesc(location, nbrElems, elementDescription);
        if(tmpret != 4)
            elementDescription.clear();

        //qDebug() << location << elementDescription << data;
        usrDtaSrc << UserDataSource(location, elementDescription, data);

        i++;
    }

    return ret;
}

int QTofDaqDll::getSumSpectrumFromShMem(QVector<double> &Spectrum, bool Normalize)
{
    int ret= -1;
    if(m_TwGetSumSpectrumFromShMem ){
        ret = m_TwGetSumSpectrumFromShMem(Spectrum.data(), Normalize);
    }
    return ret;
}

int QTofDaqDll::registerUserDataBuf(QString Location, QStringList ElementDescription, int CompressionLevel)
{
    int ret= -1;
    if(m_TwRegisterUserDataBuf ){
        int NbrElements   = ElementDescription.size();
        char* ElementDescriptionRaw = 0;
        if(NbrElements > 0){
            QByteArray descBa;
            ElementDescriptionRaw = new char[NbrElements*256];
            memset(ElementDescriptionRaw, 0, NbrElements*256);
            int i = 0;
            foreach (auto desc, ElementDescription) {
                descBa=desc.toLocal8Bit();
                memcpy(ElementDescriptionRaw+(i*256), descBa.data(), descBa.size());
                i++;
            }
        }

        auto LocationBa   = Location.toLocal8Bit();
        ret = m_TwRegisterUserDataBuf(LocationBa.data(), NbrElements, ElementDescriptionRaw, CompressionLevel);

        if(ElementDescriptionRaw != 0)
            delete[] ElementDescriptionRaw;
    }
    return ret;
}

int QTofDaqDll::registerUserDataBuf(UserDataSource source, int CompressionLevel)
{
    return registerUserDataBuf(source.location(), source.descriptions(), CompressionLevel);
}



bool QTofDaqDll::isInitialized() const
{
    auto dll_isnull   = m_data_aquisition_dll == Q_NULLPTR;
    if(dll_isnull)
        return false;

    auto isempty      = m_dll_name.isEmpty();
    auto isnull       = m_dll_name.isNull();

    auto dll_isloaded = m_data_aquisition_dll->isLoaded();

    return !isempty && !isnull && !dll_isnull && dll_isloaded;
}

QString QTofDaqDll::getTwErrorDescription(int error) const
{
    QString description = "";
    switch(error)
    {
    case 0:{description = "TwDaqRecNotRunning";break;}
    case 1:{description = "TwAcquisitionActive";break;}
    case 2:{description = "TwNoActiveAcquisition";break;}
    case 3:{description = "TwFileNotFound";break;}
    case 4:{description = "TwSuccess";break;}
    case 5:{description = "TwError";break;}
    case 6:{description = "TwOutOfBounds";break;}
    case 7:{description = "TwNoData";break;}
    case 8:{description = "TwTimeout";break;}
    case 9:{description = "TwValueAdjusted";break;}
    case 10:{description = "TwInvalidParameter";break;}
    case 11:{description = "TwInvalidValue";break;}
    case 12:{description = "TwAborted";break;}
    default:{description = "UNKNOWN ERROR ["+QString::number(error)+"]";break;}
    }
    return description;
}


}

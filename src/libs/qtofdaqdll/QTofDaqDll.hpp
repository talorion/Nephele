#ifndef QTOFDAQDLL_HPP
#define QTOFDAQDLL_HPP

#include "qtofdaqdll_global.hpp"
#include <QObject>
#include <QDebug>
#include <QLibrary>
#include <QPointer>
#include <type_traits>

#if defined( Q_OS_WIN )
#include <Windows.h>
#endif

struct _TSharedMemoryDesc;
typedef _TSharedMemoryDesc TSharedMemoryDesc;

struct _TPeakPar;
typedef _TPeakPar TPeakPar;

namespace talorion {

  using TwCleanupDllPtr                 = std::add_pointer<void()>::type;
  using TwGetDllVersionPtr              = std::add_pointer<double()>::type;
  using TwDaqActivePtr                  = std::add_pointer<bool()>::type;
  using TwGetTimeoutPtr                 = std::add_pointer<int()>::type;
  using TwSetTimeoutPtr                 = std::add_pointer<void(int)>::type;
  using TwTofDaqRunningPtr              = std::add_pointer<bool()>::type;

  using TwGetDaqParameterPtr            = std::add_pointer<char*(char*)>::type;
  using TwGetDaqParameterIntPtr         = std::add_pointer<int(char*)>::type;
  using TwGetDaqParameterBoolPtr        = std::add_pointer<bool(char*)>::type;
  using TwGetDaqParameterFloatPtr       = std::add_pointer<float(char*)>::type;
  using TwGetDaqParameterInt64Ptr       = std::add_pointer<qint64(char*)>::type;
  using TwGetDaqParameterDoublePtr      = std::add_pointer<double(char*)>::type;

  using TwSetDaqParameterPtr            = std::add_pointer<qint32(char*, char*)>::type;
  using TwSetDaqParameterIntPtr         = std::add_pointer<qint32(char*, int)>::type;
  using TwSetDaqParameterBoolPtr        = std::add_pointer<qint32(char*, bool)>::type;
  using TwSetDaqParameterFloatPtr       = std::add_pointer<qint32(char*, float)>::type;
  using TwSetDaqParameterInt64Ptr       = std::add_pointer<qint32(char*, qint64)>::type;
  using TwSetDaqParameterDoublePtr      = std::add_pointer<qint32(char*, double)>::type;

  using TwStartAcquisitionPtr           = std::add_pointer<qint32()>::type;
  using TwStopAcquisitionPtr            = std::add_pointer<qint32()>::type;
  using TwContinueAcquisitionPtr        = std::add_pointer<qint32()>::type;

  using TwGetDescriptorPtr              = std::add_pointer<int(TSharedMemoryDesc*)>::type;
  using TwGetPeakParametersPtr          = std::add_pointer<int(TPeakPar*, int)>::type;
  using TwGetMassCalibPtr               = std::add_pointer<int(int* , int* , double* , int* , double* , double* , double* )>::type;
  using TwGetSpecXaxisFromShMemPtr      = std::add_pointer<int(double* , int , char* , double )>::type;
  using TwGetTofSpectrumFromShMemPtr    = std::add_pointer<int(float* , int , int , int , bool )>::type;
  using TwGetBufTimeFromShMemPtr        = std::add_pointer<int(double* , int , int )>::type;
  using TwGetStickSpectrumFromShMemPtr  = std::add_pointer<int(float* , float* , int , int , int )>::type;

  class QTOFDAQDLLSHARED_EXPORT QTofDaqDll: public QObject
  {
    Q_OBJECT
  public:
    explicit QTofDaqDll(QObject *par = Q_NULLPTR);
    virtual ~QTofDaqDll();

  private:
    Q_DISABLE_COPY(QTofDaqDll)

  public:
    int init(QString dll_name = "C:\\Tofwerk\\TofDaq\\TofDaqDll.dll", int timeout=5000);
    void dispose();

    void setDllName(QString dll_name);

    int twErrCheck(const int error) const;

    Qt::HANDLE threadId() const;

  public slots:
    void cleanupDll(void);

    double getDllVersion()const;
    bool tofDaqRunning()const;
    bool daqActive()const;

    void setTimeout(int timeout=500)const;
    int getTimeout(void);

    QString getDaqParameter(const QString& para)const;
    int getDaqParameterInt(const QString& para)const;
    bool getDaqParameterBool(const QString& para)const;
    float getDaqParameterFloat(const QString& para)const;
    qint64 getDaqParameterInt64(const QString& para)const;
    double getDaqParameterDouble(const QString& para)const;

    int setDaqParameter(const QString& para, QString value)const;
    int setDaqParameterInt(const QString& para, int value)const;
    int setDaqParameterBool(const QString& para, bool value)const;
    int setDaqParameterFloat(const QString& para, float value)const;
    int setDaqParameterInt64(const QString& para, qint64 value)const;
    int setDaqParameterDouble(const QString& para, double value)const;

    int startAcquisition(void);
    int stopAcquisition(void);
    int continueAcquisition(void);

    int getDescriptor(TSharedMemoryDesc* pBufDesc);
    int getPeakParameters(TPeakPar* PeakPar, int PeakIndex);
    int getAllPeakParameters(QStringList &mass_labels, QVector<float> &masses, QVector<float> &masses_hi, QVector<float> &masses_lo);
    int getMassCalib_unsave(int &mode, int &nbrParams, QVector<double> &p, int &nbrPoints, QVector<double> &mass, QVector<double> &tof, QVector<double> &weight);
    int getMassCalib(int &mode, int &nbrParams, QVector<double>& p, int &nbrPoints, QVector<double>& mass, QVector<double>& tof, QVector<double>& weight);
    int getSpecXaxisFromShMem_unsave(QVector<double> &SpecAxis, int Type, double maxMass);
    int getSpecXaxisFromShMem(QVector<double> &SpecAxis, int Type, double maxMass=0.0);
    int getTofSpectrumFromShMem_unsave(QVector<float> &Spectrum, int SegmentIndex, int SegmentEndIndex, int BufIndex, bool Normalize);
    //int GetTofSpectrumFromShMem_unsave(QVector<double> &Spectrum, int SegmentIndex, int SegmentEndIndex, int BufIndex, bool Normalize);
    int getTofSpectrumFromShMem(QVector<float>& Spectrum, int SegmentIndex, int SegmentEndIndex, int BufIndex, bool Normalize=false);
    int getTofSpectrumFromShMem(QVector<float>& Spectrum, int BufIndex);
    int getBufTimeFromShMem(double &BufTime, int BufIndex, int WriteIndex);
    int getStickSpectrumFromShMem_unsave(QVector<float> &Spectrum, QVector<float> &Masses, int SegmentIndex, int SegmentEndIndex, int BufIndex);
    int getStickSpectrumFromShMem(QVector<float> &Spectrum, QVector<float> &Masses, int SegmentIndex, int SegmentEndIndex, int BufIndex);

  public:
    bool isInitialized()const;
    QString getTwErrorDescription(int error)const;

  private:
    template<typename T>
    T resolveMethod(const QString& method_name);

  private:
    QString m_absolutePath;
    QString m_dll_name;
    QPointer<QLibrary> m_data_aquisition_dll;
#if defined( Q_OS_WIN )
    DLL_DIRECTORY_COOKIE m_cookie;
#endif

  private:
    TwCleanupDllPtr     m_TwCleanupDll;

    TwGetDllVersionPtr  m_TwGetDllVersion;
    TwDaqActivePtr      m_TwDaqActive;
    TwGetTimeoutPtr     m_TwGetTimeout;
    TwSetTimeoutPtr     m_TwSetTimeout;
    TwTofDaqRunningPtr  m_TwTofDaqRunning;

    TwGetDaqParameterPtr  m_TwGetDaqParameter;
    TwGetDaqParameterIntPtr  m_TwGetDaqParameterInt;
    TwGetDaqParameterBoolPtr  m_TwGetDaqParameterBool;
    TwGetDaqParameterFloatPtr  m_TwGetDaqParameterFloat;
    TwGetDaqParameterInt64Ptr  m_TwGetDaqParameterInt64;
    TwGetDaqParameterDoublePtr  m_TwGetDaqParameterDouble;

    TwSetDaqParameterPtr  m_TwSetDaqParameter;
    TwSetDaqParameterIntPtr  m_TwSetDaqParameterInt;
    TwSetDaqParameterBoolPtr  m_TwSetDaqParameterBool;
    TwSetDaqParameterFloatPtr  m_TwSetDaqParameterFloat;
    TwSetDaqParameterInt64Ptr  m_TwSetDaqParameterInt64;
    TwSetDaqParameterDoublePtr  m_TwSetDaqParameterDouble;

    TwStartAcquisitionPtr m_TwStartAcquisition;
    TwStopAcquisitionPtr m_TwStopAcquisition;
    TwContinueAcquisitionPtr m_TwContinueAcquisition;

    TwGetDescriptorPtr m_TwGetDescriptor;
    TwGetPeakParametersPtr m_TwGetPeakParameters;
    TwGetMassCalibPtr m_TwGetMassCalib;
    TwGetSpecXaxisFromShMemPtr m_TwGetSpecXaxisFromShMem;
    TwGetTofSpectrumFromShMemPtr m_TwGetTofSpectrumFromShMem;
    TwGetBufTimeFromShMemPtr m_TwGetBufTimeFromShMem;
    TwGetStickSpectrumFromShMemPtr m_TwGetStickSpectrumFromShMem;

  private:
    TSharedMemoryDesc* m_shared_memory_descriptor;
  };

  template<typename T>
  T QTofDaqDll::resolveMethod(const QString& method_name)
  {
    if(m_data_aquisition_dll.isNull())
      return Q_NULLPTR;

    if(!m_data_aquisition_dll->isLoaded())
      return Q_NULLPTR;

    if(method_name.trimmed().isEmpty())
      return Q_NULLPTR;

    QString decorated_method_name = "_"+method_name;

    T tmp = (T) m_data_aquisition_dll->resolve(decorated_method_name.toLocal8Bit());
    //T tmp = reinterpret_cast<T>( m_data_aquisition_dll->resolve(decorated_method_name.toLocal8Bit()));
    if (!tmp){
        qDebug()<<"Could not resolve "+method_name<<m_data_aquisition_dll->errorString();
        return Q_NULLPTR;
      }
    return tmp;
  }

}

#endif // QTOFDAQDLL_HPP

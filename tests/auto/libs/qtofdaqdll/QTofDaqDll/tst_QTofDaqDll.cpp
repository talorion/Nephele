#include <QString>
#include <QtTest>
#include <QTime>

#include <QCoreApplication>
//#include <QTofDaqDll.hpp>

#include<QTofDaqDll.hpp>

#if defined( Q_OS_WIN )
#include <WTypes.h>
#else
#define __int64 long long
#define __stdcall
#define __declspec( dllexport )
#endif
#include "TofDaqDll.h"

//#ifdef Q_PROCESSOR_X86_64
#if defined( Q_OS_WIN )
#define tofdaqdll "C:\\Tofwerk\\TofDaq\\TofDaqDll.dll"
#define tofdaqrec "C:\\Tofwerk\\TofDaq\\TofDaqRec.exe"
//#define tofdaqrec_arg "-PB:C:\\Tofwerk\\Data\\templates\\Data_2016.10.14_15h03m49s.h5"
#define tofdaqrec_arg "-PB:C:\\Tofwerk\\Data\\templates\\testfile.h5"
#define tofdaqdll_alt "C:\\Tofwerk\\TofDaq_1.98\\TofDaqDll.dll"
#endif

//#else
//#define tofdaqdll "C:\\Tofwerk\\TofDaq_1.99_x86\\TofDaqDll.dll"
//#define tofdaqdll_alt "C:\\Tofwerk\\TofDaq_1.98_x86\\TofDaqDll.dll"
//#endif

using namespace talorion;

class tst_QTofDaqDll : public QObject
{
  Q_OBJECT
public:
  tst_QTofDaqDll();
  virtual ~tst_QTofDaqDll();

private Q_SLOTS:

#if defined( Q_OS_WIN )
  void initTestCase(); //will be called before the first test function is executed.
  void cleanupTestCase(); //will be called after the last test function was executed.

  void init(); //will be called before each test function is executed.
  void cleanup(); //will be called after every test function.

  void qtofdaqdllSupportsTwgetdllversion();
  void qtofdaqdllSupportsTwdaqactive();
  void qtofdaqdllSupportsTwgettimeout();
  void qtofdaqdllSupportsTwsettimeout();
  void qtofdaqdllSupportsTwtofdaqrunning();
  void qtofdaqdllSupportsTwgetdaqparameter();
  void qtofdaqdllSupportsTwgetdaqparameterint();
  void qtofdaqdllSupportsTwgetdaqparameterbool();
  void qtofdaqdllSupportsTwgetdaqparameterfloat();
  void qtofdaqdllSupportsTwgetdaqparameterint64();
  void qtofdaqdllSupportsTwgetdaqparameterdouble();
  void qtofdaqdllSupportsTwsetdaqparameter();
  void qtofdaqdllSupportsTwsetdaqparameterint();
  void qtofdaqdllSupportsTwsetdaqparameterbool();
  void qtofdaqdllSupportsTwsetdaqparameterfloat();
  void qtofdaqdllSupportsTwsetdaqparameterint64();
  void qtofdaqdllSupportsTwsetdaqparameterdouble();
  void qtofdaqdllSupportsTwgetdescriptor();
  void qtofdaqdllSupportsTwstartacquisition();
  void qtofdaqdllSupportsTwWaitForNewData();
  void qtofdaqdllSupportsTwstopacquisition();
  void qtofdaqdllSupportsTwcontinueacquisition();
  void qtofdaqdllSupportsTwgetpeakparameters();
  void qtofdaqdllSupportsTwgetallpeakparameters();
  void qtofdaqdllSupportsTwgetmasscalib();
  void qtofdaqdllSupportsTwgetspecxaxisfromshmemSampleindex();
  void qtofdaqdllSupportsTwgetspecxaxisfromshmemMass();
  void qtofdaqdllSupportsTwgetspecxaxisfromshmemTof();
  void qtofdaqdllSupportsTwgettofspectrumfromshmem();
  void qtofdaqdllSupportsTwgetbuftimefromshmem();
  void qtofdaqdllSupportsTwgetstickspectrumfromshmem();
  void qtofdaqdllSupportsTwRegisterUserDataBuf();
  void qtofdaqdllSupportsTwRegisterUserDataBufWithoutElementDescription();
  void qtofdaqdllSupportsQueryNumberOfDataSources();
  void qtofdaqdllSupportsTwGetRegUserDataSources();
  void qtofdaqdllSupportsTwGetRegUserDataDesc();
  void qtofdaqdllSupportsTwReadRegUserData();
  void qtofdaqdllSupportsTwUnregisterUserData();
  void qtofdaqdllSupportsTwUpdateUserData();

  void qtofdaqdllTwgetmasscalibResizesTheInputVectors();

  void registeredUserDataCanBeQueried();
  void registeredUserDescCanBeQueried();
  void registeredUserDataSupportsSubgroups();
  void registeredUserDataCanBeCalledTwice();
  void registeredUserDataLocationsCanBeQueried();
  void registeredUserDataSourcesCanBeQueried();

#endif

private:
  QProcess* m_tofdaqrecexe;
  QTofDaqDll daqdll;

};

tst_QTofDaqDll::tst_QTofDaqDll():
  QObject(),
  m_tofdaqrecexe(Q_NULLPTR)
{
  qsrand((uint)QTime::currentTime().msec());
}

tst_QTofDaqDll::~tst_QTofDaqDll()
{
  if(m_tofdaqrecexe != Q_NULLPTR){
      m_tofdaqrecexe->terminate();
      delete m_tofdaqrecexe;
    }
}

#if defined( Q_OS_WIN )
void tst_QTofDaqDll::initTestCase()
{
  //QTofDaqDll daqdll;
  daqdll.init(tofdaqdll);
  if(daqdll.tofDaqRunning() == false){
      m_tofdaqrecexe = new QProcess();
      //m_tofdaqrecexe->setNativeArguments(tofdaqrec_arg);
      m_tofdaqrecexe->setArguments(QStringList(tofdaqrec_arg));
      m_tofdaqrecexe->start(tofdaqrec);
    }
  QTRY_VERIFY_WITH_TIMEOUT(daqdll.tofDaqRunning(), 2000);
}

void tst_QTofDaqDll::cleanupTestCase()
{
  //QTofDaqDll daqdll;
  //daqdll.init(tofdaqdll);
  if(daqdll.tofDaqRunning() == true){
      daqdll.closeTofDaqRec();
    }

  QTRY_VERIFY_WITH_TIMEOUT(daqdll.tofDaqRunning() == false, 2000);
}

void tst_QTofDaqDll::init()
{
  QVERIFY(daqdll.isInitialized());

  //this is a bug in tofdaq for some reason NbrSamples is set to 1 when using playback
  daqdll.setDaqParameterInt("NbrSamples", 124496);
}

void tst_QTofDaqDll::cleanup()
{
  daqdll.unregisterAllUserData();
  QStringList locations;
  QVector<int> nbrElements;
  QVector<int> type;
  daqdll.getRegUserDataSources(locations, nbrElements, type);
  QCOMPARE(locations.size(), 0);

  QVERIFY(daqdll.isInitialized());
}

void tst_QTofDaqDll::qtofdaqdllSupportsTwgetdllversion()
{
  //QTofDaqDll daqdll;
  //daqdll.init(tofdaqdll);
  auto version = daqdll.getDllVersion();
  QVERIFY(version>0);
}

void tst_QTofDaqDll::qtofdaqdllSupportsTwdaqactive()
{
  //QTofDaqDll daqdll;
  //daqdll.init(tofdaqdll);
  auto active = daqdll.daqActive();
  QVERIFY(active == false);
}

void tst_QTofDaqDll::qtofdaqdllSupportsTwgettimeout()
{
  //QTofDaqDll daqdll;
  //daqdll.init(tofdaqdll);
  auto timeout = daqdll.getTimeout();
  QVERIFY(timeout >0 );
}

void tst_QTofDaqDll::qtofdaqdllSupportsTwsettimeout()
{
  //QTofDaqDll daqdll;
  //daqdll.init(tofdaqdll);
  auto timeout = daqdll.getTimeout();
  daqdll.setTimeout(timeout+1);
  auto timeout_after = daqdll.getTimeout();
  QVERIFY(timeout != timeout_after);
}

void tst_QTofDaqDll::qtofdaqdllSupportsTwtofdaqrunning()
{
  //QTofDaqDll daqdll;
  //daqdll.init(tofdaqdll);
  auto running = daqdll.tofDaqRunning();
  QCOMPARE(running , true);
}

void tst_QTofDaqDll::qtofdaqdllSupportsTwgetdaqparameter()
{
  //QTofDaqDll daqdll;
  //daqdll.init(tofdaqdll);
  auto DaqName = daqdll.getDaqParameter("DaqName");
  QCOMPARE(DaqName ,QString(""));
}

void tst_QTofDaqDll::qtofdaqdllSupportsTwgetdaqparameterint()
{
  //QTofDaqDll daqdll;
  //daqdll.init(tofdaqdll);
  auto DaqMode = daqdll.getDaqParameterInt("DaqMode");
  QCOMPARE(DaqMode , 0);
}

void tst_QTofDaqDll::qtofdaqdllSupportsTwgetdaqparameterbool()
{
  //QTofDaqDll daqdll;
  //daqdll.init(tofdaqdll);
  auto NegativeIonMode = daqdll.getDaqParameterBool("NegativeIonMode");
  QCOMPARE(NegativeIonMode , false);
}

void tst_QTofDaqDll::qtofdaqdllSupportsTwgetdaqparameterfloat()
{
  //QTofDaqDll daqdll;
  //daqdll.init(tofdaqdll);
  //double TofDaqVersion_first = daqdll.TwGetDllVersion();
  auto TofDaqVersion_second = daqdll.getDaqParameterFloat("TofDaqVersion");
  QVERIFY(TofDaqVersion_second > 0.0f);
}

void tst_QTofDaqDll::qtofdaqdllSupportsTwgetdaqparameterint64()
{
  //QTofDaqDll daqdll;
  //daqdll.init(tofdaqdll);
  auto BlockPulseWidth = daqdll.getDaqParameterInt64("BlockPulseWidth");
  QVERIFY(BlockPulseWidth >= 100);// 	width of block pulse in ns. Min: 100 ns, Max: BlockPeriod-BlockPulseDelay
}

void tst_QTofDaqDll::qtofdaqdllSupportsTwgetdaqparameterdouble()
{
  //QTofDaqDll daqdll;
  //daqdll.init(tofdaqdll);
  auto DioStartDelay = daqdll.getDaqParameterDouble("DioStartDelay");
  QCOMPARE(DioStartDelay , 0.0);
}

void tst_QTofDaqDll::qtofdaqdllSupportsTwsetdaqparameter()
{
  //QTofDaqDll daqdll;
  //daqdll.init(tofdaqdll);
  auto DataFileName_bevore = daqdll.getDaqParameter("DataFileName");
  auto ret = daqdll.setDaqParameter("DataFileName", QDateTime::currentDateTime().toString("yyyy.MM.dd-hh.mm.ss.zzz")+".h5");
  auto DataFileName_after = daqdll.getDaqParameter("DataFileName");
  daqdll.setDaqParameter("DataFileName", DataFileName_bevore);
  QVERIFY(ret == 4 && DataFileName_bevore != DataFileName_after);
}

void tst_QTofDaqDll::qtofdaqdllSupportsTwsetdaqparameterint()
{
  //QTofDaqDll daqdll;
  //daqdll.init(tofdaqdll);
  auto NbrWaveforms_bevore = daqdll.getDaqParameterInt("NbrWaveforms");
  auto ret = daqdll.setDaqParameterInt("NbrWaveforms", NbrWaveforms_bevore+1);
  auto NbrWaveforms_after = daqdll.getDaqParameterInt("NbrWaveforms");
  daqdll.setDaqParameterInt("NbrWaveforms", NbrWaveforms_bevore);
  QVERIFY(ret == 4 && NbrWaveforms_bevore != NbrWaveforms_after);
}

void tst_QTofDaqDll::qtofdaqdllSupportsTwsetdaqparameterbool()
{
  //QTofDaqDll daqdll;
  //daqdll.init(tofdaqdll);
  auto DioActiveInvert_bevore = daqdll.getDaqParameterBool("DioActiveInvert");
  auto ret = daqdll.setDaqParameterBool("DioActiveInvert", !DioActiveInvert_bevore);
  auto DioActiveInvert_after = daqdll.getDaqParameterBool("DioActiveInvert");
  daqdll.setDaqParameterBool("DioActiveInvert", DioActiveInvert_bevore);
  QVERIFY(ret == 4 && DioActiveInvert_bevore != DioActiveInvert_after);
}

void tst_QTofDaqDll::qtofdaqdllSupportsTwsetdaqparameterfloat()
{
  //QTofDaqDll daqdll;
  //daqdll.init(tofdaqdll);
  auto SampleInterval_bevore = daqdll.getDaqParameterFloat("SampleInterval");
  auto ret = daqdll.setDaqParameterFloat("SampleInterval", SampleInterval_bevore+1);
  auto SampleInterval_after = daqdll.getDaqParameterFloat("DioActiveInvert");
  daqdll.setDaqParameterFloat("SampleInterval", SampleInterval_bevore);
  QCOMPARE(ret, 4);
  QVERIFY(SampleInterval_bevore != SampleInterval_after);
}

void tst_QTofDaqDll::qtofdaqdllSupportsTwsetdaqparameterint64()
{
  //QTofDaqDll daqdll;
  //daqdll.init(tofdaqdll);
  auto StartDelay_bevore = daqdll.getDaqParameterInt64("StartDelay");
  auto ret = daqdll.setDaqParameterInt64("StartDelay", StartDelay_bevore+1);
  auto StartDelay_after = daqdll.getDaqParameterInt64("StartDelay");
  daqdll.setDaqParameterInt64("StartDelay", StartDelay_bevore);
  QVERIFY(ret == 4 && StartDelay_bevore != StartDelay_after);
}

void tst_QTofDaqDll::qtofdaqdllSupportsTwsetdaqparameterdouble()
{
  //QTofDaqDll daqdll;
  //daqdll.init(tofdaqdll);
  auto DioStartDelay_bevore = daqdll.getDaqParameterDouble("DioStartDelay");
  auto ret = daqdll.setDaqParameterDouble("DioStartDelay", DioStartDelay_bevore+1);
  auto StartDelay_after = daqdll.getDaqParameterDouble("DioStartDelay");
  daqdll.setDaqParameterDouble("DioStartDelay", DioStartDelay_bevore);
  QVERIFY(ret == 4 && DioStartDelay_bevore != StartDelay_after);
}

void tst_QTofDaqDll::qtofdaqdllSupportsTwgetdescriptor()
{
  //QTofDaqDll daqdll;
  //daqdll.init(tofdaqdll);
  TSharedMemoryDesc* pBufDesc = new TSharedMemoryDesc();
  auto ret = daqdll.getDescriptor(pBufDesc);
  QVERIFY(((ret == 4) || (ret == 9)));
}

void tst_QTofDaqDll::qtofdaqdllSupportsTwstartacquisition()
{
  //disable file write
  daqdll.setDaqParameterBool("AutoSaveCombined", false);
  daqdll.setDaqParameterBool("AutoSaveEvents", false);
  daqdll.setDaqParameterBool("AutoSavePeaks", false);
  //PlaybackMassTable.txt
  //

  auto ret = daqdll.startAcquisition();
  auto aqu_active = daqdll.daqActive();
  QCOMPARE(ret, 4);
  QCOMPARE(aqu_active, true);
}

void tst_QTofDaqDll::qtofdaqdllSupportsTwWaitForNewData()
{
  QCOMPARE(daqdll.daqActive(), true);
  TSharedMemoryDesc* pBufDesc = new TSharedMemoryDesc();
  auto ret = daqdll.waitForNewData(2000, pBufDesc, Q_NULLPTR, false);
  QCOMPARE(ret, 4);
}

void tst_QTofDaqDll::qtofdaqdllSupportsTwstopacquisition()
{
  QCOMPARE(daqdll.daqActive(), true);

  daqdll.setTimeout(2000);
  daqdll.stopAcquisition();
  QThread::msleep(100);
  daqdll.stopAcquisition();
  QThread::msleep(100);
  auto ret = daqdll.stopAcquisition();
  auto aqu_active = daqdll.daqActive();
  QVERIFY(((ret == 4) || (ret == 2)));
  QCOMPARE(aqu_active, false);
  //QTRY_VERIFY_WITH_TIMEOUT(daqdll.tofDaqRunning() == false, 2000);
}

void tst_QTofDaqDll::qtofdaqdllSupportsTwcontinueacquisition()
{
  //QVERIFY(m_tofdaqrecexe == Q_NULLPTR); // cound not configure noHW automatically
  //QTofDaqDll daqdll;
  //daqdll.init(tofdaqdll);
  auto ret = daqdll.continueAcquisition();
  QVERIFY(((ret == 4) || (ret == 2)));
}

void tst_QTofDaqDll::qtofdaqdllSupportsTwgetpeakparameters()
{
  //QTofDaqDll daqdll;
  //daqdll.init(tofdaqdll);
  //daqdll.setDaqParameter("MassTableName", "nominal");
  TPeakPar* PeakPar = new TPeakPar();
  auto ret = daqdll.getPeakParameters(PeakPar, 0);
  QVERIFY(((ret == 4) || (ret == 9)));
}

void tst_QTofDaqDll::qtofdaqdllSupportsTwgetallpeakparameters()
{
  //QTofDaqDll daqdll;
  //daqdll.init(tofdaqdll);
  //daqdll.setDaqParameter("MassTableName", "nominal");

  QStringList     mass_labels;
  QVector<float>  masses;
  QVector<float>  masses_hi;
  QVector<float>  masses_lo;

  auto ret = daqdll.getAllPeakParameters(mass_labels, masses, masses_hi, masses_lo);
  QCOMPARE(ret, 4);
  QVERIFY((mass_labels.size()>0) && (masses.size()>0) && (masses_hi.size()>0) && (masses_lo.size()>0) );
}

void tst_QTofDaqDll::qtofdaqdllSupportsTwgetmasscalib()
{
  //QTofDaqDll daqdll;
  //daqdll.init(tofdaqdll);
  int mode=0;
  int nbrParams=0;
  QVector<double> p;
  int nbrPoints =0;
  QVector<double> mass;
  QVector<double> tof;
  QVector<double> weight;
  int ret = daqdll.getMassCalib(mode, nbrParams, p, nbrPoints, mass, tof, weight);
  QVERIFY(((ret == 4) || (ret == 9)));
}

void tst_QTofDaqDll::qtofdaqdllSupportsTwgetspecxaxisfromshmemSampleindex()
{
  //QTofDaqDll daqdll;
  //daqdll.init(tofdaqdll);
  QVector<double> x_axis_sampleindex;
  auto ret = daqdll.getSpecXaxisFromShMem(x_axis_sampleindex, 0);
  QVERIFY(((ret == 4) || (ret == 9)|| (ret == 2))&& x_axis_sampleindex.size()>0 );
}

void tst_QTofDaqDll::qtofdaqdllSupportsTwgetspecxaxisfromshmemMass()
{
  //QTofDaqDll daqdll;
  //daqdll.init(tofdaqdll);
  QVector<double> x_axis_mass;
  auto ret = daqdll.getSpecXaxisFromShMem(x_axis_mass, 1);
  QVERIFY(((ret == 4) || (ret == 9)|| (ret == 2))&& x_axis_mass.size()>0 );
}

void tst_QTofDaqDll::qtofdaqdllSupportsTwgetspecxaxisfromshmemTof()
{
  //QTofDaqDll daqdll;
  //daqdll.init(tofdaqdll);
  QVector<double> x_axis_tof;
  auto ret = daqdll.getSpecXaxisFromShMem(x_axis_tof, 2);
  QVERIFY(((ret == 4) || (ret == 9)|| (ret == 2)) && x_axis_tof.size()>0 );
}

void tst_QTofDaqDll::qtofdaqdllSupportsTwgettofspectrumfromshmem()
{
  //QTofDaqDll daqdll;
  //daqdll.init(tofdaqdll);
  QVector<float> Spectrum;
  auto ret = daqdll.getTofSpectrumFromShMem(Spectrum, 0, 0, 0);
  QVERIFY(((ret == 4) || (ret == 9)|| (ret == 2)) && Spectrum.size()>0 );
}

void tst_QTofDaqDll::qtofdaqdllSupportsTwgetbuftimefromshmem()
{
  //QTofDaqDll daqdll;
  //daqdll.init(tofdaqdll);
  double BufTime  = 0;
  int BufIndex    = 1;
  int WriteIndex  = 0;

  auto ret = daqdll.getBufTimeFromShMem(BufTime, BufIndex, WriteIndex);
  QVERIFY(((ret == 4) || (ret == 9)|| (ret == 2)) && BufTime>0 );
}

void tst_QTofDaqDll::qtofdaqdllSupportsTwgetstickspectrumfromshmem()
{
  //QTofDaqDll daqdll;
  //daqdll.init(tofdaqdll);
  QVector<float> Spectrum;
  QVector<float> Masses;
  int SegmentIndex = 0;
  int SegmentEndIndex=0;
  int BufIndex=1;

  auto ret = daqdll.getStickSpectrumFromShMem(Spectrum, Masses, SegmentIndex, SegmentEndIndex, BufIndex);
  QVERIFY(((ret == 4) || (ret == 9)|| (ret == 2)) && Spectrum.size()>0 );
}

void tst_QTofDaqDll::qtofdaqdllSupportsTwRegisterUserDataBuf()
{
  QString Location("/NEPHELE");
  QStringList ElementDescription;
  ElementDescription << "Ele1" << "Ele2" << "Ele3" << "Ele4";
  //int NbrElements = ElementDescription;
  int CompressionLevel = 0;
  auto ret = daqdll.registerUserDataBuf(Location, ElementDescription, CompressionLevel);
  QCOMPARE(ret , 4);
}

void tst_QTofDaqDll::qtofdaqdllSupportsTwRegisterUserDataBufWithoutElementDescription()
{
  QString Location("/NEPHELE");
  //QStringList ElementDescription;
  //ElementDescription << "Ele1" << "Ele2" << "Ele3" << "Ele4";
  //int NbrElements = ElementDescription;
  int NbrElements = 4;
  int CompressionLevel = 0;
  auto ret = daqdll.registerUserDataBuf(Location, NbrElements, CompressionLevel);
  QCOMPARE(ret , 4);
}

void tst_QTofDaqDll::qtofdaqdllSupportsQueryNumberOfDataSources()
{
  int arrayLength = 0;

  auto ret = daqdll.queryNumberOfDataSources(arrayLength);

  QVERIFY(((ret == 4) || (ret == 9)));
}

void tst_QTofDaqDll::qtofdaqdllSupportsTwGetRegUserDataSources()
{
  QStringList location;
  QVector<int> nbrElements;
  QVector<int> type;

  auto ret = daqdll.getRegUserDataSources(location, nbrElements, type);
  QVERIFY(((ret == 4) || (ret == 9)));
}

void tst_QTofDaqDll::qtofdaqdllSupportsTwGetRegUserDataDesc()
{
  QString location("/NEPHELE");
  //int nbrElements;
  QStringList elementDescription;

  auto ret = daqdll.getRegUserDataDesc(location, elementDescription);
  QVERIFY(((ret == 4) || (ret == 7)));
}

void tst_QTofDaqDll::qtofdaqdllSupportsTwReadRegUserData()
{
  QString Location("/NEPHELE");
  QStringList ElementDescription;
  ElementDescription << "Ele1" << "Ele2" << "Ele3" << "Ele4";
  //int NbrElements = ElementDescription;
  int CompressionLevel = 0;
  daqdll.registerUserDataBuf(Location, ElementDescription, CompressionLevel);

  QVector<double> data;
  data.clear();
  data.resize(ElementDescription.size());
  auto ret = daqdll.readRegUserData(Location, data);

  QCOMPARE(ret , 4);
}

void tst_QTofDaqDll::qtofdaqdllSupportsTwUnregisterUserData()
{
  QString Location("/NEPHELE");
  QStringList ElementDescription;
  ElementDescription << "Ele1" << "Ele2" << "Ele3" << "Ele4";
  //int NbrElements = ElementDescription;
  int CompressionLevel = 0;
  daqdll.registerUserDataBuf(Location, ElementDescription, CompressionLevel);

  auto ret = daqdll.unregisterUserData(Location);
  QCOMPARE(ret , 4);

  QStringList locations;
  QVector<int> nbrElements;
  QVector<int> type;
  ret = daqdll.getRegUserDataSources(locations, nbrElements, type);
  QCOMPARE(ret , 4);
  QCOMPARE(locations.size() , 0);
}

void tst_QTofDaqDll::qtofdaqdllSupportsTwUpdateUserData()
{
  QString Location("/NEPHELE");
  QStringList ElementDescription;
  ElementDescription << "Ele1" << "Ele2" << "Ele3" << "Ele4";
  //int NbrElements = ElementDescription;
  int CompressionLevel = 0;
  daqdll.registerUserDataBuf(Location, ElementDescription, CompressionLevel);

  QVector<double> Data;
  foreach (auto ele , ElementDescription) {
      Q_UNUSED(ele);
      double dt = 0 + (qrand() / ( RAND_MAX / (100-0) ) ) ;
      Data.append(dt);
    }
  auto ret = daqdll.updateUserData(Location, Data);
  QCOMPARE(ret , 4);

  QVector<double> ExpData;
  ExpData.resize(ElementDescription.size());
  ret = daqdll.readRegUserData(Location, ExpData);
  QCOMPARE(ret , 4);
  QCOMPARE(Data , ExpData);

}

void tst_QTofDaqDll::qtofdaqdllTwgetmasscalibResizesTheInputVectors()
{
  //QTofDaqDll daqdll;
  //daqdll.init(tofdaqdll);
  int mode=0;
  int nbrParams=0;
  QVector<double> p;
  int nbrPoints =0;
  QVector<double> mass;
  QVector<double> tof;
  QVector<double> weight;
  daqdll.getMassCalib(mode, nbrParams, p, nbrPoints, mass, tof, weight);
  QCOMPARE(mass.size(), nbrParams);
}

void tst_QTofDaqDll::registeredUserDataCanBeQueried()
{
  QString Location("/NEPHELE");
  QStringList ElementDescription;
  ElementDescription << "Ele1" << "Ele2" << "Ele3" << "Ele4";
  //int NbrElements = ElementDescription;
  int CompressionLevel = 0;
  daqdll.registerUserDataBuf(Location, ElementDescription, CompressionLevel);

  int arrayLength = 0;
  QStringList locations;
  locations<<Location;
  QVector<int> nbrElements;
  QVector<int> type;

  auto ret = daqdll.getRegUserDataSources(arrayLength, locations, nbrElements, type);
  QCOMPARE(arrayLength, 1);
  QCOMPARE(nbrElements.size(), 1);
  QCOMPARE(nbrElements.at(0), ElementDescription.size());
  QVERIFY(((ret == 4) || (ret == 9)));
}

void tst_QTofDaqDll::registeredUserDescCanBeQueried()
{
  QString Location("/NEPHELE");
  QStringList ElementDescription;
  ElementDescription << "Ele1" << "Ele2" << "Ele3" << "Ele4";
  //int NbrElements = ElementDescription;
  int CompressionLevel = 0;
  daqdll.registerUserDataBuf(Location, ElementDescription, CompressionLevel);

  QStringList elementDescriptionQ;

  auto ret = daqdll.getRegUserDataDesc(Location, elementDescriptionQ);
  QCOMPARE(ElementDescription , elementDescriptionQ);
  QVERIFY(elementDescriptionQ.size() > 0);
  QVERIFY(((ret == 4) || (ret == 9)));
}

void tst_QTofDaqDll::registeredUserDataSupportsSubgroups()
{
  QString Location("/NEPHELE");
  QStringList ElementDescription;
  ElementDescription << "/group1/Ele1" << "/group2/Ele2" << "/group3/Ele3" << "/group4/Ele4";
  int CompressionLevel = 0;
  daqdll.registerUserDataBuf(Location, ElementDescription, CompressionLevel);

  QStringList elementDescriptionQ;
  auto ret = daqdll.getRegUserDataDesc(Location, elementDescriptionQ);

  //QStringList ElementDescriptionExp;
  //ElementDescriptionExp << "Ele1" << "Ele2" << "Ele3" << "Ele4";

  QCOMPARE(ElementDescription , elementDescriptionQ);
  QVERIFY(elementDescriptionQ.size() > 0);
  QVERIFY(((ret == 4) || (ret == 9)));
}

void tst_QTofDaqDll::registeredUserDataCanBeCalledTwice()
{
  QString Location("/NEPHELE");
  QStringList ElementDescription;
  ElementDescription << "Ele1" << "Ele2" << "Ele3" << "Ele4";
  int CompressionLevel = 0;
  daqdll.registerUserDataBuf(Location, ElementDescription, CompressionLevel);
  daqdll.registerUserDataBuf(Location, ElementDescription, CompressionLevel);

  QStringList elementDescriptionQ;
  auto ret = daqdll.getRegUserDataDesc(Location, elementDescriptionQ);

  QCOMPARE(ElementDescription , elementDescriptionQ);
  QVERIFY(elementDescriptionQ.size() > 0);
  QVERIFY(((ret == 4) || (ret == 9)));
}

void tst_QTofDaqDll::registeredUserDataLocationsCanBeQueried()
{
  QStringList Locations;
  Locations << "/NEPHELE/group1/" << "/NEPHELE/group2/" << "/NEPHELE/group3/" << "/NEPHELE/group4/";
  QStringList ElementDescription;
  ElementDescription << "Ele1" << "Ele2" << "Ele3" << "Ele4";
  foreach (auto Location, Locations) {
      daqdll.registerUserDataBuf(Location, ElementDescription, 0);
    }
  //Locations.prepend("/NEPHELE");

  QStringList locationsExp;
  QVector<int> nbrElements;
  QVector<int> type;
  auto ret = daqdll.getRegUserDataSources(locationsExp, nbrElements, type);

  QVERIFY(((ret == 4) || (ret == 9)));
  QCOMPARE(Locations, locationsExp);

}

void tst_QTofDaqDll::registeredUserDataSourcesCanBeQueried()
{
  QStringList Locations;
  Locations << "/NEPHELE/group0/" << "/NEPHELE/group1/" << "/NEPHELE/group2/" << "/NEPHELE/group3/" << "/NEPHELE/group4/";
  QStringList ElementDescription;
  int k=0;
  foreach (auto Location, Locations) {
      ElementDescription.clear();
      for(int j = 1; j<5; j++){ElementDescription << QString("Ele%1").arg(k++);}
      daqdll.registerUserDataBuf(Location, ElementDescription, 0);
    }

  UserDataSourceList usrDtaSrc;
  daqdll.readRegUserData(usrDtaSrc);

  QCOMPARE(usrDtaSrc.size(), Locations.size());

}
#endif

QTEST_MAIN(tst_QTofDaqDll)

#include "tst_QTofDaqDll.moc"

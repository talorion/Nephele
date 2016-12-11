#include <QString>
#include <QtTest>
#include <QCoreApplication>

class tst_QTofDaqDll : public QObject
{
  Q_OBJECT

public:
  tst_QTofDaqDll();

private Q_SLOTS:
  #if defined( Q_OS_WIN )
  void qtofdaqdllWrapsTofdaqdll();
  void qtofdaqdllCanChangeDllPathAtRuntime();
  void qtofdaqdllInitializeDiposesLoadedLibraries();
  void qtofdaqdllCanCallDisposeAnytime();

  void qtofdaqdllSupportsTwcleanupdll();
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
  void qtofdaqdllSupportsTwstartacquisition();
  void qtofdaqdllSupportsTwstopacquisition();
  void qtofdaqdllSupportsTwcontinueacquisition();
  void qtofdaqdllSupportsTwgetdescriptor();
  void qtofdaqdllSupportsTwgetpeakparameters();
  void qtofdaqdllSupportsTwgetallpeakparameters();
  void qtofdaqdllSupportsTwgetmasscalib();
  void qtofdaqdllSupportsTwgetspecxaxisfromshmemSampleindex();
  void qtofdaqdllSupportsTwgetspecxaxisfromshmemMass();
  void qtofdaqdllSupportsTwgetspecxaxisfromshmemTof();
  void qtofdaqdllSupportsTwgettofspectrumfromshmem();
  void qtofdaqdllSupportsTwgetbuftimefromshmem();
  void qtofdaqdllSupportsTwgetstickspectrumfromshmem();

  void qtofdaqdllTwgetmasscalibResizesTheInputVectors();
  #endif

};

tst_QTofDaqDll::tst_QTofDaqDll():QObject()
{
}

#if defined( Q_OS_WIN )
void tst_QTofDaqDll::qtofdaqdllWrapsTofdaqdll()
{
  QTofDaqDll daqdll;
  auto initialized = daqdll.init("C:\\Tofwerk\\TofDaq_1.98_API\\bin\\x64\\TofDaqDll.dll");
  auto version = daqdll.getDllVersion();
  QVERIFY(initialized == 0 && version>0);
}

void tst_QTofDaqDll::qtofdaqdllCanChangeDllPathAtRuntime()
{
  QTofDaqDll daqdll;
  daqdll.init("C:\\Tofwerk\\TofDaq_1.98_API\\bin\\x64\\TofDaqDll.dll");
  auto first_version = daqdll.getDllVersion();
  daqdll.setDllName("C:\\Tofwerk\\TofDaq_1.97_API\\bin\\x64\\TofDaqDll.dll");
  auto second_version = daqdll.getDllVersion();
  QVERIFY(first_version!=second_version);
}

void tst_QTofDaqDll::qtofdaqdllInitializeDiposesLoadedLibraries()
{
  QTofDaqDll daqdll;
  daqdll.init("C:\\Tofwerk\\TofDaq_1.98_API\\bin\\x64\\TofDaqDll.dll");
  auto first_version = daqdll.getDllVersion();
  daqdll.init("C:\\Tofwerk\\TofDaq_1.97_API\\bin\\x64\\TofDaqDll.dll");
  auto second_version = daqdll.getDllVersion();
  QVERIFY(first_version!=second_version);
}

void tst_QTofDaqDll::qtofdaqdllCanCallDisposeAnytime()
{
  QTofDaqDll daqdll;
  daqdll.dispose();
  daqdll.init("C:\\Tofwerk\\TofDaq_1.98_API\\bin\\x64\\TofDaqDll.dll");
  daqdll.dispose();
  auto version = daqdll.getDllVersion();
  QVERIFY(version == 0.0);
}

void tst_QTofDaqDll::qtofdaqdllSupportsTwcleanupdll()
{
  QTofDaqDll daqdll;
  daqdll.init("C:\\Tofwerk\\TofDaq_1.98_API\\bin\\x64\\TofDaqDll.dll");
  daqdll.cleanupDll();
  QVERIFY(true);
}

void tst_QTofDaqDll::qtofdaqdllSupportsTwgetdllversion()
{
  QTofDaqDll daqdll;
  daqdll.init("C:\\Tofwerk\\TofDaq_1.98_API\\bin\\x64\\TofDaqDll.dll");
  auto version = daqdll.getDllVersion();
  QVERIFY(version>0);
}

void tst_QTofDaqDll::qtofdaqdllSupportsTwdaqactive()
{
  QTofDaqDll daqdll;
  daqdll.init("C:\\Tofwerk\\TofDaq_1.98_API\\bin\\x64\\TofDaqDll.dll");
  auto active = daqdll.daqActive();
  QVERIFY(active == false);
}

void tst_QTofDaqDll::qtofdaqdllSupportsTwgettimeout()
{
  QTofDaqDll daqdll;
  daqdll.init("C:\\Tofwerk\\TofDaq_1.98_API\\bin\\x64\\TofDaqDll.dll");
  auto timeout = daqdll.getTimeout();
  QVERIFY(timeout >0 );
}

void tst_QTofDaqDll::qtofdaqdllSupportsTwsettimeout()
{
  QTofDaqDll daqdll;
  daqdll.init("C:\\Tofwerk\\TofDaq_1.98_API\\bin\\x64\\TofDaqDll.dll");
  auto timeout = daqdll.getTimeout();
  daqdll.setTimeout(timeout+1);
  auto timeout_after = daqdll.getTimeout();
  QVERIFY(timeout != timeout_after);
}

void tst_QTofDaqDll::qtofdaqdllSupportsTwtofdaqrunning()
{
  QTofDaqDll daqdll;
  daqdll.init("C:\\Tofwerk\\TofDaq_1.98_API\\bin\\x64\\TofDaqDll.dll");
  auto running = daqdll.tofDaqRunning();
  QCOMPARE(running , true);
}

void tst_QTofDaqDll::qtofdaqdllSupportsTwgetdaqparameter()
{
  QTofDaqDll daqdll;
  daqdll.init("C:\\Tofwerk\\TofDaq_1.98_API\\bin\\x64\\TofDaqDll.dll");
  auto DaqName = daqdll.getDaqParameter("DaqName");
  QCOMPARE(DaqName ,QString(""));
}

void tst_QTofDaqDll::qtofdaqdllSupportsTwgetdaqparameterint()
{
  QTofDaqDll daqdll;
  daqdll.init("C:\\Tofwerk\\TofDaq_1.98_API\\bin\\x64\\TofDaqDll.dll");
  auto DaqMode = daqdll.getDaqParameterInt("DaqMode");
  QCOMPARE(DaqMode , 0);
}

void tst_QTofDaqDll::qtofdaqdllSupportsTwgetdaqparameterbool()
{
  QTofDaqDll daqdll;
  daqdll.init("C:\\Tofwerk\\TofDaq_1.98_API\\bin\\x64\\TofDaqDll.dll");
  auto NegativeIonMode = daqdll.getDaqParameterBool("NegativeIonMode");
  QCOMPARE(NegativeIonMode , false);
}

void tst_QTofDaqDll::qtofdaqdllSupportsTwgetdaqparameterfloat()
{
  QTofDaqDll daqdll;
  daqdll.init("C:\\Tofwerk\\TofDaq_1.98_API\\bin\\x64\\TofDaqDll.dll");
  //double TofDaqVersion_first = daqdll.TwGetDllVersion();
  auto TofDaqVersion_second = daqdll.getDaqParameterFloat("TofDaqVersion");
  QVERIFY(TofDaqVersion_second > 0.0f);
}

void tst_QTofDaqDll::qtofdaqdllSupportsTwgetdaqparameterint64()
{
  QTofDaqDll daqdll;
  daqdll.init("C:\\Tofwerk\\TofDaq_1.98_API\\bin\\x64\\TofDaqDll.dll");
  auto BlockPulseWidth = daqdll.getDaqParameterInt64("BlockPulseWidth");
  QVERIFY(BlockPulseWidth >= 100);// 	width of block pulse in ns. Min: 100 ns, Max: BlockPeriod-BlockPulseDelay
}

void tst_QTofDaqDll::qtofdaqdllSupportsTwgetdaqparameterdouble()
{
  QTofDaqDll daqdll;
  daqdll.init("C:\\Tofwerk\\TofDaq_1.98_API\\bin\\x64\\TofDaqDll.dll");
  auto DioStartDelay = daqdll.getDaqParameterDouble("DioStartDelay");
  QCOMPARE(DioStartDelay , 0.0);
}

void tst_QTofDaqDll::qtofdaqdllSupportsTwsetdaqparameter()
{
  QTofDaqDll daqdll;
  daqdll.init("C:\\Tofwerk\\TofDaq_1.98_API\\bin\\x64\\TofDaqDll.dll");
  auto DataFileName_bevore = daqdll.getDaqParameter("DataFileName");
  auto ret = daqdll.setDaqParameter("DataFileName", QDateTime::currentDateTime().toString("yyyy.MM.dd-hh.mm.ss.zzz")+".h5");
  auto DataFileName_after = daqdll.getDaqParameter("DataFileName");
  daqdll.setDaqParameter("DataFileName", DataFileName_bevore);
  QVERIFY(ret == 4 && DataFileName_bevore != DataFileName_after);
}

void tst_QTofDaqDll::qtofdaqdllSupportsTwsetdaqparameterint()
{
  QTofDaqDll daqdll;
  daqdll.init("C:\\Tofwerk\\TofDaq_1.98_API\\bin\\x64\\TofDaqDll.dll");
  auto NbrWaveforms_bevore = daqdll.getDaqParameterInt("NbrWaveforms");
  auto ret = daqdll.setDaqParameterInt("NbrWaveforms", NbrWaveforms_bevore+1);
  auto NbrWaveforms_after = daqdll.getDaqParameterInt("NbrWaveforms");
  daqdll.setDaqParameterInt("NbrWaveforms", NbrWaveforms_bevore);
  QVERIFY(ret == 4 && NbrWaveforms_bevore != NbrWaveforms_after);
}

void tst_QTofDaqDll::qtofdaqdllSupportsTwsetdaqparameterbool()
{
  QTofDaqDll daqdll;
  daqdll.init("C:\\Tofwerk\\TofDaq_1.98_API\\bin\\x64\\TofDaqDll.dll");
  auto DioActiveInvert_bevore = daqdll.getDaqParameterBool("DioActiveInvert");
  auto ret = daqdll.setDaqParameterBool("DioActiveInvert", !DioActiveInvert_bevore);
  auto DioActiveInvert_after = daqdll.getDaqParameterBool("DioActiveInvert");
  daqdll.setDaqParameterBool("DioActiveInvert", DioActiveInvert_bevore);
  QVERIFY(ret == 4 && DioActiveInvert_bevore != DioActiveInvert_after);
}

void tst_QTofDaqDll::qtofdaqdllSupportsTwsetdaqparameterfloat()
{
  QTofDaqDll daqdll;
  daqdll.init("C:\\Tofwerk\\TofDaq_1.98_API\\bin\\x64\\TofDaqDll.dll");
  auto SampleInterval_bevore = daqdll.getDaqParameterFloat("SampleInterval");
  auto ret = daqdll.setDaqParameterFloat("SampleInterval", SampleInterval_bevore+1e-7);
  auto SampleInterval_after = daqdll.getDaqParameterFloat("DioActiveInvert");
  daqdll.setDaqParameterFloat("SampleInterval", SampleInterval_bevore);
  QVERIFY(ret == 4 && SampleInterval_bevore != SampleInterval_after);
}

void tst_QTofDaqDll::qtofdaqdllSupportsTwsetdaqparameterint64()
{
  QTofDaqDll daqdll;
  daqdll.init("C:\\Tofwerk\\TofDaq_1.98_API\\bin\\x64\\TofDaqDll.dll");
  auto StartDelay_bevore = daqdll.getDaqParameterInt64("StartDelay");
  auto ret = daqdll.setDaqParameterInt64("StartDelay", StartDelay_bevore+1);
  auto StartDelay_after = daqdll.getDaqParameterInt64("StartDelay");
  daqdll.setDaqParameterInt64("StartDelay", StartDelay_bevore);
  QVERIFY(ret == 4 && StartDelay_bevore != StartDelay_after);
}

void tst_QTofDaqDll::qtofdaqdllSupportsTwsetdaqparameterdouble()
{
  QTofDaqDll daqdll;
  daqdll.init("C:\\Tofwerk\\TofDaq_1.98_API\\bin\\x64\\TofDaqDll.dll");
  auto DioStartDelay_bevore = daqdll.getDaqParameterDouble("DioStartDelay");
  auto ret = daqdll.setDaqParameterDouble("DioStartDelay", DioStartDelay_bevore+1);
  auto StartDelay_after = daqdll.getDaqParameterDouble("DioStartDelay");
  daqdll.setDaqParameterDouble("DioStartDelay", DioStartDelay_bevore);
  QVERIFY(ret == 4 && DioStartDelay_bevore != StartDelay_after);
}

void tst_QTofDaqDll::qtofdaqdllSupportsTwstartacquisition()
{
  QTofDaqDll daqdll;
  daqdll.init("C:\\Tofwerk\\TofDaq_1.98_API\\bin\\x64\\TofDaqDll.dll");
  auto ret = daqdll.startAcquisition();
  auto aqu_active = daqdll.daqActive();
  QVERIFY(ret == 4 && (aqu_active == true));
}

void tst_QTofDaqDll::qtofdaqdllSupportsTwstopacquisition()
{
  QTofDaqDll daqdll;
  daqdll.init("C:\\Tofwerk\\TofDaq_1.98_API\\bin\\x64\\TofDaqDll.dll");
  //daqdll.TwStartAcquisition();
  auto ret = daqdll.stopAcquisition();
  auto aqu_active = daqdll.daqActive();
  QVERIFY(((ret == 4) || (ret == 2)) && (aqu_active == false));
}

void tst_QTofDaqDll::qtofdaqdllSupportsTwcontinueacquisition()
{
  QTofDaqDll daqdll;
  daqdll.init("C:\\Tofwerk\\TofDaq_1.98_API\\bin\\x64\\TofDaqDll.dll");
  auto ret = daqdll.continueAcquisition();
  QVERIFY(((ret == 4) || (ret == 2)));
}

void tst_QTofDaqDll::qtofdaqdllSupportsTwgetdescriptor()
{
  QTofDaqDll daqdll;
  daqdll.init("C:\\Tofwerk\\TofDaq_1.98_API\\bin\\x64\\TofDaqDll.dll");
  TSharedMemoryDesc* pBufDesc = new TSharedMemoryDesc();
  auto ret = daqdll.getDescriptor(pBufDesc);
  QVERIFY(((ret == 4) || (ret == 9)));
}

void tst_QTofDaqDll::qtofdaqdllSupportsTwgetpeakparameters()
{
  QTofDaqDll daqdll;
  daqdll.init("C:\\Tofwerk\\TofDaq_1.98_API\\bin\\x64\\TofDaqDll.dll");
  TPeakPar* PeakPar = new TPeakPar();
  auto ret = daqdll.getPeakParameters(PeakPar, 0);
  QVERIFY(((ret == 4) || (ret == 9)));
}

void tst_QTofDaqDll::qtofdaqdllSupportsTwgetallpeakparameters()
{
  QTofDaqDll daqdll;
  daqdll.init("C:\\Tofwerk\\TofDaq_1.98_API\\bin\\x64\\TofDaqDll.dll");

  QStringList     mass_labels;
  QVector<float>  masses;
  QVector<float>  masses_hi;
  QVector<float>  masses_lo;

  auto ret = daqdll.getAllPeakParameters(mass_labels, masses, masses_hi, masses_lo);
  QVERIFY((ret == 4) && (mass_labels.size()>0) && (masses.size()>0) && (masses_hi.size()>0) && (masses_lo.size()>0) );
}

void tst_QTofDaqDll::qtofdaqdllSupportsTwgetmasscalib()
{
  QTofDaqDll daqdll;
  daqdll.init("C:\\Tofwerk\\TofDaq_1.98_API\\bin\\x64\\TofDaqDll.dll");
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
  QTofDaqDll daqdll;
  daqdll.init("C:\\Tofwerk\\TofDaq_1.98_API\\bin\\x64\\TofDaqDll.dll");
  QVector<double> x_axis_sampleindex;
  auto ret = daqdll.getSpecXaxisFromShMem(x_axis_sampleindex, 0);
  QVERIFY(((ret == 4) || (ret == 9)|| (ret == 2))&& x_axis_sampleindex.size()>0 );
}

void tst_QTofDaqDll::qtofdaqdllSupportsTwgetspecxaxisfromshmemMass()
{
  QTofDaqDll daqdll;
  daqdll.init("C:\\Tofwerk\\TofDaq_1.98_API\\bin\\x64\\TofDaqDll.dll");
  QVector<double> x_axis_mass;
  auto ret = daqdll.getSpecXaxisFromShMem(x_axis_mass, 1);
  QVERIFY(((ret == 4) || (ret == 9)|| (ret == 2))&& x_axis_mass.size()>0 );
}

void tst_QTofDaqDll::qtofdaqdllSupportsTwgetspecxaxisfromshmemTof()
{
  QTofDaqDll daqdll;
  daqdll.init("C:\\Tofwerk\\TofDaq_1.98_API\\bin\\x64\\TofDaqDll.dll");
  QVector<double> x_axis_tof;
  auto ret = daqdll.getSpecXaxisFromShMem(x_axis_tof, 2);
  QVERIFY(((ret == 4) || (ret == 9)|| (ret == 2)) && x_axis_tof.size()>0 );
}

void tst_QTofDaqDll::qtofdaqdllSupportsTwgettofspectrumfromshmem()
{
  QTofDaqDll daqdll;
  daqdll.init("C:\\Tofwerk\\TofDaq_1.98_API\\bin\\x64\\TofDaqDll.dll");
  QVector<float> Spectrum;
  auto ret = daqdll.getTofSpectrumFromShMem(Spectrum, 0, 0, 0);
  QVERIFY(((ret == 4) || (ret == 9)|| (ret == 2)) && Spectrum.size()>0 );
}

void tst_QTofDaqDll::qtofdaqdllSupportsTwgetbuftimefromshmem()
{
  QTofDaqDll daqdll;
  daqdll.init("C:\\Tofwerk\\TofDaq_1.98_API\\bin\\x64\\TofDaqDll.dll");
  double BufTime  = 0;
  int BufIndex    = 1;
  int WriteIndex  = 0;

  auto ret = daqdll.getBufTimeFromShMem(BufTime, BufIndex, WriteIndex);
  QVERIFY(((ret == 4) || (ret == 9)|| (ret == 2)) && BufTime>0 );
}

void tst_QTofDaqDll::qtofdaqdllSupportsTwgetstickspectrumfromshmem()
{
  QTofDaqDll daqdll;
  daqdll.init("C:\\Tofwerk\\TofDaq_1.98_API\\bin\\x64\\TofDaqDll.dll");
  QVector<float> Spectrum;
  QVector<float> Masses;
  int SegmentIndex = 0;
  int SegmentEndIndex=0;
  int BufIndex=1;

  auto ret = daqdll.getStickSpectrumFromShMem(Spectrum, Masses, SegmentIndex, SegmentEndIndex, BufIndex);
  QVERIFY(((ret == 4) || (ret == 9)|| (ret == 2)) && Spectrum.size()>0 );
}

void tst_QTofDaqDll::qtofdaqdllTwgetmasscalibResizesTheInputVectors()
{
  QTofDaqDll daqdll;
  daqdll.init("C:\\Tofwerk\\TofDaq_1.98_API\\bin\\x64\\TofDaqDll.dll");
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
#endif

QTEST_MAIN(tst_QTofDaqDll)

#include "tst_QTofDaqDll.moc"

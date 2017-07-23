#include "TofDaqDll.h"
#include "TofDaqDllHelper.hpp"

void TwCleanupDll()
{
    talorion::TofDaqDllHelper::destroy();
}

double TwGetDllVersion()
{
    return 1.99;
}

bool TwDaqActive()
{
    talorion::TofDaqDllHelper& helper = talorion::TofDaqDllHelper::get_instance();
    return helper.DaqActive();
}

int TwGetTimeout()
{
    talorion::TofDaqDllHelper& helper = talorion::TofDaqDllHelper::get_instance();
    return helper.getTimeout();
}

void TwSetTimeout(int timeout)
{
    talorion::TofDaqDllHelper& helper = talorion::TofDaqDllHelper::get_instance();
    return helper.setTimeout(timeout);
}

bool TwTofDaqRunning()
{
    talorion::TofDaqDllHelper& helper = talorion::TofDaqDllHelper::get_instance();

    return helper.getTofDaqRunning();
}

TwRetVal TwCloseTofDaqRec()
{
    return TwSuccess;
}

char *TwGetDaqParameter(char *Parameter)
{
    (void)Parameter;
    return nullptr;
}

int TwGetDaqParameterInt(char *Parameter)
{
    (void)Parameter;
    return 0;
}

bool TwGetDaqParameterBool(char *Parameter)
{
    (void)Parameter;
    return false;
}

float TwGetDaqParameterFloat(char *Parameter)
{
    (void)Parameter;
    return 0.0;
}

long long TwGetDaqParameterInt64(char *Parameter)
{
    (void)Parameter;
    return 0;
}

double TwGetDaqParameterDouble(char *Parameter)
{
    (void)Parameter;
    return 0.0;
}

TwRetVal TwSetDaqParameter(char *Parameter, char *ValueString)
{
    (void)Parameter;
    (void)ValueString;
    return TwSuccess;
}

TwRetVal TwSetDaqParameterInt(char *Parameter, int Value)
{
    (void)Parameter;
    (void)Value;
    return TwSuccess;
}

TwRetVal TwSetDaqParameterBool(char *Parameter, bool Value)
{
    (void)Parameter;
    (void)Value;
    return TwSuccess;
}

TwRetVal TwSetDaqParameterFloat(char *Parameter, float Value)
{
    (void)Parameter;
    (void)Value;
    return TwSuccess;
}

TwRetVal TwSetDaqParameterInt64(char *Parameter, long long Value)
{
    (void)Parameter;
    (void)Value;
    return TwSuccess;
}

TwRetVal TwSetDaqParameterDouble(char *Parameter, double Value)
{
    (void)Parameter;
    (void)Value;
    return TwSuccess;
}

TwRetVal TwStartAcquisition()
{

    return TwSuccess;
}

TwRetVal TwStopAcquisition()
{

    return TwSuccess;
}

TwRetVal TwContinueAcquisition()
{

    return TwSuccess;
}

TwRetVal TwGetDescriptor(TSharedMemoryDesc *pBufDesc)
{
    (void)pBufDesc;
    return TwSuccess;
}

TwRetVal TwGetPeakParameters(TPeakPar *PeakPar, int PeakIndex)
{
    (void)PeakPar;
    (void)PeakIndex;
    return TwSuccess;
}

TwRetVal TwGetMassCalib(int *mode, int *nbrParams, double *p, int *nbrPoints, double *mass, double *tof, double *weight)
{
    (void)mode;
    (void)nbrParams;
    (void)p;
    (void)nbrPoints;
    (void)mass;
    (void)tof;
    (void)weight;
    return TwSuccess;
}

TwRetVal TwGetSpecXaxisFromShMem(double *SpecAxis, int Type, char *UnitLabel, double maxMass)
{
    (void)SpecAxis;
    (void)Type;
    (void)UnitLabel;
    (void)maxMass;
    return TwSuccess;
}

TwRetVal TwGetTofSpectrumFromShMem(float *Spectrum, int SegmentIndex, int SegmentEndIndex, int BufIndex, bool Normalize)
{
    (void)Spectrum;
    (void)SegmentIndex;
    (void)SegmentEndIndex;
    (void)BufIndex;
    (void)Normalize;
    return TwSuccess;
}

TwRetVal TwGetBufTimeFromShMem(double *BufTime, int BufIndex, int WriteIndex)
{
    (void)BufTime;
    (void)BufIndex;
    (void)WriteIndex;
    return TwSuccess;
}

TwRetVal TwGetStickSpectrumFromShMem(float *Spectrum, float *Masses, int SegmentIndex, int SegmentEndIndex, int BufIndex)
{
    (void)Spectrum;
    (void)Masses;
    (void)SegmentIndex;
    (void)SegmentEndIndex;
    (void)BufIndex;
    return TwSuccess;
}

TwRetVal TwGetRegUserDataSources(int *arrayLength, char *location, int *nbrElements, int *type)
{
    (void)arrayLength;
    (void)location;
    (void)nbrElements;
    (void)type;
    return TwSuccess;
}

TwRetVal TwRegisterUserDataBuf(char *Location, int NbrElements, char *ElementDescription, int CompressionLevel)
{
    (void)Location;
    (void)NbrElements;
    (void)ElementDescription;
    (void)CompressionLevel;
    return TwSuccess;
}

TwRetVal TwGetRegUserDataDesc(char *location, int *nbrElements, char *elementDescription)
{
    (void)location;
    (void)nbrElements;
    (void)elementDescription;
    return TwSuccess;
}

TwRetVal TwReadRegUserData(char *Location, int NbrElements, double *Data)
{
    (void)Location;
    (void)NbrElements;
    (void)Data;
    return TwSuccess;
}

TwRetVal TwUnregisterUserData(char *Location)
{
    (void)Location;
    return TwSuccess;
}

TwRetVal TwUpdateUserData(char *Location, int NbrElements, double *Data)
{
    (void)Location;
    (void)NbrElements;
    (void)Data;
    return TwSuccess;
}

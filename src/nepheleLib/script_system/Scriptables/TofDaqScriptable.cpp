#include "TofDaqScriptable.hpp"

#include <QTofDaqDll.hpp>

namespace talorion {

QTofDaqDll* TofDaqScriptable::s_daqDll = new QTofDaqDll();

TofDaqScriptable::TofDaqScriptable(QObject *par) :
    abstract_scriptable_object("direct_daq", par)
{
    this->init();
}

int TofDaqScriptable::init(QString dll_name)
{
    return s_daqDll->init(dll_name);
}

void TofDaqScriptable::dispose()
{
    return s_daqDll->dispose();
}

int TofDaqScriptable::initialize_dll() const
{
    //return s_daqDll->
    return 4;
}

void TofDaqScriptable::cleanup_dll() const
{
    return s_daqDll->cleanupDll();
}

int TofDaqScriptable::start_aquisition() const
{
    return s_daqDll->startAcquisition();
}

int TofDaqScriptable::stop_aquisition() const
{
    return s_daqDll->stopAcquisition();
}

bool TofDaqScriptable::aquisition_active() const
{
    return s_daqDll->daqActive();
}

bool TofDaqScriptable::tof_daq_running() const
{
    return s_daqDll->tofDaqRunning();
}

int TofDaqScriptable::register_user_data(const QStringList &value_names, const QString &path, int cmp_lvl) const
{
    return s_daqDll->registerUserDataBuf(path, value_names, cmp_lvl);
}

int TofDaqScriptable::UpdateUserData(QVector<double> &Data, const QString &path) const
{
    return s_daqDll->updateUserData(path, Data);
}

int TofDaqScriptable::unregister_user_data(const QString &path) const
{
    return s_daqDll->unregisterUserData(path);
}

int TofDaqScriptable::get_reg_user_data_sources(QStringList &locations, QVector<int> &nbrElements, QVector<int> &type)
{
    return s_daqDll->getRegUserDataSources(locations, nbrElements, type);
}

int TofDaqScriptable::get_reg_user_data_desc(const QString &path, QStringList &descriptions)
{
    return s_daqDll->getRegUserDataDesc(path, descriptions);
}

int TofDaqScriptable::read_reg_user_data(const QString &path, QVector<double> &Data)
{
    return s_daqDll->readRegUserData(path, Data);
}

double TofDaqScriptable::read_reg_user_data(const QString path, const QString varname)
{
    QStringList descriptions;
    int ret = get_reg_user_data_desc(path,descriptions);
    qDebug() << "reg_user_data_descriptions: " << descriptions;
    int dataindex;
    QVector<double> data(descriptions.length());
    if (descriptions.contains(varname))
    {
        qDebug() << "Found Variable!";
        dataindex = descriptions.indexOf(varname);
        ret = read_reg_user_data(path, data);
    }
    if (data.length()>0 && data.length()>dataindex)
    {
        qDebug() << "Matching Index available!";
        return data.at(dataindex);
    }
    return 0;
}

int TofDaqScriptable::read_spectrum(QVector<float> &buffer_Spectrum, int BufIndex, int SegmentIndex, int SegmentEndIndex, bool Normalize) const
{
    return s_daqDll->getTofSpectrumFromShMem(buffer_Spectrum, SegmentIndex, SegmentEndIndex, BufIndex, Normalize);
}

int TofDaqScriptable::read_average_spectrum(QVector<double> &buffer_avg_spectrum, bool Normalize) const
{
    return s_daqDll->getSumSpectrumFromShMem(buffer_avg_spectrum, Normalize);
}

int TofDaqScriptable::read_traces(QVector<float> &buffer_Spectrum, QVector<float> buffer_Masses, int BufIndex, int SegmentIndex, int SegmentEndIndex) const
{
    return s_daqDll->getStickSpectrumFromShMem(buffer_Spectrum, buffer_Masses, SegmentIndex, SegmentEndIndex, BufIndex);
}

//int TofDaqScriptable::wait_for_new_data(int Timeout, shared_memory_descriptor &pBufDesc, shared_memory_pointer &pShMem, bool WaitForEventReset)
//{
//    return s_daqDll->waitForNewData(Timeout, pBufDesc, pShMem, WaitForEventReset);
//}

QString TofDaqScriptable::read_parameter(const QString &para) const
{
    return s_daqDll->getDaqParameter(para);
}

int TofDaqScriptable::read_int_parameter(const QString &para) const
{
    return s_daqDll->getDaqParameterInt(para);
}

bool TofDaqScriptable::read_bool_parameter(const QString &para) const
{
    return s_daqDll->getDaqParameterBool(para);
}

float TofDaqScriptable::read_float_parameter(const QString &para) const
{
    return s_daqDll->getDaqParameterFloat(para);
}

qint64 TofDaqScriptable::read_int64_parameter(const QString &para) const
{
    return s_daqDll->getDaqParameterInt64(para);
}

double TofDaqScriptable::read_double_parameter(const QString &para) const
{
    return s_daqDll->getDaqParameterDouble(para);
}

int TofDaqScriptable::write_parameter(const QString &para, QString value) const
{
    return s_daqDll->setDaqParameter(para, value);
}

int TofDaqScriptable::write_int_parameter(const QString &para, int value) const
{
    return s_daqDll->setDaqParameterInt(para, value);
}

int TofDaqScriptable::write_bool_parameter(const QString &para, bool value) const
{
    return s_daqDll->setDaqParameterBool(para, value);
}

int TofDaqScriptable::write_float_parameter(const QString &para, float value) const
{
    return s_daqDll->setDaqParameterFloat(para, value);
}

int TofDaqScriptable::write_int64_parameter(const QString &para, qint64 value) const
{
    return s_daqDll->setDaqParameterInt64(para, value);
}

int TofDaqScriptable::write_double_parameter(const QString &para, double value) const
{
    return s_daqDll->setDaqParameterDouble(para, value);
}
} // namespace talorion

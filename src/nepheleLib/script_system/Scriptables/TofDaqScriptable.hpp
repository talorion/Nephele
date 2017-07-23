#ifndef TOFDAQSCRIPTABLE_HPP
#define TOFDAQSCRIPTABLE_HPP

#include "core/abstract_scriptable_object.hpp"

namespace talorion {

//class shared_memory_descriptor;
//class shared_memory_pointer;
class QTofDaqDll;

class TofDaqScriptable : public abstract_scriptable_object
{
    Q_OBJECT
public:
    explicit TofDaqScriptable(QObject *par = nullptr);
    virtual ~TofDaqScriptable()=default;

    int init(QString dll_name = "C:\\Tofwerk\\TofDaq_1.97_noHW\\TofDaqDll.dll");
    void dispose();

private:
    Q_DISABLE_COPY(TofDaqScriptable)

public slots:
    int initialize_dll() const;
    void cleanup_dll() const;

    int start_aquisition() const;
    int stop_aquisition() const;
    bool aquisition_active() const;
    bool tof_daq_running() const;
    int register_user_data(const QStringList& value_names, const QString& path = "/NEPHELE", int cmp_lvl =0) const;
    int UpdateUserData(QVector<double>& Data, const QString& path = "/NEPHELE") const;
    int unregister_user_data(const QString& path) const;
    int get_reg_user_data_sources(QStringList& locations, QVector<int>& nbrElements, QVector<int>& type);
    int get_reg_user_data_desc(const QString& path, QStringList& descriptions);
    int read_reg_user_data(const QString& path, QVector<double>& Data);

    int read_spectrum(QVector<float>& buffer_Spectrum, int BufIndex, int SegmentIndex = -1, int SegmentEndIndex = -1,bool Normalize = false) const;
    int read_average_spectrum(QVector<double>& buffer_avg_spectrum, bool Normalize = false) const;
    int read_traces( QVector<float>& buffer_Spectrum, QVector<float> buffer_Masses, int BufIndex, int SegmentIndex = -1, int SegmentEndIndex = -1) const;

    //int wait_for_new_data(int Timeout,shared_memory_descriptor& pBufDesc,shared_memory_pointer& pShMem,bool WaitForEventReset);
    QString read_parameter(const QString& para) const;
    int read_int_parameter(const QString& para) const;
    bool read_bool_parameter(const QString& para) const;
    float read_float_parameter(const QString& para) const;
    qint64 read_int64_parameter(const QString& para) const;
    double read_double_parameter(const QString& para) const;

    int write_parameter(const QString& para, QString value) const;
    int write_int_parameter(const QString& para, int value) const;
    int write_bool_parameter(const QString& para, bool value) const;
    int write_float_parameter(const QString& para, float value) const;
    int write_int64_parameter(const QString& para, qint64 value) const;
    int write_double_parameter(const QString& para, double value) const;

private:
    static QTofDaqDll* s_daqDll;
};

} // namespace talorion

#endif // TOFDAQSCRIPTABLE_HPP

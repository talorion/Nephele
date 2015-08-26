#ifndef SIMULATED_FC_H
#define SIMULATED_FC_H

#include <QObject>

class simulated_fc
{
public:
    explicit simulated_fc();
    ~simulated_fc();
    //Q_DISABLE_COPY(simulated_fc)

    int get_Id() const;
    void set_Id(int value);

    double get_Smin() const;
    void set_Smin(double value);

    double get_Smax() const;
    void set_Smax(double value);

    double get_Amin() const;
    void set_Amin(double value);

    double get_Amax() const;
    void set_Amax(double value);

    double get_Set() const;
    void set_Set(double value);

    double get_Act() const;

    QString get_Name() const;
    void set_Name(const QString &value);

    QString get_Units() const;
    void set_Units(const QString &value);

private:
    int id_;
    double smin_;
    double smax_;
    double amin_;
    double amax_;
    double set_;
    //double act;
    QString name_;
    QString units_;
};

#endif // SIMULATED_FC_H

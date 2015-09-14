#ifndef SIMULATED_AV_HPP
#define SIMULATED_AV_HPP

#include <QObject>

class simulated_av : public QObject
{
    Q_OBJECT
public:
    explicit simulated_av(QObject *par = 0);
    ~simulated_av();
    Q_DISABLE_COPY(simulated_av)

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

#endif // SIMULATED_AV_HPP

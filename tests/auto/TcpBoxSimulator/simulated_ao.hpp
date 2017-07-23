#ifndef SIMULATED_AO_HPP
#define SIMULATED_AO_HPP

#include <QString>
#include "tcpboxsimulator_global.hpp"


class TCPBOXSIMULATORSHARED_EXPORT simulated_ao
{
public:
    simulated_ao();
    ~simulated_ao();
    Q_DISABLE_COPY(simulated_ao)

    int get_id() const;
    void setId(int get_id);

    bool get_set() const;
    void setSet(bool get_set);

    QString get_name() const;
    void setName(const QString &get_name);

    QString get_units() const;
    void setUnits(const QString &get_units);

private:
    int id_;
    bool set_;
    //double act;
    QString name_;
    QString units_;
};

#endif // SIMULATED_AO_HPP

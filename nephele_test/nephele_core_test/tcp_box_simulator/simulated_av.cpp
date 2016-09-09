#include "simulated_av.hpp"

simulated_av::simulated_av(QObject *par) :
    QObject(par),
    id_(0),
    smin_(0),
    smax_(0),
    amin_(0),
    amax_(0),
    set_(0),
    // act(0),
    name_(),
    units_()
{

}

simulated_av::~simulated_av()
{

}
int simulated_av::get_Id() const
{
    return id_;
}

void simulated_av::set_Id(int value)
{
    id_ = value;
}
double simulated_av::get_Smin() const
{
    return smin_;
}

void simulated_av::set_Smin(double value)
{
    smin_ = value;
}
double simulated_av::get_Smax() const
{
    return smax_;
}

void simulated_av::set_Smax(double value)
{
    smax_ = value;
}
double simulated_av::get_Amin() const
{
    return amin_;
}

void simulated_av::set_Amin(double value)
{
    amin_ = value;
}
double simulated_av::get_Amax() const
{
    return amax_;
}

void simulated_av::set_Amax(double value)
{
    amax_ = value;
}
double simulated_av::get_Set() const
{
    return set_;
}

void simulated_av::set_Set(double value)
{
    value = std::min(value, get_Smax());
    value = std::max(value, get_Smin());
    set_ = value;
}
double simulated_av::get_Act() const
{
    double rnd = double(qrand()) / RAND_MAX ;
    double percent = (get_Set()*0.01);

    double act =(((percent*2)-percent)*rnd)+get_Set();
    act = std::min(act, get_Amax());
    act = std::max(act, get_Amin());
    return act;
}

QString simulated_av::get_Name() const
{
    return name_;
}

void simulated_av::set_Name(const QString &value)
{
    name_ = value;
}
QString simulated_av::get_Units() const
{
    return units_;
}

void simulated_av::set_Units(const QString &value)
{
    units_ = value;
}

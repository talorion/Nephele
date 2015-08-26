#include "simulated_fc.h"

simulated_fc::simulated_fc():
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

simulated_fc::~simulated_fc()
{

}

int simulated_fc::get_Id() const
{
    return id_;
}

void simulated_fc::set_Id(int value)
{
    id_ = value;
}
double simulated_fc::get_Smin() const
{
    return smin_;
}

void simulated_fc::set_Smin(double value)
{
    smin_ = value;
}
double simulated_fc::get_Smax() const
{
    return smax_;
}

void simulated_fc::set_Smax(double value)
{
    smax_ = value;
}
double simulated_fc::get_Amin() const
{
    return amin_;
}

void simulated_fc::set_Amin(double value)
{
    amin_ = value;
}
double simulated_fc::get_Amax() const
{
    return amax_;
}

void simulated_fc::set_Amax(double value)
{
    amax_ = value;
}
double simulated_fc::get_Set() const
{
    return set_;
}

void simulated_fc::set_Set(double value)
{
    value = std::max(value, get_Amax());
    value = std::min(value, get_Amin());
    set_ = value;
}
double simulated_fc::get_Act() const
{
    double rnd = double(qrand()) / RAND_MAX ;
    double ten_percent = (get_Set()*0.1);
    return (((ten_percent*2)-ten_percent)*rnd)+get_Set();
}

QString simulated_fc::get_Name() const
{
    return name_;
}

void simulated_fc::set_Name(const QString &value)
{
    name_ = value;
}
QString simulated_fc::get_Units() const
{
    return units_;
}

void simulated_fc::set_Units(const QString &value)
{
    units_ = value;
}











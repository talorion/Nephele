#include "simulated_ao.hpp"

simulated_ao::simulated_ao():
    id_(0),
    set_(0),
    name_(),
    units_()
{

}

simulated_ao::~simulated_ao()
{

}
int simulated_ao::get_id() const
{
    return id_;
}

void simulated_ao::setId(int id)
{
    id_ = id;
}
bool simulated_ao::get_set() const
{
    return set_;
}

void simulated_ao::setSet(bool set)
{
    set_ = set;
}
QString simulated_ao::get_name() const
{
    return name_;
}

void simulated_ao::setName(const QString &name)
{
    name_ = name;
}
QString simulated_ao::get_units() const
{
    return units_;
}

void simulated_ao::setUnits(const QString &units)
{
    units_ = units;
}






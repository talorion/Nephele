#include "analogvalue.hpp"

#include <QVariant>


namespace talorion {

    analogValue::analogValue(QString name, QString units, double smin, double smax, double amin, double amax, double set, int id, int box_id, int entity, QObject *par) :
        QObject(par),
        setVal(set),
        actVal(0),
        sminVal(smin),
        smaxVal(smax),
        aminVal(amin),
        amaxVal(amax),
        nameVal(name),
        unitsVal(units),
        idVal(id),
        boxidVal(box_id),
        m_entity(entity)
    {
    }


    analogValue::~analogValue()
    {

    }

    double analogValue::getSet()
    {
        return setVal;
    }

    double analogValue::getAct()
    {
        return actVal;
    }

    double analogValue::getSmin()
    {
        return sminVal;
    }

    double analogValue::getSmax()
    {
        return smaxVal;
    }
    double analogValue::getAmin()
    {
        return aminVal;
    }

    double analogValue::getAmax()
    {
        return amaxVal;
    }

    QString analogValue::getName()
    {
        return nameVal;
    }

    QString analogValue::getUnits()
    {
        return unitsVal;
    }

    int analogValue::getId() const
    {
        return idVal;
    }

    int analogValue::getEntity() const
    {
        return m_entity;
    }

    int analogValue::getBoxidVal() const
    {
        return boxidVal;
    }

    void analogValue::setAct(double val)
    {
        if(actVal != val){
            actVal = val;
            emit act_value_changed();
        }
    }

    void analogValue::setSet(double val)
    {
        if(setVal != val){
            setVal = val;
            emit set_value_changed();
        }
    }
}

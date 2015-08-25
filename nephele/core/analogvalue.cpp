#include "analogvalue.hpp"

namespace talorion {

    analogValue::analogValue(QString name, QString units, double smin, double smax, double amin, double amax, double set, int id, QObject *par) :
        QObject(par),
        setVal(0),
        actVal(0),
        sminVal(0),
        smaxVal(0),
        aminVal(0),
        amaxVal(0),
        nameVal(),
        unitsVal(),
        idVal(0)
    {
        nameVal = name;
        unitsVal = units;
        sminVal = smin;
        smaxVal = smax;
        aminVal = amin;
        amaxVal = amax;
        setVal = set;
        idVal = id;
    }

    analogValue::analogValue(QVariant descriptor, QObject *par):
        QObject(par),
        setVal(0),
        actVal(0),
        sminVal(0),
        smaxVal(0),
        aminVal(0),
        amaxVal(0),
        nameVal(),
        unitsVal(),
        idVal(0)
    {
        Q_UNUSED(descriptor)
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

    int analogValue::getId()
    {
        return idVal;
    }

    void analogValue::changeSetByGui(double val)
    {
        if (setVal != val)
        {
            setVal = val;
            emit setChangedByGui(val, idVal);
        }
    }

    void analogValue::updateActByConnection(double val)
    {
        if (actVal != val)
        {
            actVal = val;
            emit actChanged(val);
        }
    }

    void analogValue::updateSetByConnection(double val)
    {
        emit setChangedByConnection(val);
    }
}

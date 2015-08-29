#include "analogvalue.hpp"

#include <QVariant>

namespace talorion {

    analogValue::analogValue(QString name, QString units, double smin, double smax, double amin, double amax, double set, int id, int box_id, int hash, QObject *par) :
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
        hashVal(hash)
    {
//        nameVal = name;
//        unitsVal = units;
//        sminVal = smin;
//        smaxVal = smax;
//        aminVal = amin;
//        amaxVal = amax;
//        setVal = set;
        //idVal = id;
    }

//    analogValue::analogValue(QVariant descriptor, QObject *par):
//        QObject(par),
//        setVal(0),
//        actVal(0),
//        sminVal(0),
//        smaxVal(0),
//        aminVal(0),
//        amaxVal(0),
//        nameVal(),
//        unitsVal(),
//        idVal(0)
//    {
//        Q_UNUSED(descriptor)
//    }

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

//    void analogValue::changeSetByGui(double val)
//    {
//        if (setVal != val)
//        {
//            setVal = val;
//            emit setChangedByGui(val, idVal);
//        }
//    }
    int analogValue::getHashVal() const
    {
        return hashVal;
    }

    int analogValue::getBoxidVal() const
    {
        return boxidVal;
    }


//    void analogValue::updateActByConnection(double val)
//    {
//        if (actVal != val)
//        {
//            actVal = val;
//            emit actChanged(val);
//            emit actChanged();
//        }
//    }

//    void analogValue::updateSetByConnection(double val)
//    {
//        emit setChangedByConnection(val);
//        //emit set_value_changed();
//    }

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

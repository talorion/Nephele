#ifndef ANALOGVALUE_H
#define ANALOGVALUE_H

#include <QObject>

QT_BEGIN_NAMESPACE
class QVariant;
QT_END_NAMESPACE

#include "core/event_manager.hpp"

namespace talorion {

    class analogValue : public QObject
    {
        Q_OBJECT
        friend class entity_manager;//onlz entity_manager is allowed to create analogValue
    private:
        analogValue(QString nameVal, QString unitsVal, double smin, double smax, double amin, double amax, double setVal, int id, int box_id=0,int hash=0, QObject *par = 0);
//        analogValue(QVariant descriptor, QObject *par = 0);
        ~analogValue(void);
        Q_DISABLE_COPY(analogValue)

    public:
        double getSet();
        double getAct();
        double getSmin();
        double getSmax();
        double getAmin();
        double getAmax();
        QString getName();
        QString getUnits();
        int getId() const;
        int getBoxidVal() const;
        int getHashVal() const;

//        void updateActByConnection(double val);
//        void updateSetByConnection(double val);

        void setAct(double val);
        void setSet(double val);

    signals:
        //void setChangedByGui(double val, int id);
        //void setChangedByConnection(double val);
        void set_value_changed();
        //void actChanged(double val);
        void act_value_changed();

//    public slots:
//        void changeSetByGui(double val);

    private:
        double setVal;
        double actVal;
        double sminVal;
        double smaxVal;
        double aminVal;
        double amaxVal;
        QString nameVal;
        QString unitsVal;
        const int idVal;
        const int boxidVal;
        const int hashVal;
    };
}

#endif // ANALOGVALUE_H

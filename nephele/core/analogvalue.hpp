#ifndef ANALOGVALUE_H
#define ANALOGVALUE_H

#include <QObject>

QT_BEGIN_NAMESPACE
class QVariant;
QT_END_NAMESPACE


namespace talorion {

    class analogValue : public QObject
    {
        Q_OBJECT
    public:
        analogValue(QString nameVal, QString unitsVal, double smin, double smax, double amin, double amax, double setVal, int id, QObject *par = 0);
        analogValue(QVariant descriptor, QObject *par = 0);
        ~analogValue(void);
        Q_DISABLE_COPY(analogValue)

        double getSet();
        double getAct();
        double getSmin();
        double getSmax();
        double getAmin();
        double getAmax();
        QString getName();
        QString getUnits();
        int getId();
        void updateActByConnection(double val);
        void updateSetByConnection(double val);

    signals:
        void setChangedByGui(double val, int id);
        void setChangedByConnection(double val);
        void actChanged(double val);

    public slots:
        void changeSetByGui(double val);

    private:
        double setVal;
        double actVal;
        double sminVal;
        double smaxVal;
        double aminVal;
        double amaxVal;
        QString nameVal;
        QString unitsVal;
        int idVal;
    };
}

#endif // ANALOGVALUE_H

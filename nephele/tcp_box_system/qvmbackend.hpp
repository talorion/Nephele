#ifndef QVMBACKEND_H
#define QVMBACKEND_H

#include <QObject>
#include <QVariant>
#include <QDebug>

#include "tcpdriver.hpp"

namespace talorion {

    class qvmbackend : public QObject
    {
        Q_OBJECT
    public:
        qvmbackend(QObject *par = 0);
        ~qvmbackend();
        Q_DISABLE_COPY(qvmbackend)

        int count();

    signals:
        //void newAnalogValue(analogValue* av);
        void newAnalogValue(int);
        void avSetChangeCommand(QByteArray command);

        void change_act_value(int, double);
        void change_set_value(int, double);

    public slots:
        void processData(QVariantMap desc, tcpDriverDataTypes::dataType type, int box_id);
        void logError(QString errorString);
    private slots:
        void avSetChangeProxy(int entity);
        void avSetChangeProxy(double value, int id);
    private:
        //QList<analogValue*> analog;
        QList<int> analog;
    };
}

#endif // QVMBACKEND_H

#ifndef QVMBACKEND_H
#define QVMBACKEND_H

#include <QObject>
#include <QVariant>
#include "analogvalue.h"
#include <QDebug>
#include "tcpdriver.h"

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
        void newAnalogValue(analogValue* av);
        void avSetChangeCommand(QByteArray command);

    public slots:
        void processData(QVariantMap desc, tcpDriverDataTypes::dataType type);
        void logError(QString errorString);
    private slots:
        void avSetChangeProxy(double value, int id);
    private:
        QList<analogValue*> analog;
    };
}

#endif // QVMBACKEND_H

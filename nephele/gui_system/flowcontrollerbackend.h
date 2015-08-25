#ifndef FLOWCONTROLLERBACKEND_H
#define FLOWCONTROLLERBACKEND_H

#include <QObject>
#include <QVariant>
#include "analogvalue.h"
#include <QDebug>
#include "tcpdriver.h"

namespace talorion {

    class flowControllerBackend : public QObject
    {
        Q_OBJECT
    public:
        flowControllerBackend(QObject *par = 0);
        ~flowControllerBackend(void);
        Q_DISABLE_COPY(flowControllerBackend)

        int count();

    signals:
        void newFlowcontroller(analogValue* fc);
        void fcSetChangeCommand(QByteArray command);

    public slots:
        void processData(QVariantMap desc, tcpDriverDataTypes::dataType type);
        void logError(QString errorString);
    private slots:
        void fcSetChangeProxy(double value, int id);
    private:
        QList<analogValue*> flowcontroller;
    };
}

#endif // FLOWCONTROLLERBACKEND_H

#ifndef FLOWCONTROLLERBACKEND_H
#define FLOWCONTROLLERBACKEND_H

#include <QObject>
#include <QVariant>
#include <QDebug>


#include "core/analogvalue.hpp"
#include "tcpdriver.hpp"

namespace talorion {
    class flowControllerBackend : public QObject
    {
        Q_OBJECT
    public:
        flowControllerBackend(QObject *par = 0);
        int count();

    signals:
        void newFlowcontroller(analogValue* fc);
        void fcSetChangeCommand(QByteArray command);

    public slots:
        void processData(QVariantMap desc, tcpDriverDataTypes::dataType type, int box_id);
        void logError(QString errorString);
    private slots:
        void fcSetChangeProxy(int hash);
        void fcSetChangeProxy(double value, int id);
    private:
        QList<analogValue*> flowcontroller;
    };
}

#endif // FLOWCONTROLLERBACKEND_H

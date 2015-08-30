#ifndef FLOWCONTROLLERBACKEND_H
#define FLOWCONTROLLERBACKEND_H

#include <QObject>
#include <QVariant>
#include <QDebug>

#include "tcpdriverdatatypes.hpp"

namespace talorion {
    class tcpDriver;

    class flowControllerBackend : public QObject
    {
        Q_OBJECT
    public:
        flowControllerBackend(QObject *par = 0);
        ~flowControllerBackend(void);                                  // not desctructable
        Q_DISABLE_COPY(flowControllerBackend)
        int count();

    signals:
        //void newFlowcontroller(analogValue* fc);
        //void newFlowcontroller(int entity);
        void fcSetChangeCommand(QByteArray command);

        void change_act_value(int, double);
        void change_set_value(int, double);

    public slots:
        void processData(QVariantMap desc, tcpDriverDataTypes::dataType type, int box_id);
        void logError(QString errorString);
    private slots:
        void fcSetChangeProxy(int entity);
        void fcSetChangeProxy(double value, int id);
    private:
        //QList<analogValue*> flowcontroller;
        QList<int> flowcontroller;
        QList<double> actbuffer;
        QList<double> setbuffer;
        //const tcpDriver* drv;
    };
}

#endif // FLOWCONTROLLERBACKEND_H

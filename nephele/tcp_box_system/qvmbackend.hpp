#ifndef QVMBACKEND_H
#define QVMBACKEND_H

#include "abstract_backend.hpp"
#include <QVariant>
#include <QDebug>


namespace talorion {

    class qvmbackend : public abstract_backend
    {
        Q_OBJECT
    public:
        qvmbackend(QObject *par = 0);
        ~qvmbackend();
        Q_DISABLE_COPY(qvmbackend)

        int count();

    signals:
        //void newAnalogValue(analogValue* av);
        //void newAnalogValue(int);
        //void avSetChangeCommand(QByteArray command);

        //void change_act_value(int, double);
        //void change_set_value(int, double);

    public slots:
        virtual void processData(QVariantMap desc, tcpDriverDataTypes::dataType type, int box_id) Q_DECL_OVERRIDE;
        virtual void logError(QString errorString)Q_DECL_OVERRIDE;
    private slots:
        virtual void fcSetChangeProxy(int entity)Q_DECL_OVERRIDE;
        virtual void fcSetChangeProxy(double value, int id) Q_DECL_OVERRIDE;

        virtual void dvSetChangeProxy(int entity)Q_DECL_OVERRIDE ;
        virtual void dvSetChangeProxy(int value, int id)Q_DECL_OVERRIDE ;
    private:
        //QList<analogValue*> analog;
        //QList<int> analog;
        QList<int> analog;
        QList<double> actbuffer;
        QMap<int, double> setbuffer;
        //QList<double> setbuffer;
        bool block_next_read;
    };
}

#endif // QVMBACKEND_H

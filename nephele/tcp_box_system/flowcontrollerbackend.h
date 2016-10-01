#ifndef FLOWCONTROLLERBACKEND_H
#define FLOWCONTROLLERBACKEND_H

#include "abstract_backend.hpp"
#include <QVariant>
#include <QDebug>



namespace talorion {
    class tcpDriver;

    class flowControllerBackend : public abstract_backend
    {
        Q_OBJECT
    public:
        flowControllerBackend(abstract_backend *par = 0);
        ~flowControllerBackend(void);                                  // not desctructable
        Q_DISABLE_COPY(flowControllerBackend)
        int count();

    signals:
        //void newFlowcontroller(analogValue* fc);
        //void newFlowcontroller(int entity);
        //void fcSetChangeCommand(QByteArray command);

        //void change_act_value(int, double);
        //void change_set_value(int, double);

    public slots:
        virtual void processData(QVariantMap desc, tcpDriverDataTypes::dataType type, int box_id) Q_DECL_OVERRIDE;
        virtual void logError(QString errorString, int source) Q_DECL_OVERRIDE;
    private slots:
        virtual void fcSetChangeProxy(int entity) Q_DECL_OVERRIDE;
        virtual void fcSetChangeProxy(double value, int id)Q_DECL_OVERRIDE ;

        virtual void dvSetChangeProxy(int entity)Q_DECL_OVERRIDE ;
        virtual void dvSetChangeProxy(int value, int id)Q_DECL_OVERRIDE ;
    private:
        //QList<analogValue*> flowcontroller;
        QList<int> flowcontroller;
        QList<double> actbuffer;
        QMap<int, double> setbuffer;
        //QList<double> setbuffer;
        //const tcpDriver* drv;
        bool block_next_read;

        int m_box_id;
    };
}

#endif // FLOWCONTROLLERBACKEND_H

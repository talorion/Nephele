#ifndef TALORION_ABSTRACT_BACKEND_HPP
#define TALORION_ABSTRACT_BACKEND_HPP

#include <QObject>
#include <QMap>
#include <QVariantMap>

#include "tcpdriverdatatypes.hpp"
#include "backend_type.hpp"

#include "JsonRpc/JrpcDatatype.hpp"

#include <core/EntityManager.hpp>

#include <tcp_box_system/JsonRpc/jsonrpcrequest.hpp>

namespace talorion {

    class abstract_backend : public QObject
    {
        Q_OBJECT
    public:
        explicit abstract_backend(QObject *par = 0);
        virtual ~abstract_backend();
        Q_DISABLE_COPY(abstract_backend)

        QString name()const{return doGetName();}

        backend_type backendType()const {return doGetBackendType();}

        QByteArray getInfoCommand() const{return doGetInfoCommand();}
        QByteArray getMinimalSetActCommand() const{return doGetMinimalSetActCommand();}

    signals:
        void fcSetChangeCommand(QByteArray command);
        void sendCommand(QSharedPointer<JsonRpcRequest> request);
        void change_act_value(int, double);
        void change_set_value(int, double);

        void change_digital_act_value(int, bool);
        void change_digital_set_value(int, bool);

        void processDataFinished();

    public slots:
        virtual void processData(QVariantMap desc, tcpDriverDataTypes::dataType type, int box_id)=0;
        virtual void logError(QString errorString, int source)=0;

        virtual EntityManager::EntityID newJsonRpcMethod(const QString &, const JrpcDatatypeList &, const QStringList &){return EntityManager::invalid_id;}

        virtual void newAO(int , QString , QString , double , double , double , QVariant ){}
        virtual void newAI(int , QString , QString , double , double , double , QVariant ){}
        virtual void newAIO(int , QString , QString , double , double , double , double , double , double , QVariant ){}

        virtual void newFC(int , QString , QString , double , double , double , double , double , double , double , int , int , QVariant ){}
        virtual void newAV(int , QString , QString , double , double , double , double , double , double , QVariant ){}

        virtual void newDO(int , QString , bool , QVariant ){}
        virtual void newDI(int , QString , bool , QVariant ){}
        virtual void newDIO(int , QString , bool , bool , QVariant ){}

        virtual void newIpAddress(QString ){}

        virtual void updateAO(int , double, QVariant = QVariant() ){}
        virtual void updateAI(int ,  double, QVariant = QVariant() ){}
        virtual void updateAIO(int , double , double, QVariant = QVariant() ){}

        virtual void updateFC(int , double , double, QVariant = QVariant() ){}
        virtual void updateAV(int , double , double, QVariant = QVariant() ){}

        virtual void updateDO(int , bool, QVariant = QVariant() ){}
        virtual void updateDI(int , bool, QVariant = QVariant() ){}
        virtual void updateDIO(int , bool , bool, QVariant = QVariant() ){}

        virtual void updateSetValue(int , double, qint32 ) {}

        virtual void finishedParsing(QList<int> ) {}

        virtual void updateName(QString ) {}

    private slots:
        virtual void fcSetChangeProxy(int entity)=0;
        virtual void fcSetChangeProxy(double value, int id)=0;

        virtual void dvSetChangeProxy(int entity)=0;
        virtual void dvSetChangeProxy(int value, int id)=0;

    private:
        virtual QString doGetName()const = 0;
        virtual backend_type doGetBackendType()const = 0;

        virtual QByteArray doGetInfoCommand() const = 0;
        virtual QByteArray doGetMinimalSetActCommand() const = 0;

    };

} // namespace talorion

#endif // TALORION_ABSTRACT_BACKEND_HPP

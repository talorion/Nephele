#ifndef JSONRPCBACKEND_HPP
#define JSONRPCBACKEND_HPP

#include "../abstract_backend.hpp"

#include "abstractresponseparser.hpp"

#include <core/EntityManager.hpp>

#include <tcp_box_system/JsonRpc/jsonrpcrequest.hpp>

namespace talorion {

class JsonRpcBackend : public abstract_backend
{
    Q_OBJECT
public:
    explicit JsonRpcBackend(EntityManager::EntityID box_id, QObject *par = Q_NULLPTR);
    virtual ~JsonRpcBackend();

private:
    Q_DISABLE_COPY(JsonRpcBackend)

public:
    EntityManager::EntityID getBoxId()const{return m_box_id;}

    bool isValid()const{return EntityManager::isValid(getBoxId());}
    QString ip()const{return EntityManager::get_instance()->get_ip_address_component(getBoxId());}
    quint16 port()const{return EntityManager::get_instance()->get_port_component(getBoxId());}
    bool connected()const{return EntityManager::get_instance()->get_connection_state_component(getBoxId());}

    QSharedPointer<JsonRpcRequest> getDefaultRequest();

    QSharedPointer<JsonRpcRequest> getBoxNameRequest();
    QSharedPointer<JsonRpcRequest> getAllMethodsRequest()const;
    QSharedPointer<JsonRpcRequest> getAllRequest();
    QSharedPointer<JsonRpcRequest> getActRequest();

    // abstract_backend interface
    EntityManager::EntityID getGetAllId();
    EntityManager::EntityID getSetSetpointId();
    EntityManager::EntityID getGetActId() const;
    EntityManager::EntityID getSetBoxNameId();
    EntityManager::EntityID getGetBoxNameId();

public slots:
    virtual void processData(QSharedPointer<JsonRpcRequest> request, JsonRpcResponse* response) ;
    virtual void processData(QVariantMap desc, tcpDriverDataTypes::dataType type, int box_id) Q_DECL_OVERRIDE{Q_UNUSED(desc);Q_UNUSED(type);Q_UNUSED(box_id);}
    virtual void logError(QString errorString, int source) Q_DECL_OVERRIDE{Q_UNUSED(errorString);Q_UNUSED(source);}

    void socketConnected();
    void socketDisconnected();

private slots:
    virtual void fcSetChangeProxy(int entity) Q_DECL_OVERRIDE;
    virtual void fcSetChangeProxy(double value, int id) Q_DECL_OVERRIDE;
    virtual void dvSetChangeProxy(int entity) Q_DECL_OVERRIDE{Q_UNUSED(entity);}
    virtual void dvSetChangeProxy(int value, int id) Q_DECL_OVERRIDE{Q_UNUSED(value);Q_UNUSED(id);}

    void nameChangedProxy(EntityManager::EntityID en);

private:
    virtual QString doGetName() const Q_DECL_OVERRIDE{return "JSON RPC Backend";}
    virtual backend_type doGetBackendType() const Q_DECL_OVERRIDE{return backend_type::JSONRPC;}
    virtual QByteArray doGetInfoCommand() const Q_DECL_OVERRIDE{return "getAll";}
    virtual QByteArray doGetMinimalSetActCommand() const Q_DECL_OVERRIDE{return "getAll";}

    EntityManager::EntityID getMethodByName(const QString name)const;

    void blockSetUpdate(int id, qint32 requestId);
    void unblockSetUpdate(int id, qint32 requestId);
    void unblockSetUpdate(qint32 requestId);
    bool isSetUpdateBlocked(int id);

    void receivedGetAll() ;

private:
    const EntityManager::EntityID m_box_id;
    EntityManager::EntityID m_getAllMethodsId;
    EntityManager::EntityID m_getAllId;
    EntityManager::EntityID m_getActId;
    EntityManager::EntityID m_setSetpointId;
    EntityManager::EntityID m_setBoxNameId;
    EntityManager::EntityID m_getBoxNameId;

    QMap<QString, AbstractResponseParser*> m_methodParsers;
    AbstractResponseParser* m_fallbackParser;
    AbstractResponseParser* m_genericParser;

    QString m_lastNameRead;
    QMap<int, double> analog_actbuffer;
    QMap<int, double> analog_setbuffer;
    QMap<int, int> digital_actbuffer;
    QMap<int, int> digital_setbuffer;

    QMap<int, qint32> blockedSetValues;

    int m_sendGetAct;

    // abstract_backend interface
public slots:
    virtual EntityManager::EntityID newJsonRpcMethod(const QString &methodname, const JrpcDatatypeList &paramTypes, const QStringList &paramNames) Q_DECL_OVERRIDE;

    virtual void newAO(int id, QString name, QString units, double set, double smin, double smax, QVariant metadata)Q_DECL_OVERRIDE;
    virtual void newAI(int id, QString name, QString units, double act, double amin, double amax, QVariant metadata)Q_DECL_OVERRIDE;
    virtual void newAIO(int id, QString name, QString units, double set, double smin, double smax, double act, double amin, double amax, QVariant metadata)Q_DECL_OVERRIDE;

    virtual void newFC(int id, QString name, QString units, double set, double smin, double smax, double act, double amin, double amax, double T, int read_counter, int error_counter, QVariant metadata)Q_DECL_OVERRIDE;
    virtual void newAV(int id, QString name, QString units, double set, double smin, double smax, double act, double amin, double amax, QVariant metadata)Q_DECL_OVERRIDE;

    virtual void newDO(int id, QString name, bool set, QVariant metadata)Q_DECL_OVERRIDE;
    virtual void newDI(int id, QString name, bool act, QVariant metadata)Q_DECL_OVERRIDE;
    virtual void newDIO(int id, QString name, bool act, bool set, QVariant metadata)Q_DECL_OVERRIDE;

    virtual void newIpAddress(QString newip)Q_DECL_OVERRIDE;

    virtual void finishedParsing(QList<int> allIds) Q_DECL_OVERRIDE;

    virtual void updateName(QString set) Q_DECL_OVERRIDE;


    // abstract_backend interface
public slots:
    virtual void updateAO(int id, double set, QVariant metadata = QVariant()) Q_DECL_OVERRIDE;
    virtual void updateAI(int id, double act, QVariant metadata = QVariant()) Q_DECL_OVERRIDE;
    virtual void updateAIO(int id, double set, double act, QVariant metadata = QVariant()) Q_DECL_OVERRIDE;
    virtual void updateFC(int id, double set, double act, QVariant metadata = QVariant()) Q_DECL_OVERRIDE;
    virtual void updateAV(int id, double set, double act, QVariant metadata = QVariant()) Q_DECL_OVERRIDE;
    virtual void updateDO(int id, bool set, QVariant metadata = QVariant()) Q_DECL_OVERRIDE;
    virtual void updateDI(int id, bool act, QVariant metadata = QVariant()) Q_DECL_OVERRIDE;
    virtual void updateDIO(int id, bool act, bool set, QVariant metadata = QVariant()) Q_DECL_OVERRIDE;

    virtual void updateSetValue(int id, double set, qint32 requestId) Q_DECL_OVERRIDE;
};

} // namespace talorion

#endif // JSONRPCBACKEND_HPP

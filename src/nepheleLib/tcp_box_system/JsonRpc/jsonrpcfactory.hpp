#ifndef JSONRPCFACTORY_HPP
#define JSONRPCFACTORY_HPP

#include <QString>
#include <QVariant>

#include "JrpcDatatype.hpp"
//#include "jsonrpcresponse.hpp"
#include "core/EntityManager.hpp"

namespace talorion {

class JsonRpcResponse;
class JsonRpcRequest;
//using JsonRpcRequest = QByteArray;
class JsonRpcMethodDescriptor;
struct JsonRpcError;

struct JsonRpcMethodDescriptorObject{
  const QString methodname;
  const JrpcDatatypeList paramTypes;
  const QStringList paramNames;
};

class JsonRpcFactory
{
private:
    JsonRpcFactory() = delete ;
    JsonRpcFactory( const JsonRpcFactory& ) = delete;
    JsonRpcFactory& operator=( const JsonRpcFactory& ) = delete;
    ~JsonRpcFactory() = delete ;

    static qint32 nextMethodId(){return ++s_nextMethodId;}

    static qint32 generateRequestId(qint16 methodId);

    static JsonRpcError* createError(const QJsonValue& errorval);

public:
    static EntityManager::EntityID createMethodDescriptorEntity(const QString& methodname, const JrpcDatatypeList &paramTypes, const QStringList& paramNames = QStringList(), qint16 methodId=-1);

    static JsonRpcMethodDescriptor* createMethodDescriptor(EntityManager::EntityID  entity);
    static JsonRpcMethodDescriptor* createMethodDescriptor(const QString& methodname, const JrpcDatatypeList &paramTypes, const QStringList& paramNames = QStringList(), qint16 methodId=-1);
    static JsonRpcMethodDescriptor* createMethodDescriptor(const QByteArray& json);
    static JsonRpcMethodDescriptor* createMethodDescriptor(const QVariant& var);

    static JsonRpcMethodDescriptorObject* createMethodDescriptorObject(const QVariant& var);

    static QSharedPointer<JsonRpcRequest> createRequest(const QString& methodname, const QVariant& params, qint32 id, bool internalSource = true);
    static QSharedPointer<JsonRpcRequest> createRequest(const JsonRpcMethodDescriptor& desc, const QVariant& params = QVariant() );
    static QSharedPointer<JsonRpcRequest> createRequest(EntityManager::EntityID method, const QVariant& params = QVariant() );

    static QList<JsonRpcResponse*> createResponses(QByteArray &json);
    static JsonRpcResponse* createResponse(const QByteArray& json);

private:
    static qint16 s_nextMethodId;
    static qint16 s_nextRequestId;
    static bool s_randInit;
};

} // namespace talorion

#endif // JSONRPCFACTORY_HPP

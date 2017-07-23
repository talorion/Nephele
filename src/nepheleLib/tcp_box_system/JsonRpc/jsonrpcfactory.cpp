#include "jsonrpcfactory.hpp"


#include "jsonrpcmethoddescriptor.hpp"
#include "jsonrpcresponse.hpp"

#include <QDebug>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QTime>
#include <QVariantMap>
#include "jsonrpcrequest.hpp"
#include <core/ManagerLocator.hpp>

namespace talorion {

  qint16 JsonRpcFactory::s_nextMethodId = 0;
  qint16 JsonRpcFactory::s_nextRequestId = 0;
  bool JsonRpcFactory::s_randInit = false;

  qint32 JsonRpcFactory::generateRequestId(qint16 methodId)
  {
    if(s_randInit == false){
        qsrand(static_cast<quint64>(QTime::currentTime().msecsSinceStartOfDay()));
        s_randInit = true;
      }

    qint16 randomValue = qrand() % 0xFFFF;
    qint32 requestId = (qint32)( (qint32)randomValue<<16 | (qint32)methodId );

    return requestId;

//    Q_UNUSED(methodId);

//    if(s_nextRequestId == 0)
//      s_nextRequestId = 1;

//    return ++s_nextRequestId;
  }

  EntityManager::EntityID JsonRpcFactory::createMethodDescriptorEntity(const QString &methodname, const JrpcDatatypeList &paramTypes, const QStringList &paramNames, qint16 methodId)
  {
    EntityManager& mng = ManagerLocator::entityManager();

    auto new_id = mng.createNewEntity();
    mng.createComponentAndAddTo(NAME_COMPONENT, new_id );
    mng.createComponentAndAddTo(PARAMETER_TYPES_COMPONENT, new_id );
    mng.createComponentAndAddTo(PARAMETER_NAMES_COMPONENT, new_id );
    mng.createComponentAndAddTo(METHOD_ID_COMPONENT, new_id );
    mng.createComponentAndAddTo(SERIAL_VERSION_UID_COMPONENT, new_id );

    if(methodId < 0)
      methodId = nextMethodId();

    mng.setComponentDataForEntity(NAME_COMPONENT,               new_id, methodname);
    auto tmp = QVariant::fromValue(paramTypes);
    mng.setComponentDataForEntity(PARAMETER_TYPES_COMPONENT,               new_id, tmp);
    mng.setComponentDataForEntity(PARAMETER_NAMES_COMPONENT,               new_id, paramNames);
    mng.setComponentDataForEntity(METHOD_ID_COMPONENT,               new_id, methodId);
    mng.setComponentDataForEntity(SERIAL_VERSION_UID_COMPONENT, new_id, mng.get_JsonMethodDescriptor_uid());

    return new_id;
  }

  JsonRpcMethodDescriptor *JsonRpcFactory::createMethodDescriptor(EntityManager::EntityID entity)
  {
    //EntityManager& mng = ManagerLocator::entityManager();
    if(ManagerLocator::entityManager().isValid(entity) == false)
      return Q_NULLPTR;

    auto desc = new JsonRpcMethodDescriptor(entity);
    return desc;
  }

  JsonRpcMethodDescriptor *JsonRpcFactory::createMethodDescriptor(const QString &methodname, const JrpcDatatypeList &paramTypes, const QStringList& paramNames, qint16 methodId)
  {

    //    //desc->m_methodName = methodname;
    //    desc->setMethodName(methodname);
    //    //desc->m_paramTypes = paramTypes;
    //    desc->setParamTypes(paramTypes);
    //    //desc->m_paramNames = paramNames;
    //    desc->setParamNames(paramNames);

    //    //desc->m_methodId   = methodId;
    //    desc->setMethodId(methodId);
    auto id = createMethodDescriptorEntity(methodname, paramTypes, paramNames, methodId);
    return createMethodDescriptor(id);
  }

  JsonRpcMethodDescriptor *JsonRpcFactory::createMethodDescriptor(const QByteArray &json)
  {
    auto doc = QJsonDocument::fromJson(json);

    if(doc.isNull())
      return Q_NULLPTR;

    //{"name": "setIP", "params": {"addr1": 21, "addr2": 21, "addr3": 21, "addr4": 21}}
    auto desc_obj = doc.object();
    if(desc_obj.isEmpty())
      return Q_NULLPTR;

    auto methodnameval = desc_obj["name"];
    if(methodnameval == QJsonValue::Undefined )
      return Q_NULLPTR;

    auto methodname = methodnameval.toString();

    auto paramTypeseval = desc_obj["params"];
    if(paramTypeseval == QJsonValue::Undefined )
      return Q_NULLPTR;

    auto paramTypeseobj = paramTypeseval.toArray();
    QStringList paramNames;
    JrpcDatatypeList paramTypes;
    foreach (auto elem, paramTypeseobj) {
        auto elemObj = elem.toObject();
        if(elemObj.isEmpty())
          continue;

        if(elemObj.size() != 1)
          continue;
        auto name = elemObj.keys().at(0);
        auto elemtype = elemObj.value(name);
        if(elemtype.isDouble() == false)
          continue;

        auto type_dbl = elemtype.toDouble(JRPCT_VOID);
        JrpcDataTypeId type = (JrpcDataTypeId)(int) type_dbl;
        if(type == JRPCT_VOID)
          continue;

        paramTypes.append(type);
        paramNames.append(name);
      }

    qint16 methodId = nextMethodId();

    return createMethodDescriptor(methodname, paramTypes, paramNames, methodId);
  }

  JsonRpcMethodDescriptor *JsonRpcFactory::createMethodDescriptor(const QVariant &var)
  {
    //    if(var.canConvert<QVariantMap>() == false)
    //        return Q_NULLPTR;

    //    auto varMap = var.value<QVariantMap>();

    //    if(varMap.contains("name")==false)
    //        return Q_NULLPTR;

    //    if(varMap.contains("params")==false)
    //        return Q_NULLPTR;

    //    QStringList paramNames;
    //    JrpcDatatypeList paramTypes;
    //    auto paramsMember = varMap["params"];
    //    if(paramsMember.canConvert<QVariantList>() == true){

    //        auto paramsList = paramsMember.value<QVariantList>();
    //        foreach (auto param, paramsList) {
    //            if(param.canConvert<QVariantMap>() == false)
    //                continue;

    //            auto paramsMap = param.value<QVariantMap>();

    //            for(auto it=paramsMap.begin(); it !=paramsMap.end(); ++it){
    //                auto name = it.key();
    //                auto type_var = it.value();
    //                if(type_var.canConvert<JrpcDataTypeId>()==false)
    //                    continue;

    //                auto type = type_var.value<JrpcDataTypeId>();

    //                paramTypes.append(type);
    //                paramNames.append(name);

    //            }
    //        }
    //    }
    //    auto nameMember = varMap["name"];
    //    auto methodname = nameMember.toString();

    auto obj = createMethodDescriptorObject(var);
    if(obj == Q_NULLPTR)
      return Q_NULLPTR;

    auto methodname = obj->methodname;
    auto paramTypes = obj->paramTypes;
    auto paramNames = obj->paramNames;

    delete obj;

    qint16 methodId = nextMethodId();

    return createMethodDescriptor(methodname, paramTypes, paramNames, methodId);
  }

  JsonRpcMethodDescriptorObject* JsonRpcFactory::createMethodDescriptorObject(const QVariant &var)
  {
    if(var.canConvert<QVariantMap>() == false)
      return Q_NULLPTR;

    auto varMap = var.value<QVariantMap>();

    if(varMap.contains("name")==false)
      return Q_NULLPTR;

    if(varMap.contains("params")==false)
      return Q_NULLPTR;

    QStringList paramNames;
    JrpcDatatypeList paramTypes;
    auto paramsMember = varMap["params"];
    if(paramsMember.canConvert<QVariantList>() == true){

        auto paramsList = paramsMember.value<QVariantList>();
        foreach (auto param, paramsList) {
            if(param.canConvert<QVariantMap>() == false)
              continue;

            auto paramsMap = param.value<QVariantMap>();

            for(auto it=paramsMap.begin(); it !=paramsMap.end(); ++it){
                auto name = it.key();
                auto type_var = it.value();
                if(type_var.canConvert<JrpcDataTypeId>()==false)
                  continue;

                auto type = type_var.value<JrpcDataTypeId>();

                paramTypes.append(type);
                paramNames.append(name);

              }
          }
      }
    auto nameMember = varMap["name"];
    auto methodname = nameMember.toString();


    return new JsonRpcMethodDescriptorObject{methodname, paramTypes, paramNames};
  }

  QSharedPointer<JsonRpcRequest> JsonRpcFactory::createRequest(const QString& methodname, const QVariant& params, qint32 id, bool internalSource)
  {
    QJsonObject jrpcObject;
    jrpcObject["jsonrpc"] = QStringLiteral("2.0");
    jrpcObject["method"] = methodname;
    if((params.isValid()==true) && (params.isNull()==false)){
        //qDebug()<< Q_FUNC_INFO <<params;
        jrpcObject["params"] = QJsonValue::fromVariant(params);
      }

    if(id == 0){
        jrpcObject["zid"] = QJsonValue();
      }else{
        jrpcObject["zid"] = id;
      }
    QJsonDocument doc(jrpcObject);

    auto req = new JsonRpcRequest(doc.toJson(QJsonDocument::Compact));
    if(id == 0){
        req->m_id = QString("null");
      }else{
        req->m_id = QString::number(id);
      }

    //dirty workarround to keep id as last element
    req->replace("\"zid\":", "\"id\":");
    req->m_methodId  = 0;
    req->m_requestId = id;
    req->m_methodName = methodname;
    req->m_internalSource = internalSource;

    return QSharedPointer<JsonRpcRequest>(req);
  }

  QSharedPointer<JsonRpcRequest> JsonRpcFactory::createRequest(const JsonRpcMethodDescriptor &desc, const QVariant &params)
  {
    //qDebug()<< Q_FUNC_INFO <<params;

    auto methodname = desc.methodName();
    auto methodid   = desc.methodId();

    qint32 requestId = generateRequestId(methodid);

    auto tmp = createRequest(methodname, params, requestId);
    tmp->m_methodId = methodid;

    return tmp;
  }

  QSharedPointer<JsonRpcRequest> JsonRpcFactory::createRequest(EntityManager::EntityID method, const QVariant &params)
  {
    //EntityManager& mng = ManagerLocator::entityManager();
    if(ManagerLocator::entityManager().isValid(method) == false)
      return Q_NULLPTR;

    auto desc = new JsonRpcMethodDescriptor(method);
    if(desc == Q_NULLPTR)
      return Q_NULLPTR;

    //qDebug()<< Q_FUNC_INFO <<params;
    auto req = createRequest(*desc, params);

    delete desc;

    return req;
  }

  QList<JsonRpcResponse *> JsonRpcFactory::createResponses( QByteArray &json)
  {


    //auto openB = json.count("{\"jsonrpc\": \"2.0\",");
    //auto openB = json.count('{');
    //auto closeB = json.count('}');

    QList<JsonRpcResponse *> ret;

    //if(openB != closeB)
    //  return ret;

    QList<int> startpoints;
    QList<int> endpoints;
    //===================
    int j = 0;
    int i = 0;
    //foreach (auto ch, json) {
    j = json.indexOf("{");
    if(j<0)
      return ret;
    for( ;j<json.size(); j++){
        auto ch = json[j];
        if(ch == '{'){
            if(i == 0){startpoints << j;}
            i++;
          }
        if(ch == '}'){
            i--;
            if(i == 0){endpoints << j;}
          }
      }
    //===================

    //===================
//        int st = -1;
//        while ((st = json.indexOf("{\"jsonrpc\":", st)) != -1) {
//            startpoints<<st;
//            ++st;
//          }

//        foreach (st, startpoints) {
//            if(st<0)
//              continue;
//            if(st>json.size())
//              continue;
//            int cnt = 1;
//            int end = -1;
//            for(int i = st; i<json.size(); i++){
//                auto ch = json[i];
//                if(ch == '{'){cnt++;}
//                if(ch == '}'){cnt--;}
//                if(cnt == 0){end = i; break;}
//              }
//            endpoints << end;
//          }
    //===================

    //QByteArray tmp(json);
    if(startpoints.length() == 0 || endpoints.length() == 0 )
      return ret;

    for(int k=0; k<startpoints.size(); k++){
        if((0 <= k && k < startpoints.size())== false )
          continue;
        if((0 <= k && k < endpoints.size())== false )
          continue;

        auto sp = startpoints.at(k);
        auto ep = endpoints.at(k);

        if(sp<0)
          continue;

        if(ep<0)
          continue;

        auto resp = createResponse(json.mid(sp, ep-sp+1));
        if(resp == Q_NULLPTR )
          continue;

        ret.append(resp);
        //json.remove()
      }

    int tmp =0;
    foreach (auto e, endpoints) {
        tmp = qMax(tmp, e);
      }
    if(tmp>0)
      json.remove(0, tmp-0+1);

    return ret;
  }

  JsonRpcResponse *JsonRpcFactory::createResponse(const QByteArray &json)
  {

    QJsonParseError err;
    auto doc = QJsonDocument::fromJson(json, &err);

    if(doc.isNull()){
        //qDebug()<<err.errorString() << err.offset;
        return Q_NULLPTR;
      }

    auto desc_obj = doc.object();
    if(desc_obj.isEmpty())
      return Q_NULLPTR;

    auto jsonrpceval = desc_obj["jsonrpc"];
    if(jsonrpceval == QJsonValue::Undefined )
      return Q_NULLPTR;

    auto jsonrpc = jsonrpceval.toString();
    if(jsonrpc != QString("2.0"))
      return Q_NULLPTR;

    //auto resultval = desc_obj["result"];
    auto resultval = desc_obj.value("result");
    auto errorval = desc_obj["error"];
    if(resultval == QJsonValue::Undefined && errorval == QJsonValue::Undefined)
      return Q_NULLPTR;

    //Either the result member or error member MUST be included, but both members MUST NOT be included.
    //if(resultval != QJsonValue::Undefined && errorval != QJsonValue::Undefined)
    //  return Q_NULLPTR;

    auto idval = desc_obj["id"];
    if(idval == QJsonValue::Undefined )
      return Q_NULLPTR;

    JsonRpcError* error = createError(errorval);
    QVariant result;
    if(resultval != QJsonValue::Undefined){
        result = resultval.toVariant();
      }

    QString id;
    if(idval.isString()){
        id = idval.toString();
        //}else if(idval.isDouble()){
        //    auto dbl = idval.toDouble();
        //    id = QString::number(dbl);
      }else{
        auto dbl = idval.toInt();
        id = QString::number(dbl);
      }


    JsonRpcResponse * resp =  new JsonRpcResponse(json);
    resp->setError(error);
    resp->setId(id);
    resp->setResult(result);

    return resp;
  }

  JsonRpcError *JsonRpcFactory::createError(const QJsonValue &errorval)
  {
    if(errorval == QJsonValue::Undefined)
      return Q_NULLPTR;

    auto errorObj = errorval.toObject();
    if(errorObj.isEmpty())
      return Q_NULLPTR;

    auto codeval = errorObj["code"];
    if(codeval == QJsonValue::Undefined )
      return Q_NULLPTR;

    if(errorObj.contains("message") == false)
      return Q_NULLPTR;

    //auto messageval = errorObj["message"];
    auto messageval = errorObj.value("message");
    if(messageval == QJsonValue::Undefined )
      return Q_NULLPTR;

    auto dataval = errorObj["data"];
    if(dataval == QJsonValue::Undefined )
      return Q_NULLPTR;

    if(messageval.isObject() == true)
      return Q_NULLPTR;

    qint32 code = codeval.toInt();
    QString message = messageval.toString();
    QVariantMap data;
    auto dataObj = dataval.toObject();
    if(dataObj.isEmpty() == false)
      data = dataObj.toVariantMap();

    JsonRpcError* error = new JsonRpcError{code, message, data};
    return error;

  }

} // namespace talorion

#include "JsonRpcBackend.hpp"


#include "boxnameresponseparser.hpp"
#include "getactresponseparser.hpp"
#include "getallmethodsresponseparser.hpp"
#include "getallresponseparser.hpp"
#include "setipresponseparser.hpp"
#include "setsetpointresponseparser.hpp"

#include <tcp_box_system/TcpBoxEntity/TcpBoxEntityFacorty.hpp>
#include <tcp_box_system/JsonRpcDriver/jsonrpcdriverutils.hpp>
#include <tcp_box_system/JsonRpc/jsonrpcfactory.hpp>
#include <tcp_box_system/JsonRpc/jsonrpcresponse.hpp>

namespace talorion {

  JsonRpcBackend::JsonRpcBackend(EntityManager::EntityID box_id, QObject *par) :
    abstract_backend(par),
    m_box_id(box_id),
    m_getAllMethodsId(EntityManager::invalid_id),
    m_getAllId(EntityManager::invalid_id),
    m_getActId(EntityManager::invalid_id),
    m_setSetpointId(EntityManager::invalid_id),
    m_setBoxNameId(EntityManager::invalid_id),
    m_getBoxNameId(EntityManager::invalid_id),
    m_methodParsers(),
    m_fallbackParser(Q_NULLPTR),
    m_genericParser(Q_NULLPTR),
    m_lastNameRead(),
    analog_actbuffer(),
    analog_setbuffer(),
    digital_actbuffer(),
    digital_setbuffer(),
    blockedSetValues(),
    m_sendGetAct(0)
  {
    AbstractResponseParser* gamParser = new GetAllMethodsResponseParser();
    m_methodParsers.insert("getAllMethods", gamParser);
    gamParser->connectToBackend(this);

    AbstractResponseParser* gaParser = new GetAllResponseParser();
    m_methodParsers.insert("getAll", gaParser);
    gaParser->connectToBackend(this);

    AbstractResponseParser* sspParser = new SetSetpointResponseParser();
    m_methodParsers.insert("setSetpoint", sspParser);
    sspParser->connectToBackend(this);

    //    AbstractResponseParser* sspParser = new SetSetpointResponseParser();
    //    m_methodParsers.insert("setSetpoint", sspParser);
    //    sspParser->connectToBackend(this);

    AbstractResponseParser* sipParser = new SetIPResponseParser();
    m_methodParsers.insert("setIP", sipParser);
    sipParser->connectToBackend(this);

    AbstractResponseParser* gactParser = new GetActResponseParser();
    m_methodParsers.insert("getAct", gactParser);
    gactParser->connectToBackend(this);

    AbstractResponseParser* boxnmaeParser = new BoxNameResponseParser();
    m_methodParsers.insert("getBoxName", boxnmaeParser);
    m_methodParsers.insert("setBoxName", boxnmaeParser);
    boxnmaeParser->connectToBackend(this);

    connect(EventManager::get_instance(),SIGNAL(name_component_changed(int)), this, SLOT(nameChangedProxy(EntityManager::EntityID)));

  }

  JsonRpcBackend::~JsonRpcBackend()
  {
//      auto tmp = m_methodParsers;
//      for(auto it=tmp.begin(); it != tmp.end(); ++it){
//          auto pars = it.value();
//          delete pars;
//      }
//      m_methodParsers.clear();
  }

  QSharedPointer<JsonRpcRequest> JsonRpcBackend::getDefaultRequest()
  {
    auto vals = TcpBoxEntityFacorty::getValuesOfBox(getBoxId());
    if(vals.isEmpty())
      return getAllRequest();

    m_sendGetAct++;
    if((m_sendGetAct % 100) == 0 )
      return getAllRequest();

    return getActRequest();
  }

  QSharedPointer<JsonRpcRequest> JsonRpcBackend::getBoxNameRequest()
  {
    auto request = JsonRpcFactory::createRequest(m_getBoxNameId);
    return request;
  }

  QSharedPointer<JsonRpcRequest> JsonRpcBackend::getAllMethodsRequest() const
  {
    auto request = JsonRpcFactory::createRequest(m_getAllMethodsId);
    return request;
  }

  QSharedPointer<JsonRpcRequest> JsonRpcBackend::getAllRequest()
  {
    QSharedPointer<JsonRpcRequest> request;
    if(EntityManager::isValid(getGetAllId()) == true)
      request = JsonRpcFactory::createRequest(getGetAllId());

    return request;
  }

  QSharedPointer<JsonRpcRequest> JsonRpcBackend::getActRequest()
  {
    QSharedPointer<JsonRpcRequest> request;
    if(EntityManager::isValid(getGetActId()) == true)
      request = JsonRpcFactory::createRequest(getGetActId());

    return request;

  }

  void JsonRpcBackend::processData(QSharedPointer<JsonRpcRequest>request, JsonRpcResponse *response)
  {
    if(request.isNull()){
        emit processDataFinished();
        return;
      }

    auto method = request->methodName();
    auto it = m_methodParsers.constFind(method);
    AbstractResponseParser* parser = Q_NULLPTR;
    if(it != m_methodParsers.cend())
      parser = it.value();

    if(parser == Q_NULLPTR)
      parser = m_fallbackParser;

    if(response == Q_NULLPTR){
        //qDebug()<<"no response for method "<<method;
        qDebug()<< Q_FUNC_INFO <<"No response within "<<request->timeout()<<"ms for request "<<request->methodName();
        unblockSetUpdate(request->requestId());
        emit processDataFinished();
        return;
      }

    if(response->isError()){
        qDebug()<<"method "<<method<<" responded with error"<<response->error()->m_message;
        unblockSetUpdate(request->requestId());
        emit processDataFinished();
        return;
      }

    if(parser == Q_NULLPTR){
        qDebug()<<"no parser for method "<<method;
        emit processDataFinished();
        return;
      }

    auto ret = parser->parseResponse(request, response);
    //if(request->methodName() == getAllRequest()->methodName())
    //  receivedGetAll();

    if(ret == true){
        emit processDataFinished();
        return;
      }

    if(m_genericParser == Q_NULLPTR){
        qDebug()<<"no generic parser for method "<<method;
        emit processDataFinished();
        return;
      }

    ret = m_genericParser->parseResponse(request, response);
    if(ret == false){
        qDebug()<<"generic parser failed for "<<method;
        emit processDataFinished();
        return;
      }

    emit processDataFinished();

  }

  void JsonRpcBackend::socketConnected()
  {
    m_getBoxNameId    = newJsonRpcMethod("getBoxName", JrpcDatatypeList(), QStringList());
    m_getAllMethodsId = newJsonRpcMethod("getAllMethods", JrpcDatatypeList(), QStringList());
    newJsonRpcMethod("randomize", JrpcDatatypeList(), QStringList());
    m_getActId        =  newJsonRpcMethod("getAct", JrpcDatatypeList(), QStringList());
    m_getAllId        =  newJsonRpcMethod("getAll", JrpcDatatypeList(), QStringList());

    emit sendCommand(getAllMethodsRequest());
    emit sendCommand(getBoxNameRequest());
    emit sendCommand(getAllRequest());
    emit sendCommand(getActRequest());

    EntityManager::get_instance()->slot_connection_state_component(getBoxId(), true);
  }

  void JsonRpcBackend::socketDisconnected()
  {
    EntityManager::get_instance()->slot_connection_state_component(getBoxId(), false);
  }

  void JsonRpcBackend::fcSetChangeProxy(int entity)
  {
    if(EntityManager::isValid(entity)== false )
      return;

    double value = EntityManager::get_instance()->get_analogSetValue_component(entity);
    int       id = EntityManager::get_instance()->get_id_component(entity);

    auto fc = TcpBoxEntityFacorty::getAIOValue(getBoxId(), id);
    auto ao = TcpBoxEntityFacorty::getAOValue(getBoxId(), id);
    if(fc != entity && ao != entity)
      return;

    fcSetChangeProxy(value, id);

  }

  void JsonRpcBackend::fcSetChangeProxy(double value, int id)
  {
    QSharedPointer<JsonRpcRequest> request;
    if(EntityManager::isValid(getSetSetpointId()) == true){
        request = JsonRpcFactory::createRequest(getSetSetpointId(), QVariant(QVariantList{QVariant(id), QVariant(value)}));
      }

    if(request.isNull() == false){
        analog_setbuffer[id] =value;
        emit sendCommand(request);
        blockSetUpdate(id, request->requestId());
      }
  }

  void JsonRpcBackend::nameChangedProxy(EntityManager::EntityID en)
  {
    if(EntityManager::isValid(en) == false)
      return;

    if(EntityManager::isValid(m_box_id) == false)
      return;

    if(m_box_id != en)
      return;

    auto name = ManagerLocator::entityManager().get_name_component(en);
    if(name.isEmpty() || name.isNull())
      return;

    if(name == m_lastNameRead)
      return;

   auto request = JsonRpcFactory::createRequest(getSetBoxNameId(), QVariantList{QVariant::fromValue(name)});
    emit sendCommand(request);

  }

  EntityManager::EntityID JsonRpcBackend::getMethodByName(const QString name) const
  {
    EntityManager& emngr = ManagerLocator::entityManager();
    auto methods = TcpBoxEntityFacorty::getMethodsOfBox(getBoxId());
    foreach (auto methodId, methods) {
        QString tmp = emngr.get_name_component(methodId);
        if(tmp == name)
          return methodId;
      }
    return EntityManager::invalid_id;
  }

  void JsonRpcBackend::blockSetUpdate(int id, qint32 requestId)
  {
    //If there is already an item with the key key, that item's value is replaced with value.
    blockedSetValues.insert(id, requestId);
  }

  void JsonRpcBackend::unblockSetUpdate(int id, qint32 requestId)
  {
    auto blockerIt = blockedSetValues.find(id);

    if(blockerIt == blockedSetValues.end())
      return;

    auto blocker = blockerIt.value();
    if(blocker == requestId)
      blockedSetValues.remove(id);
  }

  void JsonRpcBackend::unblockSetUpdate(qint32 requestId)
  {
    int id = -1;
    for (auto it = blockedSetValues.begin(); it!= blockedSetValues.end(); ++it) {
        auto blocker = it.value();
        if(blocker == requestId){
            id = it.key();
            break;
          }
      }
    if(id != -1 )
      blockedSetValues.remove(id);

  }

  bool JsonRpcBackend::isSetUpdateBlocked(int id)
  {
    return blockedSetValues.contains(id);
  }

  void JsonRpcBackend::receivedGetAll()
  {
    m_sendGetAct = 1;
  }

  EntityManager::EntityID JsonRpcBackend::getGetActId() const
  {
    return m_getActId;
  }

  EntityManager::EntityID JsonRpcBackend::getSetBoxNameId()
  {
    if(EntityManager::isValid(m_setBoxNameId) == false){
        m_setBoxNameId = getMethodByName(QString("setBoxName"));
      }
    return m_setBoxNameId;
  }

  EntityManager::EntityID JsonRpcBackend::getGetBoxNameId()
  {
    if(EntityManager::isValid(m_getBoxNameId) == false){
        m_getBoxNameId = getMethodByName(QString("getBoxName"));
      }
    return m_getBoxNameId;
  }

  EntityManager::EntityID JsonRpcBackend::getSetSetpointId()
  {
    if(EntityManager::isValid(m_setSetpointId) == false){
        m_setSetpointId = getMethodByName(QString("setSetpoint"));
      }
    return m_setSetpointId;
  }

  EntityManager::EntityID JsonRpcBackend::getGetAllId()
  {
    if(EntityManager::isValid(m_getAllId) == false){
        m_getAllId = getMethodByName(QString(getInfoCommand()));
      }
    return m_getAllId;
  }

  EntityManager::EntityID JsonRpcBackend::newJsonRpcMethod(const QString & methodname, const JrpcDatatypeList & paramTypes, const QStringList &paramNames)
  {
    EntityManager& mng = ManagerLocator::entityManager();

    EntityManager::EntityID meth =EntityManager::invalid_id;

    auto methods = TcpBoxEntityFacorty::getMethodsOfBox(getBoxId());
    foreach (auto methodId, methods) {
        QString tmpName = mng.get_name_component(methodId);
        auto tmpParamTypesVar = mng.componentDataForEntity(PARAMETER_TYPES_COMPONENT, methodId);
        if (tmpParamTypesVar.canConvert<JrpcDatatypeList>() == false)
          continue;
        auto tmpParamTypes = tmpParamTypesVar.value<JrpcDatatypeList>();
        auto tmpParamNamesVar = mng.componentDataForEntity(PARAMETER_TYPES_COMPONENT, methodId);
        if (tmpParamNamesVar.canConvert<QStringList>() == false)
          continue;
        auto tmpParamNames = tmpParamNamesVar.value<QStringList>();

        if(methodname == tmpName && paramTypes == tmpParamTypes && paramNames == tmpParamNames)
          return methodId;

      }

    if(meth == EntityManager::invalid_id){
        meth = JsonRpcFactory::createMethodDescriptorEntity(methodname, paramTypes, paramNames);
        TcpBoxEntityFacorty::addMethodToBox(getBoxId(), meth);
      }

    return meth;
  }

  void JsonRpcBackend::newAO(int id, QString name, QString units, double set, double smin, double smax, QVariant metadata)
  {
    auto ao =TcpBoxEntityFacorty::getAOValue(getBoxId(),id);
    if(EntityManager::isValid(ao) == false){
        EntityManager::get_instance()->createNewAnalogOutputValue(name, units,smin,smax,set,id, getBoxId());
        analog_setbuffer.insert(id, -1);
      }

    updateAO(id, set, metadata);
  }

  void JsonRpcBackend::newAI(int id, QString name, QString units, double act, double amin, double amax, QVariant metadata)
  {
    auto ai =TcpBoxEntityFacorty::getAIValue(getBoxId(),id);
    if(EntityManager::isValid(ai) == false){
        EntityManager::get_instance()->createNewAnalogInputValue(name,units,amin,amax,id,getBoxId());
        analog_actbuffer.insert(id,-1);
      }

    updateAI(id, act, metadata);
  }

  void JsonRpcBackend::newAIO(int id, QString name, QString units, double set, double smin, double smax, double act, double amin, double amax, QVariant metadata)
  {
    auto aio =TcpBoxEntityFacorty::getAIOValue(getBoxId(), id);
    if(EntityManager::isValid(aio) == false){
        EntityManager::get_instance()->createNewAnalogValue(name, units,smin,smax, amin,amax,set,id,getBoxId());
        analog_setbuffer.insert(id, -1);
        analog_actbuffer.insert(id, -1);
      }

    updateAIO(id, set, act, metadata);
  }

  void JsonRpcBackend::newFC(int id, QString name, QString units, double set, double smin, double smax, double act, double amin, double amax, double T, int read_counter, int error_counter, QVariant metadata)
  {
    Q_UNUSED(T);
    Q_UNUSED(read_counter);
    Q_UNUSED(error_counter);

    auto fc =TcpBoxEntityFacorty::getAIOValue(getBoxId(),id);
    if(EntityManager::isValid(fc) == false){
        EntityManager::get_instance()->createNewAnalogValue(name, units,smin,smax, amin,amax,set,id,getBoxId());
        analog_setbuffer.insert(id, -1);
        analog_actbuffer.insert(id, -1);
      }

    updateAIO(id, set, act, metadata);

  }

  void JsonRpcBackend::newAV(int id, QString name, QString units, double set, double smin, double smax, double act, double amin, double amax, QVariant metadata)
  {
    auto av =TcpBoxEntityFacorty::getAIOValue(getBoxId(),id);
    if(EntityManager::isValid(av) == false){
        EntityManager::get_instance()->createNewAnalogValue(name, units,smin,smax, amin,amax,set,id,getBoxId());
        analog_setbuffer.insert(id, -1);
        analog_actbuffer.insert(id, -1);
      }

    updateAIO(id, set, act, metadata);
  }

  void JsonRpcBackend::newDO(int id, QString name, bool set, QVariant metadata)
  {
    auto dov =TcpBoxEntityFacorty::getDOValue(getBoxId(),id);
    if(EntityManager::isValid(dov) == false){
        EntityManager::get_instance()->createNewDigitalOutputValue(name,set,id, getBoxId());
        digital_setbuffer.insert(id, !set);
      }

    updateDO(id, set, metadata);

  }

  void JsonRpcBackend::newDI(int id, QString name, bool act, QVariant metadata)
  {
    auto di =TcpBoxEntityFacorty::getDOValue(getBoxId(),id);
    if(EntityManager::isValid(di) == false){
        EntityManager::get_instance()->createNewDigitalInputValue(name,QString(),id,getBoxId());
        digital_actbuffer.insert(id, !act);
      }

    updateDI(id, act, metadata);
  }

  void JsonRpcBackend::newDIO(int id, QString name, bool act, bool set, QVariant metadata)
  {
    auto dio =TcpBoxEntityFacorty::getDOValue(getBoxId(),id);
    if(EntityManager::isValid(dio) == false){
        EntityManager::get_instance()->createNewDigitalValue(name, set,id,getBoxId());
        digital_setbuffer.insert(id, !set);
        digital_actbuffer.insert(id, !set);
      }

    updateDIO(id, act, set, metadata);

  }

  void JsonRpcBackend::newIpAddress(QString newip)
  {
      Q_UNUSED(newip)
  }

  void JsonRpcBackend::finishedParsing(QList<int> allIds)
  {
    auto boxvals = TcpBoxEntityFacorty::getValuesOfBox(getBoxId());

    foreach (auto boxval, boxvals) {
        auto valid = EntityManager::get_instance()->get_id_component(boxval);
        if(allIds.contains(valid))
          continue;

        int tmp = boxval;
        EntityManager::get_instance()->removeEntity(tmp);

      }

  }

  void JsonRpcBackend::updateName(QString set)
  {
    if(m_lastNameRead == set)
      return;
    m_lastNameRead = set;

    emit ManagerLocator::entityManager().slot_change_name_component(getBoxId(), set);
  }

  void JsonRpcBackend::updateAO(int id, double set, QVariant metadata)
  {
    auto ao =TcpBoxEntityFacorty::getAOValue(getBoxId(),id);
    if(EntityManager::isValid(ao) == false)
      return;

    if(isSetUpdateBlocked(id) == true)
      return;

    if(analog_setbuffer[id] != set){
        analog_setbuffer[id] = set;
        emit change_set_value(ao,set);
      }

    EntityManager::get_instance()->slot_change_metadata_component(ao, metadata);

  }

  void JsonRpcBackend::updateAI(int id, double act, QVariant metadata)
  {
    auto ai =TcpBoxEntityFacorty::getAIValue(getBoxId(),id);
    if(EntityManager::isValid(ai) == false)
      return;

    if(analog_actbuffer[id] != act){
        analog_actbuffer[id] = act;
        emit change_act_value(ai,act);
      }

    EntityManager::get_instance()->slot_change_metadata_component(ai, metadata);

  }

  void JsonRpcBackend::updateAIO(int id, double set, double act, QVariant metadata)
  {
    auto aio =TcpBoxEntityFacorty::getAIOValue(getBoxId(), id);
    if(EntityManager::isValid(aio) == false)
      return;

    if(analog_actbuffer[id] != act){
        analog_actbuffer[id] = act;
        emit change_act_value(aio,act);
      }

    if(isSetUpdateBlocked(id) == true)
      return;

    if(analog_setbuffer[id] != set){
        analog_setbuffer[id] = set;
        emit change_set_value(aio,set);
      }

    EntityManager::get_instance()->slot_change_metadata_component(aio, metadata);
  }

  void JsonRpcBackend::updateFC(int id, double set, double act, QVariant metadata)
  {
    auto fc =TcpBoxEntityFacorty::getAIOValue(getBoxId(),id);
    if(EntityManager::isValid(fc) == false)
      return;

    if(analog_actbuffer[id] != act){
        analog_actbuffer[id] = act;
        emit change_act_value(fc,act);
      }

    if(isSetUpdateBlocked(id) == true)
      return;

    if(analog_setbuffer[id] != set){
        analog_setbuffer[id] = set;
        emit change_set_value(fc,set);
      }

    EntityManager::get_instance()->slot_change_metadata_component(fc, metadata);

  }

  void JsonRpcBackend::updateAV(int id, double set, double act, QVariant metadata)
  {
    auto av =TcpBoxEntityFacorty::getAIOValue(getBoxId(),id);
    if(EntityManager::isValid(av) == false)
      return;

    if(analog_actbuffer[id] != act){
        analog_actbuffer[id] = act;
        emit change_act_value(av,act);
      }

    if(isSetUpdateBlocked(id) == true)
      return;

    if(analog_setbuffer[id] != set){
        analog_setbuffer[id] = set;
        emit change_set_value(av,set);
      }

    EntityManager::get_instance()->slot_change_metadata_component(av, metadata);
  }

  void JsonRpcBackend::updateDO(int id, bool set, QVariant metadata)
  {
    auto dov =TcpBoxEntityFacorty::getDOValue(getBoxId(),id);
    if(EntityManager::isValid(dov) == false)
      return;

    if(isSetUpdateBlocked(id) == true)
      return;

    int setInt = (set ? 1 : 0);
    if(digital_setbuffer[id] != setInt){
        digital_setbuffer[id] = setInt;
        emit change_set_value(dov,setInt);
      }

    EntityManager::get_instance()->slot_change_metadata_component(dov, metadata);
  }

  void JsonRpcBackend::updateDI(int id, bool act, QVariant metadata)
  {
    auto di =TcpBoxEntityFacorty::getDOValue(getBoxId(),id);
    if(EntityManager::isValid(di) == false)
      return;

    int actInt = (act ? 1 : 0);
    if(digital_actbuffer[id] != actInt){
        digital_actbuffer[id] = actInt;
        emit change_act_value(di,actInt);
      }

    EntityManager::get_instance()->slot_change_metadata_component(di, metadata);

  }

  void JsonRpcBackend::updateDIO(int id, bool act, bool set, QVariant metadata)
  {
    auto dio =TcpBoxEntityFacorty::getDOValue(getBoxId(),id);
    if(EntityManager::isValid(dio) == false)
      return;

    int actInt = (act ? 1 : 0);
    if(digital_actbuffer[id] != actInt){
        digital_actbuffer[id] = actInt;
        emit change_act_value(dio,actInt);
      }

    if(isSetUpdateBlocked(id) == true)
      return;

    int setInt = (set ? 1 : 0);
    if(digital_setbuffer[id] != setInt){
        digital_setbuffer[id] = setInt;
        emit change_set_value(dio,setInt);
      }

    EntityManager::get_instance()->slot_change_metadata_component(dio, metadata);
  }

  void JsonRpcBackend::updateSetValue(int id, double set, qint32 requestId)
  {
    Q_UNUSED(set);

    auto value = TcpBoxEntityFacorty::getValue(getBoxId(),id);
    if(EntityManager::isValid(value) == false)
      return;

    //int setInt = (set==0 ? 0 : 1);
    //if(digital_setbuffer[id] != set){
    //analog_setbuffer[id] = set;
    //digital_setbuffer[id] = setInt;
    //emit change_set_value(dio,set);
    //}

    unblockSetUpdate(id, requestId);
  }


} // namespace talorion

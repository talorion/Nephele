#include "PVPBackend.hpp"
#include "PVPResponse.hpp"

#include "PVPParameter.hpp"

#include <tcp_box_system/TcpBoxEntity/TcpBoxEntityFacorty.hpp>

namespace talorion {

PVPBackend::PVPBackend(EntityManager::EntityID box_id, QObject *par):
    abstract_backend(par),
    m_box_id(box_id),
    m_setpointIterator(0)
{

}

PVPBackend::~PVPBackend()
{

}

QSharedPointer<PVPRequest> PVPBackend::getDefaultRequest()
{
    return getActRequest();
}

QSharedPointer<PVPRequest> PVPBackend::getActRequest()
{
    m_nextReq++;
    if(m_nextReq == m_parameters.cend()){
        m_nextReq = m_parameters.cbegin();
        //m_setpointIterator++;
        //if(m_setpointIterator > 10)
        //    m_setpointIterator = 0;
    }

    QSharedPointer<PVPParameter> par =  *m_nextReq;

    //if(m_setpointIterator == 0)
    //    return PVPFactory::createReadSetPointRequest(par);

    return PVPFactory::createReadRequest(par);
}

void PVPBackend::processData(QSharedPointer<PVPRequest> request, PVPResponse *response)
{

    if(request.isNull()){
        emit processDataFinished();
        return;
    }

    if(response == Q_NULLPTR){
        emit processDataFinished();
        return;
    }

    auto paraNum = response->parameterNumber();
    QSharedPointer<PVPParameter> param = nullptr;
    for(auto p: m_parameters){
        if(p == nullptr)
            continue;
        if(paraNum ==  p->actParameterNumber())
            param = p;

        if(paraNum ==  p->setParameterNumber())
            param = p;
    }

    if(param.isNull()){
        qDebug()<<"no parser for paraNum "<<paraNum;
        emit processDataFinished();
        return;
    }

    param->parseResponse(request, response);
    emit processDataFinished();
}

void PVPBackend::processData(QVariantMap desc, tcpDriverDataTypes::dataType type, int box_id)
{
    Q_UNUSED(desc);
    Q_UNUSED(type);
    Q_UNUSED(box_id);
}

void PVPBackend::logError(QString errorString, int source)
{
    Q_UNUSED(errorString);
    Q_UNUSED(source);
}

void PVPBackend::socketConnected()
{
    //    m_requestList.clear();
    quint8 boxAddress = EntityManager::get_instance()->get_port_component(getBoxId());
    //    QList<quint16> params{330, 331, 332, 333, 334, 335, 336, 337};
    //    for(auto param: params){
    //        auto xx = PVPFactory::createRequest(boxAddress, PVPAction::PVP_READ, param,QString("----=?"));
    //        m_requestList.push_back(xx);
    //    }
    //    m_nextReq = m_requestList.cbegin();

    //m_getBoxNameId    = newJsonRpcMethod("getBoxName", JrpcDatatypeList(), QStringList());
    //m_getAllMethodsId = newJsonRpcMethod("getAllMethods", JrpcDatatypeList(), QStringList());
    //newJsonRpcMethod("randomize", JrpcDatatypeList(), QStringList());
    //m_getActId        =  newJsonRpcMethod("getAct", JrpcDatatypeList(), QStringList());
    //m_getAllId        =  newJsonRpcMethod("getAll", JrpcDatatypeList(), QStringList());

    //emit sendCommand(getAllMethodsRequest());
    //emit sendCommand(getBoxNameRequest());
    //emit sendCommand(getAllRequest());
    //emit sendCommand(getActRequest());

    auto fc1 = (PVPFactory::createAnalogPCUParameter(getBoxId(), "FC_NO", "sccm", 100,    boxAddress, 330, 700));
    fc1->connectToBackend(this);
    m_parameters.push_back(fc1);
    auto fc2 = (PVPFactory::createAnalogPCUParameter(getBoxId(), "FC_H2O", "sccm", 15,    boxAddress, 331, 701));
    fc2->connectToBackend(this);
    m_parameters.push_back(fc2);
    auto u1 = (PVPFactory::createAnalogPCUParameter(getBoxId(),  "U_QL", "V", 180,  boxAddress, 332, 702));
    u1->connectToBackend(this);
    m_parameters.push_back(u1);
    auto u2 = (PVPFactory::createAnalogPCUParameter(getBoxId(),  "U_NC", "V", 10,   boxAddress, 333, 703));
    u2->connectToBackend(this);
    m_parameters.push_back(u2);
    auto u3 = (PVPFactory::createAnalogPCUParameter(getBoxId(),  "U_drift", "V", 1000, boxAddress, 334, 704));
    u3->connectToBackend(this);
    m_parameters.push_back(u3);
    auto u4 = (PVPFactory::createAnalogPCUParameter(getBoxId(),  "U_Jet", "V", 180,  boxAddress, 335, 705));
    u4->connectToBackend(this);
    m_parameters.push_back(u4);
    auto u5 = (PVPFactory::createAnalogPCUParameter(getBoxId(),  "U_FEX", "V", 300,  boxAddress, 336, 706));
    u5->connectToBackend(this);
    m_parameters.push_back(u5);
    auto i1 = (PVPFactory::createAnalogPCUParameter(getBoxId(),  "I1", "mA", 10, boxAddress, 337, 707));
    i1->connectToBackend(this);
    m_parameters.push_back(i1);
    auto hc = (PVPFactory::createDigitalPCUParameter(getBoxId(),  "HC",  boxAddress,  20, 20));
    hc->connectToBackend(this);
    m_parameters.push_back(hc);
    auto do1 = (PVPFactory::createDigitalPCUParameter(getBoxId(),  "DO1", boxAddress,   0, 0));
    do1->connectToBackend(this);
    m_parameters.push_back(do1);
    auto do2 = (PVPFactory::createDigitalPCUParameter(getBoxId(),  "DO2", boxAddress,   1, 1));
    do2->connectToBackend(this);
    m_parameters.push_back(do2);
    auto do3 = (PVPFactory::createDigitalPCUParameter(getBoxId(),  "DO3", boxAddress,   2, 2));
    do3->connectToBackend(this);
    m_parameters.push_back(do3);
    auto do4 = (PVPFactory::createDigitalPCUParameter(getBoxId(),  "DO4", boxAddress,   3, 3));
    do4->connectToBackend(this);
    m_parameters.push_back(do4);
    auto do5 = (PVPFactory::createDigitalPCUParameter(getBoxId(),  "DO5", boxAddress,   4, 4));
    do5->connectToBackend(this);
    m_parameters.push_back(do5);
    auto do6 = (PVPFactory::createDigitalPCUParameter(getBoxId(),  "DO6", boxAddress,   5, 5));
    do6->connectToBackend(this);
    m_parameters.push_back(do6);
    auto status = (PVPFactory::createMetadataPCUParameter(getBoxId(),  "Status", boxAddress,   310));
    status->connectToBackend(this);
    m_parameters.push_back(status);
    auto devTy = (PVPFactory::createMetadataPCUParameter(getBoxId(),  "DeviceType", boxAddress,   320));
    devTy->connectToBackend(this);
    m_parameters.push_back(devTy);
    auto swVer = (PVPFactory::createMetadataPCUParameter(getBoxId(),  "Software Version", boxAddress,   321));
    swVer->connectToBackend(this);
    m_parameters.push_back(swVer);

    m_nextReq = m_parameters.cbegin();

    for(auto par: m_parameters){
        auto getSetPointRequest = PVPFactory::createReadSetPointRequest(par);
        emit sendCommand(getSetPointRequest);
    }

    EntityManager::get_instance()->slot_connection_state_component(getBoxId(), true);
}

void PVPBackend::socketDisconnected()
{
    EntityManager::get_instance()->slot_connection_state_component(getBoxId(), false);
}

void PVPBackend::updateAIO(int id, double set, double act, QVariant)
{
    //auto aio =TcpBoxEntityFacorty::getAIOValue(getBoxId(), id);
    auto aio = id;
    if(EntityManager::isValid(aio) == false)
        return;

    id = EntityManager::get_instance()->get_id_component(aio);

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
}

void PVPBackend::updateDIO(int id, bool set, bool act, QVariant)
{
    auto aio = id;
    if(EntityManager::isValid(aio) == false)
        return;

    id = EntityManager::get_instance()->get_id_component(aio);

    //if(digital_actbuffer[id] != act){
    //    digital_actbuffer[id] = act;
    emit change_digital_act_value(aio, act);
    //  }

    //if(isSetUpdateBlocked(id) == true)
    //  return;

    //if(digital_setbuffer[id] != set){
    //    digital_setbuffer[id] = set;
    emit change_digital_set_value(aio, set);
    //}
}

void PVPBackend::fcSetChangeProxy(int entity)
{
    if(EntityManager::isValid(entity)== false )
        return;

    double value = EntityManager::get_instance()->get_analogSetValue_component(entity);
    int       id = EntityManager::get_instance()->get_id_component(entity);

    auto fc = TcpBoxEntityFacorty::getAIOValue(getBoxId(), id);
    auto ao = TcpBoxEntityFacorty::getAOValue(getBoxId(), id);
    if(fc != entity && ao != entity)
        return;

    //fcSetChangeProxy(value, id);
    QSharedPointer<PVPParameter> param = nullptr;
    foreach (auto p, m_parameters) {
        if(p->getValue() == entity){
            param = p;
            break;
        }
    }

    if(param.isNull())
        return;

    auto request = PVPFactory::createSetSetpointRequest(param);
    if(request.isNull() == false){
        analog_setbuffer[id] = value;
        emit sendCommand(request);
        blockSetUpdate(id, request->requestId());
    }
}

void PVPBackend::fcSetChangeProxy(double value, int id)
{


    //    QSharedPointer<JsonRpcRequest> request;
    //    //auto req PVPFactory::createRequest()
    //    if(request.isNull() == false){
    //        analog_setbuffer[id] = value;
    //        emit sendCommand(request);
    //        blockSetUpdate(id, request->requestId());
    //    }
}

void PVPBackend::dvSetChangeProxy(int entity)
{
    if(EntityManager::isValid(entity)== false )
        return;

    bool value = EntityManager::get_instance()->get_digitalSetValue_component(entity);
    int       id = EntityManager::get_instance()->get_id_component(entity);

    auto fc = TcpBoxEntityFacorty::getDIOValue(getBoxId(), id);
    auto ao = TcpBoxEntityFacorty::getDOValue(getBoxId(), id);
    if(fc != entity && ao != entity)
        return;

    QSharedPointer<PVPParameter> param = nullptr;
    foreach (auto p, m_parameters) {
        if(p->getValue() == entity){
            param = p;
            break;
        }
    }

    if(param.isNull())
        return;

    auto request = PVPFactory::createSetSetpointRequest(param);
    if(request.isNull() == false){
        analog_setbuffer[id] = value;
        emit sendCommand(request);
        blockSetUpdate(id, request->requestId());
    }
}

void PVPBackend::dvSetChangeProxy(int value, int id)
{
    //Q_UNUSED(value);
    //Q_UNUSED(id);
}

QString PVPBackend::doGetName() const
{
    return QString("PVPBackend");
}

backend_type PVPBackend::doGetBackendType() const
{
    return backend_type::PFEIFFER_VACUUM_PROTOCOL;
}

QByteArray PVPBackend::doGetInfoCommand() const
{
    return "";
}

QByteArray PVPBackend::doGetMinimalSetActCommand() const
{
    return "";
}

void PVPBackend::blockSetUpdate(int id, qint32 requestId)
{
    blockedSetValues.insert(id, requestId);
}

void PVPBackend::unblockSetUpdate(int id, qint32 requestId)
{
    auto blockerIt = blockedSetValues.find(id);

    if(blockerIt == blockedSetValues.end())
        return;

    auto blocker = blockerIt.value();
    if(blocker == requestId)
        blockedSetValues.remove(id);
}

void PVPBackend::unblockSetUpdate(qint32 requestId)
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

bool PVPBackend::isSetUpdateBlocked(int id)
{
    return blockedSetValues.contains(id);
}

} // namespace talorion

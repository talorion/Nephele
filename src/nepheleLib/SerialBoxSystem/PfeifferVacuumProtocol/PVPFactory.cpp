#include "PVPFactory.hpp"
#include "PVPParameter.hpp"
#include "PVPRequest.hpp"
#include "PVPResponse.hpp"

namespace talorion {

int PVPFactory::s_nextParameterId = 0;
quint32 PVPFactory::s_nextRequestId = 0;

QList<PVPResponse *> PVPFactory::createResponses(const QByteArray &buffer)
{
    QList<PVPResponse *> ret;

    bool ok = false;
    auto raw_telegrams = buffer.split(13);//CR
    for(auto telegram: raw_telegrams){
        int i = 0;
        if(telegram.trimmed().size() < 13)
            continue;

        auto address = readIntValue(telegram, &ok,i, 3);
        if(ok == false)
            continue;
        i += 3;

        auto action = readIntValue(telegram, &ok,i, 2);
        if(ok == false)
            continue;
        i += 2;

        auto parameterNumber = readIntValue(telegram, &ok,i, 3);
        if(ok == false)
            continue;
        i += 3;

        auto dataLength  = readIntValue(telegram, &ok,i, 2);
        if(ok == false)
            continue;
        i += 2;

        //auto intdata  = readIntValue(telegram, &ok,i, dataLength);
        //if(ok == false)
        //    continue;
        i += dataLength;

        auto crc        = readIntValue(telegram, &ok,i, 3);
        if(ok == false)
            continue;

        quint8 ch_crc = 0;
        for(int j=0; j<i; j++){
            ch_crc += telegram[j];
        }
        if(crc != ch_crc)
            continue;

        QByteArray raw_data = telegram.mid(10, dataLength);

        PVPResponse * resp = new PVPResponse();
        resp->m_msg = telegram;

        resp->m_adress = address;
        resp->m_action = PVPAction(action);
        resp->m_parameterNumber = parameterNumber;
        resp->m_dataLength = dataLength;
        resp->m_data = raw_data;
        resp->m_crc8 = dataLength;
        resp->m_dataLength = ch_crc;

        ret.push_back(resp);
    }

    return ret;
}

QSharedPointer<PVPRequest> PVPFactory::createRequest(quint8 adress, PVPAction action , quint16 parameterNumber, QVariant data, quint8 dataLength)
{
    QString data_str = data.toString();
    if(dataLength == 0){
        dataLength = data_str.length();
    }

    QString adress_str= QString("%1").arg(QString::number(adress), 3, QLatin1Char('0'));
    QString action_str= QString("%1").arg(QString::number((quint8)action), 2, QLatin1Char('0'));
    QString parameterNumber_str= QString("%1").arg(QString::number(parameterNumber), 3, QLatin1Char('0'));
    QString dataLength_str= QString("%1").arg(QString::number(dataLength), 2, QLatin1Char('0'));
    QString data_str_str= QString("%1").arg(data_str, dataLength, QLatin1Char('0'));
    QString tmp = adress_str + action_str + parameterNumber_str + dataLength_str + data_str_str;

    auto msg = tmp.toLocal8Bit();
    quint8 crc = 0;
    for(auto c: msg){
        crc += c;
    }
    QString crc_str = QString("%1").arg(QString::number(crc), 3, QLatin1Char('0'));
    tmp += crc_str;
    tmp.append(13); //CR

    auto ba =  tmp.toLocal8Bit();

    PVPRequest* req = new PVPRequest();

    req->m_msg = ba;
    req->m_adress = adress;
    req->m_action = action;
    req->m_parameterNumber = parameterNumber;
    req->m_dataLength = dataLength;
    req->m_data = data;
    req->m_crc8 = crc;
    req->m_requestId =s_nextRequestId++;

    return QSharedPointer<PVPRequest>(req);
}

QSharedPointer<PVPRequest> PVPFactory::createReadRequest(QSharedPointer<PVPParameter> parameter)
{
        quint8 adress =parameter->getBoxAddress();
        PVPAction action  = PVPAction::PVP_READ;
        quint16 parameterNumber = parameter->actParameterNumber();
        //if(parameterNumber == )
        QVariant data = QString("----=?");
        quint8 dataLength = 6;
        return createRequest(adress, action, parameterNumber, data, dataLength);
}

QSharedPointer<PVPRequest> PVPFactory::createSetSetpointRequest(QSharedPointer<PVPParameter> parameter)
{
    quint8 adress =parameter->getBoxAddress();
    PVPAction action  = PVPAction::PVP_WRITE;
    quint16 parameterNumber = parameter->setParameterNumber();
    QVariant data = parameter->getSetValueAsString();
    quint8 dataLength = 6;
    return createRequest(adress, action, parameterNumber, data, dataLength);
}

QSharedPointer<PVPRequest> PVPFactory::createReadSetPointRequest(QSharedPointer<PVPParameter> parameter)
{
    quint8 adress =parameter->getBoxAddress();
    PVPAction action  = PVPAction::PVP_READ;
    quint16 parameterNumber = parameter->setParameterNumber();
    //if(parameterNumber == )
    QVariant data = QString("----=?");
    quint8 dataLength = 6;
    return createRequest(adress, action, parameterNumber, data, dataLength);
}

QSharedPointer<PVPParameter> PVPFactory::createAnalogPCUParameter(EntityManager::EntityID box, const QString& parameterName, QString unit,  double mx, quint8 adress, quint16 actParameterNumber, quint16 setParameterNumber)
{
    PVPParameter* par = new PVPParameter();

    par->m_boxAddress = adress;
    par->m_actParameterNumber = actParameterNumber;
    par->m_setParameterNumber = setParameterNumber;
    if(setParameterNumber == 0xFFFF)
        par->m_setParameterNumber = actParameterNumber;

    //int id = (((int)(actParameterNumber))<<16) + setParameterNumber;
    //int id = actParameterNumber;
    int id = s_nextParameterId++;
    auto value = EntityManager::get_instance()->createNewAnalogValue(parameterName, unit, 0, mx, 0, mx, 0, id, box);

    par->m_box = box;
    par->m_value = value;

    return QSharedPointer<PVPParameter>(par);
}

QSharedPointer<PVPParameter> PVPFactory::createDigitalPCUParameter(EntityManager::EntityID box, const QString &parameterName, quint8 adress, quint16 actParameterNumber, quint16 setParameterNumber)
{
    PVPParameter* par = new PVPParameter();

    par->m_boxAddress = adress;
    par->m_actParameterNumber = actParameterNumber;
    par->m_setParameterNumber = setParameterNumber;
    if(setParameterNumber == 0xFFFF)
        par->m_setParameterNumber = actParameterNumber;

    //int id = (((int)(actParameterNumber))<<16) + setParameterNumber;
    int id = s_nextParameterId++;
    auto value = EntityManager::get_instance()->createNewDigitalValue(parameterName, false, id, box);

    par->m_box = box;
    par->m_value = value;

    return QSharedPointer<PVPParameter>(par);
}

QSharedPointer<PVPParameter> PVPFactory::createMetadataPCUParameter(EntityManager::EntityID box, const QString &parameterName, quint8 adress, quint16 actParameterNumber, quint16 setParameterNumber)
{
    PVPParameter* par = new PVPParameter();

    par->m_boxAddress = adress;
    par->m_actParameterNumber = actParameterNumber;
    par->m_setParameterNumber = setParameterNumber;
    if(setParameterNumber == 0xFFFF)
        par->m_setParameterNumber = actParameterNumber;

    //int id = (((int)(actParameterNumber))<<16) + setParameterNumber;
    //int id = s_nextParameterId++;
    //auto value = EntityManager::get_instance()->createNewDigitalValue(parameterName, false, id, box);

    par->m_box = box;
    par->m_value = EntityManager::invalid_id;

    return QSharedPointer<PVPParameter>(par);
}

int PVPFactory::readIntValue(const QByteArray &telegram, bool* ok, int idx, int len)
{
    if(telegram.size() < idx+len){
        *ok = false;
        return 0;
    }

    auto raw_data            = telegram.mid(idx,len);
    QString str_data(raw_data);
    auto data = str_data.toInt(ok);
    if(*ok == false){
        *ok = false;
        return 0;
    }

    return data;
}



} // namespace talorion

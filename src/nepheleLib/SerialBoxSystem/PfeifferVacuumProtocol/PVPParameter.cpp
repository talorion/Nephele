#include "PVPParameter.hpp"

namespace talorion {

PVPParameter::PVPParameter(QObject *par):
    QObject(par),
    m_box(EntityManager::invalid_id),
    m_value(EntityManager::invalid_id)
{

}

void PVPParameter::connectToBackend(abstract_backend *back)
{
    connect(this, SIGNAL(updateAI(int,double)), back, SLOT(updateAI(int,double)));
    connect(this, SIGNAL(updateAIO(int,double,double)), back, SLOT(updateAIO(int,double,double)));
    connect(this, SIGNAL(updateAO(int,double)), back, SLOT(updateAO(int,double)));

    connect(this, SIGNAL(updateAV(int,double,double)),back, SLOT(updateAV(int,double,double)));
    connect(this, SIGNAL(updateFC(int,double,double)),back, SLOT(updateFC(int,double,double)));

    connect(this, SIGNAL(updateDI(int,bool)), back, SLOT(updateDI(int,bool)));
    connect(this, SIGNAL(updateDIO(int,bool,bool)), back, SLOT(updateDIO(int,bool,bool)));
    connect(this, SIGNAL(updateDO(int,bool)), back, SLOT(updateDO(int,bool)));

    connect(this, SIGNAL(finishedParsing(QList<int>)),back, SLOT(finishedParsing(QList<int>)));

}

bool PVPParameter::parseResponse(QSharedPointer<PVPRequest> request, PVPResponse *response)
{
    if(response == nullptr)
        return false;

    quint16 paranum = response->parameterNumber();

    if(paranum != actParameterNumber() && paranum != setParameterNumber())
        return false;

    auto raw_data = response->data();
    //auto data = response->data();
    //if(data.canConvert(QString) == false)
    //    return false;

    //QVariant raw_data = data.toString();
    //int raw_data_int = raw_data.toInt(&ok, 16);

    if(isAnalogValue())
        return parseAnalogValue(paranum, raw_data);

    if(isDigitalValue())
        return parseDigitalValue(paranum, raw_data);

    if(m_value == EntityManager::invalid_id)
        return parseMetaValue(paranum, raw_data);

    return false;
}

quint16 PVPParameter::actParameterNumber() const
{
    return m_actParameterNumber;
}

quint16 PVPParameter::setParameterNumber() const
{
    return m_setParameterNumber;
}

bool PVPParameter::isDigitalValue() const
{
    return EntityManager::get_instance()->is_digital_value(m_value);
}

bool PVPParameter::isAnalogValue() const
{
    return EntityManager::get_instance()->is_analog_value(m_value);
}

bool PVPParameter::parseDigitalValue(quint16 paranum, const QVariant& raw_data)
{
    //bool ok;
    auto raw_data_str = raw_data.toString();

    bool data = getDigActValue();

    if(raw_data_str == "111111")
        data = true;

    if(raw_data_str == "000000")
        data = false;

    bool set = getDigSetValue();
    bool act = getDigActValue();

    if(paranum == actParameterNumber()){
        act = data;
    }

    if(paranum == setParameterNumber()){
        set = data;
    }

    emit updateDIO(m_value, set, act);
    return true;
}

bool PVPParameter::parseAnalogValue(quint16 paranum, const QVariant& raw_data)
{
    bool ok;
    auto raw_data_str = raw_data.toString();
    auto raw_data_int = raw_data_str.toInt(&ok, 16);
    if(ok == false)
        return false;

    double data = (raw_data_int*1.0)/4000.0*getMaxValue();

    double set = getSetValue();
    double act = getActValue();

    if(paranum == actParameterNumber()){
        act = data;
    }

    if(paranum == setParameterNumber()){
        set = data;
    }

    emit updateAIO(m_value, set, act);
    return true;
}

bool PVPParameter::parseMetaValue(quint16 paranum, const QVariant &raw_data)
{
    QVariantMap metadata;

    QVariant metadataVar = EntityManager::get_instance()->get_metadata_component(m_box);
    if(metadataVar.canConvert<QVariantMap>())
        metadata = metadataVar.value<QVariantMap>();

    switch(paranum){
    case 310:{
        auto ba = raw_data.toByteArray();
        if(ba.size()!=6)
            return false;
        QStringList names {"EP", "EEP", "TL", "TH", "SC", "HC"};

        int i=0;
        for(auto name: names){
            metadata.insert(name, (bool)((ba[i]) == '1'));
            i++;
        }
        break;
    }
    case 320:{
        QString devTy = raw_data.toString();
        metadata.insert("Device Type", devTy);
        break;
    }
    case 321:{
        QString swVer = raw_data.toString();
        metadata.insert("Software Version", swVer);
        break;
    }
    default:{break;}
    }

    EntityManager::get_instance()->slot_change_metadata_component(m_box, metadata);
    return true;
}

double PVPParameter::getMaxValue() const
{
    return EntityManager::get_instance()->get_setMax_component(m_value);
}

double PVPParameter::getActValue() const
{
    return EntityManager::get_instance()->get_analogActValue_component(m_value);
}

double PVPParameter::getSetValue() const
{
    return EntityManager::get_instance()->get_analogSetValue_component(m_value);
}

bool PVPParameter::getDigActValue() const
{
    return EntityManager::get_instance()->get_digitalActValue_component(m_value);
}

bool PVPParameter::getDigSetValue() const
{
    return EntityManager::get_instance()->get_digitalSetValue_component(m_value);
}

EntityManager::EntityID PVPParameter::getValue() const
{
    return m_value;
}

QString PVPParameter::getSetValueAsString() const
{
    if(isAnalogValue()){
        int setValueRaw = getSetValue()*4000.0/getMaxValue();

        return  QString("%1").arg(QString::number(setValueRaw, 16), 6, QLatin1Char('0'));
    }

    if(isDigitalValue()){
        bool setValueRaw = getDigSetValue();
        if(setValueRaw == true)
            return QString("111111");
        else
            return QString("000000");
        //return  QString("%1").arg(QString::number(setValueRaw), 6, QLatin1Char('0'));
    }

    return "_RANGE";
}

quint8 PVPParameter::getBoxAddress() const
{
    return m_boxAddress;
}



} // namespace talorion

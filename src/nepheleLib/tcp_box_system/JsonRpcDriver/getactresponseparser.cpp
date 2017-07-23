#include "getactresponseparser.hpp"

#include <tcp_box_system/JsonRpc/jsonrpcresponse.hpp>

namespace talorion {
GetActResponseParser::GetActResponseParser(QObject *par) :
    AbstractResponseParser(par),
    allIds()
{

}

void GetActResponseParser::doConnectToBackend(abstract_backend *back)
{
    clearAllIds();

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

bool GetActResponseParser::doParseResponse(QSharedPointer<JsonRpcRequest> request, JsonRpcResponse *response)
{
    clearAllIds();

    Q_UNUSED(request);
    auto varia = response->result();

    if(varia.canConvert<QVariantMap>() == false)
        return false;
    auto varMap= varia.value<QVariantMap>();

    QVariantList aoList;
    if(varMap.contains("AO") == true){
        auto var = varMap.value("AO");
        aoList = var.value<QVariantList>();
    }
    foreach (auto elem, aoList) {
        parseAO(elem);
    }

    QVariantList aiList;
    if(varMap.contains("AI") == true){
        auto var = varMap.value("AI");
        aiList = var.value<QVariantList>();
    }
    foreach (auto elem, aiList) {
        parseAI(elem);
    }

    QVariantList aioList;
    if(varMap.contains("AIO") == true){
        auto var = varMap.value("AIO");
        aioList = var.value<QVariantList>();
    }
    foreach (auto elem, aioList) {
        parseAIO(elem);
    }

    QVariantList fcList;
    if(varMap.contains("FC") == true){
        auto var = varMap.value("FC");
        fcList = var.value<QVariantList>();
    }
    foreach (auto elem, fcList) {
        parseFC(elem);
    }

    QVariantList avList;
    if(varMap.contains("AV") == true){
        auto var = varMap.value("AV");
        avList = var.value<QVariantList>();
    }
    foreach (auto elem, avList) {
        parseAV(elem);
    }

    QVariantList doList;
    if(varMap.contains("DO") == true){
        auto var = varMap.value("DO");
        doList = var.value<QVariantList>();
    }
    foreach (auto elem, doList) {
        parseDO(elem);
    }

    QVariantList diList;
    if(varMap.contains("DI") == true){
        auto var = varMap.value("DI");
        diList = var.value<QVariantList>();
    }
    foreach (auto elem, diList) {
        parseDI(elem);
    }

    QVariantList dioList;
    if(varMap.contains("DIO") == true){
        auto var = varMap.value("DIO");
        dioList = var.value<QVariantList>();
    }
    foreach (auto elem, dioList) {
        parseDIO(elem);
    }

    emit finishedParsing(allIds);

    return true;
}

void GetActResponseParser::parseAO(QVariant var)
{
    if(var.canConvert<QVariantMap>() == false)
        return ;
    auto varMap= var.value<QVariantMap>();

    //id
    if(varMap.contains("i") == false)
        return;
    auto varVal = varMap.value("i");
    auto id = varVal.value<int>();

    //set
    if(varMap.contains("s") == false)
        return;
    varVal = varMap.value("s");
    auto set = varVal.value<double>();

    emit updateAO(id,set);

    allIds.append(id);
}

void GetActResponseParser::parseAI(QVariant var)
{
    if(var.canConvert<QVariantMap>() == false)
        return ;
    auto varMap= var.value<QVariantMap>();

    //id
    if(varMap.contains("i") == false)
        return;
    auto varVal = varMap.value("i");
    auto id = varVal.value<int>();

    //act
    if(varMap.contains("a") == false)
        return;
    varVal = varMap.value("a");
    auto act = varVal.value<double>();

    emit updateAI(id,act);
    allIds.append(id);
}

void GetActResponseParser::parseAIO(QVariant var)
{
    if(var.canConvert<QVariantMap>() == false)
        return ;
    auto varMap= var.value<QVariantMap>();

    //id
    if(varMap.contains("i") == false)
        return;
    auto varVal = varMap.value("i");
    auto id = varVal.value<int>();

    //set
    if(varMap.contains("s") == false)
        return;
    varVal = varMap.value("s");
    auto set = varVal.value<double>();

    //act
    if(varMap.contains("a") == false)
        return;
    varVal = varMap.value("a");
    auto act = varVal.value<double>();

    emit updateAIO(id, set, act);
    allIds.append(id);
}

void GetActResponseParser::parseFC(QVariant var)
{
    if(var.canConvert<QVariantMap>() == false)
        return ;
    auto varMap= var.value<QVariantMap>();

    //id
    if(varMap.contains("i") == false)
        return;
    auto varVal = varMap.value("i");
    auto id = varVal.value<int>();

    //set
    if(varMap.contains("s") == false)
        return;
    varVal = varMap.value("s");
    auto set = varVal.value<double>();

    //act
    if(varMap.contains("a") == false)
        return;
    varVal = varMap.value("a");
    auto act = varVal.value<double>();

    emit updateFC(id,set, act);

    allIds.append(id);
}

void GetActResponseParser::parseAV(QVariant var)
{
    if(var.canConvert<QVariantMap>() == false)
        return ;
    auto varMap= var.value<QVariantMap>();

    //id
    if(varMap.contains("i") == false)
        return;
    auto varVal = varMap.value("i");
    auto id = varVal.value<int>();

    //set
    if(varMap.contains("s") == false)
        return;
    varVal = varMap.value("s");
    auto set = varVal.value<double>();

    //act
    if(varMap.contains("a") == false)
        return;
    varVal = varMap.value("a");
    auto act = varVal.value<double>();

    emit updateAV(id, set, act);
    allIds.append(id);
}

void GetActResponseParser::parseDO(QVariant var)
{
    if(var.canConvert<QVariantMap>() == false)
        return ;
    auto varMap= var.value<QVariantMap>();

    //id
    if(varMap.contains("i") == false)
        return;
    auto varVal = varMap.value("i");
    auto id = varVal.value<int>();

    //set
    if(varMap.contains("s") == false)
        return;
    varVal = varMap.value("s");
    auto set = varVal.value<bool>();

    emit updateDO(id, set);
    allIds.append(id);
}

void GetActResponseParser::parseDI(QVariant var)
{
    if(var.canConvert<QVariantMap>() == false)
        return ;
    auto varMap= var.value<QVariantMap>();

    //id
    if(varMap.contains("i") == false)
        return;
    auto varVal = varMap.value("i");
    auto id = varVal.value<int>();

    //set
    if(varMap.contains("a") == false)
        return;
    varVal = varMap.value("a");
    auto act = varVal.value<bool>();

    emit updateDI(id, act);
    allIds.append(id);
}

void GetActResponseParser::parseDIO(QVariant var)
{
    if(var.canConvert<QVariantMap>() == false)
        return ;
    auto varMap= var.value<QVariantMap>();

    //id
    if(varMap.contains("i") == false)
        return;
    auto varVal = varMap.value("i");
    auto id = varVal.value<int>();

    //set
    if(varMap.contains("s") == false)
        return;
    varVal = varMap.value("s");
    auto set = varVal.value<bool>();

    //act
    if(varMap.contains("a") == false)
        return;
    varVal = varMap.value("a");
    auto act = varVal.value<bool>();

    emit updateDIO(id, act, set);
    allIds.append(id);
}

void GetActResponseParser::clearAllIds()
{
    allIds.clear();

}
}

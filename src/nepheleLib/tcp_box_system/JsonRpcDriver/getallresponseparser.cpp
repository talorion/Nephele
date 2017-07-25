#include "getallresponseparser.hpp"


#include <QVariantList>
#include <QVariantList>
#include <QVariantMap>
#include <QVariantMap>
#include <QtGlobal>

#include <tcp_box_system/JsonRpc/jsonrpcresponse.hpp>

namespace talorion {

GetAllResponseParser::GetAllResponseParser(QObject *par):
    AbstractResponseParser(par),
    allIds(),
    aoIds(),
    aiIds(),
    aioIds(),
    fcIds(),
    avIds(),
    doIds(),
    diIds(),
    dioIds()
{

}

void GetAllResponseParser::doConnectToBackend(abstract_backend *back)
{
    clearAllIds();

    connect(this, SIGNAL(newAI(int,QString,QString,double,double,double,QVariant)),
            back, SLOT(newAI(int,QString,QString,double,double,double,QVariant)));
    connect(this, SIGNAL(newAIO(int,QString,QString,double,double,double,double,double,double,QVariant)),
            back, SLOT(newAIO(int,QString,QString,double,double,double,double,double,double,QVariant)));
    connect(this, SIGNAL(newAO(int,QString,QString,double,double,double,QVariant)),
            back, SLOT(newAO(int,QString,QString,double,double,double,QVariant)));

    connect(this, SIGNAL(newAV(int,QString,QString,double,double,double,double,double,double,QVariant)),
            back, SLOT(newAV(int,QString,QString,double,double,double,double,double,double,QVariant)));
    connect(this, SIGNAL(newFC(int,QString,QString,double,double,double,double,double,double,double,int,int,QVariant)),
            back, SLOT(newFC(int,QString,QString,double,double,double,double,double,double,double,int,int,QVariant)));

    connect(this, SIGNAL(newDI(int,QString,bool,QVariant)),
            back, SLOT(newDI(int,QString,bool,QVariant)));
    connect(this, SIGNAL(newDIO(int,QString,bool,bool,QVariant)),
            back, SLOT(newDIO(int,QString,bool,bool,QVariant)));
    connect(this, SIGNAL(newDO(int,QString,bool,QVariant)),
            back, SLOT(newDO(int,QString,bool,QVariant)));

    connect(this, SIGNAL(finishedParsing(QList<int>)),back, SLOT(finishedParsing(QList<int>)));
}

bool GetAllResponseParser::doParseResponse(QSharedPointer<JsonRpcRequest> request, JsonRpcResponse *response)
{
    clearAllIds();

    Q_UNUSED(request);
    //{"FC": [{"id": 0, "smin": 0.0, "smax": 1000.000, "amin": 0.0, "amax": 1000.000, "set": 0.0, "act": 0.0, "T": 32.07345, "name": "FC", "units": "sccm", "read counter": 132, "error counter": 0}, {"id": 1, "smin": 0.0, "smax": 0.0, "amin": 0.0, "amax": 0.0, "set": 0.0, "act": 0.0, "T": 0.0, "name": "FC", "units": "sccm", "read counter": 0, "error counter": 255}]}
    auto varia = response->result();

    //qDebug()<<request->methodName()<<" returned: "<< response->rawData();

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

void GetAllResponseParser::parseAO(QVariant var)
{
    if(var.canConvert<QVariantMap>() == false)
        return ;
    auto varMap= var.value<QVariantMap>();

    //id
    if(varMap.contains("id") == false)
        return;
    auto varVal = varMap.value("id");
    auto id = varVal.value<int>();

    //units
    if(varMap.contains("units") == false)
        return;
    varVal = varMap.value("units");
    auto units = varVal.value<QString>();

    //smin
    if(varMap.contains("smin") == false)
        return;
    varVal = varMap.value("smin");
    auto smin = varVal.value<double>();

    //smax
    if(varMap.contains("smax") == false)
        return;
    varVal = varMap.value("smax");
    auto smax = varVal.value<double>();

    //set
    if(varMap.contains("set") == false)
        return;
    varVal = varMap.value("set");
    auto set = varVal.value<double>();

    //name
    if(varMap.contains("name") == false)
        return;
    varVal = varMap.value("name");
    auto name = varVal.value<QString>();

    emit newAO(id,name,units,set,smin,smax, varMap);
    aoIds.append(id);
    allIds.append(id);

}

void GetAllResponseParser::parseAI(QVariant var)
{
    if(var.canConvert<QVariantMap>() == false)
        return ;
    auto varMap= var.value<QVariantMap>();

    //id
    if(varMap.contains("id") == false)
        return;
    auto varVal = varMap.value("id");
    auto id = varVal.value<int>();

    //units
    if(varMap.contains("units") == false)
        return;
    varVal = varMap.value("units");
    auto units = varVal.value<QString>();

    //amin
    if(varMap.contains("amin") == false)
        return;
    varVal = varMap.value("amin");
    auto amin = varVal.value<double>();

    //amax
    if(varMap.contains("amax") == false)
        return;
    varVal = varMap.value("amax");
    auto amax = varVal.value<double>();

    //act
    if(varMap.contains("act") == false)
        return;
    varVal = varMap.value("act");
    auto act = varVal.value<double>();

    //name
    if(varMap.contains("name") == false)
        return;
    varVal = varMap.value("name");
    auto name = varVal.value<QString>();

    emit newAI(id,name,units,act,amin,amax,varMap);
    aiIds.append(id);
    allIds.append(id);
}

void GetAllResponseParser::parseAIO(QVariant var)
{
    if(var.canConvert<QVariantMap>() == false)
        return ;
    auto varMap= var.value<QVariantMap>();

    //id
    if(varMap.contains("id") == false)
        return;
    auto varVal = varMap.value("id");
    auto id = varVal.value<int>();

    //units
    if(varMap.contains("units") == false)
        return;
    varVal = varMap.value("units");
    auto units = varVal.value<QString>();

    //smin
    if(varMap.contains("smin") == false)
        return;
    varVal = varMap.value("smin");
    auto smin = varVal.value<double>();

    //smax
    if(varMap.contains("smax") == false)
        return;
    varVal = varMap.value("smax");
    auto smax = varVal.value<double>();

    //set
    if(varMap.contains("set") == false)
        return;
    varVal = varMap.value("set");
    auto set = varVal.value<double>();

    //amin
    if(varMap.contains("amin") == false)
        return;
    varVal = varMap.value("amin");
    auto amin = varVal.value<double>();

    //amax
    if(varMap.contains("amax") == false)
        return;
    varVal = varMap.value("amax");
    auto amax = varVal.value<double>();

    //act
    if(varMap.contains("act") == false)
        return;
    varVal = varMap.value("act");
    auto act = varVal.value<double>();

    //name
    if(varMap.contains("name") == false)
        return;
    varVal = varMap.value("name");
    auto name = varVal.value<QString>();

    emit newAIO(id,name,units,set, smin, smax, act, amin, amax,varMap);
    aioIds.append(id);
    allIds.append(id);
}

void GetAllResponseParser::parseFC(QVariant var)
{
    if(var.canConvert<QVariantMap>() == false)
        return ;
    auto varMap= var.value<QVariantMap>();

    //id
    if(varMap.contains("id") == false)
        return;
    auto varVal = varMap.value("id");
    auto id = varVal.value<int>();

    //smin
    if(varMap.contains("smin") == false)
        return;
    varVal = varMap.value("smin");
    auto smin = varVal.value<double>();

    //smax
    if(varMap.contains("smax") == false)
        return;
    varVal = varMap.value("smax");
    auto smax = varVal.value<double>();

    //amin
    if(varMap.contains("amin") == false)
        return;
    varVal = varMap.value("amin");
    auto amin = varVal.value<double>();

    //amax
    if(varMap.contains("amax") == false)
        return;
    varVal = varMap.value("amax");
    auto amax = varVal.value<double>();

    //set
    if(varMap.contains("set") == false)
        return;
    varVal = varMap.value("set");
    auto set = varVal.value<double>();

    //act
    if(varMap.contains("act") == false)
        return;
    varVal = varMap.value("act");
    auto act = varVal.value<double>();

    //T
    if(varMap.contains("T") == false)
        return;
    varVal = varMap.value("T");
    auto T = varVal.value<double>();

    //name
    if(varMap.contains("name") == false)
        return;
    varVal = varMap.value("name");
    auto name = varVal.value<QString>();

    //units
    if(varMap.contains("units") == false)
        return;
    varVal = varMap.value("units");
    auto units = varVal.value<QString>();

    //read counter
    if(varMap.contains("read counter") == false)
        return;
    varVal = varMap.value("read counter");
    auto read_counter = varVal.value<int>();

    //error counter
    if(varMap.contains("error counter") == false)
        return;
    varVal = varMap.value("error counter");
    auto error_counter = varVal.value<int>();

    emit newFC(id, name, units, set, smin, smax, act, amin, amax, T, read_counter, error_counter,varMap);
    fcIds.append(id);
    allIds.append(id);
}

void GetAllResponseParser::parseAV(QVariant var)
{
    if(var.canConvert<QVariantMap>() == false)
        return ;
    auto varMap= var.value<QVariantMap>();

    //id
    if(varMap.contains("id") == false)
        return;
    auto varVal = varMap.value("id");
    auto id = varVal.value<int>();

    //units
    if(varMap.contains("units") == false)
        return;
    varVal = varMap.value("units");
    auto units = varVal.value<QString>();

    //smin
    if(varMap.contains("smin") == false)
        return;
    varVal = varMap.value("smin");
    auto smin = varVal.value<double>();

    //smax
    if(varMap.contains("smax") == false)
        return;
    varVal = varMap.value("smax");
    auto smax = varVal.value<double>();

    //set
    if(varMap.contains("set") == false)
        return;
    varVal = varMap.value("set");
    auto set = varVal.value<double>();

    //amin
    if(varMap.contains("amin") == false)
        return;
    varVal = varMap.value("amin");
    auto amin = varVal.value<double>();

    //amax
    if(varMap.contains("amax") == false)
        return;
    varVal = varMap.value("amax");
    auto amax = varVal.value<double>();

    //act
    if(varMap.contains("act") == false)
        return;
    varVal = varMap.value("act");
    auto act = varVal.value<double>();

    //name
    if(varMap.contains("name") == false)
        return;
    varVal = varMap.value("name");
    auto name = varVal.value<QString>();

    emit newAV(id,name,units,set, smin, smax, act, amin, amax, varMap);
    avIds.append(id);
    allIds.append(id);
}

void GetAllResponseParser::parseDO(QVariant var)
{
    if(var.canConvert<QVariantMap>() == false)
        return ;
    auto varMap= var.value<QVariantMap>();

    //id
    if(varMap.contains("id") == false)
        return;
    auto varVal = varMap.value("id");
    auto id = varVal.value<int>();

    //name
    if(varMap.contains("name") == false)
        return;
    varVal = varMap.value("name");
    auto name = varVal.value<QString>();

    //set
    if(varMap.contains("set") == false)
        return;
    varVal = varMap.value("set");
    auto set = varVal.value<bool>();

    emit newDO(id, name, set, varMap);
    doIds.append(id);
    allIds.append(id);
}

void GetAllResponseParser::parseDI(QVariant var)
{
    if(var.canConvert<QVariantMap>() == false)
        return ;
    auto varMap= var.value<QVariantMap>();

    //id
    if(varMap.contains("id") == false)
        return;
    auto varVal = varMap.value("id");
    auto id = varVal.value<int>();

    //name
    if(varMap.contains("name") == false)
        return;
    varVal = varMap.value("name");
    auto name = varVal.value<QString>();

    //set
    if(varMap.contains("act") == false)
        return;
    varVal = varMap.value("act");
    auto act = varVal.value<bool>();

    emit newDI(id, name, act, varMap);
    diIds.append(id);
    allIds.append(id);
}

void GetAllResponseParser::parseDIO(QVariant var)
{
    if(var.canConvert<QVariantMap>() == false)
        return ;
    auto varMap= var.value<QVariantMap>();

    //id
    if(varMap.contains("id") == false)
        return;
    auto varVal = varMap.value("id");
    auto id = varVal.value<int>();

    //name
    if(varMap.contains("name") == false)
        return;
    varVal = varMap.value("name");
    auto name = varVal.value<QString>();

    //set
    if(varMap.contains("set") == false)
        return;
    varVal = varMap.value("set");
    auto set = varVal.value<bool>();

    //act
    if(varMap.contains("act") == false)
        return;
    varVal = varMap.value("act");
    auto act = varVal.value<bool>();

    emit newDIO(id, name, act, set, varMap);
    dioIds.append(id);
    allIds.append(id);
}

void GetAllResponseParser::clearAllIds()
{
    allIds.clear();

    aoIds.clear();
    aiIds.clear();
    aioIds.clear();

    fcIds.clear();
    avIds.clear();

    doIds.clear();
    diIds.clear();
    dioIds.clear();
}

} // namespace talorion

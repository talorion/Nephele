#include <QtTest>
#include <QString>
#include <QCoreApplication>
#include <QByteArray>

#include <tcp_box_system/JsonRpc/jsonrpcresponse.hpp>

#include "tcp_box_system/JsonRpc/jsonrpcfactory.hpp"
#include "tcp_box_system/JsonRpc/JrpcDatatype.hpp"
#include "tcp_box_system/JsonRpc/jsonrpcrequest.hpp"
#include "tcp_box_system/JsonRpc/jsonrpcmethoddescriptor.hpp"

//QStringLiteral exp("{\"jsonrpc\":\"2.0\",\"method\":\"subtract\",\"params\":[42,23],\"id\":1}");

#define METHOD_NAME "subtract"
#define METHOD_PARA_0 42
#define METHOD_PARA_1 23
#define METHOD_PARAMS QVariantList{METHOD_PARA_0, METHOD_PARA_1}
#define METHOD_ID 1
#define METHOD_ID_NULL "null"

#define METHOD_PARAM_TYPES {JRPCT_UINT32, JRPCT_UINT32}

using namespace talorion;

class tst_JsonRpc : public QObject
{
    Q_OBJECT

public:
    tst_JsonRpc();
    ~tst_JsonRpc();

private slots:
    //void initTestCase(); //will be called before the first testfunction is executed.
    //void cleanupTestCase(); //will be called after the last testfunction was executed.
    //void init(); //will be called before each testfunction is executed.
    //void cleanup(); //will be called before each testfunction is executed.

    void JsonRpcMethodDescriptorIsCreatedByAFactory();
    void JsonRpcRequestIsCreatedByAFactory();
    void JsonRpcRequestIsAValidJsonRpcRequest();
    void JsonRpcRequestIdIsNullWhenGivenIdIsZero();
    
    void JsonRpcRequestExamples_data();
    void JsonRpcRequestExamples();

    void JsonRpcRequestCanBeCreatedByMethodDescriptor();
    void JsonRpcRequestGeneratesIdForEachRequest();
    void JsonRpcRequestEncodesMedodIdInRequestId();

    void JsonRpcMethodDescriptorCanBeCreatedFromJson();
    void JsonRpcMethodDescriptorCanBeCreatedFromVariant();
    void JsonRpcMethodDescriptorIsNotCreatedFromInvalidJson();

    void JsonRpcMethodDescriptorExamples_data();
    void JsonRpcMethodDescriptorExamples();

    void JsonRpcMethodDescriptorExamplesVariant_data();
    void JsonRpcMethodDescriptorExamplesVariant();

    void JsonRpcMethodDescriptorExamplesWithArgs_data();
    void JsonRpcMethodDescriptorExamplesWithArgs();

    void JsonRpcResponseCanBeCreatedFromJson();
    void JsonRpcResponseCanHandleJRPCErrors();
    void JsonRpcResponseExamples_data();
    void JsonRpcResponseExamples();
};



tst_JsonRpc::tst_JsonRpc()
{

}

tst_JsonRpc::~tst_JsonRpc()
{

}

void tst_JsonRpc::JsonRpcMethodDescriptorIsCreatedByAFactory()
{
    auto desc = JsonRpcFactory::createMethodDescriptor(METHOD_NAME, METHOD_PARAM_TYPES);

    QVERIFY(dynamic_cast<JsonRpcMethodDescriptor*>(desc) != Q_NULLPTR);
}

void tst_JsonRpc::JsonRpcRequestIsCreatedByAFactory()
{
    auto req = JsonRpcFactory::createRequest(METHOD_NAME, METHOD_PARAMS, METHOD_ID);

    //QVERIFY(dynamic_cast<QSharedPointer<JsonRpcRequest>* >(&req) != Q_NULLPTR);
    QVERIFY(req.isNull() == false);
}

void tst_JsonRpc::JsonRpcRequestIsAValidJsonRpcRequest()
{
    QString exp_str = QString("{\"jsonrpc\":\"2.0\",\"method\":\"%1\",\"params\":[%2,%3],\"id\":%4}").arg(METHOD_NAME).arg(METHOD_PARA_0).arg(METHOD_PARA_1).arg(METHOD_ID);
    QByteArray exp = exp_str.toLocal8Bit();
    auto req = JsonRpcFactory::createRequest(METHOD_NAME, METHOD_PARAMS, METHOD_ID);

    QCOMPARE(req->toJson(), exp);
}

void tst_JsonRpc::JsonRpcRequestIdIsNullWhenGivenIdIsZero()
{
    QString exp_str = QString("{\"jsonrpc\":\"2.0\",\"method\":\"%1\",\"params\":[%2,%3],\"id\":%4}").arg(METHOD_NAME).arg(METHOD_PARA_0).arg(METHOD_PARA_1).arg(METHOD_ID_NULL);
    QByteArray exp = exp_str.toLocal8Bit();
    auto req = JsonRpcFactory::createRequest(METHOD_NAME, METHOD_PARAMS, 0);

    QCOMPARE(req->id(), QString(METHOD_ID_NULL));
    QCOMPARE(req->toJson(), exp);
}

void tst_JsonRpc::JsonRpcRequestExamples_data()
{
    QTest::addColumn<QString>("methodname");
    QTest::addColumn<QVariant>("params");
    QTest::addColumn<qint32>("methodId");
    QTest::addColumn<QString>("result");

    QTest::newRow("positional parameters")            << "subtract" << QVariant(QVariantList{42,23})                            << 1 <<"{\"jsonrpc\":\"2.0\",\"method\":\"subtract\",\"params\":[42,23],\"id\":1}";
    QTest::newRow("positional parameters reversed")   << "subtract" << QVariant(QVariantList{23,42})                            << 2 <<"{\"jsonrpc\":\"2.0\",\"method\":\"subtract\",\"params\":[23,42],\"id\":2}";
    //QTest::newRow("named parameters")                 << "subtract" << QVariant(QVariantMap{{"subtrahend",23} ,{"minuend",42}}) << 3 <<"{\"jsonrpc\":\"2.0\",\"method\":\"subtract\",\"params\":{\"subtrahend\":23,\"minuend\":42},\"id\":3}";
    QTest::newRow("named parameters reversed")        << "subtract" << QVariant(QVariantMap{{"minuend",42} ,{"subtrahend",23}}) << 4 <<"{\"jsonrpc\":\"2.0\",\"method\":\"subtract\",\"params\":{\"minuend\":42,\"subtrahend\":23},\"id\":4}";
    QTest::newRow("no parameters")                    << "foobar"   << QVariant()                                               << 1 <<"{\"jsonrpc\":\"2.0\",\"method\":\"foobar\",\"id\":1}";

    QTest::newRow("getAllMethods")                    << "getAllMethods"   << QVariant()                                        << 1 <<"{\"jsonrpc\":\"2.0\",\"method\":\"getAllMethods\",\"id\":1}";
    QTest::newRow("getAll")                           << "getAll"   << QVariant()                                               << 1 <<"{\"jsonrpc\":\"2.0\",\"method\":\"getAll\",\"id\":1}";
    QTest::newRow("getNumSlaves")                     << "getNumSlaves"   << QVariant()                                         << 1 <<"{\"jsonrpc\":\"2.0\",\"method\":\"getNumSlaves\",\"id\":1}";
    QTest::newRow("scanSlaves")                       << "scanSlaves"   << QVariant()                                           << 1 <<"{\"jsonrpc\":\"2.0\",\"method\":\"scanSlaves\",\"id\":1}";
    QTest::newRow("reset")                            << "reset"   << QVariant()                                                << 1 <<"{\"jsonrpc\":\"2.0\",\"method\":\"reset\",\"id\":1}";

    QTest::newRow("setSetpoint")                      << "setSetpoint"   << QVariant(QVariantList{1, 23.42})                    << 1 <<"{\"jsonrpc\":\"2.0\",\"method\":\"setSetpoint\",\"params\":[1,23.42],\"id\":1}";
    QTest::newRow("setSlaveScanRange")                << "setSlaveScanRange"   << QVariant(QVariantList{0, 100})                << 1 <<"{\"jsonrpc\":\"2.0\",\"method\":\"setSlaveScanRange\",\"params\":[0,100],\"id\":1}";
    QTest::newRow("setSlaveName")                     << "setSlaveName"   << QVariant(QVariantList{"H3O+"})                     << 1 <<"{\"jsonrpc\":\"2.0\",\"method\":\"setSlaveName\",\"params\":[\"H3O+\"],\"id\":1}";
    QTest::newRow("setIP")                            << "setIP"   << QVariant(QVariantList{192,168,0,6})                       << 1 <<"{\"jsonrpc\":\"2.0\",\"method\":\"setIP\",\"params\":[192,168,0,6],\"id\":1}";
}

void tst_JsonRpc::JsonRpcRequestExamples()
{
    QFETCH(QString, methodname);
    QFETCH(QVariant, params);
    QFETCH(qint32, methodId);
    QFETCH(QString, result);

    QByteArray exp = result.toLocal8Bit();
    auto req = JsonRpcFactory::createRequest(methodname, params, methodId);

    QCOMPARE(req->toJson(), exp);

}

void tst_JsonRpc::JsonRpcRequestCanBeCreatedByMethodDescriptor()
{
    auto desc = JsonRpcFactory::createMethodDescriptor(METHOD_NAME, METHOD_PARAM_TYPES);
    auto req = JsonRpcFactory::createRequest(*desc, METHOD_PARAMS);

    QString exp_str = QString("{\"jsonrpc\":\"2.0\",\"method\":\"%1\",\"params\":[%2,%3],\"id\":%4}").arg(METHOD_NAME).arg(METHOD_PARA_0).arg(METHOD_PARA_1).arg(req->id());
    QByteArray exp = exp_str.toLocal8Bit();

    QCOMPARE(req->toJson(), exp);
}

void tst_JsonRpc::JsonRpcRequestGeneratesIdForEachRequest()
{
    auto desc = JsonRpcFactory::createMethodDescriptor(METHOD_NAME, METHOD_PARAM_TYPES);
    auto req1 = JsonRpcFactory::createRequest(*desc, METHOD_PARAMS);
    auto req2 = JsonRpcFactory::createRequest(*desc, METHOD_PARAMS);

    QVERIFY(req1->id() != req2->id());
}

void tst_JsonRpc::JsonRpcRequestEncodesMedodIdInRequestId()
{
    auto desc = JsonRpcFactory::createMethodDescriptor(METHOD_NAME, METHOD_PARAM_TYPES);
    auto req1 = JsonRpcFactory::createRequest(*desc, METHOD_PARAMS);

    QCOMPARE(desc->methodId() , req1->methodId());
}

void tst_JsonRpc::JsonRpcMethodDescriptorCanBeCreatedFromJson()
{
    QByteArray desc_json("{\"name\": \"getAll\", \"params\": {}}");
    auto desc = JsonRpcFactory::createMethodDescriptor(desc_json);
    QVERIFY(dynamic_cast<JsonRpcMethodDescriptor*>(desc) != Q_NULLPTR);
}

void tst_JsonRpc::JsonRpcMethodDescriptorCanBeCreatedFromVariant()
{
    auto desc_variant = QVariant(QVariantMap{{"name",QVariant("setSetpoint")},{"params", QVariant(QVariantList{QVariantMap{{"slaveID",QVariant(21)},{"setpoint",QVariant(4)}}})}});
    auto desc = JsonRpcFactory::createMethodDescriptor(desc_variant);
    QVERIFY(dynamic_cast<JsonRpcMethodDescriptor*>(desc) != Q_NULLPTR);
}

void tst_JsonRpc::JsonRpcMethodDescriptorIsNotCreatedFromInvalidJson()
{
    QByteArray desc_invalidjson("{\"name\": \"getAll\", \"params\": {}");
    auto desc = JsonRpcFactory::createMethodDescriptor(desc_invalidjson);
    QVERIFY(dynamic_cast<JsonRpcMethodDescriptor*>(desc) == Q_NULLPTR);
}

void tst_JsonRpc::JsonRpcMethodDescriptorExamples_data()
{
    QTest::addColumn<QByteArray>("desc_json");
    QTest::addColumn<QString>("methodname");
    QTest::addColumn<QStringList>("paramNames");
    QTest::addColumn<JrpcDatatypeList>("paramTypes");


    QTest::newRow("getAll") << QByteArray("{\"name\": \"getAll\", \"params\": {}}") << "getAll" << QStringList()<< JrpcDatatypeList();
    QTest::newRow("getNumSlaves") << QByteArray("{\"name\": \"getNumSlaves\", \"params\": {}}") << "getNumSlaves" << QStringList()<< JrpcDatatypeList();
    QTest::newRow("scanSlaves") << QByteArray("{\"name\": \"scanSlaves\", \"params\": {}}") << "scanSlaves" << QStringList()<< JrpcDatatypeList();
    QTest::newRow("reset") << QByteArray("{\"name\": \"reset\", \"params\": {}}") << "reset" << QStringList()<< JrpcDatatypeList();
    QTest::newRow("getAllMethods") << QByteArray("{\"name\": \"getAllMethods\", \"params\": {}}") << "getAllMethods" << QStringList()<< JrpcDatatypeList();
}

void tst_JsonRpc::JsonRpcMethodDescriptorExamples()
{
    QFETCH(QByteArray, desc_json);
    QFETCH(QString, methodname);
    QFETCH(QStringList, paramNames);
    QFETCH(JrpcDatatypeList, paramTypes);

    auto desc = JsonRpcFactory::createMethodDescriptor(desc_json);
    QVERIFY(dynamic_cast<JsonRpcMethodDescriptor*>(desc) != Q_NULLPTR);

    QCOMPARE(desc->methodName() , methodname);
    QCOMPARE(desc->paramNames() , paramNames);
    QCOMPARE(desc->paramTypes() , paramTypes);
}

void tst_JsonRpc::JsonRpcMethodDescriptorExamplesVariant_data()
{
    QTest::addColumn<QVariant>("desc_var");
    QTest::addColumn<QString>("methodname");
    QTest::addColumn<QStringList>("paramNames");
    QTest::addColumn<JrpcDatatypeList>("paramTypes");

    QTest::newRow("getAll") << QVariant(QVariantMap{{"name",QVariant("getAll")},{"params", QVariant()}}) << "getAll" << QStringList()<< JrpcDatatypeList();
    QTest::newRow("getNumSlaves") << QVariant(QVariantMap{{"name",QVariant("getNumSlaves")},{"params", QVariant()}}) << "getNumSlaves" << QStringList()<< JrpcDatatypeList();
    QTest::newRow("scanSlaves") << QVariant(QVariantMap{{"name",QVariant("scanSlaves")},{"params", QVariant()}}) << "scanSlaves" << QStringList()<< JrpcDatatypeList();
    QTest::newRow("reset") << QVariant(QVariantMap{{"name",QVariant("reset")},{"params", QVariant()}}) << "reset" << QStringList()<< JrpcDatatypeList();
    QTest::newRow("getAllMethods") << QVariant(QVariantMap{{"name",QVariant("getAllMethods")},{"params", QVariant()}}) << "getAllMethods" << QStringList()<< JrpcDatatypeList();
    QTest::newRow("setSetpoint") << QVariant(QVariantMap{{"name",QVariant("setSetpoint")},{"params", (QVariantList{QVariantMap{{"slaveID",QVariant(21)}},QVariantMap{{"setpoint",QVariant(4)}}})}})<< "setSetpoint" << QStringList{"slaveID", "setpoint"}<< JrpcDatatypeList{JRPCT_UINT64, JRPCT_REAL32};
    QTest::newRow("setSlaveScanRange") << QVariant(QVariantMap{{"name",QVariant("setSlaveScanRange")},{"params", (QVariantList{QVariantMap{{"start",QVariant(21)}},QVariantMap{{"end",QVariant(21)}}})}}) << "setSlaveScanRange" << QStringList{"start", "end"}<< JrpcDatatypeList{JRPCT_UINT64, JRPCT_UINT64};
    QTest::newRow("setSlaveName") << QVariant(QVariantMap{{"name",QVariant("setSlaveName")},{"params", (QVariantList{QVariantMap{{"name",QVariant(8)}}})}}) << "setSlaveName" << QStringList{"name"}<< JrpcDatatypeList{JRPCT_STRING};
    QTest::newRow("setIP") << QVariant(QVariantMap{{"name",QVariant("setIP")},{"params", (QVariantList{QVariantMap{{"addr1",QVariant(21)}},QVariantMap{{"addr2",QVariant(21)}},QVariantMap{{"addr3",QVariant(21)}},QVariantMap{{"addr4",QVariant(21)}}})}}) << "setIP" << QStringList{"addr1", "addr2", "addr3", "addr4"}<< JrpcDatatypeList{JRPCT_UINT64, JRPCT_UINT64,JRPCT_UINT64,JRPCT_UINT64};
    QTest::newRow("setModbusTimeout") << QVariant(QVariantMap{{"name",QVariant("setModbusTimeout")},{"params", (QVariantList{QVariantMap{{"timeout_ms",QVariant(21)}}})}}) << "setModbusTimeout" << QStringList{"timeout_ms"}<< JrpcDatatypeList{JRPCT_UINT64};
}

void tst_JsonRpc::JsonRpcMethodDescriptorExamplesVariant()
{
    QFETCH(QVariant, desc_var);
    QFETCH(QString, methodname);
    QFETCH(QStringList, paramNames);
    QFETCH(JrpcDatatypeList, paramTypes);

    auto desc = JsonRpcFactory::createMethodDescriptor(desc_var);
    QVERIFY(dynamic_cast<JsonRpcMethodDescriptor*>(desc) != Q_NULLPTR);

    QCOMPARE(desc->methodName() , methodname);
    QCOMPARE(desc->paramNames() , paramNames);
    QCOMPARE(desc->paramTypes() , paramTypes);
}

void tst_JsonRpc::JsonRpcMethodDescriptorExamplesWithArgs_data()
{
    QTest::addColumn<QByteArray>("desc_json");
    QTest::addColumn<QString>("methodname");
    QTest::addColumn<QStringList>("paramNames");
    QTest::addColumn<JrpcDatatypeList>("paramTypes");

    //QTest::newRow("setSetpoint") << QByteArray("{\"name\": \"setSetpoint\", \"params\": {\"slaveID\": 21, \"setpoint\": 4}}") << "setSetpoint" << QStringList{"slaveID", "setpoint"}<< JrpcDatatypeList{JRPCT_UINT64, JRPCT_REAL32};
    //QTest::newRow("setSlaveScanRange") << QByteArray("{\"name\": \"setSlaveScanRange\", \"params\": {\"start\": 21, \"end\": 21}}") << "setSlaveScanRange" << QStringList{"start", "end"}<< JrpcDatatypeList{JRPCT_UINT64, JRPCT_UINT64};
    //QTest::newRow("setSlaveName") << QByteArray("{\"name\": \"setSlaveName\", \"params\": {\"name\": 8}}") << "setSlaveName" << QStringList{"name"}<< JrpcDatatypeList{JRPCT_STRING};
    //QTest::newRow("setIP") << QByteArray("{\"name\": \"setIP\", \"params\": {\"addr1\": 21, \"addr2\": 21, \"addr3\": 21, \"addr4\": 21}}") << "setIP" << QStringList{"addr1", "addr2", "addr3", "addr4"}<< JrpcDatatypeList{JRPCT_UINT64, JRPCT_UINT64,JRPCT_UINT64,JRPCT_UINT64};

    QTest::newRow("setSetpoint") << QByteArray("{\"name\": \"setSetpoint\", \"params\": [{\"slaveID\": 21}, {\"setpoint\": 4}]}") << "setSetpoint" << QStringList{"slaveID", "setpoint"}<< JrpcDatatypeList{JRPCT_UINT64, JRPCT_REAL32};
    QTest::newRow("setSlaveScanRange") << QByteArray("{\"name\": \"setSlaveScanRange\", \"params\": [{\"start\": 21}, {\"end\": 21}]}") << "setSlaveScanRange" << QStringList{"start", "end"}<< JrpcDatatypeList{JRPCT_UINT64, JRPCT_UINT64};
    QTest::newRow("setSlaveName") << QByteArray("{\"name\": \"setSlaveName\", \"params\": [{\"name\": 8}]}") << "setSlaveName" << QStringList{"name"}<< JrpcDatatypeList{JRPCT_STRING};
    QTest::newRow("setIP") << QByteArray("{\"name\": \"setIP\", \"params\": [{\"addr1\": 21}, {\"addr2\": 21}, {\"addr3\": 21}, {\"addr4\": 21}]}") << "setIP" << QStringList{"addr1", "addr2", "addr3", "addr4"}<< JrpcDatatypeList{JRPCT_UINT64, JRPCT_UINT64,JRPCT_UINT64,JRPCT_UINT64};

}

void tst_JsonRpc::JsonRpcMethodDescriptorExamplesWithArgs()
{
    //QSKIP("There is a bug in the Protocol: Objects do not guarantee order of their members");

    QFETCH(QByteArray, desc_json);
    QFETCH(QString, methodname);
    QFETCH(QStringList, paramNames);
    QFETCH(JrpcDatatypeList, paramTypes);

    auto desc = JsonRpcFactory::createMethodDescriptor(desc_json);
    QVERIFY(dynamic_cast<JsonRpcMethodDescriptor*>(desc) != Q_NULLPTR);

    QCOMPARE(desc->methodName() , methodname);
    QCOMPARE(desc->paramNames() , paramNames);
    QCOMPARE(desc->paramTypes() , paramTypes);
}

void tst_JsonRpc::JsonRpcResponseCanBeCreatedFromJson()
{
    QByteArray desc_json("{\"jsonrpc\": \"2.0\", \"result\": 19, \"id\": 1}");
    auto resp = JsonRpcFactory::createResponse(desc_json);
    QVERIFY(dynamic_cast<JsonRpcResponse*>(resp) != Q_NULLPTR);
}

void tst_JsonRpc::JsonRpcResponseCanHandleJRPCErrors()
{
    QByteArray desc_json("{\"jsonrpc\": \"2.0\", \"error\": {\"code\": -32601, \"message\": \"Method not found\"}, \"id\": \"1\"}");
    auto resp = JsonRpcFactory::createResponse(desc_json);
    QVERIFY(dynamic_cast<JsonRpcResponse*>(resp) != Q_NULLPTR);
}

void tst_JsonRpc::JsonRpcResponseExamples_data()
{
    QTest::addColumn<QByteArray>("resp_json");
    QTest::addColumn<bool>("isValid");
    QTest::addColumn<bool>("isError");
    QTest::addColumn<QVariant>("result");
    QTest::addColumn<QString>("id");
    QTest::addColumn<JsonRpcError *>("error");

    QTest::newRow("subtract")     << QByteArray("{\"jsonrpc\": \"2.0\", \"result\": 19, \"id\": 1}") << true << false <<QVariant::fromValue(19) <<"1"<<(JsonRpcError*)(Q_NULLPTR);
    QTest::newRow("subtract2")    << QByteArray("{\"jsonrpc\": \"2.0\", \"result\": -19, \"id\": 2}") << true << false <<QVariant::fromValue(-19) <<"2"<<(JsonRpcError*)(Q_NULLPTR);
    QTest::newRow("foobar")       << QByteArray("{\"jsonrpc\": \"2.0\", \"error\": {\"code\": -32601, \"message\": \"Method not found\"}, \"id\": \"1\"}") << false << true <<QVariant() <<"1"<<new JsonRpcError{-32601, "Method not found",QVariantMap()};
    QTest::newRow("foobar2")      << QByteArray("{\"jsonrpc\": \"2.0\", \"error\": {\"code\": -32700, \"message\": \"Parse error\"}, \"id\": null}") << false << true <<QVariant() <<"0"<<new JsonRpcError{-32700, "Parse error",QVariantMap()};
    QTest::newRow("foobar3")      << QByteArray("{\"jsonrpc\": \"2.0\", \"error\": {\"code\": -32600, \"message\": \"Invalid Request\"}, \"id\": null}") << false << true <<QVariant() <<"0"<<new JsonRpcError{-32600, "Invalid Request",QVariantMap()};

    QTest::newRow("getAllMethods")<< QByteArray("{\"jsonrpc\": \"2.0\", \"result\": [{\"name\": \"getAll\", \"params\": []}, {\"name\": \"setSetpoint\", \"params\": [{\"slaveID\": 21}, {\"setpoint\": 4}]}, {\"name\": \"getNumSlaves\", \"params\": []}, {\"name\": \"setSlaveScanRange\", \"params\": [{\"start\": 21}, {\"end\": 21}]}, {\"name\": \"scanSlaves\", \"params\": []}, {\"name\": \"setModbusTimeout\", \"params\": [{\"timeout_ms\": 23}]}, {\"name\": \"reset\", \"params\": []}, {\"name\": \"setSlaveName\", \"params\": [{\"name\": 8}]}, {\"name\": \"setIP\", \"params\": [{\"addr1\": 21}, {\"addr2\": 21}, {\"addr3\": 21}, {\"addr4\": 21}]}, {\"name\": \"getAllMethods\", \"params\": []}], \"id\": 253231107}")
                                  << true
                                  << false
                                  << QVariant(QVariantList{(QVariantMap{{"name",QVariant("getAll")},{"params", QVariantList()}}),
                                                           (QVariantMap{{"name",QVariant("setSetpoint")},{"params", (QVariantList{QVariantMap{{"slaveID",QVariant(21)}},QVariantMap{{"setpoint",QVariant(4)}}})}}),
                                                           (QVariantMap{{"name",QVariant("getNumSlaves")},{"params", QVariantList()}}),
                                                           (QVariantMap{{"name",QVariant("setSlaveScanRange")},{"params", QVariant(QVariantList{QVariantMap{{"start",QVariant(21)}},QVariantMap{{"end",QVariant(21)}}})}}),
                                                           (QVariantMap{{"name",QVariant("scanSlaves")},{"params", QVariantList()}}),
                                                           (QVariantMap{{"name",QVariant("setModbusTimeout")}, {"params", QVariant(QVariantList{QVariantMap{{"timeout_ms",QVariant(23)}}})}}),
                                                           (QVariantMap{{"name",QVariant("reset")},{"params", QVariantList()}}),
                                                           (QVariantMap{{"name",QVariant("setSlaveName")},{"params", QVariant(QVariantList{QVariantMap{{"name",QVariant(8)}}})}}),
                                                           (QVariantMap{{"name",QVariant("setIP")},{"params", QVariant(QVariantList{QVariantMap{{"addr1",QVariant(21)}},QVariantMap{{"addr2",QVariant(21)}},QVariantMap{{"addr3",QVariant(21)}},QVariantMap{{"addr4",QVariant(21)}}})}}),
                                                           (QVariantMap{{"name",QVariant("getAllMethods")},{"params", QVariantList()}})
                                              })
                                  << "253231107"
                                  << (JsonRpcError*)(Q_NULLPTR);

    QTest::newRow("getAll")<< QByteArray("{\"jsonrpc\": \"2.0\", \"result\": {\"AIO\": [], \"AI\": []}, \"id\": 1284177927}")
                           << true
                           << false
                           << QVariant(QVariantMap{{"AIO",QVariant(QVariantList())}, {"AI",QVariant(QVariantList())}})
                           << "1284177927"
                           << (JsonRpcError*)(Q_NULLPTR);

    QTest::newRow("setSetpoint")<< QByteArray("{\"jsonrpc\": \"2.0\", \"error\": {\"code\": -32602, \"message\": \"Invalid params\"}, \"id\": 1032519681}")
                                << false
                                << true
                                << QVariant()
                                << "1032519681"
                                <<new JsonRpcError{-32602, "Invalid params",QVariantMap()};

    QTest::newRow("getNumSlaves")<< QByteArray("{\"jsonrpc\": \"2.0\", \"result\": 0, \"id\": 683802625}")
                                 << true
                                 << false
                                 << QVariant(0)
                                 << "683802625"
                                 << (JsonRpcError*)(Q_NULLPTR);

    QTest::newRow("setSlaveScanRange")<< QByteArray("{\"jsonrpc\": \"2.0\", \"result\": [1,100], \"id\": -1889861631}")
                                      << true
                                      << false
                                      << QVariant(QVariantList{QVariant(1), QVariant(100)})
                                      << "-1889861631"
                                      << (JsonRpcError*)(Q_NULLPTR);

    QTest::newRow("scanSlaves")<< QByteArray("{\"jsonrpc\": \"2.0\", \"result\": 0, \"id\": -658243583}")
                               << true
                               << false
                               << QVariant(0)
                               << "-658243583"
                               << (JsonRpcError*)(Q_NULLPTR);

    QTest::newRow("setModbusTimeout")<< QByteArray("{\"jsonrpc\": \"2.0\", \"result\": [10], \"id\": -1494614015}")
                                     << true
                                     << false
                                     << QVariant(QVariantList{QVariant(10)})
                                     << "-1494614015"
                                     << (JsonRpcError*)(Q_NULLPTR);

    QTest::newRow("setSlaveName")<< QByteArray("{\"jsonrpc\": \"2.0\", \"error\": {\"code\": -32602, \"message\": \"Invalid params\"}, \"id\": 801243137}")
                                 << false
                                 << true
                                 << QVariant()
                                 << "801243137"
                                 << new JsonRpcError{-32602, "Invalid params",QVariantMap()};

    QTest::newRow("setIP")<< QByteArray("{\"jsonrpc\": \"2.0\", \"result\": [192,168,0,6], \"id\": 1392771073}")
                          << true
                          << false
                          << QVariant(QVariantList{QVariant(192), QVariant(168), QVariant(0), QVariant(6)})
                          << "1392771073"
                          << (JsonRpcError*)(Q_NULLPTR);

    QTest::newRow("reset")<< QByteArray("{\"jsonrpc\": \"2.0\", \"result\": null, \"id\": 1314783233}")
                          << true
                          << false
                          << QVariant::fromValue(Q_NULLPTR)
                          << "1314783233"
                          << (JsonRpcError*)(Q_NULLPTR);

}

void tst_JsonRpc::JsonRpcResponseExamples()
{
    QFETCH(QByteArray, resp_json);
    QFETCH(bool, isValid);
    QFETCH(bool, isError);
    QFETCH(QVariant, result);
    QFETCH(QString, id);
    QFETCH(JsonRpcError*, error);

    auto resp = JsonRpcFactory::createResponse(resp_json);
    QVERIFY(dynamic_cast<JsonRpcResponse*>(resp) != Q_NULLPTR);

    QCOMPARE(resp->result() , result);
    QCOMPARE(resp->isValid() , isValid);
    QCOMPARE(resp->isError() , isError);
    QCOMPARE(resp->id() , id);
    if(error != Q_NULLPTR)
        QCOMPARE(*(resp->error()) , *error);
    else
        QCOMPARE((resp->error()) , error);
}

QTEST_MAIN(tst_JsonRpc)

#include "tst_JsonRpc.moc"

#include "jsonrpcdriver.hpp"


#include <QEventLoop>
#include <QTimer>

#include <tcp_box_system/TcpBoxEntity/TcpBoxEntityFacorty.hpp>

#include <tcp_box_system/JsonRpcDriver/abstractresponseparser.hpp>
#include <tcp_box_system/JsonRpcDriver/jsonrpcdriverutils.hpp>

#include <tcp_box_system/JsonRpc/jsonrpcfactory.hpp>
#include <tcp_box_system/JsonRpc/jsonrpcrequest.hpp>
#include <tcp_box_system/JsonRpc/jsonrpcresponse.hpp>

#ifndef POLLINTERVAL_MS
#define POLLINTERVAL_MS 35
#endif

namespace talorion {
JsonRpcDriver::JsonRpcDriver(EntityManager::EntityID box, QObject *par) :
    AbstractTcpDriver(box, par),
    m_pollTimer(),
    m_reconnectTimer(),
    m_tcpSocket(Q_NULLPTR),
    m_cmdQ(),
    m_pendingRequests(),
    m_back(getBoxId(), this),
    m_requestCounter(),
    m_responseCounter(),
    m_timoutErrorCounter(),
    m_socketErrorCounter(),
    m_parseErrorCounter(),
    m_unmatchedResponseCounter(),
    m_consecutiveErrorCounter(),
    m_consecutiveParseErrorCounter(),
    m_avgRequestTime(),
    m_buffer()
{
    connect(&m_back,SIGNAL(sendCommand(QSharedPointer<JsonRpcRequest>)),this,SLOT(sendMethod(QSharedPointer<JsonRpcRequest>)));
    connect(&m_back,SIGNAL(processDataFinished()), this, SIGNAL(pollNextQueueElement()));
}

JsonRpcDriver::~JsonRpcDriver()
{
    m_pollTimer.stop();

    disconectDevice();
    waitForDisconnected(1000);
}

bool JsonRpcDriver::isDeviceConnected() const
{
    if(tcpSocket() == Q_NULLPTR)
        return false;

    auto sock_st = tcpSocket()->state();
    return sock_st == QAbstractSocket::ConnectedState;
}

bool JsonRpcDriver::connectDevice() {

    qDebug()<< Q_FUNC_INFO;

    if(isDeviceConnected() == true)
        return true;

    if(m_back.isValid() == false){
        qDebug()<< Q_FUNC_INFO << "invalid box";
        return false;
    }

    QString ip = m_back.ip();
    if(ip.isEmpty() || ip.isNull()){
        qDebug()<< Q_FUNC_INFO << "invalid ip";
        return false;
    }

    quint16 port = m_back.port();
    //    if(port <= 0 || port > 0xFFFF){
    //        qDebug()<< Q_FUNC_INFO << "invalid port";
    //        return false;
    //      }

    if(tcpSocket() == Q_NULLPTR){
        m_tcpSocket = new QTcpSocket();
        connect(tcpSocket(), SIGNAL(connected()),this, SIGNAL(deviceConnected()));
        connect(tcpSocket(), SIGNAL(connected()),this, SLOT(afterDeviceConnect()));
        connect(tcpSocket(), SIGNAL(disconnected()), this, SLOT(afterDeviceDisconnect()));
        connect(tcpSocket(), SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(tcpError(QAbstractSocket::SocketError)));
        //m_tcpSocket->setReadBufferSize(512);
    }

    if(tcpSocket()->state() == QAbstractSocket::UnconnectedState)
        tcpSocket()->connectToHost(ip, port);

    //m_reconnectTimer.setInterval(5000);
    //m_reconnectTimer.setSingleShot(true);
    //connect(&m_reconnectTimer, SIGNAL(timeout()), this, SLOT(connectDevice()));
    //m_reconnectTimer.start();

    return true;
}

void JsonRpcDriver::disconectDevice(){

    qDebug()<< Q_FUNC_INFO;
    m_reconnectTimer.stop();

    if(isDeviceConnected() == false)
        return;

    disconnect(tcpSocket(), SIGNAL(readyRead()), this, SLOT(socketReadyRead()));
    //disconnect(tcpSocket(), SIGNAL(disconnected()), &disco_evt_loop, SLOT(quit()));
    //disconnect(tcpSocket(), SIGNAL(error(QAbstractSocket::SocketError)), &disco_evt_loop, SLOT(quit()));
    if(tcpSocket() == Q_NULLPTR)
        return;

    tcpSocket()->disconnectFromHost();
    waitForDisconnected(1000);
    m_buffer .clear();

    delete m_tcpSocket;
    m_tcpSocket = Q_NULLPTR;

    emit deviceDisconnected();
    m_pollTimer.stop();

    for(auto it = m_pendingRequests.begin(); it != m_pendingRequests.end();){
        //auto id = it.key();
        auto req = it.value();
        if(req.isNull()){
            it = m_pendingRequests.erase(it);
            continue;
        }else{
            it = m_pendingRequests.erase(it);
            //req.clear();
        }
        //++it;
    }
}

void JsonRpcDriver::sendMethod(EntityManager::EntityID box, const QString &methodname, const QVariant &params, qint32 id)
{
    if(EntityManager::isValid(box)== false)
        return;

    if(box != getBoxId())
        return;

    sendMethod(methodname, params, id, false);
}

void JsonRpcDriver::sendMethod(const QString &methodname, const QVariant &params, qint32 id, bool internalSoure)
{
    auto request = JsonRpcFactory::createRequest(methodname, params, id,internalSoure);
    sendMethod(request);
}

void JsonRpcDriver::sendMethod(QSharedPointer<JsonRpcRequest> request)
{
    if(request.isNull())
        return;
    m_cmdQ.enqueue(request);
}

void JsonRpcDriver::sendDefaultRequest()
{
    //auto getInfoCmd = m_back.getAllRequest();
    auto getInfoCmd = m_back.getDefaultRequest();
    if(getInfoCmd.isNull())
        return;
    sendJrpcRequest(getInfoCmd);
}

void JsonRpcDriver::afterDeviceConnect()
{
    m_reconnectTimer.stop();

    m_requestCounter = 0;
    m_responseCounter = 0;

    m_timoutErrorCounter = 0;
    m_socketErrorCounter = 0;
    m_parseErrorCounter = 0;

    m_unmatchedResponseCounter = 0;

    m_consecutiveErrorCounter = 0;

    m_avgRequestTime = 0;

    m_back.socketConnected();
    m_buffer.clear();

    connect(tcpSocket(), SIGNAL(readyRead()), this, SLOT(socketReadyRead()));
    auto getAllMethods = m_back.getAllMethodsRequest();
    sendMethod(getAllMethods);

    m_pollTimer.setInterval(POLLINTERVAL_MS);
    m_pollTimer.setSingleShot(true);
    connect(&m_pollTimer,SIGNAL(timeout()),this,SLOT(pollQueue()));
    connect(this,SIGNAL(pollNextQueueElement()),&m_pollTimer,SLOT(start()));
    connect(EventManager::get_instance(), SIGNAL(sendJsonRpcRequest(int,QString,QVariant,qint32)), this, SLOT(sendMethod(EntityManager::EntityID,QString,QVariant,qint32)));

    m_pollTimer.start();
}

void JsonRpcDriver::afterDeviceDisconnect()
{
    qDebug()<<Q_FUNC_INFO<<"Tcp Socket Disconnected...";
    m_back.socketDisconnected();
    disconectDevice();
}

void JsonRpcDriver::socketReadyRead()
{

    QList<JsonRpcResponse *> respez;
    bool resp_invalid = false;

    auto tmp = tcpSocket()->readAll();

    m_buffer.append(tmp);

    respez = JsonRpcFactory::createResponses(m_buffer);
    resp_invalid = (respez.isEmpty());

    if(resp_invalid == true){
        m_consecutiveParseErrorCounter++;
        if(m_consecutiveParseErrorCounter < 10){
            return;
        }
    }
    //m_timoutTimer.stop();

    if(resp_invalid == true){
        m_parseErrorCounter++;
        qDebug()<< Q_FUNC_INFO << "Could not parse response"<<m_buffer;
        errorReport();
        if (m_pollTimer.isActive() == false){emit pollNextQueueElement();}
        return;
    }

    foreach (auto resp, respez) {
        parseResponse(resp);
    }

    respez.clear();
    m_buffer.clear();
    if (m_pollTimer.isActive() == false){emit pollNextQueueElement();}

}

void JsonRpcDriver::pollQueue()
{
    if(m_cmdQ.isEmpty() == true){
        sendDefaultRequest();
        return;
    }

    auto val = m_cmdQ.dequeue();
    if(val.isNull()){
        sendDefaultRequest();
        return;
    }

    sendJrpcRequest(val);

}

void JsonRpcDriver::tcpError(QAbstractSocket::SocketError tcpErr)
{
    Q_UNUSED(tcpErr)
    m_socketErrorCounter++;
    qDebug()<< Q_FUNC_INFO <<"Box "<< getBoxId()<<": "<< tcpSocket()->errorString();
    errorReport();
    if (m_pollTimer.isActive() == false){emit pollNextQueueElement();}
}

void JsonRpcDriver::requestTimedOut(QString req_id)
{
    m_timoutErrorCounter++;
    m_consecutiveErrorCounter++;

    errorReport();
    auto requestit = m_pendingRequests.find(req_id);
    if(requestit == m_pendingRequests.end()){
        qDebug()<< Q_FUNC_INFO <<"No response for timed out request"<< req_id;
        if (m_pollTimer.isActive() == false){emit pollNextQueueElement();}
    }else{
        qDebug()<< Q_FUNC_INFO <<"No response for timed out request"<< req_id;
        auto request = requestit.value();
        m_back.processData(request, Q_NULLPTR);
        request.clear();
    }

}

bool JsonRpcDriver::waitForDisconnected(int msecs)
{
    if(tcpSocket() == Q_NULLPTR)
        return false;

    //#if defined( Q_OS_WIN )
    //QAbstractSocket::waitForDisconnected may fail randomly on Windows.
    //Consider using the event loop and the disconnected() signal if your
    //software will run on Windows.
    QTimer timeoutTimer;

    QEventLoop disco_evt_loop;
    connect(tcpSocket(), SIGNAL(disconnected()), &disco_evt_loop, SLOT(quit()));
    connect(tcpSocket(), SIGNAL(error(QAbstractSocket::SocketError)), &disco_evt_loop, SLOT(quit()));
    connect(&timeoutTimer, SIGNAL(timeout()), &disco_evt_loop, SLOT(quit()));

    if(msecs >= 0){
        timeoutTimer.setInterval(msecs);
        timeoutTimer.setSingleShot(true);
        timeoutTimer.start();
    }

    if(tcpSocket()->state() == QAbstractSocket::UnconnectedState){
        return true;
        disconnect(tcpSocket(), SIGNAL(disconnected()), &disco_evt_loop, SLOT(quit()));
        disconnect(tcpSocket(), SIGNAL(error(QAbstractSocket::SocketError)), &disco_evt_loop, SLOT(quit()));
    }

    disco_evt_loop.exec();

    disconnect(tcpSocket(), SIGNAL(disconnected()), &disco_evt_loop, SLOT(quit()));
    disconnect(tcpSocket(), SIGNAL(error(QAbstractSocket::SocketError)), &disco_evt_loop, SLOT(quit()));

    bool timed_out = timeoutTimer.isActive() == false;
    bool error_occurred = false;
    timeoutTimer.stop();

    if(timed_out == true)
        return false;

    if(error_occurred == true)
        return false;

    return true;
}

void JsonRpcDriver::sendJrpcRequest(QSharedPointer<JsonRpcRequest> request)
{
    if(isDeviceConnected() == false)
        return;

    if(request.isNull()){
        qDebug()<< Q_FUNC_INFO <<"request is null";
        if (m_pollTimer.isActive() == false){emit pollNextQueueElement();}
        return;
    }

    m_buffer.clear();
    m_consecutiveParseErrorCounter = 0;

    QByteArray datagram = request->toJson();

    if(tcpSocket()->write(datagram) == -1){
        qDebug()<< Q_FUNC_INFO <<"Error sending request"<< tcpSocket()->errorString();
        if (m_pollTimer.isActive() == false){emit pollNextQueueElement();}
        return;
    }

    request->wasSent(this, m_avgRequestTime);
    m_requestCounter++;

    if(request->internalSource() == false){
        qDebug()<< Q_FUNC_INFO <<datagram;
    }

    //if(request != Q_NULLPTR){
    if(request.isNull() == false){
        if(m_pendingRequests.contains(request->id()) == false)
            m_pendingRequests.insert(request->id(), request);
    }

    for(auto it = m_pendingRequests.begin(); it != m_pendingRequests.end();){
        //auto id = it.key();
        auto req = it.value();
        if(req.isNull()){
            it = m_pendingRequests.erase(it);
            continue;
        }
        if(req->timedOut()){
            req.clear();
            //it = m_pendingRequests.erase(it);
            //continue;
        }

        ++it;
    }

}

void JsonRpcDriver::parseResponse(JsonRpcResponse *resp)
{
    bool resp_invalid = (resp == Q_NULLPTR);

    if(resp_invalid == true){
        qDebug()<< Q_FUNC_INFO << "Could not parse response";
        return;
    }

    if(resp->isError()){
        qDebug()<< Q_FUNC_INFO <<resp->rawData();
        return;
    }

    if(resp->isValid() == false){
        qDebug()<< Q_FUNC_INFO << "Response not Valid";
        return;
    }

    auto reponse_id = resp->id();
    auto it = m_pendingRequests.find(reponse_id);
    if(it == m_pendingRequests.end() ){
        qDebug()<< Q_FUNC_INFO << "No pending request for response" <<reponse_id;
        m_unmatchedResponseCounter++;
        errorReport();
        return;
    }

    auto req = it.value();
    req->wasReceived();
    //m_pendingRequests.remove(reponse_id);
    m_pendingRequests.remove(req->id());

    m_back.processData(req, resp);
    m_responseCounter++;
    m_consecutiveErrorCounter = 0;

    if(req->internalSource() == false){
        emit EventManager::get_instance()->receivedJsonRpcResponse(getBoxId(),req->methodName(), resp->result(), resp->id());
        qDebug()<< Q_FUNC_INFO << resp->rawData();
    }

    auto start = req->sendTime();
    auto elap = start.elapsed();

    if(m_avgRequestTime == 0 && req->id()>100){
        m_avgRequestTime = elap;
        //m_avgRequestTime = 25;
    }

    double lower  = 0.8*m_avgRequestTime;
    double upper  = 1.2*m_avgRequestTime;
    double number = elap;

    if (fabs((number-lower)) <= (upper-lower)){
        m_avgRequestTime = (m_avgRequestTime*9 + elap)/10;
    }else{
        if(req->id()>100){
            qDebug()<< Q_FUNC_INFO << "unusal request time: "<< elap<<"ms id"<<req->id()<<" avg"<< m_avgRequestTime;
        }else{
            m_avgRequestTime = (m_avgRequestTime*9 + elap)/10;
        }
    }

    //delete req;
    //req.clear();
    delete resp;

}

void JsonRpcDriver::errorReport() const
{
    qDebug()<< Q_FUNC_INFO
            << "Requests:" << m_requestCounter
            << "Responses:" << m_responseCounter
            << "timoutErrors:" << m_timoutErrorCounter
            << "socketErrors:" << m_socketErrorCounter
            << "parseErrors:" << m_parseErrorCounter
            << "UnmatchedResponses:" << m_unmatchedResponseCounter
            << "consecutiveErrors:" << m_consecutiveErrorCounter;
}

JsonRpcDriver::CommandQueue_t JsonRpcDriver::cmdQ() const
{
    return m_cmdQ;
}

JsonRpcDriver::MethodsList_t JsonRpcDriver::methodNames() const
{
    return JsonRpcDriverUtils::getMethodNamesOfTcpBox(getBoxId());
}


} // namespace talorion

#include "SerialPCUDriver.hpp"

#include <SerialBoxSystem/PfeifferVacuumProtocol/PVPFactory.hpp>

#include <QEventLoop>

#include <core/EventManager.hpp>

#ifndef PVP_POLLINTERVAL_MS
#define PVP_POLLINTERVAL_MS 35
#endif

namespace talorion {

SerialPCUDriver::SerialPCUDriver(EntityManager::EntityID box, QObject *par) :
    AbstractTcpDriver(box, par),
    m_pollTimer(),
    m_reconnectTimer(),
    m_serialSocket(Q_NULLPTR),
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
    connect(&m_back,SIGNAL(sendCommand(QSharedPointer<PVPRequest>)),this,SLOT(sendMethod(QSharedPointer<PVPRequest>)));
    connect(&m_back,SIGNAL(processDataFinished()), this, SIGNAL(pollNextQueueElement()));
}

SerialPCUDriver::~SerialPCUDriver()
{
    m_pollTimer.stop();

    disconectDevice();
    waitForDisconnected(1000);
}

bool SerialPCUDriver::isDeviceConnected() const
{
    if(serialSocket() == Q_NULLPTR)
        return false;

    //auto sock_st = serialSocket()->state();
    //return sock_st == QAbstractSocket::ConnectedState;
    return true;
}

SerialPCUDriver::CommandQueue_t SerialPCUDriver::cmdQ() const
{
    return m_cmdQ;
}

bool SerialPCUDriver::connectDevice()
{
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

    //quint16 port = m_back.port();

    if(serialSocket() == Q_NULLPTR){
        m_serialSocket = new QSerialPort();
        m_serialSocket->setBaudRate(QSerialPort::Baud9600);
        //m_serialSocket->setDataBits(QSerialPort::);
        m_serialSocket->setParity(QSerialPort::NoParity);
        m_serialSocket->setStopBits(QSerialPort::OneStop);
        //m_serialSocket->setStopBits(STOPBITS_10);
        //connect(serialSocket(), SIGNAL(connected()),this, SIGNAL(deviceConnected()));
        //connect(serialSocket(), SIGNAL(connected()),this, SLOT(afterDeviceConnect()));
        //connect(serialSocket(), SIGNAL(disconnected()), this, SLOT(afterDeviceDisconnect()));
        //connect(serialSocket(), SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(tcpError(QAbstractSocket::SocketError)));
    }

    //if(serialSocket()->state() == QAbstractSocket::UnconnectedState)
    //    serialSocket()->connectToHost(ip, port);

    serialSocket()->setPortName(ip);

    if (!serialSocket()->open(QIODevice::ReadWrite)) {
        qDebug()<< Q_FUNC_INFO << "Can't open"<< (ip)<<", error code "<<serialSocket()->error();
        return false;
    }

    emit deviceConnected();
    afterDeviceConnect();
    return true;
}

void SerialPCUDriver::disconectDevice()
{
    m_reconnectTimer.stop();

    if(isDeviceConnected() == false)
        return;

    disconnect(serialSocket(), SIGNAL(readyRead()), this, SLOT(socketReadyRead()));
    if(serialSocket() == Q_NULLPTR)
        return;

    serialSocket()->close();
    waitForDisconnected(1000);
    m_buffer.clear();

    delete m_serialSocket;
    m_serialSocket = Q_NULLPTR;

    emit deviceDisconnected();
    m_pollTimer.stop();

    for(auto it = m_pendingRequests.begin(); it != m_pendingRequests.end();){
        auto req = it.value();
        if(req.isNull()){
            it = m_pendingRequests.erase(it);
            continue;
        }else{
            it = m_pendingRequests.erase(it);
        }
    }
}

void SerialPCUDriver::sendMethod(QSharedPointer<PVPRequest> request)
{
    if(request.isNull())
        return;
    m_cmdQ.enqueue(request);
}

void SerialPCUDriver::sendDefaultRequest()
{
    auto getInfoCmd = m_back.getDefaultRequest();
    if(getInfoCmd.isNull())
        return;
    sendPVPRequest(getInfoCmd);
}

void SerialPCUDriver::afterDeviceConnect()
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

    connect(serialSocket(), SIGNAL(readyRead()), this, SLOT(socketReadyRead()));
    //auto getAllMethods = m_back.getAllMethodsRequest();
    //sendMethod(getAllMethods);

    m_pollTimer.setInterval(PVP_POLLINTERVAL_MS);
    m_pollTimer.setSingleShot(true);
    connect(&m_pollTimer,SIGNAL(timeout()),this,SLOT(pollQueue()));
    connect(this,SIGNAL(pollNextQueueElement()),&m_pollTimer,SLOT(start()));
    //connect(EventManager::get_instance(), SIGNAL(sendJsonRpcRequest(int,QString,QVariant,qint32)), this, SLOT(sendMethod(EntityManager::EntityID,QString,QVariant,qint32)));

    m_pollTimer.start();
}

void SerialPCUDriver::afterDeviceDisconnect()
{
    m_back.socketDisconnected();
    disconectDevice();
}

void SerialPCUDriver::socketReadyRead()
{
    QList<PVPResponse *> respez;
    bool resp_invalid = false;

    auto tmp = serialSocket()->readAll();

    m_buffer.append(tmp);

    if(m_buffer.trimmed().size() < 13)//minimum packet size
       return;

    respez = PVPFactory::createResponses(m_buffer);
    resp_invalid = (respez.isEmpty());

    if(resp_invalid == true){
        m_consecutiveParseErrorCounter++;
        if(m_consecutiveParseErrorCounter < 10){
            return;
        }
    }

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

void SerialPCUDriver::pollQueue()
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

    sendPVPRequest(val);
}

void SerialPCUDriver::requestTimedOut(QString req_id)
{
    m_timoutErrorCounter++;
    m_consecutiveErrorCounter++;

    errorReport();
    auto requestit = m_pendingRequests.find(req_id);
    if(requestit == m_pendingRequests.end()){
        qDebug()<< Q_FUNC_INFO <<"No response for timed out request"<< req_id;
        //if (m_pollTimer.isActive() == false){emit pollNextQueueElement();}
        emit pollNextQueueElement();
    }else{
        qDebug()<< Q_FUNC_INFO <<"No response for timed out request"<< req_id;
        auto request = requestit.value();
        //m_back.processData(request, Q_NULLPTR);
        //request.clear();
        //if (m_pollTimer.isActive() == false){emit pollNextQueueElement();}
        m_pendingRequests.remove(request->id());
        emit pollNextQueueElement();
    }
}

bool SerialPCUDriver::waitForDisconnected(int msecs)
{
    if(serialSocket() == Q_NULLPTR)
        return false;

    //#if defined( Q_OS_WIN )
    //QAbstractSocket::waitForDisconnected may fail randomly on Windows.
    //Consider using the event loop and the disconnected() signal if your
    //software will run on Windows.
    QTimer timeoutTimer;

    QEventLoop disco_evt_loop;
    connect(serialSocket(), SIGNAL(disconnected()), &disco_evt_loop, SLOT(quit()));
    connect(serialSocket(), SIGNAL(error(QAbstractSocket::SocketError)), &disco_evt_loop, SLOT(quit()));
    connect(&timeoutTimer, SIGNAL(timeout()), &disco_evt_loop, SLOT(quit()));

    if(msecs >= 0){
        timeoutTimer.setInterval(msecs);
        timeoutTimer.setSingleShot(true);
        timeoutTimer.start();
    }

    //if(serialSocket()->state() == QAbstractSocket::UnconnectedState){
    //    return true;
    //    disconnect(tcpSocket(), SIGNAL(disconnected()), &disco_evt_loop, SLOT(quit()));
    //    disconnect(tcpSocket(), SIGNAL(error(QAbstractSocket::SocketError)), &disco_evt_loop, SLOT(quit()));
    //}

    disco_evt_loop.exec();

    disconnect(serialSocket(), SIGNAL(disconnected()), &disco_evt_loop, SLOT(quit()));
    disconnect(serialSocket(), SIGNAL(error(QAbstractSocket::SocketError)), &disco_evt_loop, SLOT(quit()));

    bool timed_out = timeoutTimer.isActive() == false;
    bool error_occurred = false;
    timeoutTimer.stop();

    if(timed_out == true)
        return false;

    if(error_occurred == true)
        return false;

    return true;
}

void SerialPCUDriver::sendPVPRequest(CommandQueueElem_t request)
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

    QByteArray datagram = request->toTelegramm();

    if(serialSocket()->write(datagram) == -1){
        qDebug()<< Q_FUNC_INFO <<"Error sending request"<< serialSocket()->errorString();
        if (m_pollTimer.isActive() == false){emit pollNextQueueElement();}
        return;
    }

    request->wasSent(this, m_avgRequestTime);
    m_requestCounter++;

    if(request->internalSource() == false){
        qDebug()<< Q_FUNC_INFO <<datagram;
    }

    if(request.isNull() == false){
        if(m_pendingRequests.contains(request->id()) == false)
            m_pendingRequests.insert(request->id(), request);
    }

    for(auto it = m_pendingRequests.begin(); it != m_pendingRequests.end();){
        auto req = it.value();
        if(req.isNull()){
            it = m_pendingRequests.erase(it);
            continue;
        }
        if(req->timedOut()){
            req.clear();
        }

        ++it;
    }
}

void SerialPCUDriver::parseResponse(PVPResponse *resp)
{
    bool resp_invalid = (resp == Q_NULLPTR);

    if(resp_invalid == true){
        qDebug()<< Q_FUNC_INFO << "Could not parse response";
        return;
    }

    if(resp->isError()){
        qDebug()<< Q_FUNC_INFO <<"is Error"<<resp->rawData();
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
    m_pendingRequests.remove(req->id());

    m_back.processData(req, resp);
    m_responseCounter++;
    m_consecutiveErrorCounter = 0;

    if(req->internalSource() == false){
        //emit EventManager::get_instance()->receivedJsonRpcResponse(getBoxId(),req->methodName(), resp->result(), resp->id());
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

    delete resp;
}

void SerialPCUDriver::errorReport() const
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

} // namespace talorion

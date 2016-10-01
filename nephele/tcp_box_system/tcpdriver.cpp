#include "tcpdriver.hpp"

#include "core/event_manager.hpp"
#include "core/entity_manager.hpp"

#include <QDebug>
#include <QHostAddress>

#include "flowcontrollerbackend.h"

#define POLLINTERVAL_MS 10

namespace talorion {

tcpDriver::tcpDriver(int id, QByteArray getInfoCommand, QByteArray getMinimalSetActCommand, abstract_backend* bk, QObject *par):
    QObject(par),
    tcpSocket(NULL),
    transmissionContext(),
    recBuf(),
    sendBuf(),
    getInfoCommand_val(),
    getMinimalSetActCommand_val(),
    curlyOpen(0),
    curlyClose(0),
    timeoutTimer(),
    pollTimer(),
    lastIP(),
    lastPort(0),
    ongoingRequest(false),
    requestCounter(0),
    responseCounter(0),
    queue(NULL),
    box_id(id),
    mutex(),
    m_back(bk)
{
    qDebug()<<"creating box"<<box_id;

    if(!bk)
        m_back= new flowControllerBackend();

    connect(this, SIGNAL(receivedData(QVariantMap,tcpDriverDataTypes::dataType,int)),m_back,SLOT(processData(QVariantMap,tcpDriverDataTypes::dataType,int)));
    connect(m_back, SIGNAL(fcSetChangeCommand(QByteArray)),this,SLOT(setDataCommand(QByteArray)));

    //connect(event_manager::get_instance(),SIGNAL(avSetChangeCommand(QByteArray)),this,SLOT(setDataCommand(QByteArray)));
    connect(event_manager::get_instance(),SIGNAL(send_custom_command(QString)),this,SLOT(customCommand(QString)));
    //connect(this, SIGNAL(receivedData(QVariantMap,tcpDriverDataTypes::dataType,int)),event_manager::get_instance(),SIGNAL(receivedData(QVariantMap,tcpDriverDataTypes::dataType,int)));
    connect(this,SIGNAL(error(QString)),event_manager::get_instance(),SIGNAL(error(QString)));
    connect(this,SIGNAL(receivedCustomData(QString)),event_manager::get_instance(),SIGNAL(receivedCustomData(QString)));

    connect(this, SIGNAL(disconnected(int)), event_manager::get_instance(),SIGNAL(disconnect_tcp_box(int)));


    getInfoCommand_val = getInfoCommand;
    getMinimalSetActCommand_val = getMinimalSetActCommand;
    tcpSocket = new QTcpSocket();
    queue = new tcpCommandQueue();
    connect(tcpSocket, SIGNAL(readyRead()), this, SLOT(parsePackage()));
    connect(tcpSocket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(tcpError(QAbstractSocket::SocketError)));
    connect(tcpSocket, SIGNAL(disconnected()), this, SLOT(tcpSocket_disconnected()));
    connect(tcpSocket, SIGNAL(connected()), this, SLOT(tcpSocket_connected()));
    timeoutTimer = new QTimer();
    timeoutTimer->setSingleShot(true);
    connect(timeoutTimer, SIGNAL(timeout()),this,SLOT(timeoutCheck()));

    pollTimer = new QTimer();
    pollTimer->setInterval(POLLINTERVAL_MS);
    pollTimer->setSingleShot(true);
    requestCounter = 0;
    responseCounter = 0;
    connect(pollTimer,SIGNAL(timeout()),this,SLOT(poll()));
}

tcpDriver::~tcpDriver()
{
    timeoutTimer->stop();
    pollTimer->stop();

    if(m_back)
        delete m_back;
    m_back = Q_NULLPTR;

}

bool tcpDriver::connectDevice(QString ip, qint32 port, int timeoutMs)
{
    QMutexLocker locker(&mutex);

    timeoutTimer->setInterval(timeoutMs);

    this->tcpSocket->connectToHost(ip,port);
//    if(tcpSocket->waitForConnected(timeoutMs))
//    {
        lastIP=ip;
        lastPort=port;
        //        //queue->pushFirst("uibkafc getAll", tcpDriverDataTypes::ALLDATA);
        //        queue->pushFirst(getInfoCommand_val, tcpDriverDataTypes::ALLDATA);
        //        pollTimer->start();

        return true;
//    }
    //emit error("Connection Timeout to " + ip + ":" + QString::number(port),getBox_id());
    //qDebug()<<"Connection Timeout to " + ip + ":" + QString::number(port),getBox_id();

//    return false;
}

void tcpDriver::disconect()
{
    if(!tcpSocket)
        return;
    tcpSocket->close();
}


void tcpDriver::setDataCommand(QByteArray cmd)
{
    //qDebug()<<"sending "<<getBox_id()<<cmd<<"to "<<tcpSocket->peerAddress()<<tcpSocket->peerPort()<<"from "<<tcpSocket->localAddress()<<tcpSocket->localPort();
    tcpCommand* command = new tcpCommand(cmd,tcpDriverDataTypes::SETDATA);
    queue->pushLast(command);
}

void tcpDriver::customCommand(const QString& cm)
{
    QByteArray cmd = cm.toLocal8Bit();
    tcpCommand* command = new tcpCommand(cmd,tcpDriverDataTypes::CUSTOMCOMMAND);
    queue->pushLast(command);
}

void tcpDriver::sendCommand(QByteArray cmd, tcpDriverDataTypes::dataType type)
{
    //qDebug()<<getBox_id()<<cmd;
    QMutexLocker locker(&mutex);

    recheckConnection();
    recBuf = "";
    curlyOpen = 0;
    curlyClose = 0;
    ongoingRequest = true;
    timeoutTimer->start();

    tcpSocket->write(cmd.trimmed().append("\r\n"));
    transmissionContext = type;
    requestCounter++;

}

void tcpDriver::remove_all_values()
{

    qDebug()<< "remove_all_values";
    int entity=getBox_id();
    foreach(int val, entity_manager::get_instance()->get_all_DValues()){
        int box_id_e= entity_manager::get_instance()->get_box_id_component(val);
        if(box_id_e == entity)
            entity_manager::get_instance()->delete_entity(val);
    }

    foreach(int val, entity_manager::get_instance()->get_all_Values()){
        int box_id_e= entity_manager::get_instance()->get_box_id_component(val);
        if(box_id_e == entity)
            entity_manager::get_instance()->delete_entity(val);
    }


    //entity_manager::get_instance()->slot_connection_state_component(entity, false);
}

void tcpDriver::tcpSocket_connected()
{
    entity_manager::get_instance()->slot_connection_state_component(getBox_id(), true);

    //lastIP=ip;
    //lastPort=port;
    //queue->pushFirst("uibkafc getAll", tcpDriverDataTypes::ALLDATA);
    queue->pushFirst(getInfoCommand_val, tcpDriverDataTypes::ALLDATA);
    pollTimer->start();

    emit connected(getBox_id());
}

void tcpDriver::tcpSocket_disconnected()
{
    qDebug() << "tcpSocket_disconnected()!";
    tcpSocket->abort();
    timeoutTimer->stop();
    pollTimer->stop();
    ongoingRequest = false;
    remove_all_values();
    entity_manager::get_instance()->slot_connection_state_component(getBox_id(), false);
    emit disconnected(getBox_id());
}

int tcpDriver::getBox_id() const
{
    return box_id;
}


void tcpDriver::recheckConnection()
{

    if (tcpSocket->state() != QTcpSocket::ConnectedState && lastIP != "")
    {
        if (connectDevice(lastIP, lastPort, timeoutTimer->interval()))
        {
            emit connected(getBox_id());
        }
    }

}

void tcpDriver::poll()
{
    tcpCommand* cmd = queue->getNext();
    if (cmd != NULL)
    {
        sendCommand(cmd->getCmd(),cmd->getCmdType());
        delete cmd;
    }
    else
    {
        sendCommand(getMinimalSetActCommand_val,tcpDriverDataTypes::ACTSETDATA);
    }
}

void tcpDriver::parsePackage()
{
    QMutexLocker locker(&mutex);

    QByteArray tmp = tcpSocket->readAll();
    if (transmissionContext == tcpDriverDataTypes::ALLDATA || transmissionContext == tcpDriverDataTypes::ACTSETDATA)
    {
        curlyOpen += tmp.count('{');
        curlyClose += tmp.count('}');
        if (curlyOpen>0)
            recBuf.append(tmp);
        if (curlyOpen==curlyClose && curlyOpen > 0)
        {
            timeoutTimer->stop();
            QJsonParseError* jsonerror = new QJsonParseError();
            QJsonObject obj = QJsonDocument::fromJson(recBuf,jsonerror).object();
            if (jsonerror->error == QJsonParseError::NoError)
            {
                emit receivedData(obj.toVariantMap(), transmissionContext, getBox_id());
            } else
            {
                emit error("Invalid Json received: " + jsonerror->errorString());
                qDebug()<<"Invalid Json received: " + jsonerror->errorString();
            }
            ongoingRequest = false;
            pollTimer->start();
            responseCounter++;
        }
    }
    else if (transmissionContext == tcpDriverDataTypes::SETDATA)
    {
        //qDebug()<<"receiving from "<<tcpSocket->peerAddress()<<tcpSocket->peerPort()<<"to "<<tcpSocket->localAddress()<<tcpSocket->localPort();
        ongoingRequest = false;
        timeoutTimer->stop();
        if (tmp.trimmed() != "OK")
            emit setDataError("Could not set Value: " + tmp, getBox_id());
        pollTimer->start();
        responseCounter++;
    }
    else if (transmissionContext == tcpDriverDataTypes::CUSTOMCOMMAND)
    {
        ongoingRequest = false;
        timeoutTimer->stop();
        QString rec= QString::fromLocal8Bit(tmp);
        emit receivedCustomData(rec, getBox_id());
        pollTimer->start();
        responseCounter++;
    }
    else if (transmissionContext == tcpDriverDataTypes::IDLE)
    {
        emit error("Unexpected Package received: " + tmp, getBox_id());
        qDebug()<<"Unexpected Package received: " + tmp, getBox_id();
    }
}

void tcpDriver::timeoutCheck()
{
    if (ongoingRequest)
    {
        ongoingRequest = false;
        pollTimer->start();
        qDebug() << "Receive Timeout!";
        emit error("Receive Timeout", getBox_id());
    }
}

void tcpDriver::tcpError(QAbstractSocket::SocketError tcpErr)
{
    Q_UNUSED(tcpErr)
    emit error(tcpSocket->errorString(), getBox_id());
    qDebug()<<tcpSocket->errorString();

    switch (tcpErr) {
    case QAbstractSocket::ConnectionRefusedError:           {qDebug() <<"Box "<< getBox_id()<<": "<< "ConnectionRefusedError"; break;}
    case QAbstractSocket::RemoteHostClosedError:            {
        qDebug() <<"Box "<< getBox_id()<<": "<< "RemoteHostClosedError";
        tcpSocket->abort();
        timeoutTimer->stop();
        pollTimer->stop();
        ongoingRequest = false;
        remove_all_values();
        entity_manager::get_instance()->slot_connection_state_component(getBox_id(), false);
        emit disconnected(getBox_id());

        break;}
    case QAbstractSocket::HostNotFoundError:                {
        qDebug() <<"Box "<< getBox_id()<<": "<< "HostNotFoundError";
        tcpSocket->abort();
        timeoutTimer->stop();
        pollTimer->stop();
        ongoingRequest = false;
        remove_all_values();
        entity_manager::get_instance()->slot_connection_state_component(getBox_id(), false);
        emit disconnected(getBox_id());

        break;}
    case QAbstractSocket::SocketAccessError:                {qDebug() <<"Box "<< getBox_id()<<": "<< "SocketAccessError"; break;}
    case QAbstractSocket::SocketResourceError:              {qDebug() <<"Box "<< getBox_id()<<": "<< "SocketResourceError"; break;}
    case QAbstractSocket::SocketTimeoutError:               {qDebug() <<"Box "<< getBox_id()<<": "<< "SocketTimeoutError"; break;}
    case QAbstractSocket::DatagramTooLargeError:            {qDebug() <<"Box "<< getBox_id()<<": "<< "DatagramTooLargeError"; break;}
    case QAbstractSocket::NetworkError:                     {
        qDebug() <<"Box "<< getBox_id()<<": "<< "NetworkError";
        tcpSocket->abort();
        timeoutTimer->stop();
        pollTimer->stop();
        ongoingRequest = false;
        remove_all_values();
        entity_manager::get_instance()->slot_connection_state_component(getBox_id(), false);
        emit disconnected(getBox_id());

        break;}
    case QAbstractSocket::AddressInUseError:                {qDebug() <<"Box "<< getBox_id()<<": "<< "AddressInUseError"; break;}
    case QAbstractSocket::SocketAddressNotAvailableError:   {qDebug() <<"Box "<< getBox_id()<<": "<< "SocketAddressNotAvailableError"; break;}
    case QAbstractSocket::UnsupportedSocketOperationError:  {qDebug() <<"Box "<< getBox_id()<<": "<< "UnsupportedSocketOperationError"; break;}
    case QAbstractSocket::ProxyAuthenticationRequiredError: {qDebug() <<"Box "<< getBox_id()<<": "<< "ProxyAuthenticationRequiredError"; break;}
    case QAbstractSocket::SslHandshakeFailedError:          {qDebug() <<"Box "<< getBox_id()<<": "<< "SslHandshakeFailedError"; break;}
    case QAbstractSocket::UnfinishedSocketOperationError:   {qDebug() <<"Box "<< getBox_id()<<": "<< "UnfinishedSocketOperationError"; break;}
    case QAbstractSocket::ProxyConnectionRefusedError:      {qDebug() <<"Box "<< getBox_id()<<": "<< "ProxyConnectionRefusedError"; break;}
    case QAbstractSocket::ProxyConnectionClosedError:       {qDebug() <<"Box "<< getBox_id()<<": "<< "ProxyConnectionClosedError"; break;}
    case QAbstractSocket::ProxyConnectionTimeoutError:      {qDebug() <<"Box "<< getBox_id()<<": "<< "ProxyConnectionTimeoutError"; break;}
    case QAbstractSocket::ProxyNotFoundError:               {qDebug() <<"Box "<< getBox_id()<<": "<< "ProxyNotFoundError"; break;}
    case QAbstractSocket::ProxyProtocolError:               {qDebug() <<"Box "<< getBox_id()<<": "<< "ProxyProtocolError"; break;}
    case QAbstractSocket::OperationError:                   {
        qDebug() <<"Box "<< getBox_id()<<": "<< "OperationError";
        //tcpSocket->abort();
        break;
    }
    case QAbstractSocket::SslInternalError:                 {qDebug() <<"Box "<< getBox_id()<<": "<< "SslInternalError"; break;}
    case QAbstractSocket::SslInvalidUserDataError:          {qDebug() <<"Box "<< getBox_id()<<": "<< "SslInvalidUserDataError"; break;}
    case QAbstractSocket::TemporaryError:                   {qDebug() <<"Box "<< getBox_id()<<": "<< "TemporaryError"; break;}
    default:                               {qDebug() <<"Box "<< getBox_id()<<": "<< "UnknownSocketError"; break;}
    }
}
}




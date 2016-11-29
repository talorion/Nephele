#include "tcpdriver.hpp"

#include "core/event_manager.hpp"
#include "core/entity_manager.hpp"

#include <QDebug>
#include <QHostAddress>
#include <QtSerialPort/QSerialPort>

#include "flowcontrollerbackend.h"

#define POLLINTERVAL_MS 50

namespace talorion {

tcpDriver::tcpDriver(int id, QByteArray getInfoCommand, QByteArray getMinimalSetActCommand, abstract_backend* bk, QObject *par):
    QObject(par),
    tcpSocket(NULL),
    serialSocket(NULL),
    transmissionContext(),
    recBuf(),
    sendBuf(),
    getInfoCommand_val(),
    getMinimalSetActCommand_val(),
    commandCounter(0),
    isScope(false),
    ScopeNumberOfMeasurements(6),
    curlyOpen(0),
    curlyClose(0),
    timeoutTimer(),
    pollTimer(),
    lastIP(),
    lastPort(0),
    SerialBaud(QSerialPort::Baud9600),
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
    {
        m_back= new flowControllerBackend();
        qDebug() << "no backend defined -> used FC...";
    }
    connect(this, SIGNAL(receivedData(QVariantMap,tcpDriverDataTypes::dataType,int)),m_back,SLOT(processData(QVariantMap,tcpDriverDataTypes::dataType,int)));
    connect(m_back, SIGNAL(fcSetChangeCommand(QByteArray)),this,SLOT(setDataCommand(QByteArray)));

    //connect(event_manager::get_instance(),SIGNAL(avSetChangeCommand(QByteArray)),this,SLOT(setDataCommand(QByteArray)));
    connect(event_manager::get_instance(),SIGNAL(send_custom_command(QString)),this,SLOT(customCommand(QString)));
    //connect(this, SIGNAL(receivedData(QVariantMap,tcpDriverDataTypes::dataType,int)),event_manager::get_instance(),SIGNAL(receivedData(QVariantMap,tcpDriverDataTypes::dataType,int)));
    connect(this,SIGNAL(error(QString)),event_manager::get_instance(),SIGNAL(error(QString)));
    connect(this,SIGNAL(receivedCustomData(QString)),event_manager::get_instance(),SIGNAL(receivedCustomData(QString)));

    connect(this, SIGNAL(disconnected(int)), event_manager::get_instance(),SIGNAL(disconnect_tcp_box(int)));


    pollTimer = new QTimer();
    pollTimer->setInterval(POLLINTERVAL_MS);
    pollTimer->setSingleShot(true);
    requestCounter = 0;
    responseCounter = 0;
    connect(pollTimer,SIGNAL(timeout()),this,SLOT(poll()));

    timeoutTimer = new QTimer();
    timeoutTimer->setSingleShot(true);
    connect(timeoutTimer, SIGNAL(timeout()),this,SLOT(timeoutCheck()));


    getInfoCommand_val = getInfoCommand;
    getMinimalSetActCommand_val = getMinimalSetActCommand;

    QString ip = entity_manager::get_instance()->get_ip_address_component(box_id);
    if (ip.startsWith("COM")) {
        serialSocket = new QSerialPort();
        queue = new tcpCommandQueue();
        connect(serialSocket, SIGNAL(readyRead()), this, SLOT(parsePackage()));
        connect(serialSocket, SIGNAL(error(QSerialPort::SerialPortError)), this, SLOT(serialError(QSerialPort::SerialPortError)));
        serialSocket->setPortName(ip);
        serialSocket->setBaudRate(SerialBaud);
        serialSocket->setDataBits(serialSocket->Data8);
        bool opened = serialSocket->open(QIODevice::ReadWrite);
        //connect(serialSocket, SIGNAL(connected()), this, SLOT(tcpSocket_connected()));
        if (opened) this->tcpSocket_connected();
    } else {
        tcpSocket = new QTcpSocket();
        queue = new tcpCommandQueue();
        connect(tcpSocket, SIGNAL(readyRead()), this, SLOT(parsePackage()));
        connect(tcpSocket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(tcpError(QAbstractSocket::SocketError)));
        connect(tcpSocket, SIGNAL(disconnected()), this, SLOT(tcpSocket_disconnected()));
        connect(tcpSocket, SIGNAL(connected()), this, SLOT(tcpSocket_connected()));
        }





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

    if (ip.startsWith("COM"))
    {
        QSerialPort* ser1;
        ser1 = new QSerialPort(ip);
        ser1->setPortName(ip);
        ser1->setBaudRate(SerialBaud);
        ser1->setDataBits(ser1->Data8);
        if (ser1->isOpen()) return true; else return false;
    }
    else
    {
        this->tcpSocket->connectToHost(ip,port);
        lastIP=ip;
        lastPort=port;
        return true;
    }
}

void tcpDriver::disconect()
{
    qDebug() << "disconnect()";

    if(this->tcpSocket) this->tcpSocket->close();

    if(this->serialSocket) this->serialSocket->close();

}


void tcpDriver::setDataCommand(QByteArray cmd)
{
    //qDebug() << "setDataCommand"<<cmd<<"to"<<getBox_id();
    tcpCommand* command = new tcpCommand(cmd,tcpDriverDataTypes::SETDATA);
    queue->pushLast(command);
}

void tcpDriver::customCommand(const QString& cm)
{
    //qDebug() << "customCommand"<<cm<<"to"<<getBox_id();
    QByteArray cmd = cm.toLocal8Bit();
    tcpCommand* command = new tcpCommand(cmd,tcpDriverDataTypes::CUSTOMCOMMAND);
    queue->pushLast(command);
}

void tcpDriver::sendCommand(QByteArray cmd, tcpDriverDataTypes::dataType type)
{

    QMutexLocker locker(&mutex);
    recheckConnection();
    recBuf = "";
    curlyOpen = 0;
    curlyClose = 0;
    ongoingRequest = true;
    timeoutTimer->start();

    if (tcpSocket){
        tcpSocket->write(cmd.trimmed());
        qDebug() << "sendCommand"<<cmd<<"to"<<getBox_id();

    }
    else {
        serialSocket->write(cmd.trimmed()+"\n");
    }
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
}

void tcpDriver::tcpSocket_connected()
{
    qDebug() << "tcpSocket_connected()" << getBox_id() << "pushing" << getInfoCommand_val;
    entity_manager::get_instance()->slot_connection_state_component(getBox_id(), true);
    queue->pushFirst(getInfoCommand_val, tcpDriverDataTypes::ALLDATA);
    pollTimer->start();
    emit connected(getBox_id());
}

void tcpDriver::tcpSocket_disconnected()
{
    qDebug() << "tcpSocket_disconnected()!" << getBox_id();
    if(tcpSocket->state() == QAbstractSocket::ConnectedState)
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
    //qDebug() << "recheckConnection"<<getBox_id()<<"lastIP: "<< lastIP;
    if (tcpSocket)
    {
        if (tcpSocket->state() != QTcpSocket::ConnectedState && lastIP != "")
        {
            if (connectDevice(lastIP, lastPort, timeoutTimer->interval()))
                emit connected(getBox_id());
        }
    }
    if (serialSocket) {
        if (serialSocket->error() == QSerialPort::NoError) return;

        qDebug() << "RECONNECT SERIAL DEVICE....";

        serialSocket->close();
        queue->flush();
        serialSocket->setPortName(lastIP);
        serialSocket->setBaudRate(SerialBaud);
        serialSocket->setDataBits(serialSocket->Data8);
        bool opened = serialSocket->open(QIODevice::ReadWrite);
        if (opened) this->tcpSocket_connected();
    }
}

void tcpDriver::poll()
{
    //qDebug() << "poll()"<<"lastIP:"<<lastIP;

    tcpCommand* cmd = queue->getNext();
    if (cmd != NULL)
    {
        sendCommand(cmd->getCmd(),cmd->getCmdType());
        //qDebug()<<"polling" << cmd;
        delete cmd;
    }
    else
    {
        QString ip = entity_manager::get_instance()->get_ip_address_component(box_id);

        if (ip.startsWith("COM") && lastIP=="") {
            sendCommand(getInfoCommand_val,tcpDriverDataTypes::ALLDATA);
            lastIP=ip;
        }
        else
            sendCommand(getMinimalSetActCommand_val,tcpDriverDataTypes::ACTSETDATA);
    }
}

void tcpDriver::parsePackage()
{

    QMutexLocker locker(&mutex);

    QString ip = entity_manager::get_instance()->get_ip_address_component(box_id);

    QByteArray tmp;
    if (ip.startsWith("COM"))
        {
        if (!serialSocket->canReadLine()) return;
        //qDebug() << "Package received.";
        tmp = serialSocket->readAll();
        tmp = tmp.trimmed();
        }
    else {
        tmp = tcpSocket->readAll();
        qDebug() << "Package from "<< ip << " received:" << tmp;

    }

    if (isScope) {
        QByteArray cmd = getInfoCommand_val;
        QByteArray currentCmd = "";
        for (int i=0; i<ScopeNumberOfMeasurements; i++) {
            int a=0;
            for (int j=0; j<i; j++) a=cmd.indexOf("|",a+1);
            if (i==commandCounter) currentCmd = cmd.mid(a+1,cmd.indexOf("|",a+1)-a-1);
        }
        int dataid = commandCounter-1;
        if (dataid < 0) dataid+=ScopeNumberOfMeasurements;
        tmp = "{\"AI\":[{\"id\": "+QByteArray::number(dataid)+", \"act\": "+tmp+"}]}";

        getMinimalSetActCommand_val = currentCmd;
        transmissionContext = tcpDriverDataTypes::ACTSETDATA;


        commandCounter++;
        if (commandCounter > ScopeNumberOfMeasurements-1) commandCounter=0;
        qDebug() << "------------------" << tmp;
    }

    if (tmp.startsWith("RIGOL TECHNOLOGIES,DS1054Z")) {
        qDebug() << "RIGOL SCOPE DETECTED!";

        tmp = "{\"AI\": [";


        for (int i=0; i<3; i++) {
            tmp=tmp+"{\"name\": \"Amplitude "+QByteArray::number(i+1)+"\", \"units\": \"V\", \"smin\": 0.00, \"smax\": 1000.00, \"amin\": 0.00, \"amax\": 1000.00, \"id\": "+QByteArray::number(i)+"},";
           // if (i<2) tmp=tmp+",";
        }
        tmp=tmp+"{\"name\": \"Phase 1->2\", \"units\": \"deg\", \"smin\": 0.00, \"smax\": 360.00, \"amin\": 0.00, \"amax\": 360.00, \"id\": 3},";
        tmp=tmp+"{\"name\": \"Phase 2->3\", \"units\": \"deg\", \"smin\": 0.00, \"smax\": 360.00, \"amin\": 0.00, \"amax\": 360.00, \"id\": 4},";
        tmp=tmp+"{\"name\": \"Frequency\", \"units\": \"Hz\", \"smin\": 0.00, \"smax\": 2e7, \"amin\": 0.00, \"amax\": 2e7, \"id\": 5}";

        tmp += "]}";

        QByteArray cmd = getInfoCommand_val;
        QByteArray currentCmd = "";
        for (int i=0; i<ScopeNumberOfMeasurements; i++) {
            int a=0;
            for (int j=0; j<i; j++) a=cmd.indexOf("|",a+1);
            if (i==commandCounter) currentCmd = cmd.mid(a+1,cmd.indexOf("|",a+1)-a-1);
        }
        isScope=true;
        getMinimalSetActCommand_val = currentCmd;
        transmissionContext = tcpDriverDataTypes::ALLDATA;

        commandCounter++;
        if (commandCounter > ScopeNumberOfMeasurements) commandCounter=0;

    }




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
                //qDebug() << "Data received; box:" << getBox_id();
                emit receivedData(obj.toVariantMap(), transmissionContext, getBox_id());
            } else
            {
                emit error("Invalid Json received: " + jsonerror->errorString());
                qDebug()<<"Invalid Json received: " + jsonerror->errorString();
            }
            ongoingRequest = false;
            pollTimer->start();
            responseCounter++;
            delete jsonerror;
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
    //qDebug()<<"timeoutCheck"<<getBox_id();
    if (ongoingRequest)
    {
        ongoingRequest = false;
        pollTimer->start();
        qDebug() << "Receive Timeout!";
        emit error("Receive Timeout", getBox_id());
    }
}

void tcpDriver::serialError(QSerialPort::SerialPortError serialErr)
{
    Q_UNUSED(serialErr)
    emit error(serialSocket->errorString(), getBox_id());
    qDebug()<<serialSocket->errorString() << getBox_id();


    switch (serialErr) {
    //case QSerialPort::BreakConditionError: { qDebug()<< "SERIALError: Box "<< getBox_id()<<": Break Condition Error"; break; }
    case QSerialPort::DeviceNotFoundError: { qDebug()<< "SERIALError: Box "<< getBox_id()<<": Device not found"; break; }
    //case QSerialPort::FramingError: { qDebug()<< "SERIALError: Box "<< getBox_id()<<": Framing Error"; break; }
    case QSerialPort::NotOpenError: { qDebug()<< "SERIALError: Box "<< getBox_id()<<": Not Open Error"; break; }
    case QSerialPort::OpenError: { qDebug()<< "SERIALError: Box "<< getBox_id()<<": Open Error"; break; }
    //case QSerialPort::ParityError: { qDebug()<< "SERIALError: Box "<< getBox_id()<<": Parity Error"; break; }
    case QSerialPort::PermissionError: { qDebug()<< "SERIALError: Box "<< getBox_id()<<": Permission Error"; break; }
    case QSerialPort::ReadError: { qDebug()<< "SERIALError: Box "<< getBox_id()<<": Read Error"; break; }
    case QSerialPort::ResourceError: { qDebug()<< "SERIALError: Box "<< getBox_id()<<": Ressource Error"; break; }
    case QSerialPort::WriteError: { qDebug()<< "SERIALError: Box "<< getBox_id()<<": Write Error"; break; }
    default: {qDebug() <<"Box "<< getBox_id()<<": "<< "Unknown SERIAL Error"; break;}

    }
}

void tcpDriver::tcpError(QAbstractSocket::SocketError tcpErr)
{
    Q_UNUSED(tcpErr)
    emit error(tcpSocket->errorString(), getBox_id());
    qDebug()<<tcpSocket->errorString() << getBox_id();

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




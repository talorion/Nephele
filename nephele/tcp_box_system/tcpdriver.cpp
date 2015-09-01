#include "tcpdriver.hpp"

#include "core/event_manager.hpp"

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

        getInfoCommand_val = getInfoCommand;
        getMinimalSetActCommand_val = getMinimalSetActCommand;
        tcpSocket = new QTcpSocket();
        queue = new tcpCommandQueue();
        connect(tcpSocket, SIGNAL(readyRead()), this, SLOT(parsePackage()));
        connect(tcpSocket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(tcpError(QAbstractSocket::SocketError)));
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

    }

    bool tcpDriver::connectDevice(QString ip, qint32 port, int timeoutMs)
    {
        QMutexLocker locker(&mutex);

        timeoutTimer->setInterval(timeoutMs);

        this->tcpSocket->connectToHost(ip,port);
        if(tcpSocket->waitForConnected(timeoutMs))
        {
            lastIP=ip;
            lastPort=port;
            //queue->pushFirst("uibkafc getAll", tcpDriverDataTypes::ALLDATA);
            queue->pushFirst(getInfoCommand_val, tcpDriverDataTypes::ALLDATA);
            pollTimer->start();
            return true;
        }
        emit error("Connection Timeout to " + ip + ":" + QString::number(port),getBox_id());
        return false;
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
                QJsonObject obj = QJsonDocument::fromJson(recBuf).object();
                emit receivedData(obj.toVariantMap(), transmissionContext, getBox_id());
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
    }
}




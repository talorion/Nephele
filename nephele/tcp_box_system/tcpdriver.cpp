#include "tcpdriver.hpp"

#include "core/event_manager.hpp"

#include <QDebug>

#define POLLINTERVAL_MS 10

namespace talorion {

    tcpDriver::tcpDriver(QByteArray getInfoCommand, QByteArray getMinimalSetActCommand, QObject *par):
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
        queue(NULL)
    {

        connect(event_manager::get_instance(),SIGNAL(avSetChangeCommand(QByteArray)),this,SLOT(setDataCommand(QByteArray)));
        connect(this, SIGNAL(receivedData(QVariantMap,tcpDriverDataTypes::dataType)),event_manager::get_instance(),SIGNAL(receivedData(QVariantMap,tcpDriverDataTypes::dataType)));
        connect(this,SIGNAL(error(QString)),event_manager::get_instance(),SIGNAL(error(QString)));

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
        emit error("Connection Timeout to " + ip + ":" + QString::number(port));
        return false;
    }


    void tcpDriver::setDataCommand(QByteArray cmd)
    {
        //genericCriticalCommand(cmd, tcpDriverDataTypes::SETDATA);
        tcpCommand* command = new tcpCommand(cmd,tcpDriverDataTypes::SETDATA);
        queue->pushLast(command);
    }

    void tcpDriver::customCommand(QByteArray cmd)
    {
        //genericCriticalCommand(cmd, tcpDriverDataTypes::SETDATA);
        tcpCommand* command = new tcpCommand(cmd,tcpDriverDataTypes::CUSTOMCOMMAND);
        queue->pushLast(command);
    }

    void tcpDriver::sendCommand(QByteArray cmd, tcpDriverDataTypes::dataType type)
    {
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

    void tcpDriver::recheckConnection()
    {
        if (tcpSocket->state() != QTcpSocket::ConnectedState && lastIP != "")
        {
            if (connectDevice(lastIP, lastPort, timeoutTimer->interval()))
            {
                emit connected();
            }
        }
    }

    void tcpDriver::poll()
    {
        //    qDebug() << "TX: " << QString::number(requestCounter) << " RX: " << QString::number(responseCounter);
        //    qDebug() << "Queue Length: " << QString::number(queue->length());
        tcpCommand* cmd = queue->getNext();
        if (cmd != NULL)
        {
            qDebug() << "Working on command: " << cmd->getCmd();
            sendCommand(cmd->getCmd(),cmd->getCmdType());
        }
        else
        {
            //sendCommand("uibkafc getActSet",tcpDriverDataTypes::ACTSETDATA);
            sendCommand(getMinimalSetActCommand_val,tcpDriverDataTypes::ACTSETDATA);
        }
    }

    void tcpDriver::parsePackage()
    {
        QByteArray tmp = tcpSocket->readAll();
        //qDebug() << tmp ;
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
                emit receivedData(obj.toVariantMap(), transmissionContext);
                ongoingRequest = false;
                pollTimer->start();
                responseCounter++;
            }
        }
        else if (transmissionContext == tcpDriverDataTypes::SETDATA)
        {
            ongoingRequest = false;
            timeoutTimer->stop();
            if (tmp.trimmed() != "OK")
                emit setDataError("Could not set Value: " + tmp);
            pollTimer->start();
            responseCounter++;
        }
        else if (transmissionContext == tcpDriverDataTypes::CUSTOMCOMMAND)
        {
            ongoingRequest = false;
            timeoutTimer->stop();
            emit receivedCustomData(tmp);
            pollTimer->start();
            responseCounter++;
        }
        else if (transmissionContext == tcpDriverDataTypes::IDLE)
        {
            emit error("Unexpected Package received: " + tmp);
        }
    }

    void tcpDriver::timeoutCheck()
    {
        if (ongoingRequest)
        {
            ongoingRequest = false;
            pollTimer->start();
            qDebug() << "Receive Timeout!";
            emit error("Receive Timeout");
        }
    }

    void tcpDriver::tcpError(QAbstractSocket::SocketError tcpErr)
    {
        Q_UNUSED(tcpErr)
        emit error(tcpSocket->errorString());
    }
}




#include "tcpdriver.hpp"

#include "core/EventManager.hpp"
#include "core/EntityManager.hpp"

#include <QDebug>
#include <QHostAddress>

#include "flowcontrollerbackend.h"

#ifndef POLLINTERVAL_MS
#define POLLINTERVAL_MS 10
#endif

namespace talorion {

  tcpDriver::tcpDriver(int id, QByteArray getInfoCommand, QByteArray getMinimalSetActCommand, abstract_backend* bk, QObject *par):
    AbstractTcpDriver(id, par),
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
    //box_id(id),
    mutex(),
    m_back(bk)
  {
    if(bk == Q_NULLPTR)
      m_back= new flowControllerBackend();

    initialize(getInfoCommand, getMinimalSetActCommand, m_back);
  }

  tcpDriver::tcpDriver(int id, abstract_backend* bk,  QObject *par):
    AbstractTcpDriver(id, par),
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
    //box_id(id),
    mutex(),
    m_back(bk)
  {
    if(bk == Q_NULLPTR)
      m_back= new flowControllerBackend();

    auto getInfoCommand = m_back->getInfoCommand();
    auto getMinimalSetActCommand = m_back->getMinimalSetActCommand();

    initialize(getInfoCommand, getMinimalSetActCommand, m_back);

  }

  tcpDriver::~tcpDriver()
  {
    timeoutTimer->stop();
    pollTimer->stop();

    if(m_back)
      delete m_back;
    m_back = Q_NULLPTR;

  }

  bool tcpDriver::connectDevice()
  {
    if(EntityManager::isValid(getBoxId()) == false)
      return false;

    QString ip = EntityManager::get_instance()->get_ip_address_component(getBoxId());
    if(ip.isEmpty() || ip.isNull())
      return false;

    quint16 port = EntityManager::get_instance()->get_port_component(getBoxId());
//    if(port < 0 || port > 0xFFFF)
//      return false;

    return connectDevice(ip, port, 4000);
  }

  bool tcpDriver::connectDevice(QString ip, qint32 port, int timeoutMs)
  {
    QMutexLocker locker(&mutex);

    timeoutTimer->setInterval(timeoutMs);

    this->tcpSocket->connectToHost(ip,port);

    lastIP=ip;
    lastPort=port;

    return true;
  }

  void tcpDriver::disconectDevice()
  {
    if(!tcpSocket)
      return;
    tcpSocket->close();
  }


  void tcpDriver::setDataCommand(QByteArray cmd)
  {
    tcpCommand* command = new tcpCommand(cmd,tcpDriverDataTypes::UIBK_ECMD_SETDATA);
    queue->pushLast(command);
  }

  void tcpDriver::customCommand(const QString& cm)
  {
    QByteArray cmd = cm.toLocal8Bit();
    tcpCommand* command = new tcpCommand(cmd,tcpDriverDataTypes::UIBK_ECMD_CUSTOMCOMMAND);
    queue->pushLast(command);
  }

  void tcpDriver::initialize(QByteArray getInfoCommand, QByteArray getMinimalSetActCommand, abstract_backend *bk)
  {
    // ========================
    tcpSocket = Q_NULLPTR;
    curlyOpen = 0;
    curlyClose = 0;
    lastPort = 0;
    ongoingRequest = false;
    requestCounter = 0;
    responseCounter = 0;
    queue = Q_NULLPTR;
    m_back = bk;
    // ========================

    qDebug()<<"creating box"<<getBoxId();

    if(!bk)
      m_back= new flowControllerBackend();

    connect(this, SIGNAL(receivedData(QVariantMap,tcpDriverDataTypes::dataType,int)),m_back,SLOT(processData(QVariantMap,tcpDriverDataTypes::dataType,int)));
    connect(m_back, SIGNAL(fcSetChangeCommand(QByteArray)),this,SLOT(setDataCommand(QByteArray)));

    connect(EventManager::get_instance(),SIGNAL(send_custom_command(QString)),this,SLOT(customCommand(QString)));
    connect(this,SIGNAL(error(QString)),EventManager::get_instance(),SIGNAL(error(QString)));
    connect(this,SIGNAL(receivedCustomData(QString)),EventManager::get_instance(),SIGNAL(receivedCustomData(QString)));

    connect(this, SIGNAL(disconnected(int)), EventManager::get_instance(),SIGNAL(disconnect_tcp_box(int)));


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

  void tcpDriver::sendCommand(QByteArray cmd, tcpDriverDataTypes::dataType type)
  {
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
    int entity=getBoxId();
    foreach(int val, EntityManager::get_instance()->get_all_DValues()){
        int box_id_e= EntityManager::get_instance()->get_box_id_component(val);
        if(box_id_e == entity)
          EntityManager::get_instance()->removeEntity(val);
      }

    foreach(int val, EntityManager::get_instance()->get_all_Values()){
        int box_id_e= EntityManager::get_instance()->get_box_id_component(val);
        if(box_id_e == entity)
          EntityManager::get_instance()->removeEntity(val);
      }
  }

  void tcpDriver::tcpSocket_connected()
  {
    EntityManager::get_instance()->slot_connection_state_component(getBoxId(), true);

    queue->pushFirst(getInfoCommand_val, tcpDriverDataTypes::UIBK_ECMD_ALLDATA);
    pollTimer->start();

    emit connected(getBoxId());
  }

  void tcpDriver::tcpSocket_disconnected()
  {
    qDebug() << "tcpSocket_disconnected()!";
    if(tcpSocket->state() == QAbstractSocket::ConnectedState)
      tcpSocket->abort();
    timeoutTimer->stop();
    pollTimer->stop();
    ongoingRequest = false;
    remove_all_values();
    EntityManager::get_instance()->slot_connection_state_component(getBoxId(), false);
    emit disconnected(getBoxId());
  }

  QTcpSocket *tcpDriver::getTcpSocket() const
  {
    return tcpSocket;
  }

  bool tcpDriver::boxIsConnected()
  {
    if(EntityManager::isValid(getBoxId()) == false)
      return false;

    return EntityManager::get_instance()->componentDataForEntity(CONNECTION_STATE_COMPONENT, getBoxId()).toBool();
  }

  abstract_backend *tcpDriver::getBack() const
  {
    return m_back;
  }

//  int tcpDriver::getBox_id() const
//  {
//    return box_id;
//  }


  void tcpDriver::recheckConnection()
  {

    if (tcpSocket->state() != QTcpSocket::ConnectedState && lastIP != "")
      {
        if (connectDevice(lastIP, lastPort, timeoutTimer->interval()))
          {
            emit connected(getBoxId());
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
        sendCommand(getMinimalSetActCommand_val,tcpDriverDataTypes::UIBK_ECMD_ACTSETDATA);
      }
  }

  void tcpDriver::parsePackage()
  {
    QMutexLocker locker(&mutex);

    QByteArray tmp = tcpSocket->readAll();
    if (transmissionContext == tcpDriverDataTypes::UIBK_ECMD_ALLDATA || transmissionContext == tcpDriverDataTypes::UIBK_ECMD_ACTSETDATA)
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
                emit receivedData(obj.toVariantMap(), transmissionContext, getBoxId());
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
    else if (transmissionContext == tcpDriverDataTypes::UIBK_ECMD_SETDATA)
      {
        ongoingRequest = false;
        timeoutTimer->stop();
        if (tmp.trimmed() != "OK")
          emit setDataError("Could not set Value: " + tmp, getBoxId());
        pollTimer->start();
        responseCounter++;
      }
    else if (transmissionContext == tcpDriverDataTypes::UIBK_ECMD_CUSTOMCOMMAND)
      {
        ongoingRequest = false;
        timeoutTimer->stop();
        QString rec= QString::fromLocal8Bit(tmp);
        emit receivedCustomData(rec, getBoxId());
        pollTimer->start();
        responseCounter++;
      }
    else if (transmissionContext == tcpDriverDataTypes::UIBK_ECMD_IDLE)
      {
        emit error("Unexpected Package received: " + tmp, getBoxId());
        qDebug()<<"Unexpected Package received: " + tmp, getBoxId();
      }
  }

  void tcpDriver::timeoutCheck()
  {
    if (ongoingRequest)
      {
        ongoingRequest = false;
        pollTimer->start();
        qDebug() << "Receive Timeout!";
        emit error("Receive Timeout", getBoxId());
      }
  }

  void tcpDriver::tcpError(QAbstractSocket::SocketError tcpErr)
  {
    Q_UNUSED(tcpErr)
    emit error(tcpSocket->errorString(), getBoxId());
    qDebug()<<tcpSocket->errorString();

    switch (tcpErr) {
      case QAbstractSocket::ConnectionRefusedError:           {qDebug() <<"Box "<< getBoxId()<<": "<< "ConnectionRefusedError"; break;}
      case QAbstractSocket::RemoteHostClosedError:            {
          qDebug() <<"Box "<< getBoxId()<<": "<< "RemoteHostClosedError";
          tcpSocket->abort();
          timeoutTimer->stop();
          pollTimer->stop();
          ongoingRequest = false;
          remove_all_values();
          EntityManager::get_instance()->slot_connection_state_component(getBoxId(), false);
          emit disconnected(getBoxId());

          break;}
      case QAbstractSocket::HostNotFoundError:                {
          qDebug() <<"Box "<< getBoxId()<<": "<< "HostNotFoundError";
          tcpSocket->abort();
          timeoutTimer->stop();
          pollTimer->stop();
          ongoingRequest = false;
          remove_all_values();
          EntityManager::get_instance()->slot_connection_state_component(getBoxId(), false);
          emit disconnected(getBoxId());

          break;}
      case QAbstractSocket::SocketAccessError:                {qDebug() <<"Box "<< getBoxId()<<": "<< "SocketAccessError"; break;}
      case QAbstractSocket::SocketResourceError:              {qDebug() <<"Box "<< getBoxId()<<": "<< "SocketResourceError"; break;}
      case QAbstractSocket::SocketTimeoutError:               {qDebug() <<"Box "<< getBoxId()<<": "<< "SocketTimeoutError"; break;}
      case QAbstractSocket::DatagramTooLargeError:            {qDebug() <<"Box "<< getBoxId()<<": "<< "DatagramTooLargeError"; break;}
      case QAbstractSocket::NetworkError:                     {
          qDebug() <<"Box "<< getBoxId()<<": "<< "NetworkError";
          tcpSocket->abort();
          timeoutTimer->stop();
          pollTimer->stop();
          ongoingRequest = false;
          remove_all_values();
          EntityManager::get_instance()->slot_connection_state_component(getBoxId(), false);
          emit disconnected(getBoxId());

          break;}
      case QAbstractSocket::AddressInUseError:                {qDebug() <<"Box "<< getBoxId()<<": "<< "AddressInUseError"; break;}
      case QAbstractSocket::SocketAddressNotAvailableError:   {qDebug() <<"Box "<< getBoxId()<<": "<< "SocketAddressNotAvailableError"; break;}
      case QAbstractSocket::UnsupportedSocketOperationError:  {qDebug() <<"Box "<< getBoxId()<<": "<< "UnsupportedSocketOperationError"; break;}
      case QAbstractSocket::ProxyAuthenticationRequiredError: {qDebug() <<"Box "<< getBoxId()<<": "<< "ProxyAuthenticationRequiredError"; break;}
      case QAbstractSocket::SslHandshakeFailedError:          {qDebug() <<"Box "<< getBoxId()<<": "<< "SslHandshakeFailedError"; break;}
      case QAbstractSocket::UnfinishedSocketOperationError:   {qDebug() <<"Box "<< getBoxId()<<": "<< "UnfinishedSocketOperationError"; break;}
      case QAbstractSocket::ProxyConnectionRefusedError:      {qDebug() <<"Box "<< getBoxId()<<": "<< "ProxyConnectionRefusedError"; break;}
      case QAbstractSocket::ProxyConnectionClosedError:       {qDebug() <<"Box "<< getBoxId()<<": "<< "ProxyConnectionClosedError"; break;}
      case QAbstractSocket::ProxyConnectionTimeoutError:      {qDebug() <<"Box "<< getBoxId()<<": "<< "ProxyConnectionTimeoutError"; break;}
      case QAbstractSocket::ProxyNotFoundError:               {qDebug() <<"Box "<< getBoxId()<<": "<< "ProxyNotFoundError"; break;}
      case QAbstractSocket::ProxyProtocolError:               {qDebug() <<"Box "<< getBoxId()<<": "<< "ProxyProtocolError"; break;}
      case QAbstractSocket::OperationError:                   {
          qDebug() <<"Box "<< getBoxId()<<": "<< "OperationError";
          //tcpSocket->abort();
          break;
        }
      case QAbstractSocket::SslInternalError:                 {qDebug() <<"Box "<< getBoxId()<<": "<< "SslInternalError"; break;}
      case QAbstractSocket::SslInvalidUserDataError:          {qDebug() <<"Box "<< getBoxId()<<": "<< "SslInvalidUserDataError"; break;}
      case QAbstractSocket::TemporaryError:                   {qDebug() <<"Box "<< getBoxId()<<": "<< "TemporaryError"; break;}
      default:                               {qDebug() <<"Box "<< getBoxId()<<": "<< "UnknownSocketError"; break;}
      }
  }
}

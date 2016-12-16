#include "TcpboxClient.hpp"

//#include <QTcpSocket>
#include "TcpboxSystemThread.hpp"
#include "TcpboxSystem.hpp"
#include "EntityManager/StaticComponentId.hpp"
#include "EventManager/EventManager.hpp"

#include <ManagerLocator.hpp>
#include <QDebug>
#include <QEventLoop>
#include <QTimer>

namespace talorion {

  TcpboxClient::TcpboxClient(TcpboxFactory::tcpbox_t tcpbox_id, const TcpboxSystem& sys,  QObject *par) :
    QObject(par),
    m_tcpbox(tcpbox_id),
    //m_connection(new ecmd_connection(this)),
    m_sys(sys)
  {
    EventManager& evt_mng      = ManagerLocator::eventManager();
    EventManager* evt_mng_ptr  = &evt_mng;

    connect(this,SIGNAL(connectBox(EntityManager::EntityID)), evt_mng_ptr, SIGNAL(connectBox(EntityManager::EntityID)));
    connect(this,SIGNAL(disconnectBox(EntityManager::EntityID)), evt_mng_ptr, SIGNAL(disconnectBox(EntityManager::EntityID)));
    connect(this, SIGNAL(sendCommandToBox(EntityManager::EntityID,QString)), evt_mng_ptr, SIGNAL(sendCommandToBox(EntityManager::EntityID,QString)));

    connect(evt_mng_ptr, SIGNAL(boxConnected(EntityManager::EntityID)),this, SLOT(slotBoxConnected(EntityManager::EntityID)));
    connect(evt_mng_ptr, SIGNAL(boxDisconnected(EntityManager::EntityID)),this, SLOT(slotBoxDisconnected(EntityManager::EntityID)));

    connect(evt_mng_ptr, SIGNAL(boxCommandStarted(EntityManager::EntityID)),this, SLOT(slotBoxCommandStarted(EntityManager::EntityID)));
    connect(evt_mng_ptr, SIGNAL(boxCommandFinished(EntityManager::EntityID)),this, SLOT(slotBoxCommandFinished(EntityManager::EntityID)));
    connect(evt_mng_ptr, SIGNAL(boxCommandError(EntityManager::EntityID)),this, SLOT(slotBoxCommandError(EntityManager::EntityID)));

  }

  void TcpboxClient::setBoxName(const QString &bx_name)
  {
    entityMng().setComponentDataForEntity(TcpboxFactory::nameComponentId(), tcpbox(), bx_name);
  }

  void TcpboxClient::setHostName(const QString &hst_name)
  {
    entityMng().setComponentDataForEntity(TcpboxFactory::hostNameComponentId(), tcpbox(), hst_name);
  }

  void TcpboxClient::setPort(const quint16 prt)
  {
    entityMng().setComponentDataForEntity(TcpboxFactory::portComponentId(), tcpbox(), prt);
  }

  void TcpboxClient::setBoxId(const quint32 bx_id)
  {
    entityMng().setComponentDataForEntity(TcpboxFactory::boxIdComponentId(), tcpbox(), bx_id);
  }

  void TcpboxClient::setTimeout(const int to)
  {
    entityMng().setComponentDataForEntity(TcpboxFactory::timeoutComponentId(), tcpbox(), to);
  }

  void TcpboxClient::setState(QAbstractSocket::SocketState st)
  {
    entityMng().setComponentDataForEntity(TcpboxFactory::connectionStateComponentId(), tcpbox(), st);
  }

  void TcpboxClient::setCommandState(ecmd_command_state_t st)
  {
    entityMng().setComponentDataForEntity(TcpboxFactory::commandStateComponentId(), tcpbox(), st);
  }

  void TcpboxClient::slotBoxConnected(EntityManager::EntityID box)
  {
    auto act_state = state();
    if(box == tcpbox() && act_state == QAbstractSocket::ConnectedState)
      emit boxConnected();

  }

  void TcpboxClient::slotBoxDisconnected(EntityManager::EntityID box)
  {
    if(box == tcpbox())
      emit boxDisconnected();
  }

  void TcpboxClient::slotBoxCommandStarted(EntityManager::EntityID box)
  {
    if(box == tcpbox())
      emit boxCommandStarted();
  }

  void TcpboxClient::slotBoxCommandFinished(EntityManager::EntityID box)
  {
    if(box == tcpbox())
      emit boxCommandFinished();
  }

  void TcpboxClient::slotBoxCommandError(EntityManager::EntityID box)
  {
    if(box == tcpbox())
      emit boxCommandError();
  }

  TcpboxFactory::tcpbox_t TcpboxClient::tcpbox() const
  {
    return m_tcpbox;
  }

  QString TcpboxClient::boxName() const
  {
    return entityMng().componentDataForEntity(TcpboxFactory::nameComponentId(), tcpbox()).toString();
  }

  QString TcpboxClient::hostName() const
  {
    return entityMng().componentDataForEntity(TcpboxFactory::hostNameComponentId(), tcpbox()).toString();
  }

  quint16 TcpboxClient::port() const
  {
    int ret=0;
    bool ok;
    int tmp = entityMng().componentDataForEntity(TcpboxFactory::portComponentId(), tcpbox()).toInt(&ok);
    if(ok)
      ret=tmp;
    return ret;
  }

  qint32 TcpboxClient::boxId() const
  {
    int ret=0;
    bool ok;
    int tmp = entityMng().componentDataForEntity(TcpboxFactory::boxIdComponentId(), tcpbox()).toInt(&ok);
    if(ok)
      ret=tmp;
    return ret;
  }

  int TcpboxClient::timeout() const
  {
    int ret=0;
    bool ok;
    int tmp = entityMng().componentDataForEntity(TcpboxFactory::timeoutComponentId(), tcpbox()).toInt(&ok);
    if(ok)
      ret=tmp;
    return ret;
  }

  QUuid TcpboxClient::serialVersionUid() const
  {
    return entityMng().componentDataForEntity(TcpboxFactory::serialVersionUidComponentId(), tcpbox()).toUuid();
  }

  ecmd_command_state_t TcpboxClient::commandState() const
  {
    auto tmp = entityMng().componentDataForEntity(TcpboxFactory::commandStateComponentId(), tcpbox());
    auto tmp_val=COMMAND_STATE_UNKNOWN;
    bool ok=false;
    auto tmp_val2 = tmp.toInt(&ok);
    if(ok)
      tmp_val = static_cast<ecmd_command_state_t>(tmp_val2);
    return tmp_val;

  }

  bool TcpboxClient::isDeleted() const
  {
    return (entityMng().entityExists(tcpbox()) == false);
  }

  bool TcpboxClient::isConfigured() const
  {
    bool ret = !hostName().isEmpty() && !hostName().isNull();
    ret = ret && (port() > 0);
    ret = ret && (boxId() > 0);
    ret = ret && (timeout() > 0);
    ret = ret && (serialVersionUid() == TcpboxFactory::getInstance().getTcpboxUid());

    return ret;
  }

  void TcpboxClient::openConnection()
  {
    emit connectBox(tcpbox());
  }

  void TcpboxClient::closeConnection()
  {
    emit disconnectBox(tcpbox());
  }

  void TcpboxClient::sendCommand(const QString &cmd)
  {
    emit sendCommandToBox(tcpbox(),cmd);
  }

  bool TcpboxClient::isCommandSupported(const QString &cmd)const
  {
    //return m_connection->is_command_supported(cmd);
    Q_UNUSED(cmd);
    return false;
  }

  bool TcpboxClient::waitForConnect()
  {
    QEventLoop tmp_evt_loop;

    QTimer::singleShot(timeout(), &tmp_evt_loop, SLOT(quit()));
    connect(this,SIGNAL(boxConnected()),&tmp_evt_loop,SLOT(quit()));
    tmp_evt_loop.exec();

#if defined( Q_OS_WIN )
    //Note: On some operating systems the connected() signal may be directly emitted from the connectToHost() call for connections to the localhost.
    QThread::currentThread()->msleep(100);
#endif

    auto act_state = state();
    return ( act_state == QAbstractSocket::ConnectedState);
  }

  bool TcpboxClient::waitForCommandStarted()
  {
    QEventLoop tmp_evt_loop;

    QTimer::singleShot(timeout(), &tmp_evt_loop, SLOT(quit()));
    connect(this,SIGNAL(boxCommandStarted()), &tmp_evt_loop, SLOT(quit()));
    //connect(this,SIGNAL(box_command_finished()), &tmp_evt_loop, SLOT(quit()));
    connect(this,SIGNAL(boxCommandError()), &tmp_evt_loop, SLOT(quit()));
    tmp_evt_loop.exec();

    auto act_cmd_state = commandState();
    return (act_cmd_state == COMMAND_STATE_STARTED);
  }

  bool TcpboxClient::waitForCommandFinished()
  {
    auto act_cmd_state = commandState();
    if(act_cmd_state != COMMAND_STATE_STARTED)
      return false;

    QEventLoop tmp_evt_loop;

    QTimer::singleShot(timeout(), &tmp_evt_loop, SLOT(quit()));
    //connect(this,SIGNAL(box_command_started()), &tmp_evt_loop, SLOT(quit()));
    connect(this,SIGNAL(boxCommandFinished()), &tmp_evt_loop, SLOT(quit()));
    connect(this,SIGNAL(boxCommandError()), &tmp_evt_loop, SLOT(quit()));
    tmp_evt_loop.exec();

    act_cmd_state = commandState();
    return (act_cmd_state == COMMAND_STATE_OK);
  }


  QAbstractSocket::SocketState TcpboxClient::state() const
  {
    auto tmp = entityMng().componentDataForEntity(TcpboxFactory::connectionStateComponentId(), tcpbox());
    auto tmp_val=QAbstractSocket::UnconnectedState;
    bool ok=false;
    auto tmp_val2 = tmp.toInt(&ok);
    if(ok)
      tmp_val = static_cast<QAbstractSocket::SocketState>(tmp_val2);
    return tmp_val;
  }

  EntityManager &TcpboxClient::entityMng() const
  {
    //return m_sys.entityManager();
    return ManagerLocator::entityManager();
  }

}

#ifndef TCPBOX_CLIENT_HPP
#define TCPBOX_CLIENT_HPP

#include <tcpboxsystem_global.hpp>

//find a way to forward declare entity_manager::entity_id_t
#include "EntityManager/EntityManager.hpp"
#include "TcpboxFactory.hpp"
#include "EcmdConnection/EcmdConnection.hpp"

#include <QObject>

namespace talorion {


  class TcpboxSystemThread;
  class TcpboxSystem;

  class TCPBOXSYSTEMSHARED_EXPORT TcpboxClient : public QObject
  {

    Q_OBJECT
  private:
    friend class TcpboxSystemThread;
    friend class TcpboxFactory;

  public:
    explicit TcpboxClient(TcpboxFactory::tcpbox_t tcpbox_id, const TcpboxSystem& sys, QObject *par = Q_NULLPTR);

  signals:
    void connectBox(EntityManager::EntityID tcpbox_id);
    void disconnectBox(EntityManager::EntityID tcpbox_id);

    void sendCommandToBox(EntityManager::EntityID tcpbox_id, const QString& cmd);

    void boxConnected();
    void boxDisconnected();

    void boxCommandStarted();
    void boxCommandFinished();
    void boxCommandError();

  public slots:
    void setBoxName(const QString& bx_name);
    void setHostName(const QString& hst_name);
    void setPort(const quint16 prt);
    void setBoxId(const quint32 bx_id);
    void setTimeout(const int to);
    void setState(QAbstractSocket::SocketState st);
    void setCommandState(ecmd_command_state_t st);

  private slots:
    void slotBoxConnected(EntityManager::EntityID box);
    void slotBoxDisconnected(EntityManager::EntityID box);

    void slotBoxCommandStarted(EntityManager::EntityID box);
    void slotBoxCommandFinished(EntityManager::EntityID box);
    void slotBoxCommandError(EntityManager::EntityID box);

  public:
    TcpboxFactory::tcpbox_t tcpbox() const;
    QString boxName()const;
    QString hostName()const;
    quint16 port()const;
    qint32 boxId()const;
    int timeout()const;
    QUuid serialVersionUid()const;
    ecmd_command_state_t commandState()const;

    bool isDeleted()const;
    bool isConfigured()const;

    void openConnection();
    void closeConnection();

    void sendCommand(const QString& cmd);
    bool isCommandSupported(const QString& cmd) const;

    bool waitForConnect();

    bool waitForCommandStarted();

    bool waitForCommandFinished();

    QAbstractSocket::SocketState state()const;

  private:
    EntityManager& entityMng()const;

  private:
    const TcpboxFactory::tcpbox_t m_tcpbox;
    //QScopedPointer<ecmd_connection> m_connection;
    const TcpboxSystem& m_sys;
  };
}

#endif // TCPBOX_CLIENT_HPP

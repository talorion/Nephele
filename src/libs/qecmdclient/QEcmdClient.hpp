#ifndef TALORION_QECMDCLIENT_HPP
#define TALORION_QECMDCLIENT_HPP

#include <QObject>
#include <QQueue>
#include <QTcpSocket>
#include <QTimer>
#include "QEcmdCommand/AbstractQEcmdCommand.hpp"
#include "qecmdclient_global.hpp"

namespace talorion {

using QEcmdCommandQueue = QQueue<AbstractQEcmdCommand>;

class QECMDCLIENTSHARED_EXPORT QEcmdClient :public QObject
{
    Q_OBJECT
public:
    enum QEcmdClientState{
        IDLE,
        CONNECTED
    };
    Q_ENUM(QEcmdClientState)

public:
    QEcmdClient(QObject *par = 0);
    virtual ~QEcmdClient();

private:
    Q_DISABLE_COPY(QEcmdClient)

signals:
    void receivedData();
    void commandSent(QString cmd);

public:
    void connectEcmdDevice(const QString &hostName, quint16 port, int timeout_msecs = 1000);
    void disconnectEcmdDevice(int timeout_msecs = 1000);
    //void reconnecerEcmd();
    bool isConnected() const;

    //void initializeConnectedDevice();
    //void disposeConnectedDevice();
    //void reinitialize();
    bool isInitialized() const;

    //void startPolling();
    //void stopPolling();
    //void restartPolling();
    bool isPolling() const;

    //long lastEcmdError();

    QEcmdClientState state() const;

    bool sendCommand(QString cmd, int timeout=200);
    bool sendCommand(const AbstractQEcmdCommand& cmd);

    AbstractQEcmdCommand lastSentCommand() const;

    bool ongoingRequest() const;

    bool commandPending() const;

private:
    void setLastSentCommand(const AbstractQEcmdCommand &lastSentCommand);
    void commandSent(int timeout);
    void commandReceivedErrorOrTimedOut();
    bool doSendCommand(const AbstractQEcmdCommand& cmd);
    bool enqueueCommand(const AbstractQEcmdCommand& cmd);

private slots:
    void parsePackage();
    void socketError(QAbstractSocket::SocketError socketError);
    void commandTimedOut();
    void dequeueCommand();

private:
    QTcpSocket* m_socket;
    AbstractQEcmdCommand m_lastSentCommand;

    bool    m_ongoingRequest;
    QTimer* m_commandTimer;

    QEcmdCommandQueue* m_cmdQueue;
    QTimer* m_cmdQueueTimer;

};

} // namespace talorion
#endif // TALORION_QECMDCLIENT_HPP

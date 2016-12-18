#ifndef TALORION_QECMDCLIENT_HPP
#define TALORION_QECMDCLIENT_HPP

#include <QObject>
#include <QTcpSocket>
#include "qecmdclient_global.hpp"

namespace talorion {

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

private:
    QTcpSocket* m_socket;

};

} // namespace talorion
#endif // TALORION_QECMDCLIENT_HPP

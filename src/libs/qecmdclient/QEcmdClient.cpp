#include "QEcmdClient.hpp"

#include <QEventLoop>
#include <QTimer>


namespace talorion {

QEcmdClient::QEcmdClient(QObject *par):
    QObject(par),
    m_socket(Q_NULLPTR)
{
    m_socket = new QTcpSocket();
}

QEcmdClient::~QEcmdClient()
{
    Q_ASSERT (m_socket != Q_NULLPTR);
    disconnectEcmdDevice();

    if(isConnected())
        m_socket->abort();

    delete m_socket;
    m_socket = Q_NULLPTR;
}

void QEcmdClient::connectEcmdDevice(const QString &hostName, quint16 port, int timeout_msecs)
{
    if(isConnected())
        return;

    Q_ASSERT (m_socket != Q_NULLPTR);

    m_socket->connectToHost(hostName, port);
    //Note: waitForConnected may fail randomly on Windows. Consider using the event loop and the connected() signal if your software will run on Windows.
    QEventLoop tmp_evt_loop;
    QTimer::singleShot(timeout_msecs, &tmp_evt_loop, SLOT(quit()));
    connect(m_socket, SIGNAL(connected()), &tmp_evt_loop,SLOT(quit()));
    tmp_evt_loop.exec();

}

void QEcmdClient::disconnectEcmdDevice(int timeout_msecs)
{
    if(!isConnected())
        return;

    Q_ASSERT (m_socket != Q_NULLPTR);

    m_socket->disconnectFromHost();
    //Note: waitForDisonnected may fail randomly on Windows. Consider using the event loop and the connected() signal if your software will run on Windows.
    QEventLoop tmp_evt_loop;
    QTimer::singleShot(timeout_msecs, &tmp_evt_loop, SLOT(quit()));
    connect(m_socket, SIGNAL(disconnected()), &tmp_evt_loop,SLOT(quit()));
    tmp_evt_loop.exec();


}

bool QEcmdClient::isConnected() const
{
    Q_ASSERT (m_socket != Q_NULLPTR);
    return m_socket->state() == QTcpSocket::ConnectedState;
}

bool QEcmdClient::isInitialized() const
{
    return false;
}

bool QEcmdClient::isPolling() const
{
    return false;
}

QEcmdClient::QEcmdClientState QEcmdClient::state() const
{
    //  if(!isConnected())
    //      return IDLE;

    //  if(!isPolling())
    //      return CONNECTED;

    //  return POLLING;
    return IDLE;
}

} // namespace talorion

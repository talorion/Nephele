#include "QEcmdClient.hpp"

#include <QEventLoop>
#include <QTimer>

#define POLLINTERVAL_MS 10

namespace talorion {

QEcmdClient::QEcmdClient(QObject *par):
    QObject(par),
    m_socket(Q_NULLPTR),
    m_lastSentCommand(),
    m_ongoingRequest(false),
    m_commandTimer(Q_NULLPTR),
    m_cmdQueue(Q_NULLPTR),
    m_cmdQueueTimer(Q_NULLPTR)
{
    m_socket = new QTcpSocket();
    connect(m_socket, SIGNAL(readyRead()), this, SLOT(parsePackage()));
    connect(m_socket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(socketError(QAbstractSocket::SocketError)));

    m_commandTimer = new QTimer();
    m_commandTimer->setSingleShot(true);
    connect(m_commandTimer, SIGNAL(timeout()), this, SLOT(commandTimedOut()));

    m_cmdQueue = new QEcmdCommandQueue();

    m_cmdQueueTimer= new QTimer();
    m_cmdQueueTimer->setSingleShot(true);
    connect(m_cmdQueueTimer, SIGNAL(timeout()), this, SLOT(dequeueCommand()));

}

QEcmdClient::~QEcmdClient()
{
    Q_ASSERT (m_socket != Q_NULLPTR);
    disconnectEcmdDevice();

    if(isConnected())
        m_socket->abort();

    disconnect(m_socket, SIGNAL(readyRead()), this, SLOT(parsePackage()));
    delete m_socket;
    m_socket = Q_NULLPTR;

    commandReceivedErrorOrTimedOut();
    disconnect(m_commandTimer, SIGNAL(timeout()), this, SLOT(commandTimedOut()));
    delete m_commandTimer;

    delete m_cmdQueue;
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

bool QEcmdClient::sendCommand(QString cmd, int timeout)
{
    return sendCommand(AbstractQEcmdCommand(cmd, timeout));
}

bool QEcmdClient::sendCommand(const AbstractQEcmdCommand &cmd)
{
    if(commandPending())
        return enqueueCommand(cmd);
    else
        return doSendCommand(cmd);
}

AbstractQEcmdCommand QEcmdClient::lastSentCommand() const
{
    return m_lastSentCommand;
}

void QEcmdClient::parsePackage()
{
    if(!ongoingRequest())
        return;

    commandReceivedErrorOrTimedOut();

//    bool noMoreData = false;
//    do{
//        QByteArray tmp = tcpSocket->readAll();
//        noMoreData = tmp.isEmpty();
//        QString str(tmp);
//    }while(parser.hasEnoughData() || noMoreData);

    emit receivedData();
}

void QEcmdClient::socketError(QAbstractSocket::SocketError socketError)
{
    Q_UNUSED(socketError);
    commandReceivedErrorOrTimedOut();
    qDebug()<<Q_FUNC_INFO;
}

void QEcmdClient::commandTimedOut()
{
    commandReceivedErrorOrTimedOut();
}

void QEcmdClient::dequeueCommand()
{
    if (!m_cmdQueue->isEmpty()){
        auto cmdEle = m_cmdQueue->dequeue();
        doSendCommand(cmdEle);
    }//else if (isPolling())

}

void QEcmdClient::setLastSentCommand(const AbstractQEcmdCommand &lastSentCommand)
{
    if(m_lastSentCommand != lastSentCommand){
        m_lastSentCommand = lastSentCommand;
        emit commandSent(m_lastSentCommand.cmdStr());
    }
}

bool QEcmdClient::ongoingRequest() const
{
    return m_ongoingRequest;
}

bool QEcmdClient::commandPending() const
{
    return ongoingRequest() || (!m_cmdQueue->isEmpty());
}

void QEcmdClient::commandSent(int timeout)
{
    m_ongoingRequest = true;
    m_commandTimer->setInterval(timeout);
    m_commandTimer->start();
}

void QEcmdClient::commandReceivedErrorOrTimedOut()
{
    m_ongoingRequest = false;
    m_commandTimer->stop();
    if(commandPending()){
        m_cmdQueueTimer->setInterval(POLLINTERVAL_MS);
        m_cmdQueueTimer->start();
    }
}

bool QEcmdClient::doSendCommand(const AbstractQEcmdCommand &cmd)
{
    if(!isConnected())
        return false;

    if(ongoingRequest())
        return false;

    auto bytes_sent = m_socket->write(cmd.toByteArray());
    auto ret        = (bytes_sent == cmd.cmdLen());
    if(ret){
        setLastSentCommand(cmd);
        commandSent(cmd.timeout());
    }

    return ret;
}

bool QEcmdClient::enqueueCommand(const AbstractQEcmdCommand &cmd)
{
    m_cmdQueue->enqueue(cmd);
    return true;
}

} // namespace talorion

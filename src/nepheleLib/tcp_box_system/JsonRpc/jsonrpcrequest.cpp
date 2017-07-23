#include "jsonrpcrequest.hpp"


#include <QTime>

#include <tcp_box_system/jsonrpcdriver.hpp>


namespace talorion {

JsonRpcRequest::~JsonRpcRequest()
{
    wasReceived();
    m_msg.clear();
    m_id.clear();
    m_methodId = 0;
    m_requestId = 0;
    //m_creationTime;
    //m_sendTime
    m_methodName.clear();
    //m_internalSource = false;
    m_timeout = 0;
    m_timoutTimer.stop();
    m_sender = Q_NULLPTR;
}

JsonRpcRequest::JsonRpcRequest(const QByteArray &msg, QObject *par):
    QObject(par),
    m_msg(msg),
    m_id(),
    m_methodId(),
    m_requestId(),
    m_creationTime(),
    m_sendTime(),
    m_methodName(),
    m_internalSource(true),
    m_timeout(500),
    m_timoutTimer(),
    m_sender(Q_NULLPTR)
{
    m_creationTime = QTime::currentTime();
    m_sendTime = m_creationTime;

    connect(&m_timoutTimer, SIGNAL(timeout()), this, SLOT(timerTimedOut()));

}

void JsonRpcRequest::wasSent(JsonRpcDriver *sender, double additional){
    m_sendTime = QTime::currentTime();
    m_sendTime.start();

    m_timeout = timeout() + (additional * 1.1);

    m_timoutTimer.stop();
    m_timoutTimer.setSingleShot(true);
    m_timoutTimer.setInterval(m_timeout);
    if(sender != Q_NULLPTR){
        m_sender = sender;
        connect(this, SIGNAL(requestTimedOut(QString)), m_sender, SLOT(requestTimedOut(QString)));
    }
    m_timoutTimer.start();

}

void JsonRpcRequest::wasReceived()
{
    m_timoutTimer.stop();
    if(m_sender != Q_NULLPTR){
        disconnect(this, SIGNAL(requestTimedOut(QString)), m_sender, SLOT(requestTimedOut(QString)));
        m_sender = Q_NULLPTR;
    }
    m_timoutTimer.stop();
}

bool JsonRpcRequest::timedOut(int ms) const
{
    Q_UNUSED(ms);
    return m_timoutTimer.isActive() == false;
}

int JsonRpcRequest::timeout() const
{
    return m_timeout;
}

bool JsonRpcRequest::internalSource() const
{
    return m_internalSource;
}

void JsonRpcRequest::timerTimedOut()
{
    emit requestTimedOut(id());
}

bool operator ==(const JsonRpcRequest &a, const JsonRpcRequest &b)
{
    return a.methodName() == b.methodName();
}





} // namespace talorion

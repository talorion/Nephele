#include "PVPRequest.hpp"

#include <tcp_box_system/abstracttcpdriver.hpp>


namespace talorion {

PVPRequest::PVPRequest():
    PVPTelegramm(),
    m_timeout(5000),
    m_timoutTimer(),
    m_internalSource(true)
{

    m_sendTime = QTime::currentTime();
    connect(&m_timoutTimer, SIGNAL(timeout()), this, SLOT(timerTimedOut()));

}

void PVPRequest::wasSent(AbstractTcpDriver *sender, double additional)
{
    m_sendTime = QTime::currentTime();
    m_sendTime.start();

    m_timeout = m_timeout + (additional * 1.1);

    m_timoutTimer.stop();
    m_timoutTimer.setSingleShot(true);
    m_timoutTimer.setInterval(m_timeout);
    if(sender != Q_NULLPTR){
        m_sender = sender;
        connect(this, SIGNAL(requestTimedOut(QString)), m_sender, SLOT(requestTimedOut(QString)));
    }
    m_timoutTimer.start();
}

void PVPRequest::wasReceived()
{
    m_timoutTimer.stop();
    if(m_sender != Q_NULLPTR){
        disconnect(this, SIGNAL(requestTimedOut(QString)), m_sender, SLOT(requestTimedOut(QString)));
        m_sender = Q_NULLPTR;
    }
    m_timoutTimer.stop();
}

bool PVPRequest::timedOut(int ms) const
{
    Q_UNUSED(ms);
    return m_timoutTimer.isActive() == false;
}

quint32 PVPRequest::requestId() const
{
    return m_requestId;
}

void PVPRequest::timerTimedOut()
{
    emit requestTimedOut(id());
}

} // namespace talorion

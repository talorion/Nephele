#ifndef PVPREQUEST_HPP
#define PVPREQUEST_HPP

#include "PVPFactory.hpp"
#include "PVPTelegramm.hpp"

namespace talorion {
class AbstractTcpDriver;

class PVPRequest:public PVPTelegramm
{
    Q_OBJECT
    friend class PVPFactory;
private:
    PVPRequest();

signals:
    void requestTimedOut(QString sender);

public:
    //QString id() const  {return m_id;}
    void wasSent(AbstractTcpDriver* sender, double additional = 0);
    void wasReceived();
    bool internalSource() const{return m_internalSource;}
    bool timedOut(int ms = -1)const;
    QTime sendTime() const{ return m_sendTime;}

    quint32 requestId() const;

private slots:
    void timerTimedOut();

private:
    bool m_internalSource;
    QTimer m_timoutTimer;
    QTime m_sendTime;
    AbstractTcpDriver * m_sender;
    int m_timeout;
    quint32 m_requestId;
};

} // namespace talorion

#endif // PVPREQUEST_HPP

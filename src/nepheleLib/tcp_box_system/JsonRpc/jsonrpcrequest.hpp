#ifndef JSONRPCREQUEST_HPP
#define JSONRPCREQUEST_HPP

#include <QObject>
#include <QByteArray>
#include <QTime>
#include <QTimer>

#include "jsonrpcfactory.hpp"

namespace talorion {

//using JsonRpcRequest = QByteArray;
  class AbstractTcpDriver;

class JsonRpcRequest: public QObject
{
    friend class JsonRpcFactory;

    Q_OBJECT
public:
    virtual ~JsonRpcRequest();

private:
    JsonRpcRequest(const QByteArray& msg, QObject *par = nullptr);
    JsonRpcRequest( const JsonRpcRequest& ) = default;
    JsonRpcRequest& operator=( const JsonRpcRequest& ) = delete;

signals:
    void requestTimedOut(QString sender);

public:
    QByteArray toJson() const{return m_msg;}
    QString id() const  {return m_id;}
    qint16 methodId() const  {return (requestId() & 0x0000FFFF) ;}
    qint32 requestId() const  {return m_requestId;}
    QByteArray replace(const char *before, const char *after){return m_msg.replace(before, after);}
    QTime creationTime() const{ return m_creationTime;}
    QTime sendTime() const{ return m_sendTime;}

    void wasSent(AbstractTcpDriver* sender, double additional = 0);
    void wasReceived();
    //void wasReSent(){m_sendTime = QTime::currentTime();m_sendTime.start();}
    QString methodName() const  {return m_methodName; }

    bool timedOut(int ms = -1)const;

    int timeout() const;

    bool internalSource() const;

private slots:
    void timerTimedOut();

private:
    QByteArray m_msg;
    QString m_id;
    qint16 m_methodId;
    qint32 m_requestId;
    QTime m_creationTime;
    QTime m_sendTime;
    QString m_methodName;
    bool m_internalSource;
    int m_timeout;
    QTimer m_timoutTimer;
    AbstractTcpDriver * m_sender;
};

bool operator ==(const JsonRpcRequest &a, const JsonRpcRequest &b);

} // namespace talorion

#endif // JSONRPCREQUEST_HPP

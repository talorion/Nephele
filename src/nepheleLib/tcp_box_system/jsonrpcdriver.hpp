#ifndef JSONRPCDRIVER_HPP
#define JSONRPCDRIVER_HPP

#include "abstracttcpdriver.hpp"

#include <QObject>
#include <QTcpSocket>
#include <QQueue>

#include "JsonRpc/jsonrpcmethoddescriptor.hpp"

#include <tcp_box_system/JsonRpc/jsonrpcrequest.hpp>

#include <tcp_box_system/JsonRpcDriver/JsonRpcBackend.hpp>


namespace talorion {

  class JsonRpcDriver : public AbstractTcpDriver
  {
    Q_OBJECT
  private:
    using CommandQueueElem_t = QSharedPointer<JsonRpcRequest>;
    using CommandQueue_t = QQueue<CommandQueueElem_t >;
    using MethodsList_t = QStringList;

  public:
    explicit JsonRpcDriver(EntityManager::EntityID box, QObject *par = Q_NULLPTR);
    virtual ~JsonRpcDriver();

  private:
      Q_DISABLE_COPY(JsonRpcDriver)

  public:
    QTcpSocket* tcpSocket() const{return m_tcpSocket;}

    bool isDeviceConnected() const;

    CommandQueue_t cmdQ() const;

    MethodsList_t methodNames()const;

  signals:
    void pollNextQueueElement();

  public slots:
    virtual bool connectDevice() Q_DECL_OVERRIDE;
    virtual void disconectDevice() Q_DECL_OVERRIDE;

    void sendMethod(EntityManager::EntityID box, const QString& methodname,const QVariant& params, qint32 id);
    void sendMethod(const QString& methodname, const QVariant& params, qint32 id, bool internalSoure = true);
    void sendMethod(QSharedPointer<JsonRpcRequest> request);
    void sendDefaultRequest();

  private slots:
    void afterDeviceConnect();
    void afterDeviceDisconnect();
    void socketReadyRead();
    void pollQueue();
    void tcpError(QAbstractSocket::SocketError tcpErr);
    void requestTimedOut(QString req_idS);

  private:
    bool waitForDisconnected(int msecs = 30000);
    void sendJrpcRequest(QSharedPointer<JsonRpcRequest> request);
    void parseResponse(JsonRpcResponse *resp);

    void errorReport() const;

  private:
    QTimer m_pollTimer;
    QTimer m_reconnectTimer;
    QTcpSocket* m_tcpSocket;
    CommandQueue_t m_cmdQ;
    QMap<QString, QSharedPointer<JsonRpcRequest> > m_pendingRequests;
    JsonRpcBackend m_back;
    int m_requestCounter;
    int m_responseCounter;
    int m_timoutErrorCounter;
    int m_socketErrorCounter;
    int m_parseErrorCounter;
    int m_unmatchedResponseCounter;
    int m_consecutiveErrorCounter;
    int m_consecutiveParseErrorCounter;

    double m_avgRequestTime;
    QByteArray m_buffer;
  };
} // namespace talorion

#endif // JSONRPCDRIVER_HPP

#ifndef SERIALPCUDRIVER_HPP
#define SERIALPCUDRIVER_HPP

#include <tcp_box_system/abstracttcpdriver.hpp>
#include <QQueue>
#include <QSerialPort>

#include <SerialBoxSystem/PfeifferVacuumProtocol/PVPBackend.hpp>
#include <SerialBoxSystem/PfeifferVacuumProtocol/PVPRequest.hpp>
#include <SerialBoxSystem/PfeifferVacuumProtocol/PVPResponse.hpp>

namespace talorion {

class SerialPCUDriver : public AbstractTcpDriver
{
private:
    using CommandQueueElem_t = QSharedPointer<PVPRequest>;
    using CommandQueue_t = QQueue<CommandQueueElem_t >;
    using MethodsList_t = QStringList;
    Q_OBJECT
public:
    explicit SerialPCUDriver(EntityManager::EntityID box, QObject *par = nullptr);
    virtual ~SerialPCUDriver();
    Q_DISABLE_COPY(SerialPCUDriver)

public:
    QSerialPort* serialSocket() const{return m_serialSocket;}
    bool isDeviceConnected() const;
    CommandQueue_t cmdQ() const;
    //MethodsList_t methodNames()const;

signals:
    void pollNextQueueElement();

public slots:
    virtual bool connectDevice() Q_DECL_OVERRIDE;
    virtual void disconectDevice() Q_DECL_OVERRIDE;

    //void sendMethod(EntityManager::EntityID box, const QString& methodname,const QVariant& params, qint32 id);
    //void sendMethod(const QString& methodname, const QVariant& params, qint32 id, bool internalSoure = true);
    void sendMethod(QSharedPointer<PVPRequest> request);
    void sendDefaultRequest();

private slots:
    void afterDeviceConnect();
    void afterDeviceDisconnect();
    void socketReadyRead();
    void pollQueue();
    //void tcpError(QAbstractSocket::SocketError tcpErr);
    virtual void requestTimedOut(QString req_id)Q_DECL_OVERRIDE;

private:
    bool waitForDisconnected(int msecs = 30000);
    void sendPVPRequest(CommandQueueElem_t request);
    void parseResponse(PVPResponse *resp);

    void errorReport() const;

private:
    QTimer m_pollTimer;
    QTimer m_reconnectTimer;
    QSerialPort* m_serialSocket;
    CommandQueue_t m_cmdQ;
    QMap<QString, CommandQueueElem_t > m_pendingRequests;
    PVPBackend m_back;
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

#endif // SERIALPCUDRIVER_HPP

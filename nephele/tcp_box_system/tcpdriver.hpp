#ifndef TCPDRIVER_H
#define TCPDRIVER_H

#include <QTcpSocket>
#include <QJsonDocument>
#include <QJsonObject>
#include <QTime>
#include <QTimer>
#include <QVariant>
#include <QThread>
#include <QCoreApplication>

#include "tcpcommandqueue.hpp"


namespace talorion {

    class tcpDriver : public QObject
    {
        Q_OBJECT

    public:
        tcpDriver(QByteArray getInfoCommand, QByteArray getMinimalSetActCommand, QObject* par=0);
         ~tcpDriver(void);
        Q_DISABLE_COPY(tcpDriver)

        enum requestStatus {OK,BUSY,DISCONNECTED,CONNECTION_ERROR};
        bool connectDevice(QString ip, qint32 port = 2701, int timeoutMs = 4000);
        void disconect();
    public slots:
        void setDataCommand(QByteArray cmd);
        void customCommand(QByteArray cmd);

    signals:
        void connected();
        void receivedData(QVariantMap data, tcpDriverDataTypes::dataType type);
        void receivedCustomData(QByteArray data);
        void setDataError(QString errorString);
        void error(QString errorString);

    private:
        void recheckConnection();
        bool waitTransmissionFinish();
        void sendCommand(QByteArray cmd, tcpDriverDataTypes::dataType type);

    private:
        QTcpSocket* tcpSocket;
        tcpDriverDataTypes::dataType transmissionContext;
        QByteArray recBuf;
        QByteArray sendBuf;
        QByteArray getInfoCommand_val;
        QByteArray getMinimalSetActCommand_val;
        int curlyOpen, curlyClose;
        QTimer* timeoutTimer;
        QTimer* pollTimer;
        QString lastIP;
        qint32  lastPort;
        bool ongoingRequest;
        int requestCounter;
        int responseCounter;
        tcpCommandQueue* queue;


    private slots:
        void poll();
        void parsePackage();
        void timeoutCheck();
        void tcpError(QAbstractSocket::SocketError tcpErr);
    };
}


#endif // TCPDRIVER_H

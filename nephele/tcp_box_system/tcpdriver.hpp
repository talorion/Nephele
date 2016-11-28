#ifndef TCPDRIVER_H
#define TCPDRIVER_H

#include <QtSerialPort/QSerialPort>

#include <QTcpSocket>
#include <QJsonDocument>
#include <QJsonObject>
#include <QTime>
#include <QTimer>
#include <QVariant>
#include <QThread>
#include <QCoreApplication>
#include <QMutex>

#include "tcpcommandqueue.hpp"

namespace talorion {

    class abstract_backend;
    class tcpDriver : public QObject
    {
        Q_OBJECT

    public:
        tcpDriver(int id, QByteArray getInfoCommand, QByteArray getMinimalSetActCommand, abstract_backend *bk=NULL, QObject* par=0);
        ~tcpDriver(void);
        Q_DISABLE_COPY(tcpDriver)

        enum requestStatus {OK,BUSY,DISCONNECTED,CONNECTION_ERROR};
        bool connectDevice(QString ip, qint32 port = 2701, int timeoutMs = 4000);
        void disconect();
        int getBox_id() const;

    public slots:
        void setDataCommand(QByteArray cmd);
        void customCommand(const QString &cm);

    signals:
        void receivedData(QVariantMap data, tcpDriverDataTypes::dataType type, int box_id=0);
        void receivedCustomData(const QString &cm, int box_id=0);
        void error(QString errorString, int box_id=0);

        void connected(int box_id=0);
        void disconnected(int box_id=0);
        void setDataError(QString errorString, int box_id=0);

    private:
        void recheckConnection();
        bool waitTransmissionFinish();
        void sendCommand(QByteArray cmd, tcpDriverDataTypes::dataType type);

        void remove_all_values();

    private slots:
        void tcpSocket_connected();
        void tcpSocket_disconnected();

    private:
        QTcpSocket* tcpSocket;
        QSerialPort* serialSocket;
        tcpDriverDataTypes::dataType transmissionContext;
        QByteArray recBuf;
        QByteArray sendBuf;
        QByteArray getInfoCommand_val;
        QByteArray getMinimalSetActCommand_val;
        int commandCounter;
        bool isScope;
        int ScopeNumberOfMeasurements;
        int curlyOpen, curlyClose;
        QTimer* timeoutTimer;
        QTimer* pollTimer;
        QString lastIP;
        qint32  lastPort;
        qint32 SerialBaud;
        bool ongoingRequest;
        int requestCounter;
        int responseCounter;
        tcpCommandQueue* queue;
        const int box_id;
        QMutex mutex;

        abstract_backend* m_back;


    private slots:
        void poll();
        void parsePackage();
        void timeoutCheck();
        void serialError(QSerialPort::SerialPortError seriualErr);
        void tcpError(QAbstractSocket::SocketError tcpErr);
    };
}


#endif // TCPDRIVER_H

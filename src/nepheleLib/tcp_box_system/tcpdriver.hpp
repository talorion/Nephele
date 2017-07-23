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
#include <QMutex>

#include "tcpcommandqueue.hpp"

#include "abstracttcpdriver.hpp"

namespace talorion {

    class abstract_backend;

    class tcpDriver : public AbstractTcpDriver
    {
        Q_OBJECT

    public:
        tcpDriver(int id, QByteArray getInfoCommand, QByteArray getMinimalSetActCommand, abstract_backend *bk=NULL, QObject* par=0);
        tcpDriver(int id, abstract_backend *bk = Q_NULLPTR, QObject* par=0);
        ~tcpDriver(void);
        Q_DISABLE_COPY(tcpDriver)

        enum requestStatus {OK,BUSY,DISCONNECTED,CONNECTION_ERROR};
        //bool connectDevice();
        virtual bool connectDevice(QString ip, qint32 port = 2701, int timeoutMs = 4000)Q_DECL_OVERRIDE;
        //void disconectDevice();
        //int getBox_id() const;

        abstract_backend *getBack() const;

        QTcpSocket *getTcpSocket() const;

        bool boxIsConnected();

    public slots:
        virtual bool connectDevice() Q_DECL_OVERRIDE;
        virtual void disconectDevice() Q_DECL_OVERRIDE;

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
        void initialize(QByteArray getInfoCommand, QByteArray getMinimalSetActCommand, abstract_backend *bk=NULL);

        void recheckConnection();
        bool waitTransmissionFinish();
        void sendCommand(QByteArray cmd, tcpDriverDataTypes::dataType type);

        void remove_all_values();

    private slots:
        void tcpSocket_connected();
        void tcpSocket_disconnected();

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
        //const int box_id;
        QMutex mutex;

        abstract_backend* m_back;


    private slots:
        void poll();
        void parsePackage();
        void timeoutCheck();
        void tcpError(QAbstractSocket::SocketError tcpErr);
    };
}


#endif // TCPDRIVER_H

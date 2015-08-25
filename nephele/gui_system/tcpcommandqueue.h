#ifndef TCPCOMMANDQUEUE_H
#define TCPCOMMANDQUEUE_H

#include <QObject>

namespace talorion {

    namespace tcpDriverDataTypes {
        enum dataType {ALLDATA, ACTSETDATA, SETDATA, CUSTOMCOMMAND, IDLE, NONE};
    }

    class tcpCommand : public QObject
    {
        Q_OBJECT

    public:
        tcpCommand(QByteArray cmd, tcpDriverDataTypes::dataType cmdType, QObject* par=0);
        ~tcpCommand();
        Q_DISABLE_COPY(tcpCommand)

        QByteArray getCmd(void);

        tcpDriverDataTypes::dataType getCmdType(void);

    private:
        QByteArray cmdVal;
        tcpDriverDataTypes::dataType cmdTypeVal;
    };

    class tcpCommandQueue : public QObject
    {
        Q_OBJECT

    public:
        tcpCommandQueue(QObject* par=0);
        ~tcpCommandQueue();
        Q_DISABLE_COPY(tcpCommandQueue)

        tcpCommand* getNext();
        int pushLast(tcpCommand* cmd);
        int pushLast(QByteArray cmd, tcpDriverDataTypes::dataType type);
        int pushFirst(tcpCommand* cmd);
        int pushFirst(QByteArray cmd, tcpDriverDataTypes::dataType type);
        int flush();
        int length();
    private:
        QList<tcpCommand*> commandList;
    };
}

#endif // TCPCOMMANDQUEUE_H

#ifndef TCPCOMMANDQUEUE_H
#define TCPCOMMANDQUEUE_H

#include <QObject>

#include "tcpcommand.hpp"

namespace talorion {

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

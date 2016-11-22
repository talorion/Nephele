#include "tcpcommandqueue.hpp"
#include <QDebug>

namespace talorion {

    tcpCommandQueue::tcpCommandQueue(QObject *par):
        QObject(par),
        commandList()
    {

    }

    tcpCommandQueue::~tcpCommandQueue()
    {

    }

    tcpCommand *tcpCommandQueue::getNext()
    {
        qDebug() << "getNext";

        if (!commandList.isEmpty())
        {
            return commandList.takeFirst();
        }
        return NULL;
    }

    int tcpCommandQueue::pushLast(tcpCommand *cmd)
    {
        qDebug() << "pushLast2";

        commandList.append(cmd);
        return commandList.length();
    }

    int tcpCommandQueue::pushFirst(tcpCommand *cmd)
    {
        return 0;
        if (commandList.length()==0) return 0;
        qDebug() << "pushFirst"<<cmd<<commandList.length();

        commandList.prepend(cmd);
        return commandList.length();
    }

    int tcpCommandQueue::pushFirst(QByteArray cmd, tcpDriverDataTypes::dataType type)
    {
        qDebug() << "pushFirst2";
        tcpCommand* tmpCmd = new tcpCommand(cmd,type);
        pushFirst(tmpCmd);

        return 0;
    }

    int tcpCommandQueue::pushLast(QByteArray cmd, tcpDriverDataTypes::dataType type)
    {
        qDebug() << "pushLast" << cmd;
        tcpCommand* tmpCmd = new tcpCommand(cmd,type);
        pushLast(tmpCmd);

        return 0;
    }

    int tcpCommandQueue::flush()
    {
        qDebug() << "flush";

        int lng = commandList.length();
        commandList.clear();

        return lng;
    }

    int tcpCommandQueue::length()
    {
        return commandList.length();
    }
}

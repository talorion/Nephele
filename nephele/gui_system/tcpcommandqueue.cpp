#include "tcpcommandqueue.h"


namespace talorion {

    tcpCommand::tcpCommand(QByteArray cmd, tcpDriverDataTypes::dataType cmdType, QObject *par):
        QObject(par),
        cmdVal(cmd),
        cmdTypeVal(cmdType)
    {
        //cmdVal = cmd;
        //cmdTypeVal = cmdType;
    }

    tcpCommand::~tcpCommand()
    {

    }

    QByteArray tcpCommand::getCmd()
    {
        return cmdVal;
    }

    tcpDriverDataTypes::dataType tcpCommand::getCmdType()
    {
        return cmdTypeVal;
    }


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
        if (!commandList.isEmpty())
        {
            return commandList.takeFirst();
        }
        return NULL;
    }

    int tcpCommandQueue::pushLast(tcpCommand *cmd)
    {
        commandList.append(cmd);
        return commandList.length();
    }

    int tcpCommandQueue::pushFirst(tcpCommand *cmd)
    {
        commandList.prepend(cmd);
        return commandList.length();
    }

    int tcpCommandQueue::pushFirst(QByteArray cmd, tcpDriverDataTypes::dataType type)
    {
        tcpCommand* tmpCmd = new tcpCommand(cmd,type);
        pushFirst(tmpCmd);

        return 0;
    }

    int tcpCommandQueue::pushLast(QByteArray cmd, tcpDriverDataTypes::dataType type)
    {
        tcpCommand* tmpCmd = new tcpCommand(cmd,type);
        pushLast(tmpCmd);

        return 0;
    }

    int tcpCommandQueue::flush()
    {
        int lng = commandList.length();
        commandList.clear();

        return lng;
    }

    int tcpCommandQueue::length()
    {
        return commandList.length();
    }
}

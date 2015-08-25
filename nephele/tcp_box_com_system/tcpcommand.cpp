#include "tcpcommand.hpp"

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
}

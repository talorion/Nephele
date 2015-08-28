#ifndef TCPCOMMAND_HPP
#define TCPCOMMAND_HPP

#include <QObject>

#include "core/tcpdriverdatatypes.hpp"

namespace talorion {

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
}


#endif // TCPCOMMAND_HPP

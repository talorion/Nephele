#include "AbstractQEcmdCommand.hpp"

namespace talorion {

AbstractQEcmdCommand::AbstractQEcmdCommand(QString _cmd, int _timeout)
    :m_cmdStr(),
      m_timeout(_timeout)
{
    setCmdStr(_cmd);
}

bool AbstractQEcmdCommand::operator ==(const AbstractQEcmdCommand &b) const
{
    return QString::compare(cmdStr(), b.cmdStr(), Qt::CaseInsensitive) == 0;
}

bool AbstractQEcmdCommand::operator !=(const AbstractQEcmdCommand &b) const
{
    return !(*this == b);
}

QString AbstractQEcmdCommand::cmdStr() const
{
    return m_cmdStr;
}

int AbstractQEcmdCommand::cmdLen() const
{
    return cmdStr().size();
}

int AbstractQEcmdCommand::timeout() const
{
    return m_timeout;
}

QByteArray AbstractQEcmdCommand::toByteArray() const
{
    return cmdStr().toLocal8Bit();
}

QString AbstractQEcmdCommand::toString() const
{
    return cmdStr();
}

void AbstractQEcmdCommand::setCmdStr(const QString &cmd_str)
{
    if(!cmd_str.endsWith("\r\n",Qt::CaseInsensitive))
        m_cmdStr = cmd_str.trimmed().append("\r\n");
    else
        m_cmdStr = cmd_str;
}

} // namespace talorion

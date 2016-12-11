#include "AbstractEcmdCommand.hpp"

namespace talorion {

  const int AbstractEcmdCommand::MaxBufferSize= 0x000FFFFF;//1048575 Byte ca 1 MB

  AbstractEcmdCommand::AbstractEcmdCommand(const QString &cmd_str, QObject *par) :
    QObject(par),
    m_command_string(cmd_str),
    m_state(COMMAND_STATE_UNKNOWN)
  {

  }

  QString AbstractEcmdCommand::commandString() const
  {
    return m_command_string;
  }

  quint64 AbstractEcmdCommand::expectedDataLength() const{
    quint64 tmp= doExpectedDataLength();
    if(tmp<=0)
      tmp=1;
    return tmp;
  }

  ecmd_command_state_t AbstractEcmdCommand::state() const
  {
    return m_state;
  }

  void AbstractEcmdCommand::setState(const ecmd_command_state_t &state)
  {
    m_state = state;
  }

} // namespace talorion

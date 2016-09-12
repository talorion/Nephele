#include "abstract_ecmd_command.hpp"

namespace talorion {

  const int abstract_ecmd_command::MaxBufferSize= 0x000FFFFF;//1048575 Byte ca 1 MB

  abstract_ecmd_command::abstract_ecmd_command(const QString &cmd_str, QObject *par) :
    QObject(par),
    m_command_string(cmd_str),
    m_state(COMMAND_STATE_UNKNOWN)
  {



  }

  QString abstract_ecmd_command::command_string() const
  {
    return m_command_string;
  }

  quint64 abstract_ecmd_command::expected_data_length() const{
    quint64 tmp= do_expected_data_length();
    if(tmp<=0)
      tmp=1;
    return tmp;
  }

  ecmd_command_state_t abstract_ecmd_command::state() const
  {
    return m_state;
  }

  void abstract_ecmd_command::setState(const ecmd_command_state_t &state)
  {
    m_state = state;
  }

} // namespace talorion

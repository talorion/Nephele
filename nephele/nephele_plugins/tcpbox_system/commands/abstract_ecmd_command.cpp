#include "abstract_ecmd_command.hpp"

namespace talorion {

  const int abstract_ecmd_command::MaxBufferSize= 0x000FFFFF;//1048575 Byte ca 1 MB

  abstract_ecmd_command::abstract_ecmd_command(const QString &cmd_str, QObject *par) :
    QObject(par),
    m_command_string(cmd_str)
  {

  }

  QString abstract_ecmd_command::command_string() const
  {
    return m_command_string;
  }
} // namespace talorion

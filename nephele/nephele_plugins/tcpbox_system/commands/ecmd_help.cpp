#include "ecmd_help.hpp"

namespace talorion {

  ecmd_help::ecmd_help(QObject *par) :
    abstract_ecmd_command(QString("help"), par)
  {

  }

  QString ecmd_help::build_command_string() const
  {
   return command_string();
  }

} // namespace talorion

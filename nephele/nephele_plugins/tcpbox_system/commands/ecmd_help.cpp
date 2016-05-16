#include "ecmd_help.hpp"

namespace talorion {

  ecmd_help::ecmd_help(QObject *par) :
    abstract_ecmd_command(QString("help"), par)
  {

  }

} // namespace talorion

#include "emcd_version.hpp"

namespace talorion {

  emcd_version::emcd_version(QObject *par) :
    abstract_ecmd_command(QString("version"), par)
  {

  }

  QString emcd_version::build_command_string() const
  {
    return command_string();
  }

} // namespace talorion

#include "emcd_version.hpp"

namespace talorion {

  emcd_version::emcd_version(QObject *par) :
    abstract_ecmd_command(QString("version"), par)
  {

  }

} // namespace talorion

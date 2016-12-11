#include "EmcdVersion.hpp"

namespace talorion {

  EmcdVersion::EmcdVersion(QObject *par) :
    AbstractEcmdCommand(QString("version"), par)
  {

  }

  QString EmcdVersion::buildCommandString() const
  {
    return commandString();
  }

} // namespace talorion

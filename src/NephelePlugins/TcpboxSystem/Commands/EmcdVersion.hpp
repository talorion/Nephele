#ifndef TALORION_EMCD_VERSION_HPP
#define TALORION_EMCD_VERSION_HPP

#include "AbstractEcmdCommand.hpp"

namespace talorion {

  class EmcdVersion : public AbstractEcmdCommand
  {
    Q_OBJECT
  public:
    explicit EmcdVersion(QObject *par = Q_NULLPTR);

    // abstract_ecmd_command interface
  public:
    virtual QString buildCommandString() const;
  };

} // namespace talorion

#endif // TALORION_EMCD_VERSION_HPP

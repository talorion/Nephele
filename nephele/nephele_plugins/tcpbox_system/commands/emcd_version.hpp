#ifndef TALORION_EMCD_VERSION_HPP
#define TALORION_EMCD_VERSION_HPP

#include "abstract_ecmd_command.hpp"

namespace talorion {

  class emcd_version : public abstract_ecmd_command
  {
    Q_OBJECT
  public:
    explicit emcd_version(QObject *par = Q_NULLPTR);

  signals:

  public slots:
  };

} // namespace talorion

#endif // TALORION_EMCD_VERSION_HPP

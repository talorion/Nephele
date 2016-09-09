#ifndef TALORION_ECMD_HELP_HPP
#define TALORION_ECMD_HELP_HPP

#include "abstract_ecmd_command.hpp"
#include <QTimer>



namespace talorion {

  class ecmd_help : public abstract_ecmd_command
  {
    Q_OBJECT
  public:
    explicit ecmd_help(QObject *par = Q_NULLPTR);


    // abstract_ecmd_command interface
  public:
    virtual QString build_command_string() const;
  };

} // namespace talorion

#endif // TALORION_ECMD_HELP_HPP

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
    virtual QString build_command_string() const Q_DECL_OVERRIDE;

     virtual bool parse_data(QByteArray &data) Q_DECL_OVERRIDE;

    // abstract_ecmd_command interface
  private:
    virtual quint64 do_expected_data_length() const Q_DECL_OVERRIDE;


  };

} // namespace talorion

#endif // TALORION_ECMD_HELP_HPP

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

  bool ecmd_help::parse_data(QByteArray &data)
  {
    QString tmp(data);

    auto lst=tmp.split("\r\n", QString::SkipEmptyParts);

    return lst.contains(command_string());
  }

  quint64 ecmd_help::do_expected_data_length() const
  {
    return command_string().length()+2;
  }

} // namespace talorion

#include "EcmdHelp.hpp"

namespace talorion {

  EcmdHelp::EcmdHelp(QObject *par) :
    AbstractEcmdCommand(QString("help"), par)
  {

  }

  QString EcmdHelp::buildCommandString() const
  {
    return commandString();
  }

  bool EcmdHelp::parseData(QByteArray &data)
  {
    QString tmp(data);

    auto lst=tmp.split("\r\n", QString::SkipEmptyParts);

    return lst.contains(commandString());
  }

  quint64 EcmdHelp::doExpectedDataLength() const
  {
    return commandString().length()+2;
  }

} // namespace talorion

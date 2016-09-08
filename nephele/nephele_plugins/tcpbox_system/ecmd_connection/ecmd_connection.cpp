#include "ecmd_connection.hpp"

#include "commands/ecmd_help.hpp"

namespace talorion {

  ecmd_connection::ecmd_connection(QObject *par) :
    QTcpSocket(par),
    m_supported_commands()
  {
    add_supported_command(new ecmd_help());

    QObject::connect(this, SIGNAL(readyRead()), this, SLOT(processReadyRead()));
    //QObject::connect(this, SIGNAL(connected()),this, SLOT(sendGreetingMessage()));
    //QObject::connect(this, SIGNAL(disconnected()), &pingTimer, SLOT(stop()));
    //QObject::connect(&pingTimer, SIGNAL(timeout()), this, SLOT(sendPing()));
    //

  }

  bool ecmd_connection::is_command_supported(const QString &cmd) const
  {
    if (cmd.isEmpty())
      return false;

    return m_supported_commands.contains(cmd);
  }

  bool ecmd_connection::send_command(const QString &cmd)
  {
    auto sttt= state();
    if(sttt != QAbstractSocket::ConnectedState)
      return false;

    if (cmd.isNull())
      return false;

    if (cmd.isEmpty())
      return false;

    if(!is_command_supported(cmd))
      return false;

    auto cmd_str =cmd.trimmed();

    QByteArray msg = cmd_str.toLocal8Bit();
    msg = msg.trimmed().append("\r\n");
    return write(msg) == msg.size();

  }

  bool ecmd_connection::add_supported_command(ecmd_connection::command_t cmd)
  {
    if(cmd.isNull())
      return false ;

    auto cmd_str = cmd->command_string();

    if (cmd_str.isEmpty())
      return false;

    if(is_command_supported(cmd_str))
      return false;

    m_supported_commands.insert(cmd_str, cmd);
    return true;

  }

  bool ecmd_connection::hasEnoughData()
  {
    return bytesAvailable()>=20;
  }

  void ecmd_connection::processReadyRead()
  {
    do {

        if (!hasEnoughData())
          return;
        //        if (currentDataType == Undefined) {
        //            if (!readProtocolHeader())
        //                return;
        //        }
        //        if (!hasEnoughData())
        //            return;
        //        processData();
        //        QString tmp(readAll());
        //        qDebug()<<tmp;

        QString tmp(readAll());
        qDebug()<<tmp;

      } while (bytesAvailable() > 0);

  }

} // namespace talorion

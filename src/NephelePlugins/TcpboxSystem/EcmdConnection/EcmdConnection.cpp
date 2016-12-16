#include "EcmdConnection.hpp"

#include "Commands/EcmdHelp.hpp"

#include <QEventLoop>

namespace talorion {

  EcmdConnection::EcmdConnection(QObject *par) :
    QTcpSocket(par),
    m_supported_commands(),
    m_current_command(Q_NULLPTR),
    m_timeout_timer()
  {
    addSupportedCommand(new EcmdHelp());

    QObject::connect(this, SIGNAL(readyRead()), this, SLOT(processReadyRead()));
    QObject::connect(this, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(socketError(QAbstractSocket::SocketError)));

    resetCurrentCommand();

    m_timeout_timer.setSingleShot(true);
    m_timeout_timer.setInterval(5000);

    connect(&m_timeout_timer, SIGNAL(timeout()), this, SLOT(commandTimedOut()));

  }

  bool EcmdConnection::isCommandSupported(const QString &cmd) const
  {
    if (cmd.isNull())
      return false;

    if (cmd.isEmpty())
      return false;

    return m_supported_commands.contains(cmd);
  }

  bool EcmdConnection::waitForCommandFinished(int timeout)
  {
    auto tmp=m_current_command;
    if(!ongoingCommand())
      return false;

    QEventLoop tmp_evt_loop;
    QTimer::singleShot(timeout, &tmp_evt_loop, SLOT(quit()));
    connect(this, SIGNAL(commandFinished()), &tmp_evt_loop, SLOT(quit()));
    tmp_evt_loop.exec();

    return tmp->state()==COMMAND_STATE_OK;
  }

  bool EcmdConnection::sendCommand(const QString &cmd)
  {
    auto sttt= state();
    if(sttt != QAbstractSocket::ConnectedState)
      return false;

    if(!isCommandSupported(cmd))
      return false;

    bool ret= false;
    auto cmd_it =m_supported_commands.find(cmd);
    if(cmd_it != m_supported_commands.end())
      ret = sendCommand(cmd_it.value());

    return ret;
  }

  bool EcmdConnection::sendCommand(EcmdConnection::command_t cmd)
  {
    if(ongoingCommand())
      return false;

    auto cmd_str =cmd->buildCommandString().trimmed();
    QByteArray msg = cmd_str.toLocal8Bit();
    msg = msg.trimmed().append("\r\n");
    bool ret= (write(msg) == msg.size());

    if(ret){
        setCurrentCommand(cmd);
        m_timeout_timer.start();
      }

    return ret;
  }

  void EcmdConnection::setCurrentCommand(EcmdConnection::command_t cmd)
  {
    if(cmd.isNull())
      return;

    m_current_command = cmd;
    m_current_command->setState(COMMAND_STATE_STARTED);

    //emit command_changed();
    emit commandStarted();

  }

  void EcmdConnection::resetCurrentCommand()
  {

    m_timeout_timer.stop();
    m_current_command.clear();

    emit commandFinished();
  }

  bool EcmdConnection::addSupportedCommand(EcmdConnection::command_t cmd)
  {
    if(cmd.isNull())
      return false ;

    auto cmd_str = cmd->commandString();

    if (cmd_str.isEmpty())
      return false;

    if(isCommandSupported(cmd_str))
      return false;

    m_supported_commands.insert(cmd_str, cmd);
    return true;

  }

  bool EcmdConnection::hasEnoughData()
  {
    if(!ongoingCommand())
      return false;

    qint64 expected_data_length=m_current_command->expectedDataLength();
    return bytesAvailable()>=expected_data_length;
  }

  bool EcmdConnection::ongoingCommand() const
  {
    return !(m_current_command.isNull());
  }


  void EcmdConnection::processReadyRead()
  {
    if(!ongoingCommand())
      return;

    do {
        if(!hasEnoughData())
          break;

        if(!m_timeout_timer.isActive()){
            m_current_command->setState(COMMAND_STATE_ERROR);
            //emit command_changed();
            break;
          }

        if(state()!=QAbstractSocket::ConnectedState){
            m_current_command->setState(COMMAND_STATE_ERROR);
            //emit command_changed();
            break;
          }

        auto tmp = readAll();
        if(!m_current_command->parseData(tmp)){
            m_current_command->setState(COMMAND_STATE_ERROR);
            //emit command_changed();
            break;
          }

        m_current_command->setState(COMMAND_STATE_OK);


      } while (true);


    resetCurrentCommand();

  }

  void EcmdConnection::socketError(QAbstractSocket::SocketError socketError)
  {
    Q_UNUSED(socketError);

  }

  void EcmdConnection::commandTimedOut()
  {

  }

  EcmdConnection::command_t EcmdConnection::currentCommand() const
  {
    return m_current_command;
  }

} // namespace talorion

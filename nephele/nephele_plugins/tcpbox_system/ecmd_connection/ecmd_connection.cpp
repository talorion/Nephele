#include "ecmd_connection.hpp"

#include "commands/ecmd_help.hpp"

#include <QEventLoop>

namespace talorion {

  ecmd_connection::ecmd_connection(QObject *par) :
    QTcpSocket(par),
    m_supported_commands(),
    m_current_command(Q_NULLPTR),
    m_timeout_timer()
  {
    add_supported_command(new ecmd_help());

    QObject::connect(this, SIGNAL(readyRead()), this, SLOT(processReadyRead()));
    QObject::connect(this, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(socket_error(QAbstractSocket::SocketError)));

    reset_current_command();

    m_timeout_timer.setSingleShot(true);
    m_timeout_timer.setInterval(5000);

    connect(&m_timeout_timer, SIGNAL(timeout()), this, SLOT(command_timed_out()));

  }

  bool ecmd_connection::is_command_supported(const QString &cmd) const
  {
    if (cmd.isNull())
      return false;

    if (cmd.isEmpty())
      return false;

    return m_supported_commands.contains(cmd);
  }

  bool ecmd_connection::wait_for_command_finished(int timeout)
  {
    auto tmp=m_current_command;
    if(!ongoing_command())
      return false;

    QEventLoop tmp_evt_loop;
    QTimer::singleShot(timeout, &tmp_evt_loop, SLOT(quit()));
    connect(this, SIGNAL(command_finished()), &tmp_evt_loop, SLOT(quit()));
    tmp_evt_loop.exec();

    return tmp->state()==COMMAND_STATE_OK;
  }

  bool ecmd_connection::send_command(const QString &cmd)
  {
    auto sttt= state();
    if(sttt != QAbstractSocket::ConnectedState)
      return false;

    if(!is_command_supported(cmd))
      return false;

    bool ret= false;
    auto cmd_it =m_supported_commands.find(cmd);
    if(cmd_it != m_supported_commands.end())
      ret = send_command(cmd_it.value());

    return ret;
  }

  bool ecmd_connection::send_command(ecmd_connection::command_t cmd)
  {
    if(ongoing_command())
      return false;

    auto cmd_str =cmd->build_command_string().trimmed();
    QByteArray msg = cmd_str.toLocal8Bit();
    msg = msg.trimmed().append("\r\n");
    bool ret= (write(msg) == msg.size());

    if(ret){
        set_current_command(cmd);
        m_timeout_timer.start();
      }

    return ret;
  }

  void ecmd_connection::set_current_command(ecmd_connection::command_t cmd)
  {
    if(cmd.isNull())
      return;

    m_current_command = cmd;
    m_current_command->setState(COMMAND_STATE_STARTED);

    //emit command_changed();
    emit command_started();

  }

  void ecmd_connection::reset_current_command()
  {

    m_timeout_timer.stop();
    m_current_command.clear();

    emit command_finished();
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
    if(!ongoing_command())
      return false;

    qint64 expected_data_length=m_current_command->expected_data_length();
    return bytesAvailable()>=expected_data_length;
  }

  bool ecmd_connection::ongoing_command() const
  {
    return !(m_current_command.isNull());
  }


  void ecmd_connection::processReadyRead()
  {
    if(!ongoing_command())
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
        if(!m_current_command->parse_data(tmp)){
            m_current_command->setState(COMMAND_STATE_ERROR);
            //emit command_changed();
            break;
          }

        m_current_command->setState(COMMAND_STATE_OK);


      } while (true);


    reset_current_command();

  }

  void ecmd_connection::socket_error(QAbstractSocket::SocketError socketError)
  {
    Q_UNUSED(socketError);

  }

  void ecmd_connection::command_timed_out()
  {

  }

  ecmd_connection::command_t ecmd_connection::current_command() const
  {
    return m_current_command;
  }

} // namespace talorion

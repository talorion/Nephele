#ifndef TALORION_ECMD_CONNECTION_HPP
#define TALORION_ECMD_CONNECTION_HPP

#include "commands/abstract_ecmd_command.hpp"

#include <QTcpSocket>
#include <QList>
#include <QPointer>
#include <QTimer>


namespace talorion {

  class ecmd_connection : public QTcpSocket
  {
    Q_OBJECT
  public:
    using command_t = QPointer<abstract_ecmd_command>;
    using commands_container = QMap<QString, command_t >;

  public:
    explicit ecmd_connection(QObject *par = Q_NULLPTR);

    bool is_command_supported(const QString &cmd)const;

    bool wait_for_command_finished(int timeout = 5000);

  public slots:
    bool send_command(const QString &cmd);

  signals:
    void readyForUse();
    void command_finished();

  private:
    bool add_supported_command(command_t cmd);
    bool hasEnoughData();
    bool ongoing_command()const;
    bool send_command(command_t cmd);

    void set_current_command(command_t cmd);
    void reset_current_command();

  private slots:
    void processReadyRead();
    void socket_error(QAbstractSocket::SocketError socketError);
    void command_timed_out();

  private:
    commands_container m_supported_commands;
    command_t m_current_command;

    QTimer m_timeout_timer;

  };

} // namespace talorion

#endif // TALORION_ECMD_CONNECTION_HPP

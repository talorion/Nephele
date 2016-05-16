#ifndef TALORION_ECMD_CONNECTION_HPP
#define TALORION_ECMD_CONNECTION_HPP

#include "commands/abstract_ecmd_command.hpp"

#include <QTcpSocket>
#include <QList>
#include <QPointer>


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

  public slots:
    bool send_command(const QString &cmd);

  signals:
    void readyForUse();

  private:
    bool add_supported_command(command_t cmd);
    bool hasEnoughData();

  private slots:
    void processReadyRead();

  private:
    commands_container m_supported_commands;

  };

} // namespace talorion

#endif // TALORION_ECMD_CONNECTION_HPP

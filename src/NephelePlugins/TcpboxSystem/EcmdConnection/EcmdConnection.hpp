#ifndef TALORION_ECMD_CONNECTION_HPP
#define TALORION_ECMD_CONNECTION_HPP

#include "Commands/AbstractEcmdCommand.hpp"

#include <QTcpSocket>
#include <QList>
#include <QPointer>
#include <QTimer>


namespace talorion {

  class EcmdConnection : public QTcpSocket
  {
    Q_OBJECT
  public:
    using command_t = QPointer<AbstractEcmdCommand>;
    using commands_container = QMap<QString, command_t >;

  public:
    explicit EcmdConnection(QObject *par = Q_NULLPTR);

    bool isCommandSupported(const QString &cmd)const;

    bool waitForCommandFinished(int timeout = 5000);

    command_t currentCommand() const;

  public slots:
    bool sendCommand(const QString &cmd);

  signals:
    void readyForUse();

    //void command_changed();
    void commandStarted();
    void commandError();
    void commandFinished();

  private:
    bool addSupportedCommand(command_t cmd);
    bool hasEnoughData();
    bool ongoingCommand()const;
    bool sendCommand(command_t cmd);

    void setCurrentCommand(command_t cmd);
    void resetCurrentCommand();

  private slots:
    void processReadyRead();
    void socketError(QAbstractSocket::SocketError socketError);
    void commandTimedOut();

  private:
    commands_container m_supported_commands;
    command_t m_current_command;

    QTimer m_timeout_timer;

  };

} // namespace talorion

#endif // TALORION_ECMD_CONNECTION_HPP

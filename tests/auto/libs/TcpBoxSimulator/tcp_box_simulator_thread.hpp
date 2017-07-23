#ifndef TCP_BOX_SIMULATOR_THREAD_HPP
#define TCP_BOX_SIMULATOR_THREAD_HPP

#include <QThread>
#include "tcpboxsimulator_global.hpp"

class TCPBOXSIMULATORSHARED_EXPORT tcp_box_simulator_thread : public QThread
{
  Q_OBJECT
public:
  explicit tcp_box_simulator_thread(QObject *par = 0);

  qint16 port() const;

  QString getIpAddress() const;

  QString getHelpCmd() const;

signals:
  void server_started();


private slots:

  // QThread interface
protected:
  virtual void run() Q_DECL_OVERRIDE;

  qint16 m_port;
  QString ipAddress;
  QString helpCmd;
};

#endif // TCP_BOX_SIMULATOR_THREAD_HPP

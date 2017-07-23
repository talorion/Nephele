#ifndef TCP_BOX_SIMULATOR_HPP
#define TCP_BOX_SIMULATOR_HPP

#include <QObject>
#include <QNetworkSession>
#include <QPointer>
#include "tcpboxsimulator_global.hpp"
#include "tcp_box_server.h"
#include "abstract_simulated_box.hpp"

class TCPBOXSIMULATORSHARED_EXPORT tcp_box_simulator : public QObject
{
  Q_OBJECT
public:
  explicit tcp_box_simulator(abstract_simulated_box *b, qint16 port =2701, QObject *par = 0);

  void close();

  QString getIpAddress() const;

  qint16 getPort() const;

private slots:
  void sessionOpened();

private:
  QPointer<tcp_box_server> tcpServer;
  QPointer<QNetworkSession> networkSession;
  QPointer<abstract_simulated_box> box;
  const qint16 m_port;
  QString ipAddress;

  // QThread interface
protected:

};

#endif // TCP_BOX_SIMULATOR_HPP

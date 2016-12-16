#include "tcp_box_simulator_thread.hpp"

#include"tcp_box_simulator.hpp"
#include "simulated_fc_box.h"
#include "tcp_box_server.h"
#include "tcp_box_simulator.hpp"

tcp_box_simulator_thread::tcp_box_simulator_thread(QObject *par) :
  QThread(par),
  m_port(0),
  ipAddress()
{

}

void tcp_box_simulator_thread::run()
{

  simulated_fc_box* fc_box = new simulated_fc_box();
  tcp_box_simulator simul(fc_box);
  Q_UNUSED(simul);

  m_port =simul.getPort();
  ipAddress = simul.getIpAddress();

  emit server_started();

  exec();

  delete fc_box;

}

QString tcp_box_simulator_thread::getIpAddress() const
{
  return ipAddress;
}

qint16 tcp_box_simulator_thread::port() const
{
  return m_port;
}

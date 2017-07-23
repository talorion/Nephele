#include "tcp_box_simulator.hpp"

#include <QNetworkConfigurationManager>
#include <QSettings>

#include "simulated_fc_box.h"

tcp_box_simulator::tcp_box_simulator(abstract_simulated_box *b, qint16 port, QObject *par) :
  QObject(par),
  tcpServer(Q_NULLPTR),
  networkSession(Q_NULLPTR),
  box(b),
  m_port(port),
  ipAddress()
{
  box = new simulated_fc_box();

  QNetworkConfigurationManager manager;
  if (manager.capabilities() & QNetworkConfigurationManager::NetworkSessionRequired) {
      // Get saved network configuration
      QSettings settings(QSettings::UserScope, QLatin1String("QtProject"));
      settings.beginGroup(QLatin1String("QtNetwork"));
      const QString id = settings.value(QLatin1String("DefaultNetworkConfiguration")).toString();
      settings.endGroup();

      // If the saved network configuration is not currently discovered use the system default
      QNetworkConfiguration config = manager.configurationFromIdentifier(id);
      if ((config.state() & QNetworkConfiguration::Discovered) != QNetworkConfiguration::Discovered) {
          config = manager.defaultConfiguration();
        }

      networkSession = new QNetworkSession(config, this);
      connect(networkSession, SIGNAL(opened()), this, SLOT(sessionOpened()));

      networkSession->open();
    } else {
      sessionOpened();
  }
}

void tcp_box_simulator::close()
{
    tcpServer->close();

}

void tcp_box_simulator::sessionOpened()
{
  if (networkSession) {
      QNetworkConfiguration config = networkSession->configuration();
      QString id;
      if (config.type() == QNetworkConfiguration::UserChoice)
        id = networkSession->sessionProperty(QLatin1String("UserChoiceConfiguration")).toString();
      else
        id = config.identifier();

      QSettings settings(QSettings::UserScope, QLatin1String("QtProject"));
      settings.beginGroup(QLatin1String("QtNetwork"));
      settings.setValue(QLatin1String("DefaultNetworkConfiguration"), id);
      settings.endGroup();
    }

  if(!box)
    return;

  tcpServer = new tcp_box_server(box, this);

  QList<QHostAddress> ipAddressesList = QNetworkInterface::allAddresses();
  for (int i = 0; i < ipAddressesList.size(); ++i) {
      if (ipAddressesList.at(i) != QHostAddress::LocalHost && ipAddressesList.at(i).toIPv4Address()) {

          ipAddress="";
          if (!tcpServer->listen(ipAddressesList.at(i),m_port)) {
              continue;
            }

          ipAddress = ipAddressesList.at(i).toString();
          break;
        }
    }
  // if we did not find one, use IPv4 localhost
  if (ipAddress.isEmpty()){
    ipAddress = QHostAddress(QHostAddress::LocalHost).toString();
    tcpServer->listen(QHostAddress(QHostAddress::LocalHost),m_port);

  }

  qDebug()<<(tr("The server is running on IP: %1 port: %2").arg(ipAddress).arg(tcpServer->serverPort()));
}


qint16 tcp_box_simulator::getPort() const
{
  return m_port;
}

QString tcp_box_simulator::getIpAddress() const
{
  return ipAddress;
}


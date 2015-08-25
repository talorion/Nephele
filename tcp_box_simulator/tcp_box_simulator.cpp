#include "tcp_box_simulator.hpp"

#include <QNetworkConfigurationManager>
#include <QSettings>
#include <QMessageBox>
#include <QTcpSocket>

tcp_box_simulator::tcp_box_simulator(QWidget *par) :
    QDialog(par),
    tcpServer(0),
    networkSession(0)
{
    statusLabel = new QLabel;
    quitButton = new QPushButton(tr("Quit"));
    quitButton->setAutoDefault(false);

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

        statusLabel->setText(tr("Opening network session."));
        networkSession->open();
    } else {
        sessionOpened();
    }


    fortunes << tr("You've been leading a dog's life. Stay off the furniture.")
             << tr("You've got to think about tomorrow.")
             << tr("You will be surprised by a loud noise.")
             << tr("You will feel hungry again in another hour.")
             << tr("You might have mail.")
             << tr("You cannot kill time without injuring eternity.")
             << tr("Computers are not intelligent. They only think they are.");

    connect(quitButton, SIGNAL(clicked()), this, SLOT(close()));

    connect(tcpServer, SIGNAL(newConnection()), this, SLOT(sendFortune()));


    QHBoxLayout *buttonLayout = new QHBoxLayout;
    buttonLayout->addStretch(1);
    buttonLayout->addWidget(quitButton);
    buttonLayout->addStretch(1);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(statusLabel);
    mainLayout->addLayout(buttonLayout);
    setLayout(mainLayout);

    setWindowTitle(tr("Fortune Server"));

}

tcp_box_simulator::~tcp_box_simulator()
{

}

void tcp_box_simulator::sessionOpened()
{
    // Save the used configuration
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


    tcpServer = new QTcpServer(this);
    if (!tcpServer->listen(QHostAddress::Any,2701)) {
        QMessageBox::critical(this, tr("Fortune Server"),
                              tr("Unable to start the server: %1.")
                              .arg(tcpServer->errorString()));
        close();
        return;
    }

    QString ipAddress;
    QList<QHostAddress> ipAddressesList = QNetworkInterface::allAddresses();
    // use the first non-localhost IPv4 address
    for (int i = 0; i < ipAddressesList.size(); ++i) {
        if (ipAddressesList.at(i) != QHostAddress::LocalHost &&
                ipAddressesList.at(i).toIPv4Address()) {
            ipAddress = ipAddressesList.at(i).toString();
            break;
        }
    }
    // if we did not find one, use IPv4 localhost
    if (ipAddress.isEmpty())
        ipAddress = QHostAddress(QHostAddress::LocalHost).toString();
    statusLabel->setText(tr("The server is running on\n\nIP: %1\nport: %2\n\n"
                            "Run the Fortune Client example now.")
                         .arg(ipAddress).arg(tcpServer->serverPort()));
}

void tcp_box_simulator::sendFortune()
{

        QByteArray block;
        QDataStream out(&block, QIODevice::WriteOnly);

//        out.setVersion(QDataStream::Qt_4_0);

//        out << (quint16)0;
//        out << fortunes.at(qrand() % fortunes.size());
//        out.device()->seek(0);
//        out << (quint16)(block.size() - sizeof(quint16));

        //out << (quint16)0;
        const char* tmp =    "{\"uibk_v\":1,\"FC\":[\n"
                       "{\"id\":0,\"smin\":0,\"smax\":200,\"amin\":0,\"amax\":200,\"set\":200,\"act\":72.07,\"name\":\"N2\",\"units\":\"sccm\"},\n"
                       "{\"id\":1,\"smin\":0,\"smax\":10,\"amin\":0,\"amax\":10,\"set\":0,\"act\":0.03201,\"name\":\"Cal\",\"units\":\"sccm\"},\n"
                       "{\"id\":2,\"smin\":0,\"smax\":100,\"amin\":0,\"amax\":100,\"set\":0,\"act\":0.06829,\"name\":\"FC_2\",\"units\":\"sccm\"},\n"
                       "{\"id\":3,\"smin\":0,\"smax\":100,\"amin\":0,\"amax\":100,\"set\":0,\"act\":0.3591,\"name\":\"FC_3\",\"units\":\"sccm\"},\n"
                       "{\"id\":4,\"smin\":0,\"smax\":100,\"amin\":0,\"amax\":100,\"set\":0,\"act\":0.01703,\"name\":\"FC_4\",\"units\":\"sccm\"},\n"
                       "{\"id\":5,\"smin\":0,\"smax\":100,\"amin\":0,\"amax\":100,\"set\":0,\"act\":0.1404,\"name\":\"FC_5\",\"units\":\"sccm\"},\n"
                       "{\"id\":6,\"smin\":0,\"smax\":100,\"amin\":0,\"amax\":100,\"set\":0,\"act\":0.02596,\"name\":\"FC_6\",\"units\":\"sccm\"},\n"
                       "{\"id\":7,\"smin\":0,\"smax\":100,\"amin\":0,\"amax\":100,\"set\":0,\"act\":0.3824,\"name\":\"FC_7\",\"units\":\"sccm\"}\n"
                       "]}\0";


        //out.device()->seek(0);


        QTcpSocket *clientConnection = tcpServer->nextPendingConnection();
        connect(clientConnection, SIGNAL(disconnected()), clientConnection, SLOT(deleteLater()));


        clientConnection->write(tmp);
        //clientConnection->disconnectFromHost();

}

#include "jsonrpcdriverutils.hpp"


#include <tcp_box_system/JsonRpc/jsonrpcfactory.hpp>
#include "tcp_box_system/JsonRpc/jsonrpcrequest.hpp"
#include <QEventLoop>
#include <QFuture>
#include <QHostInfo>
#include <QNetworkAddressEntry>
#include <QTcpSocket>
#include <QtConcurrent>
#include <tcp_box_system/TcpBoxEntity/TcpBoxEntityFacorty.hpp>
#include <tcp_box_system/backend_type.hpp>

namespace talorion {

JsonRpcDriverUtils::JsonRpcDriverUtils(QObject *parent) :
    QObject(parent)
{
}

JsonRpcDriverUtils &JsonRpcDriverUtils::instance()
{
    static JsonRpcDriverUtils inst(Q_NULLPTR);
    return inst;
}

QStringList JsonRpcDriverUtils::discoverHosts(quint16 srvPrt)
{
    QStringList ret;
    foreach (auto itrface, QNetworkInterface::allInterfaces()) {
        foreach (auto entry, itrface.addressEntries()) {
            if ( entry.ip() != QHostAddress::LocalHost) {
                QString serverName = entry.ip().toString();

                int pref = entry.prefixLength();
                auto tmp = serverName+QString("/%1").arg(pref);
                auto t =QHostAddress::parseSubnet(tmp);

                ret.append(discoverHosts(t.first, t.second, srvPrt));
            }
        }
    }
    return ret;
}

QStringList JsonRpcDriverUtils::discoverHosts(QHostAddress address, int prefix_len, quint16 srvPrt)
{
    QStringList hosts;

    auto glambda = [](auto serverName, auto serverPort, auto id) { return  JsonRpcDriverUtils::sendJrpcRequest(serverName, serverPort, "getAllMethods", QVariant(), id); };


    quint32 address_len = 0;
    if(address.protocol() == QAbstractSocket::IPv4Protocol)
        address_len = 32;
    else if(address.protocol() == QAbstractSocket::IPv6Protocol)
        return hosts;

    quint32 len = 0;
    len = qPow(2, (address_len-prefix_len));

    quint32 ip=0;
    if(address.protocol() == QAbstractSocket::IPv4Protocol)
        ip = address.toIPv4Address();
    else if(address.protocol() == QAbstractSocket::IPv6Protocol)
        return hosts;

    QMap<QString, QFuture<bool>> results;
    for(quint32 i=ip;i<ip+len;i++){
        QString serverName = QHostAddress(i).toString();
        //quint16 serverPort = 2701;
        qint32 id = 0;

        QFuture<bool> future = QtConcurrent::run(glambda, serverName, srvPrt, id);
        results.insert(serverName, future);
    }

    auto it = results.cbegin();
    for(it = results.cbegin(); it != results.cend(); ++it){
        bool result = it.value().result();
        if(result == true)
            hosts.append(it.key());
    }

    return hosts;
}

bool JsonRpcDriverUtils::sendJrpcRequest(QString serverName, quint16 serverPort, const QString &methodname, const QVariant &params, qint32 id)
{
    const int Timeout = 200;

    QTcpSocket socket;
    socket.connectToHost(serverName, serverPort);

    if (!socket.waitForConnected(Timeout)) {
        return false;
    }

    auto request = JsonRpcFactory::createRequest(methodname, params, id);
    QByteArray datagram = request->toJson();

    if(socket.write(datagram) == -1)
        return false;

    if (!socket.waitForReadyRead(Timeout)) {
        return false;
    }

    //TODO: Parse recieved Data

    return true;
}

EntityManager::EntityID JsonRpcDriverUtils::createNewTcpBoxForHost(QString serverName, quint16 serverPort)
{
//    auto box = EntityManager::get_instance()->tcpBoxExists(serverName, serverPort);
//    if(EntityManager::isValid(box))
//        return box;

    QHostInfo info = QHostInfo::fromName(serverName);
    auto hostname = info.hostName();
    if(info.error()  == QHostInfo::HostNotFound)
        hostname = serverName;

    auto box = EntityManager::get_instance()->createNewTcpBox(hostname, serverName, serverPort);
    EntityManager::get_instance()->setComponentDataForEntity(TCP_BOX_BACKEND_COMPONENT, box, QVariant::fromValue(static_cast<int>(backend_type::JSONRPC)));

    return box;

}

QStringList JsonRpcDriverUtils::getMethodNamesOfTcpBox(EntityManager::EntityID box)
{
    EntityManager& emngr = ManagerLocator::entityManager();
    QStringList methodnames;
    auto methods = TcpBoxEntityFacorty::getMethodsOfBox(box);
    foreach (auto meth_id, methods) {
        QString tmp = emngr.get_name_component(meth_id);
        methodnames.append(tmp);
    }
    return methodnames;
}

bool JsonRpcDriverUtils::compareMethods(EntityManager::EntityID rhsId, EntityManager::EntityID lhsId)
{
    //EntityManager& emngr = ManagerLocator::entityManager();
    if(ManagerLocator::entityManager().isValid(rhsId) == false)
        return ManagerLocator::entityManager().isValid(lhsId) == false;

    //auto rnme = emngr.get_name_component(rhs);
    //auto lname = emngr.get_name_component(lhs);
    //if(rnme != lname)
    //    return false;
    JsonRpcMethodDescriptor rhs(rhsId);
    JsonRpcMethodDescriptor lhs(lhsId);

    return rhs == lhs;


}


} // namespace talorion

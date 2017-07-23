#ifndef JSONRPCDRIVERUTILS_HPP
#define JSONRPCDRIVERUTILS_HPP

#include <QObject>
#include <QTimer>
#include <QUdpSocket>
#include <core/EntityManager.hpp>


namespace talorion {

class JsonRpcDriverUtils : public QObject
{
    Q_OBJECT
private:
    explicit JsonRpcDriverUtils(QObject *parent = nullptr);
    JsonRpcDriverUtils( const JsonRpcDriverUtils& ) = delete;
    JsonRpcDriverUtils& operator=( const JsonRpcDriverUtils& ) = delete;
    ~JsonRpcDriverUtils() = default;

private:
    static JsonRpcDriverUtils& instance();

public:
    static QStringList discoverHosts(quint16 srvPrt = 2701);
    static QStringList discoverHosts(QHostAddress address, int prefix_len, quint16 srvPrt);
    static bool sendJrpcRequest(QString serverName, quint16 serverPort, const QString& methodname, const QVariant& params, qint32 id);

    static EntityManager::EntityID createNewTcpBoxForHost(QString serverName, quint16 serverPort);

    static QStringList getMethodNamesOfTcpBox(EntityManager::EntityID box);

    static bool compareMethods(EntityManager::EntityID rhsId, EntityManager::EntityID lhsId);

};

} // namespace talorhsIdrion

#endif // JSONRPCDRIVERUTILS_HPP

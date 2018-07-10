#ifndef PVPBACKEND_HPP
#define PVPBACKEND_HPP

#include "PVPRequest.hpp"

#include <tcp_box_system/abstract_backend.hpp>

#include <core/EntityManager.hpp>

namespace talorion {

class PVPParameter;

class PVPBackend: public abstract_backend
{
    Q_OBJECT
public:
    explicit PVPBackend(EntityManager::EntityID box_id, QObject *par = Q_NULLPTR);
    virtual ~PVPBackend();
    Q_DISABLE_COPY(PVPBackend)

public:
    EntityManager::EntityID getBoxId()const{return m_box_id;}

    bool isValid()const{return EntityManager::isValid(getBoxId());}
    QString ip()const{return EntityManager::get_instance()->get_ip_address_component(getBoxId());}

    QSharedPointer<PVPRequest> getDefaultRequest();
    QSharedPointer<PVPRequest> getActRequest();

private:
    const EntityManager::EntityID m_box_id;

    // abstract_backend interface
public slots:
    virtual void processData(QSharedPointer<PVPRequest> request, PVPResponse* response) ;
    virtual void processData(QVariantMap desc, tcpDriverDataTypes::dataType type, int box_id) Q_DECL_OVERRIDE;
    virtual void logError(QString errorString, int source) Q_DECL_OVERRIDE;

    void socketConnected();
    void socketDisconnected();

    virtual void updateAIO(int id, double set, double act, QVariant = QVariant())Q_DECL_OVERRIDE;
    virtual void updateDIO(int id, bool set, bool act, QVariant = QVariant() )Q_DECL_OVERRIDE;

private slots:
    virtual void fcSetChangeProxy(int entity) Q_DECL_OVERRIDE;
    virtual void fcSetChangeProxy(double value, int id) Q_DECL_OVERRIDE;
    virtual void dvSetChangeProxy(int entity) Q_DECL_OVERRIDE;
    virtual void dvSetChangeProxy(int value, int id) Q_DECL_OVERRIDE;

private:
    virtual QString doGetName() const Q_DECL_OVERRIDE;
    virtual backend_type doGetBackendType() const Q_DECL_OVERRIDE;
    virtual QByteArray doGetInfoCommand() const Q_DECL_OVERRIDE;
    virtual QByteArray doGetMinimalSetActCommand() const Q_DECL_OVERRIDE;

    void blockSetUpdate(int id, qint32 requestId);
    void unblockSetUpdate(int id, qint32 requestId);
    void unblockSetUpdate(qint32 requestId);
    bool isSetUpdateBlocked(int id);

private:
    //QList<QSharedPointer<PVPRequest> > m_requestList;
    QList<QSharedPointer<PVPParameter> >::const_iterator m_nextReq;

    QMap<int, double> analog_actbuffer;
    QMap<int, double> analog_setbuffer;
    QMap<int, int> digital_actbuffer;
    QMap<int, int> digital_setbuffer;
    QMap<int, qint32> blockedSetValues;

    QList<QSharedPointer<PVPParameter> > m_parameters;
    int m_setpointIterator;
};

} // namespace talorion

#endif // PVPBACKEND_HPP

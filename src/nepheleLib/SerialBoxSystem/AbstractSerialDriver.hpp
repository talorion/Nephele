#ifndef ABSTRACTSERIALDRIVER_HPP
#define ABSTRACTSERIALDRIVER_HPP

#include <QObject>
#include "core/EntityManager.hpp"

namespace talorion {

class AbstractSerialDriver : public QObject
{
    Q_OBJECT
public:
    explicit AbstractSerialDriver(EntityManager::EntityID box, QObject *par = nullptr);

    EntityManager::EntityID getBoxId() const{return m_boxId;}

    virtual bool connectDevice(QString port, int timeoutMs = 4000){Q_UNUSED(port);Q_UNUSED(timeoutMs);return false;}

  public slots:
    virtual bool connectDevice() = 0;
    virtual void disconectDevice() = 0;

  signals:
    void deviceConnected();
    void deviceDisconnected();

  private:


  private:
    const EntityManager::EntityID m_boxId;
};

} // namespace talorion

#endif // ABSTRACTSERIALDRIVER_HPP

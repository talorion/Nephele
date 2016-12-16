#ifndef TALORION_EVENT_MANAGER_HPP
#define TALORION_EVENT_MANAGER_HPP

#include <QObject>
#include "EntityManager/EntityManager.hpp"

QT_BEGIN_NAMESPACE
class QMutex;
QT_END_NAMESPACE

namespace talorion {

  class EventManager : public QObject
  {
    Q_OBJECT
  public:
    explicit EventManager(QObject *par = Q_NULLPTR);

  signals:
    void changeComponentDataForEntity(EntityManager::ComponentID component_id, EntityManager::EntityID entity_id,  const EntityManager::ComponentData &component_data);

    void allSystemsStarted();

    void newTcpbox(EntityManager::EntityID);
    void deleteTcpbox(EntityManager::EntityID);
    void connectBox(EntityManager::EntityID);
    void disconnectBox(EntityManager::EntityID tcpbox_id);
    void sendCommandToBox(EntityManager::EntityID tcpbox_id, const QString& cmd);

    void boxConnected(EntityManager::EntityID);
    void boxDisconnected(EntityManager::EntityID);

    void boxCommandStarted(EntityManager::EntityID);
    void boxCommandFinished(EntityManager::EntityID);
    void boxCommandError(EntityManager::EntityID);

  };

} // namespace talorion

#endif // TALORION_EVENT_MANAGER_HPP

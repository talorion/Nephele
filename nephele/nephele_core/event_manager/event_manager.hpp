#ifndef TALORION_EVENT_MANAGER_HPP
#define TALORION_EVENT_MANAGER_HPP

#include <QObject>
#include "entity_manager/entity_manager.hpp"

QT_BEGIN_NAMESPACE
class QMutex;
QT_END_NAMESPACE

namespace talorion {

  class event_manager : public QObject
  {
    Q_OBJECT
  public:
    explicit event_manager(QObject *par = Q_NULLPTR);

  signals:
    void change_component_data_for_entity(entity_manager::component_id_t component_id, entity_manager::entity_id_t entity_id,  const entity_manager::component_data_t &component_data);

    void new_tcpbox(entity_manager::entity_id_t);
    void delete_tcpbox(entity_manager::entity_id_t);
    void connect_box(entity_manager::entity_id_t);
    void disconnect_box(entity_manager::entity_id_t tcpbox_id);
    void send_command_to_box(entity_manager::entity_id_t tcpbox_id, const QString& cmd);

    void box_connected(entity_manager::entity_id_t);
    void box_disconnected(entity_manager::entity_id_t);

  };

} // namespace talorion

#endif // TALORION_EVENT_MANAGER_HPP

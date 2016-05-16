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
    void connect_box(entity_manager::entity_id_t);


  };

} // namespace talorion

#endif // TALORION_EVENT_MANAGER_HPP

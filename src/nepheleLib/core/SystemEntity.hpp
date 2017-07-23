#ifndef TALORION_SYSTEMENTITY_HPP
#define TALORION_SYSTEMENTITY_HPP

#include <QList>
#include <QObject>

#include "core/EntityManager.hpp"
#include "core/abstract_system.hpp"

namespace talorion {

class AbstractSystemView;

class SystemEntity : public QObject
{
    Q_OBJECT
public:

public:
    explicit SystemEntity(EntityManager::EntityID systemId, QObject *par = 0);

    SystemViewsContainer systemViews() const;
    abstract_system::ActionsContainer actions() const;
    abstract_system::ToolbarsContainer toolbars() const;

    abstract_configuration_widget* configurationWidget() const;

    QString systemName() const;

    EntityManager::EntityID systemId() const;

private:
    EntityManager& entityManager() const;
    EventManager& eventManager()const;

private:
    const EntityManager::EntityID m_systemId;
};

} // namespace talorion

#endif // TALORION_SYSTEMENTITY_HPP

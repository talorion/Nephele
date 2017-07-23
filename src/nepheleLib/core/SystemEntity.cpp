#include "SystemEntity.hpp"
#include "SystemEntityFactory.hpp"

#include <core/ManagerLocator.hpp>

#include <core/abstract_configuration_widget.hpp>


namespace talorion {

SystemEntity::SystemEntity(EntityManager::EntityID systemId, QObject *par) :
    QObject(par),
    m_systemId(systemId)
{

}

SystemViewsContainer SystemEntity::systemViews() const
{
    auto sysViewsVar = entityManager().componentDataForEntity(SystemEntityFactory::systemViewWidgetComponentId(), systemId());
    if(sysViewsVar.canConvert<SystemViewsContainer>()){
        return sysViewsVar.value<SystemViewsContainer>();
    }
    return SystemViewsContainer();
}

abstract_system::ActionsContainer SystemEntity::actions() const
{
    auto sysViewsVar = entityManager().componentDataForEntity(SystemEntityFactory::systemActionsComponentId(), systemId());
    if(sysViewsVar.canConvert<abstract_system::ActionsContainer>()){
        return sysViewsVar.value<abstract_system::ActionsContainer>();
    }
    return abstract_system::ActionsContainer();
}

abstract_system::ToolbarsContainer SystemEntity::toolbars() const
{
    auto sysViewsVar = entityManager().componentDataForEntity(SystemEntityFactory::systemToolbarsComponentId(), systemId());
    if(sysViewsVar.canConvert<abstract_system::ToolbarsContainer>()){
        return sysViewsVar.value<abstract_system::ToolbarsContainer>();
    }
    return abstract_system::ToolbarsContainer();
}

abstract_configuration_widget *SystemEntity::configurationWidget() const
{
    auto sysViewsVar = entityManager().componentDataForEntity(SystemEntityFactory::systemConfigurationWidgetComponentId(), systemId());
    if(sysViewsVar.canConvert<abstract_configuration_widget *>()){
        return sysViewsVar.value<abstract_configuration_widget *>();
    }
    return Q_NULLPTR;
}

QString SystemEntity::systemName() const
{
    auto sysViewsVar = entityManager().componentDataForEntity(SystemEntityFactory::nameComponentId(), systemId());
    return sysViewsVar.toString();
}

EntityManager &SystemEntity::entityManager() const
{
    return ManagerLocator::entityManager();
}

EventManager &SystemEntity::eventManager() const
{
      return ManagerLocator::eventManager();
}

EntityManager::EntityID SystemEntity::systemId() const
{
    return m_systemId;
}

} // namespace talorion

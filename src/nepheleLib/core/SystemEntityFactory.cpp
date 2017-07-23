#include "SystemEntityFactory.hpp"

#include "core/StaticComponentId.hpp"
#include "core/AbstractSystemView.hpp"
#include "core/abstract_configuration_widget.hpp"
#include "abstract_system.hpp"

#include "core/ManagerLocator.hpp"

namespace talorion {

SystemEntityFactory::SystemEntityFactory(QObject *par) : QObject(par)
{
    //qRegisterMetaType<abstract_system*>("abstract_system*");
    //qRegisterMetaType<QSharedPointer<abstract_system>>("QSharedPointer<abstract_system>");
    //qRegisterMetaType<QSharedPointer<talorion::abstract_system>>("QSharedPointer<talorion::abstract_system>");

}

EntityManager &SystemEntityFactory::entityManager()
{
    return ManagerLocator::entityManager();
}

QUuid SystemEntityFactory::SystemUid()
{
    return ("{e5ff6a18-3514-4ddd-8e86-7f1a3ac134d0}");
}

EntityManager::ComponentID SystemEntityFactory::serialVersionUidComponentId()
{
    return static_component_id::SERIAL_VERSION_UID_COMPONENT;
}

EntityManager::ComponentID SystemEntityFactory::nameComponentId()
{
    return static_component_id::NAME_COMPONENT;
}

EntityManager::ComponentID SystemEntityFactory::systemComponentId()
{
    return static_component_id::SYSTEM_COMPONENT;
}

EntityManager::ComponentID SystemEntityFactory::systemViewWidgetComponentId()
{
    return static_component_id::SYSTEM_VIEW_WIDGET_COMPONENT;
}

EntityManager::ComponentID SystemEntityFactory::systemConfigurationWidgetComponentId()
{
    return static_component_id::SYSTEM_CONFIGURAION_WIDGET_COMPONENT;
}

EntityManager::ComponentID SystemEntityFactory::systemToolbarsComponentId()
{
    return static_component_id::SYSTEM_TOOLBARS_COMPONENT;
}

EntityManager::ComponentID SystemEntityFactory::systemActionsComponentId()
{
    return static_component_id::SYSTEM_ACTIONS_COMPONENT;
}

EntityManager::ComponentID SystemEntityFactory::ignoreWhileSaveingComponentId()
{
    return static_component_id::IGNORE_WHILE_SAVEING_COMPONENT;
}

abstract_configuration_widget *SystemEntityFactory::configWidgetForEntity(const EntityManager &mng, SystemEntity entity) const
{
    auto reg_sys_var  = mng.componentDataForEntity(SystemEntityFactory::systemComponentId(), entity);
    //QSharedPointer<abstract_system> reg_sys  = reg_sys_var.value<QSharedPointer<abstract_system> >();
    //auto reg_sys  = reg_sys_var.value<abstract_system* >();
    auto reg_sys  = reg_sys_var.value<QSharedPointer<abstract_system> >();

    if(!reg_sys)
        return Q_NULLPTR;

    return reg_sys->get_configuration_widget();
}

SystemEntityFactory::SystemEntityContainer SystemEntityFactory::allSystemEntities(const EntityManager &mng)
{
    auto uid= QVariant::fromValue(SystemUid());
    //auto boxes = mng.get_entities_by_components_value(serial_version_uid_component_id(), uid);
    SystemEntityFactory::SystemEntityContainer systems;
    auto all_ents= mng.allEntities();
    foreach (auto entity, all_ents) {
        auto ent_uid = mng.componentDataForEntity(serialVersionUidComponentId(),entity).toUuid();
        if(ent_uid == uid){
            systems<<entity;
        }
    }

    return systems;
}

SystemEntityFactory::SystemEntity SystemEntityFactory::createNewSystemEntity(SystemManager::AbstractSystemPtr sys)
{
    EntityManager& mng =  entityManager();
    auto sys_ent        = mng.createNewEntity();

    mng.createComponentAndAddTo(nameComponentId(), sys_ent);
    mng.createComponentAndAddTo(systemComponentId(), sys_ent);
    mng.createComponentAndAddTo(serialVersionUidComponentId(), sys_ent);
    mng.createComponentAndAddTo(ignoreWhileSaveingComponentId(), sys_ent);
    mng.createComponentAndAddTo(systemViewWidgetComponentId(), sys_ent);
    mng.createComponentAndAddTo(systemToolbarsComponentId(), sys_ent);
    mng.createComponentAndAddTo(systemActionsComponentId(), sys_ent);
    mng.createComponentAndAddTo(systemConfigurationWidgetComponentId(), sys_ent);

    mng.setComponentDataForEntity(nameComponentId(),                        sys_ent, sys->get_system_name());
    auto sys_co = QVariant::fromValue(sys);
    mng.setComponentDataForEntity(systemComponentId(),          sys_ent, sys_co);
    auto uid= QVariant::fromValue(SystemUid());
    mng.setComponentDataForEntity(serialVersionUidComponentId(), sys_ent, uid);
    mng.setComponentDataForEntity(ignoreWhileSaveingComponentId(), sys_ent, true);
    auto sysViews = QVariant::fromValue(sys->systemViews());
    mng.setComponentDataForEntity(systemViewWidgetComponentId(), sys_ent, sysViews);
    auto toolb = QVariant::fromValue(sys->toolbars());
    mng.setComponentDataForEntity(systemToolbarsComponentId(), sys_ent, toolb);
    auto act = QVariant::fromValue(sys->actions());
    mng.setComponentDataForEntity(systemActionsComponentId(), sys_ent, act);
    auto cfgwdgt = QVariant::fromValue(sys->get_configuration_widget());
    mng.setComponentDataForEntity(systemConfigurationWidgetComponentId(), sys_ent, cfgwdgt);

    return sys_ent;
}

SystemEntityFactory &SystemEntityFactory::instance()
{
    // Since it's a static variable, if the class has already been created,
    // It won't be created again.
    // And it **is** thread-safe in C++11.
    static SystemEntityFactory myInstance;

    // Return a reference to our instance.
    return myInstance;
}


} // namespace talorion

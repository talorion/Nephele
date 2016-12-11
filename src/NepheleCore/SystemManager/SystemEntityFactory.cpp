#include "SystemEntityFactory.hpp"

#include "EntityManager/StaticComponentId.hpp"
#include "SystemView/AbstractSystemView.hpp"
#include "ConfigurationWidget/AbstractConfigurationWidget.hpp"
#include "AbstractSystem.hpp"

#include <ManagerLocator.hpp>

namespace talorion {

  SystemEntityFactory::SystemEntityFactory(QObject *par) : QObject(par)
  {

  }

  EntityManager &SystemEntityFactory::entityManager() const
  {
    return ManagerLocator::entityManager();
  }

  QUuid SystemEntityFactory::SystemUid() const
  {
    return ("{e5ff6a18-3514-4ddd-8e86-7f1a3ac134d0}");
  }

  EntityManager::ComponentID SystemEntityFactory::serialVersionUidComponentId()
  {
    return StaticComponentId::SERIAL_VERSION_UID_COMPONENT;
  }

  EntityManager::ComponentID SystemEntityFactory::nameComponentId()
  {
    return StaticComponentId::NAME_COMPONENT;
  }

  EntityManager::ComponentID SystemEntityFactory::systemComponentId()
  {
    return StaticComponentId::SYSTEM_COMPONENT;
  }

  EntityManager::ComponentID SystemEntityFactory::systemViewWidgetComponentId()
  {
    return StaticComponentId::SYSTEM_VIEW_WIDGET_COMPONENT;
  }

  EntityManager::ComponentID SystemEntityFactory::systemConfigurationWidgetComponentId()
  {
    return StaticComponentId::SYSTEM_CONFIGURAION_WIDGET_COMPONENT;
  }

  EntityManager::ComponentID SystemEntityFactory::ignoreWhileSaveingComponentId()
  {
    return StaticComponentId::IGNORE_WHILE_SAVEING_COMPONENT;
  }

  AbstractConfigurationWidget *SystemEntityFactory::configWidgetForEntity(const EntityManager &mng, SystemEntity entity) const
  {
    auto reg_sys_var  = mng.componentDataForEntity(SystemEntityFactory::systemComponentId(), entity);
    QSharedPointer<AbstractSystem> reg_sys  = reg_sys_var.value<QSharedPointer<AbstractSystem> >();

    if(!reg_sys)
      return Q_NULLPTR;

    return reg_sys->configurationWidget();
  }

  SystemEntityFactory::SystemEntityContainer SystemEntityFactory::allSystemEntities(const EntityManager &mng) const
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

    mng.setComponentDataForEntity(nameComponentId(),                        sys_ent, sys->systemName());
    auto sys_co = QVariant::fromValue(sys);
    mng.setComponentDataForEntity(systemComponentId(),          sys_ent, sys_co);
    auto uid= QVariant::fromValue(SystemUid());
    mng.setComponentDataForEntity(serialVersionUidComponentId(), sys_ent, uid);
    mng.setComponentDataForEntity(ignoreWhileSaveingComponentId(), sys_ent, true);

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

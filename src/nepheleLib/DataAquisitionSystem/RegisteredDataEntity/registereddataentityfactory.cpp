#include "registereddataentityfactory.hpp"

namespace talorion {

  RegisteredDataEntityFactory::RegisteredDataEntityFactory()
  {

  }

  EntityManager::EntityList RegisteredDataEntityFactory::getAllRegisteredDataEntites()
  {
    return entMng().get_entity_by_serialVersionUID(registeredDataEntityUid());
  }

  EntityManager::EntityList RegisteredDataEntityFactory::getAllRegisteredSourceEntities()
  {
    return entMng().get_entity_by_serialVersionUID(registeredSourceEntityUid());
  }

  EntityManager::EntityID RegisteredDataEntityFactory::createNewRegisteredDataEntity(QString name, double analogActValue, EntityManager::EntityID parent)
  {
    auto data = registeredDataEntityExists(name, parent);
    if(EntityManager::isValid(data))
      return data;

    data = entMng().createNewEntity();
    entMng().createComponentAndAddTo( nameComponentId(), data );
    entMng().createComponentAndAddTo( analogActValueComponentId(), data );
    entMng().createComponentAndAddTo( serialVersionUidComponentId(), data );

    entMng().setComponentDataForEntity(nameComponentId(),               data, name);
    entMng().setComponentDataForEntity(analogActValueComponentId(),     data, analogActValue);
    entMng().setComponentDataForEntity(serialVersionUidComponentId(),   data, registeredDataEntityUid());

    if(EntityManager::isValid(parent))
      entMng().addChild(parent, data);

    emit evtMng().newRegisteredDataEntity(data);
    return data;
  }

  EntityManager::EntityID RegisteredDataEntityFactory::createNewRegisteredSourceEntity(QString name)
  {
    auto source = registeredSourceEntityExists(name);
    if(EntityManager::isValid(source))
      return source;

    source = entMng().createNewEntity();
    entMng().createComponentAndAddTo( nameComponentId(), source );
    entMng().createComponentAndAddTo( serialVersionUidComponentId(), source );

    entMng().setComponentDataForEntity(nameComponentId(),               source, name);
    entMng().setComponentDataForEntity(serialVersionUidComponentId(),   source, registeredSourceEntityUid());

    emit evtMng().newRegisteredSourceEntity(source);
    return source;
  }

  EntityManager::EntityID RegisteredDataEntityFactory::createNewRegisteredSourceEntity(const UserDataSource &src)
  {
    auto source = createNewRegisteredSourceEntity(src.location());
    int i=0;
    foreach (auto desc, src.descriptions()) {
        double val = src.data().at(i);
        createNewRegisteredDataEntity(desc, val, source);
        i++;
      }
    return source;
  }

  EntityManager::EntityID RegisteredDataEntityFactory::registeredDataEntityExists(QString dataname, EntityManager::EntityID dataparent)
  {
    auto sources = getAllRegisteredDataEntites();
    foreach (auto data, sources) {
        auto name   = entMng().componentDataForEntity(nameComponentId(), data);
        auto parent = entMng().getParent(data);
        if(dataname == name && dataparent == parent)
          return data;
      }
    return EntityManager::invalid_id;
  }

  EntityManager::EntityID RegisteredDataEntityFactory::registeredSourceEntityExists(QString sourcename)
  {
    auto sources = getAllRegisteredSourceEntities();
    foreach (auto source, sources) {
        auto name = entMng().componentDataForEntity(nameComponentId(), source);
        if(sourcename == name)
          return source;
      }
    return EntityManager::invalid_id;
  }

} // namespace talorion

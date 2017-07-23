#ifndef TALORION_SYSTEM_ENTITY_FACTORY_HPP
#define TALORION_SYSTEM_ENTITY_FACTORY_HPP

#include <QObject>
#include <QUuid>

#include "EntityManager.hpp"
#include "SystemManager.hpp"

namespace talorion {

class abstract_configuration_widget;


class SystemEntityFactory : public QObject
{
    Q_OBJECT
public:
    using SystemEntity = EntityManager::EntityID;
    using SystemEntityContainer = QList<SystemEntity>;

private:
    explicit SystemEntityFactory(QObject *par = Q_NULLPTR);
    virtual ~SystemEntityFactory()=default;

public:
    static EntityManager &entityManager();
    static SystemEntityContainer allSystemEntities(const talorion::EntityManager &mng);

    static QUuid SystemUid();

    static EntityManager::ComponentID serialVersionUidComponentId();
    static EntityManager::ComponentID nameComponentId();
    static EntityManager::ComponentID systemComponentId();
    static EntityManager::ComponentID systemViewWidgetComponentId();
    static EntityManager::ComponentID systemConfigurationWidgetComponentId();
    static EntityManager::ComponentID systemToolbarsComponentId();
    static EntityManager::ComponentID systemActionsComponentId();
    static EntityManager::ComponentID ignoreWhileSaveingComponentId();

public:
    abstract_configuration_widget* configWidgetForEntity(const talorion::EntityManager &mng, SystemEntity entity)const ;

    SystemEntity createNewSystemEntity(SystemManager::AbstractSystemPtr sys);

public:
    static SystemEntityFactory& instance();// Move assign

};

} // namespace talorion

#endif // TALORION_SYSTEM_ENTITY_FACTORY_HPP

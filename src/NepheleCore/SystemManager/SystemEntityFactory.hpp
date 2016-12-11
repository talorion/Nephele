#ifndef TALORION_SYSTEM_ENTITY_FACTORY_HPP
#define TALORION_SYSTEM_ENTITY_FACTORY_HPP

#include <QObject>
#include <QUuid>

#include "EntityManager/EntityManager.hpp"
#include "SystemManager.hpp"

namespace talorion {

class AbstractConfigurationWidget;


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
    EntityManager &entityManager()const;

    QUuid SystemUid()const ;

    static EntityManager::ComponentID serialVersionUidComponentId();
    static EntityManager::ComponentID nameComponentId();
    static EntityManager::ComponentID systemComponentId();
    static EntityManager::ComponentID systemViewWidgetComponentId();
    static EntityManager::ComponentID systemConfigurationWidgetComponentId();
    static EntityManager::ComponentID ignoreWhileSaveingComponentId();

public:
    AbstractConfigurationWidget* configWidgetForEntity(const talorion::EntityManager &mng, SystemEntity entity)const ;

    SystemEntityContainer allSystemEntities(const talorion::EntityManager &mng)const;

    SystemEntity createNewSystemEntity(SystemManager::AbstractSystemPtr sys);

public:
    static SystemEntityFactory& instance();// Move assign

};

} // namespace talorion

#endif // TALORION_SYSTEM_ENTITY_FACTORY_HPP

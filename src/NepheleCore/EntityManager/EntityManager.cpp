#include "EntityManager.hpp"

#include "EventManager/EventManager.hpp"

#include "EntityManagerDb.hpp"

#include <QDebug>
#include <QUuid>

#include "StaticComponentId.hpp"

namespace talorion {

const EntityManager::ComponentData EntityManager::invalid_data= QVariant();//Constructs an invalid variant.
QVector<double> EntityManager::invalid_doubleVector_data = QVector<double>();
EntityManager::EntityList EntityManager::invalid_entityList_data = EntityManager::EntityList();
bool EntityManager::isValid(const EntityManager::ComponentData &component_data){return component_data.isValid();}

EntityManager::EntityManager(QObject *par) :
    QObject(par),
    m_db_ptr(new EntityManagerDb()),
    m_evt_mng(),
    m_exclusive_components({CHILDREN_COMPONENT, PARENT_COMPONENT})
{
    m_evt_mng.clear();

//    m_exclusive_components.clear();
//    m_exclusive_components<<CHILDREN_COMPONENT;
//    m_exclusive_components<<PARENT_COMPONENT;

}

EntityManager::~EntityManager(){

}

QList<EntityManager::EntityID> EntityManager::allEntities() const
{
    return m_db_ptr->entities().keys();
}

QList<EntityManager::ComponentID> EntityManager::allComponents() const
{
    return m_db_ptr->components().keys();
}

EntityManager::EntityID EntityManager::createNewEntity()
{
    EntityID new_id = nextEntityId();
    return createNewEntity(new_id);
}

EntityManager::EntityID EntityManager::createNewEntity(EntityID new_id)
{
    if(entityExists(new_id))
        new_id = nextEntityId();

    m_db_ptr->addEntity(new_id, QString());
    return new_id;
}

bool EntityManager::entityExists(EntityID entity_id) const{
    return m_db_ptr->entities().contains(entity_id);
}


EntityManager::ComponentID EntityManager::createNewComponent(ComponentID component_id, QString component_name)
{
    if(!isValid(component_id)){
        component_id = nextComponentId();
    }

    if(!component_name.isNull() && componentExists(component_name)){
        component_id= getComponentByName(component_name);
    }

//    if(componentIsExclusive(component_id)){

//    }

    if(!componentExists(component_id)){
        m_db_ptr->addComponent(component_id, component_name);
    }

    return component_id;
}

EntityManager::ComponentID EntityManager::createNewComponent(const QString &component_name)
{
    return createNewComponent(invalid_id, component_name);
}

EntityManager::ComponentID EntityManager::createComponentAndAddTo(ComponentID component_id, EntityManager::EntityID entity_id)
{
    return createComponentAndAddTo(component_id, entity_id, false);
}

EntityManager::ComponentID EntityManager::getComponentByName(const QString &component_name) const
{
    auto component_id =invalid_id;

    for(auto it=m_db_ptr->components().cbegin(); it!=m_db_ptr->components().cend();++it){
        if(QString::compare(it.value(),component_name, Qt::CaseInsensitive)==0){
            component_id = it.key();
            break;
        }
    }

    return component_id;
}

bool EntityManager::componentExists(EntityManager::ComponentID component_id) const
{
    return m_db_ptr->components().contains(component_id);
}

bool EntityManager::componentExists(QString component_name) const
{
    auto component_id = getComponentByName(component_name);
    return componentExists(component_id);
}

bool EntityManager::entityHasComponent(EntityManager::EntityID entity_id, EntityManager::ComponentID component_id) const
{
    ComponentDataID component_data_id = getcomponentDataId(entity_id,component_id);
    return m_db_ptr->componentDataTable().contains(component_data_id);
}

bool EntityManager::entiyHasChildren(EntityManager::EntityID entity_id)
{
    return entityHasComponent(entity_id, CHILDREN_COMPONENT);
}

bool EntityManager::entiyHasParent(EntityManager::EntityID entity_id)
{
    return entityHasComponent(entity_id, PARENT_COMPONENT);
}

void EntityManager::addChildren(EntityManager::EntityID entity_id, EntityList &children)
{
    foreach (auto child, children) {
        addChild(entity_id, child);
    }
}

void EntityManager::addChild(EntityManager::EntityID entity_id, EntityManager::EntityID &child)
{
    if(!entiyHasChildren(entity_id)){
        EntityList* children = new EntityList();
        createComponentAndAddTo(CHILDREN_COMPONENT, entity_id, true);
        setComponentDataForEntity(CHILDREN_COMPONENT, entity_id, children, true);
    }

    EntityList& act_children = getChildren(entity_id);
    act_children.append(child);
    setComponentDataForEntity(CHILDREN_COMPONENT, entity_id, act_children);
    setParent(child, entity_id);

}

void EntityManager::removeChildren(EntityManager::EntityID entity_id, EntityList children)
{
    if(entiyHasChildren(entity_id)){
        EntityList& act_children = getChildren(entity_id);
        foreach (auto child, children) {
            removeParent(child);
            act_children.removeAll(child);
        }
        setComponentDataForEntity(CHILDREN_COMPONENT, entity_id, act_children);
    }
}

void EntityManager::removeChild(EntityManager::EntityID parent_entity_id, EntityManager::EntityID child_entity_id)
{
    if(entiyHasChildren(parent_entity_id)){
        EntityList& act_children = getChildren(parent_entity_id);
        if(act_children.removeAll(child_entity_id)>0)
            removeParent(child_entity_id);
        setComponentDataForEntity(CHILDREN_COMPONENT, parent_entity_id, act_children, true);
    }
}

EntityManager::EntityList &EntityManager::getChildren(EntityManager::EntityID entity_id)
{
    return entityListDataForEntity(CHILDREN_COMPONENT, entity_id);
}

void EntityManager::setParent(EntityManager::EntityID child_entity_id, EntityManager::EntityID parent_entity_id)
{
    if(entiyHasParent(child_entity_id)){
        removeChild(parent_entity_id, child_entity_id);
    }
    createComponentAndAddTo(PARENT_COMPONENT, child_entity_id, true);
    auto par=QVariant::fromValue(parent_entity_id);
    setComponentDataForEntity(PARENT_COMPONENT, child_entity_id, par, true);
}

void EntityManager::removeParent(EntityManager::EntityID child_entity_id)
{
    if(entiyHasParent(child_entity_id)){
        auto ent_parent= getParent(child_entity_id);
        removeComponentFromEntity(PARENT_COMPONENT, child_entity_id);
        removeChild(ent_parent, child_entity_id);
    }
}

EntityManager::EntityID EntityManager::getParent(EntityManager::EntityID child_entity_id)
{
    auto parvar= componentDataForEntity(PARENT_COMPONENT, child_entity_id);
    EntityManager::EntityID ret = invalid_id;
    if(parvar.canConvert<EntityID>())
        ret = parvar.value<EntityID>();
    return ret;
}

void EntityManager::removeEntity(EntityManager::EntityID entity_id)
{
    removeParent(entity_id);

    if(entiyHasChildren(entity_id)){
        EntityList tmplist(getChildren(entity_id));
        foreach (auto child, tmplist) {
            removeEntity(child);
        }
    }

    if(entityExists(entity_id)){
        removeAllComponentsFromEntity(entity_id);
        m_db_ptr->deleteEntity(entity_id);
    }
}

void EntityManager::removeAllComponentsFromEntity(EntityManager::EntityID entity_id)
{
    QList<ComponentID> component_ids = m_db_ptr->entityComponents().values(entity_id);
    for(auto component_id : component_ids){
        //foreach (auto component_id, component_ids) {
        removeComponentFromEntity(component_id, entity_id);
    }
}

void EntityManager::removeComponentFromEntity(EntityManager::ComponentID component_id, EntityManager::EntityID entity_id)
{
    ComponentDataID component_data_id = getcomponentDataId(entity_id,component_id);
    m_db_ptr->deleteComponentFromEntity(entity_id, component_data_id);
}

QList<EntityManager::ComponentID> EntityManager::allComponentsOfEntity(EntityManager::EntityID entity_id) const
{
    return m_db_ptr->entityComponents().values(entity_id);
}

QVector<double> &EntityManager::doubleVectorDataForEntity(EntityManager::ComponentID component_id, EntityManager::EntityID entity_id) const
{
    ComponentDataID component_data_id = getcomponentDataId(entity_id,component_id);
    auto ret = m_db_ptr->componentDoubleVectorDataTable().value(component_data_id, Q_NULLPTR);
    if(ret == Q_NULLPTR)
        return invalid_doubleVector_data;
    return *ret;
}

EntityManager::EntityList &EntityManager::entityListDataForEntity(EntityManager::ComponentID component_id, EntityManager::EntityID entity_id) const
{
    ComponentDataID component_data_id = getcomponentDataId(entity_id,component_id);
    auto ret = m_db_ptr->componentEntityListDataTable().value(component_data_id, Q_NULLPTR);
    if(ret == Q_NULLPTR)
        return invalid_entityList_data;
    return *ret;
}

void EntityManager::setComponentDataForEntity(EntityManager::ComponentID component_id, EntityManager::EntityID entity_id, ComponentData &component_data)
{
    setComponentDataForEntity(component_id, entity_id, component_data, false);
}

void EntityManager::setComponentDataForEntity(EntityManager::ComponentID component_id, EntityManager::EntityID entity_id, const ComponentData &component_data)
{
    setComponentDataForEntity(component_id, entity_id, component_data, false);
}

void EntityManager::setComponentDataForEntity(EntityManager::ComponentID component_id, EntityManager::EntityID entity_id, QVector<double> &component_data)
{
    if(componentIsExclusive(component_id))
        return;
    ComponentDataID component_data_id = getcomponentDataId(entity_id,component_id);
    m_db_ptr->setData(component_data_id, &component_data);
}

void EntityManager::setComponentDataForEntity(EntityManager::ComponentID component_id, EntityManager::EntityID entity_id, QVector<double> *component_data)
{
    if(componentIsExclusive(component_id))
        return;
    ComponentDataID component_data_id = getcomponentDataId(entity_id,component_id);
    m_db_ptr->setData(component_data_id, component_data);
}

void EntityManager::setComponentDataForEntity(EntityManager::ComponentID component_id, EntityManager::EntityID entity_id, EntityManager::EntityList &component_data)
{
    setComponentDataForEntity(component_id, entity_id, component_data, false);
}

void EntityManager::setComponentDataForEntity(EntityManager::ComponentID component_id, EntityManager::EntityID entity_id, EntityList *component_data)
{
    setComponentDataForEntity(component_id, entity_id, component_data, false);
}

bool EntityManager::isConnectedToEventManager() const
{
    return !(m_evt_mng.isNull());
}

void EntityManager::connectToEventManager(EventManager *evt_mng)
{
    //    if(evt_mng==Q_NULLPTR){
    //      m_evt_mng.clear();
    //      return;
    //      }

    if(isConnectedToEventManager())
        return;

    m_evt_mng = evt_mng;

    connect(m_evt_mng,  SIGNAL(changeComponentDataForEntity(EntityManager::ComponentID,EntityManager::EntityID,EntityManager::ComponentData)),
            this,       SLOT(slotChangeComponentDataForEntity(  EntityManager::ComponentID,EntityManager::EntityID,EntityManager::ComponentData)));

}

EventManager *EntityManager::eventManager() const
{
    if(!isConnectedToEventManager())
        return Q_NULLPTR;

    return m_evt_mng.data();
}

EntityManager::EntityList EntityManager::entitiesByComponentsValue(EntityManager::ComponentID component_id,  EntityManager::ComponentData component_data) const
{
    QList<EntityManager::EntityID> ents;
    auto tmp_cont=allEntities();
    foreach (EntityID id, tmp_cont) {
        if(entityHasComponent(id, component_id)){
            auto data = componentDataForEntity(id,component_id);
            if(data == component_data){
                ents<<id;
            }
        }
    }

    return ents;
}

EntityManager::ComponentData EntityManager::componentDataForEntity(EntityManager::ComponentID component_id, EntityManager::EntityID entity_id) const
{
    ComponentDataID component_data_id = getcomponentDataId(entity_id,component_id);
    auto data = m_db_ptr->componentDataTable().value(component_data_id, invalid_data);
    return data;
}

EntityManager::ID EntityManager::nextIdFrom(const QList<ID> &container) const
{
    ID next_id = invalid_id;
    //for(auto e : container)     {next_id = qMax(next_id, e);}
    foreach (auto e, container){next_id = qMax(next_id, e);}
    next_id++;
    return next_id;
}

EntityManager::EntityID EntityManager::nextEntityId() const
{
    ID next_id = nextIdFrom(m_db_ptr->entities().keys());
    if(next_id>max_entity_id)
        next_id = invalid_id;
    return next_id;
}

EntityManager::ComponentID EntityManager::nextComponentId() const
{
    ID next_id = nextIdFrom(m_db_ptr->components().keys());
    if(next_id>max_component_id)
        next_id = invalid_id;
    return next_id;
}

bool EntityManager::componentIsExclusive(EntityManager::ComponentID component_id) const
{
    return m_exclusive_components.contains(component_id);
}

EntityManager::ComponentID EntityManager::createComponentAndAddTo(EntityManager::ComponentID component_id, EntityManager::EntityID entity_id, bool ignore_exclusive)
{
    if(!ignore_exclusive && componentIsExclusive(component_id))
        return invalid_id;

    component_id =createNewComponent(component_id);  //make sure we have a valid component
    ComponentDataID component_data_id = getcomponentDataId(entity_id,component_id);

    m_db_ptr->addComponentToEntity(entity_id, component_id, component_data_id, invalid_data);
    return component_id;
}

void EntityManager::setComponentDataForEntity(EntityManager::ComponentID component_id, EntityManager::EntityID entity_id, const EntityManager::ComponentData &component_data, bool ignore_exclusive)
{
    if(!ignore_exclusive && componentIsExclusive(component_id))
        return;

    ComponentDataID component_data_id = getcomponentDataId(entity_id,component_id);
    m_db_ptr->setData(component_data_id, component_data);
}

void EntityManager::setComponentDataForEntity(EntityManager::ComponentID component_id, EntityManager::EntityID entity_id, EntityManager::EntityList &component_data, bool ignore_exclusive)
{
    if(!ignore_exclusive && componentIsExclusive(component_id))
        return;
    ComponentDataID component_data_id = getcomponentDataId(entity_id,component_id);
    m_db_ptr->setData(component_data_id, &component_data);
}

void EntityManager::setComponentDataForEntity(EntityManager::ComponentID component_id, EntityManager::EntityID entity_id, EntityManager::EntityList *component_data, bool ignore_exclusive)
{
    if(!ignore_exclusive && componentIsExclusive(component_id))
        return;
    ComponentDataID component_data_id = getcomponentDataId(entity_id,component_id);
    m_db_ptr->setData(component_data_id, component_data);
}

void EntityManager::slotChangeComponentDataForEntity(ComponentID component_id, EntityID entity_id, const ComponentData &component_data)
{
    setComponentDataForEntity(component_id, entity_id, component_data);
}

} // namespace talorion


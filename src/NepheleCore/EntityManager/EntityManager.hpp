#ifndef TALORION_ENTITY_MANAGER_HPP
#define TALORION_ENTITY_MANAGER_HPP

#include <QObject>
#include <QScopedPointer>
#include <QPointer>
#include <QVariant>


namespace talorion {

class EntityManagerDb;
class EventManager;

/*!
      \class EntityManager
      \brief The EntityManager class represents a manageing interface for Entities and Components in a .
      \since 0.0.1
      \ingroup core

      # Terminology
      - Entity:   The entity is a general purpose object. Usually, it only consists of a unique id.
                They "tag every coarse object as a separate item".For every discernible “thing” in your application, you have one Entity.
                Entities have no data and no methods.
      - Component: the raw data for one aspect of the object, and how it interacts with the world. "Labels the Entity as possessing this particular aspect".

      # External resources
      - [Wikipedia](https://en.wikipedia.org/wiki/Entity_component_system)
      - [Entity Systems are the future of MMOG development – Part 1](http://t-machine.org/index.php/2007/09/03/entity-systems-are-the-future-of-mmog-development-part-1/)
      - [Entity Systems are the future of MMOG development – Part 2](http://t-machine.org/index.php/2007/11/11/entity-systems-are-the-future-of-mmog-development-part-2/)
      - [Entity Systems are the future of MMOG development – Part 3](http://t-machine.org/index.php/2007/12/22/entity-systems-are-the-future-of-mmog-development-part-3/)
      - [Entity Systems are the future of MMOG development – Part 4](http://t-machine.org/index.php/2008/03/13/entity-systems-are-the-future-of-mmos-part-4/)
      - [Entity Systems are the future of MMOG development – Part 5](http://t-machine.org/index.php/2009/10/26/entity-systems-are-the-future-of-mmos-part-5/)
      - [Component-Based Entity Systems in Spielen](http://heise.de/-2262126)

      An EntityManager Should manage the ceation and destruction of Entities and Comonents. it also is responsible for adding Components to Entities, setting data of components, and accessing data of components.
      \sa EntityManagerDb
  */
class EntityManager : public QObject
{
    Q_OBJECT
public:     //typedefs
    /*!
          \typedef EntityManager::ID

          This is a typedef for a unique id.

          Synonym for int.
      */
    using ID=int;
    /*!
          \typedef EntityManager::EntityID

          This is a typedef for a Entity.

          Synonym for EntityManager::ID.
      */
    using EntityID=ID ;
    /*!
          \typedef EntityManager::ComponentID

          This is a typedef for a Component.

          Synonym for EntityManager::ID.
      */
    using ComponentID=ID ;
    /*!
        \typedef ComponentDataID

        This is a typedef for a ling between a Component and its entity

        Synonym for EntityManager::ID.
    */
    using ComponentDataID=ID ;
    /*!
        This is a typedef for the actual representation of a single components data

        Synonym for QVariant.
    */
    using ComponentData=QVariant;
    /*!
        This is a typedef for a List of Entities

        Synonym for QList<EntityManager::EntityID>.
    */
    using EntityList=QList<EntityID>;

public:     //constants
    /*!
        \brief Represents an Invalid Id

        if an Id is invalid it was either never created or destoyed. so Checking for Valitity makes shure the entity is Managed by the EntityManager

        \sa ID, isValid
    */
    static const ID invalid_id=-1;

    /*!
        \brief Represents the maimum number of entites without collisions when assigning a component to it

        when calculating a EntityManager::ComponentID EntityManager::max_entity_id is used to avoid collisions

        for a given EntityManager::ComponentID and a EntityManager::EntityID the EntityManager::ComponentDataID is calculated like:
        (component_id*max_entity_id + entity_id)*max_component_id

        EntityManager::max_entity_id must be prime

        \sa EntityID, ComponentID,  ComponentDataID, getcomponentDataId
    */
    static const EntityID max_entity_id=997;    //for component_data_id hash should ba a prime
    /*!
        \brief Represents the maimum number of Components assignable to a Entity

        when calculating a EntityManager::ComponentID EntityManager::max_entity_id is used to avoid collisions

        for a given EntityManager::ComponentID and a EntityManager::EntityID the EntityManager::ComponentDataID is calculated like:
        (component_id*max_entity_id + entity_id)*max_component_id

        EntityManager::max_component_id must be prime

        \sa EntityID, ComponentID,  ComponentDataID
    */
    static const EntityID max_component_id=1009; //for component_data_id hash should ba a prime

    /*!
        \brief Represents a EntityManager::ComponentData which marks invalid data

        invalid data may be returned when accessing data of a invalid EntityManager::ComponentID or EntityManager::EntityID

        \sa ComponentData, componentDataForEntity, isValid
    */
    static const ComponentData invalid_data;
    /*!
        \brief Represents a QVector<double> which marks invalid data

        invalid data may be returned when accessing data of a invalid EntityManager::ComponentID or EntityManager::EntityID
        since QVector<double> data is stored in a different manner to keep the raw pointers without copying the invalid datapoint for QVector<double>
        is also different.

        \sa doubleVectorDataForEntity()
    */
    static QVector<double> invalid_doubleVector_data;
    /*!
        \brief Represents a EntityManager::EntityList which marks invalid data

        invalid data may be returned when accessing data of a invalid EntityManager::ComponentID or EntityManager::EntityID
        since EntityManager::EntityList data is stored in a different manner so invalid data for EntityManager::EntityList is also different.

        \sa EntityList ,doubleVectorDataForEntity
    */
    static EntityList invalid_entityList_data;

public:     //convenient fnctions
    /*!
        Returns \c true if the given EntityManager::ID is Valid; otherwise returns \c false.

        \sa invalid_id
    */
    static bool isValid(ID id){return id > invalid_id;}
    /*!
        Returns a EntityManager::ComponentDataID for a given EntityManager::EntityID and EntityManager::EntityID

        for a given EntityManager::ComponentID and a EntityManager::EntityID the EntityManager::ComponentDataID is calculated like:
        (component_id*max_entity_id + entity_id)*max_component_id

        \sa EntityID, ComponentID,  ComponentDataID, max_entity_id, max_component_id
    */
    static ComponentDataID getcomponentDataId(EntityID entity_id, ComponentID component_id){return (component_id*max_entity_id + entity_id)*max_component_id;}
    /*!
        Returns \c true if the given EntityManager::ComponentData is Valid; otherwise returns \c false.

        \sa invalid_data
    */
    static bool isValid(const ComponentData& component_data);

public:

    /*!
     \brief Constructs an EntityManager with parent object par.

     The parent of an EntityManager may be viewed as the object's owner.

     Setting parent to 0 constructs an EntityManager with no parent.
     after creation an EntityManager has 0 Entities and Components

     \param par
    */
    explicit EntityManager(QObject *par = Q_NULLPTR);

    /*!
     \brief Destroys the EntityManager, deleting all its Entities and Components.

     \param par
    */
    virtual ~EntityManager();

public:

    /*!
     \brief returns a EntityList of all Entities the EntityManager contains.

     All Entities created by the EntityManager which are not removed should be returned.

     \return EntityList
    */
    EntityList allEntities()const;

    /*!
     \brief returns a QList<ComponentID> of all Components the EntityManager contains.

     All Components created by the EntityManager which are not removed should be returned.

     \return EntityList
    */
    QList<ComponentID> allComponents() const;

    /*!
     \brief creates a new Entity

     always results in a valid entity which exists within the EntityManager

     \return EntityID
    */
    EntityID createNewEntity();
    /*!
     \brief creates a new Entity

     \param new_id if new_id is valid and does not exist a new entity with new_id as EntityID is created, otherwise a new EntityID is generated
     \return EntityID
    */
    EntityID createNewEntity(EntityID new_id);
    /*!
     \brief checks if a given EntityID is already created by the EntityManager

     \param entity_id
     \return bool
    */
    bool entityExists(EntityID entity_id) const;

    /*!
     \brief creates a new component

     \param component_id if component_id is valid and does not exist a new component with component_id as ComponentID is created, otherwise a new ComponentID is generated.
     \param component_name is a short description of the Component
     \return ComponentID
    */
    ComponentID createNewComponent(ComponentID component_id=invalid_id, QString component_name=QString());
    /*!
     \brief creates a new component

     \param component_name is a short description of the Component
     \return ComponentID
    */
    ComponentID createNewComponent(const QString& component_name);
    /*!
     \brief creates a component and adds it to a given entity

     there are a few exclusive Components which can only be added to Entities by specific functions like Children or Parent component
     these components cannot be added to Entities by external clienets

     \param component_id Component which should be crated
     \param entity_id Entity which the Component should be added to
     \return ComponentID
    */
    ComponentID createComponentAndAddTo(ComponentID component_id, EntityID entity_id);
    /*!
     \brief returns a component by its name

     when a Component is created with a valid component name it can be returned by this function
     if no component with given component_name a invalid_id is returned

     \param component_name
     \return ComponentID
    */
    ComponentID getComponentByName(const QString& component_name)const;

    /*!
     \brief returns \c true if a given ComponentID exists in the EntityManager otherwise returns \c false

     \param component_id
     \return bool
    */
    bool componentExists(ComponentID component_id) const;
    /*!
     \brief returns \c true if a ComponentID with a given name exists in the EntityManager otherwise returns \c false

     \param component_name
     \return bool
    */
    bool componentExists(QString component_name) const;
    /*!
     \brief returns \c true if a Entity has a given Component otherwise returns \c false
     usually a component is added to an Entity by createComponentAndAddTo function

     \param entity_id
     \param component_id
     \return bool
    */
    bool entityHasComponent(EntityID entity_id, ComponentID component_id) const;
    /*!
     \brief returns \c true if a Entity has a Children component otherwise return \c false

     \param entity_id
     \return bool
    */
    bool entiyHasChildren(EntityID entity_id);
    /*!
     \brief returns \c true if a Entity has a Parent component otherwise return \c false

     \param entity_id
     \return bool
    */
    bool entiyHasParent(EntityID entity_id);


    /*!
     \brief adds a given EntityList as Children to a given Entity

     if the Entity has no Children a Children Component is Created and added to entity_id
     for each child the Parent component is set to entity_id

     \param entity_id
     \param children
    */
    void addChildren(EntityID entity_id, EntityList &children);
    /*!
     \brief adds a given Entity as Child to a given Entity

     if the Entity has no Children a Children Component is Created and added to entity_id
     for the child the Parent component is set to entity_id

     \param entity_id
     \param child
    */
    void addChild(EntityID entity_id, EntityID &child);

    /*!
     \brief removes a given EntityList from a given Entities Children

     \param entity_id
     \param children
    */
    void removeChildren(EntityID entity_id, EntityList children);
    /*!
     \brief removes a given Entity from a given Entities Children

     \param parent_entity_id
     \param child_entity_id
    */
    void removeChild(EntityID parent_entity_id, EntityID child_entity_id);

    /*!
     \brief returns a EntityList of all Children of a given Entity

     \param entity_id
     \return EntityList
    */
    EntityList& getChildren(EntityID entity_id);
    /*!
     \brief adds or replaces a given Entities Parent Component

     if the child_entity_id has no Parent Component one is created and added
     \param child_entity_id
     \param parent_entity_id
    */
    void setParent(EntityID child_entity_id, EntityID parent_entity_id);
    /*!
     \brief removes the Parent Component from a given Entity

     also removes the Entity from the previous Parents Children

     \param child_entity_id
    */
    void removeParent(EntityID child_entity_id);

    /*!
     \brief returns the Parent of a given Entity

     returns a invalid_id if the child_entity_id has no Parent component

     \param child_entity_id
     \return EntityID
    */
    EntityID getParent(EntityID child_entity_id);

    /*!
     \brief removes a given Entity from the Entity Manager

     if entity_id has Childrn these are also removed
     if entity_id was a child it is removed from the Children of its Parent
     all ComponentData for the entity is also removed

     \param entity_id
    */
    void removeEntity(EntityID entity_id);

    /*!
     \brief removes all Components from a given Entity

     \param entity_id
    */
    void removeAllComponentsFromEntity(EntityID entity_id);
    /*!
     \brief removes a Component from a given Entity

     \param component_id
     \param entity_id
    */
    void removeComponentFromEntity(ComponentID component_id, EntityID entity_id);

    /*!
     \brief returns a QList<ComponentID> of all Components added to a entity

     \param entity_id
     \return QList<ComponentID>
    */
    QList<ComponentID> allComponentsOfEntity(EntityID entity_id)const;

    /*!
     \brief

     \param component_id
     \param entity_id
     \return ComponentData
    */
    ComponentData componentDataForEntity(ComponentID component_id, EntityID entity_id) const;
    /*!
     \brief

     \param component_id
     \param entity_id
     \return QVector<double>
    */
    QVector<double>& doubleVectorDataForEntity(ComponentID component_id, EntityID entity_id) const;
    /*!
     \brief

     \param component_id
     \param entity_id
     \return EntityList
    */
    EntityList& entityListDataForEntity(ComponentID component_id, EntityID entity_id) const;

    /*!
     \brief

     \param component_id
     \param entity_id
     \param component_data
    */
    void setComponentDataForEntity(ComponentID component_id, EntityID entity_id,  ComponentData &component_data);
    /*!
     \brief

     \param component_id
     \param entity_id
     \param component_data
    */
    void setComponentDataForEntity(ComponentID component_id, EntityID entity_id,  const ComponentData &component_data);
    /*!
     \brief

     \param component_id
     \param entity_id
     \param component_data
    */
    void setComponentDataForEntity(ComponentID component_id, EntityID entity_id,  QVector<double> &component_data);
    /*!
     \brief

     \param component_id
     \param entity_id
     \param component_data
    */
    void setComponentDataForEntity(ComponentID component_id, EntityID entity_id,  QVector<double> *component_data);
    /*!
     \brief

     \param component_id
     \param entity_id
     \param component_data
    */
    void setComponentDataForEntity(ComponentID component_id, EntityID entity_id,  EntityList &component_data);
    /*!
     \brief

     \param component_id
     \param entity_id
     \param component_data
    */
    void setComponentDataForEntity(ComponentID component_id, EntityID entity_id,  EntityList *component_data);

    /*!
     \brief

     \return bool
    */
    bool isConnectedToEventManager()const;
    /*!
     \brief

     \param evt_mng
    */
    void connectToEventManager(EventManager* evt_mng);
    /*!
     \brief

     \return EventManager
    */
    EventManager* eventManager()const;

    /*!
     \brief

     \param component_id
     \param component_data
     \return EntityList
    */
    EntityList entitiesByComponentsValue(ComponentID component_id, ComponentData component_data)const;

private:
    /*!
     \brief

     \param container
     \return ID
    */
    ID nextIdFrom(const QList<ID>& container)const;
    /*!
     \brief

     \return EntityID
    */
    EntityID nextEntityId()const;
    /*!
     \brief

     \return ComponentID
    */
    ComponentID nextComponentId()const;

    /*!
     \brief

     \param component_id
     \return bool
    */
    bool componentIsExclusive(ComponentID component_id)const;
    /*!
     \brief

     \param component_id
     \param entity_id
     \param ignore_exclusive
     \return ComponentID
    */
    ComponentID createComponentAndAddTo(ComponentID component_id, EntityID entity_id, bool ignore_exclusive);
    /*!
     \brief

     \param component_id
     \param entity_id
     \param component_data
     \param ignore_exclusive
    */
    void setComponentDataForEntity(ComponentID component_id, EntityID entity_id,  const ComponentData &component_data, bool ignore_exclusive);
    /*!
     \brief

     \param component_id
     \param entity_id
     \param component_data
     \param ignore_exclusive
    */
    void setComponentDataForEntity(ComponentID component_id, EntityID entity_id,  EntityList &component_data, bool ignore_exclusive);
    /*!
     \brief

     \param component_id
     \param entity_id
     \param component_data
     \param ignore_exclusive
    */
    void setComponentDataForEntity(ComponentID component_id, EntityID entity_id,  EntityList *component_data, bool ignore_exclusive);

private slots:
    /*!
     \brief

     \param component_id
     \param entity_id
     \param component_data
    */
    void slotChangeComponentDataForEntity(EntityManager::ComponentID component_id, EntityManager::EntityID entity_id,  const EntityManager::ComponentData &component_data);


private:
    QScopedPointer<EntityManagerDb> const m_db_ptr; /**< TODO: describe */
    QPointer<EventManager> m_evt_mng; /**< TODO: describe */

    EntityList m_exclusive_components; /**< TODO: describe */
};

} // namespace talorion

#endif // TALORION_ENTITY_MANAGER_HPP

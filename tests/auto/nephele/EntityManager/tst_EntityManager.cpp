#include <QString>
#include <QtTest>

#include <core/ManagerLocator.hpp>
#include <core/EntityManager.hpp>
#include <core/EventManager.hpp>
//#include <core/StaticComponentId.hpp>

using namespace talorion;

class tst_EntityManager : public QObject
{
  Q_OBJECT

public:
  tst_EntityManager();

private:
  bool isPrime (int n);

private Q_SLOTS:
  void entityManagerHasZeroEntitiesAfterCreation();
  void createNewEntityResultsInValidId();
  void createdEntityExistsAfterCreation();
  void createNewEntityResultsDifferentIds();
  void entityManagerHasZeroComponentsAfterCreation();
  void createNewComponentResultsInValidId();
  void createdComponentExistsAfterCreation();
  void createdComponentIsAddedToEntity();
  void maxEntityIdIsPrime();
  void maxComponentIdIsPrime();
  void cannotCreateInvalidComponent();
  void createdComponentCanBeAddedToEntity();
  void entityCanBeDeleted();
  void createdComponentNoLongerExistsAfterEntityDeletion();
  void componentDataIsInvalidAfterCreation();
  void componentDataCanBeSet();
  void componentCanBeCreatedByName();
  void componentNamesAreUnique();
  void nullComponentNamesAreNotUnique();
  void emptyComponentNamesAreUnique();
  void entityManagerStartsWithNoEventManagerConnected();
  void entityManagerIsConnectedAfterEventManagerConnected();
  void entityManagerCanOnlyBeConnectedOnce();
  void entityManagerCanFindEntitiesByComponentsValue();
  void entityManagerCanFindEntitiesByComponentsValueOnly();
  void entityManagerCanHandleQvectordouble();
  void entityManagerCanHandleQlistofentities();
  void entityManagerCanHandleQint64();

  void entitiesCanHaveChildren();
  void entitiesHaveNoChildrenAfterCreation();
  void entitiesCanHaveParent();
  void entitiesHaveNoParentAfterCreation();
  void entitiesChildCanBeSetByEntityManager();
  void entitiesChildrenCanBeSetByEntityManager();
  void whenEntityIsAddedAsChildParentIsSet();
  void whenChildEntityIsDeletedItIsRemovedFromParentsChilds();
  void whenEntityIsDeletedItDeletesAllChildren();
  void ParentComponentCanNotBeAddedFromOutsideOfEntityManager();
  void ChildrenComponentCanNotBeAddedFromOutsideOfEntityManager();

};

tst_EntityManager::tst_EntityManager()
{
}

bool tst_EntityManager::isPrime(int n)
{
  int i, flag=0;
  for(i=2;i<=n/2;++i){if(n%i==0){flag=1;break;}}
  return (flag==0);
}

void tst_EntityManager::entityManagerHasZeroEntitiesAfterCreation()
{
  EntityManager mng;                          /*Arrange*/
  auto all_entinites = mng.allEntities();    /*Act*/
  QCOMPARE(all_entinites.size(), 0);              /*Assert*/
}

void tst_EntityManager::createNewEntityResultsInValidId()
{
  EntityManager mng;
  auto entity_id = mng.createNewEntity();
  QVERIFY(EntityManager::isValid(entity_id));
}

void tst_EntityManager::createdEntityExistsAfterCreation()
{
  EntityManager mng;
  auto entity_id = mng.createNewEntity();
  QVERIFY(mng.entityExists(entity_id));
}

void tst_EntityManager::createNewEntityResultsDifferentIds()
{
  EntityManager mng;
  auto entity_id = mng.createNewEntity();
  auto other_entity_id = mng.createNewEntity();
  QVERIFY(entity_id != other_entity_id);
}

void tst_EntityManager::entityManagerHasZeroComponentsAfterCreation()
{
  EntityManager mng;                          /*Arrange*/
  auto all_components = mng.allComponents(); /*Act*/
  QCOMPARE(all_components.size(), 0);             /*Assert*/
}

void tst_EntityManager::createNewComponentResultsInValidId()
{
  EntityManager mng;
  auto component_id   = mng.createNewComponent();
  QVERIFY(EntityManager::isValid(component_id));
}

void tst_EntityManager::createdComponentExistsAfterCreation()
{
  EntityManager mng;
  auto component_id   = mng.createNewComponent();
  QVERIFY(mng.componentExists(component_id));
}

void tst_EntityManager::createdComponentIsAddedToEntity()
{
  EntityManager mng;
  auto entity_id      = mng.createNewEntity();
  auto component_id   = mng.createNewComponent();
  mng.createComponentAndAddTo(component_id, entity_id);
  QVERIFY(mng.entityHasComponent(entity_id, component_id));
}

void tst_EntityManager::maxEntityIdIsPrime()
{
  QVERIFY(isPrime(EntityManager::max_entity_id));
}

void tst_EntityManager::maxComponentIdIsPrime()
{
  QVERIFY(isPrime(EntityManager::max_component_id));
}

void tst_EntityManager::cannotCreateInvalidComponent()
{
  EntityManager mng(0);
  auto component_id   = mng.createNewComponent(EntityManager::invalid_id);
  QVERIFY(EntityManager::isValid(component_id));
}

void tst_EntityManager::createdComponentCanBeAddedToEntity()
{
  EntityManager mng(0);
  auto entity_id          = mng.createNewEntity();
  auto component_id       = mng.createNewComponent();
  auto added_component_id = mng.createComponentAndAddTo(component_id, entity_id);
  QCOMPARE(added_component_id, component_id);
}

void tst_EntityManager::entityCanBeDeleted()
{
  EntityManager mng(0);
  auto entity_id = mng.createNewEntity();
  mng.removeEntity(entity_id);
  QVERIFY(!mng.entityExists(entity_id));
}

void tst_EntityManager::createdComponentNoLongerExistsAfterEntityDeletion()
{
  EntityManager mng(0);
  auto entity_id = mng.createNewEntity();
  auto component_id   = mng.createNewComponent();
  mng.createComponentAndAddTo(component_id, entity_id);
  mng.removeEntity(entity_id);
  QVERIFY(!mng.entityHasComponent(entity_id, component_id));
}

void tst_EntityManager::componentDataIsInvalidAfterCreation()
{
  EntityManager mng(0);
  auto entity_id = mng.createNewEntity();
  auto component_id   = mng.createNewComponent();
  mng.createComponentAndAddTo(component_id, entity_id);
  auto component_data =mng.componentDataForEntity(component_id, entity_id);
  QVERIFY(!EntityManager::isValid(component_data));
}

void tst_EntityManager::componentDataCanBeSet()
{
  EntityManager mng(0);
  auto entity_id = mng.createNewEntity();
  auto component_id   = mng.createNewComponent();
  mng.createComponentAndAddTo(component_id, entity_id);
  QVariant data(0);
  mng.setComponentDataForEntity(component_id, entity_id, data);
  auto component_data =mng.componentDataForEntity(component_id, entity_id);
  QVERIFY(EntityManager::isValid(component_data));
}

void tst_EntityManager::componentCanBeCreatedByName()
{
  EntityManager mng(0);
  QString comp_name("some_component");
  mng.createNewComponent(comp_name);
  QVERIFY(mng.componentExists(comp_name));
}

void tst_EntityManager::componentNamesAreUnique()
{
  EntityManager mng(0);
  QString comp_name("some_component");
  auto first_component_id   =mng.createNewComponent(comp_name);
  auto second_component_id  =mng.createNewComponent(comp_name);
  QCOMPARE(first_component_id, second_component_id);
}

void tst_EntityManager::nullComponentNamesAreNotUnique()
{
  EntityManager mng(0);
  QString comp_name;
  auto first_component_id   =mng.createNewComponent(comp_name);
  auto second_component_id  =mng.createNewComponent(comp_name);
  QVERIFY(first_component_id != second_component_id);
}

void tst_EntityManager::emptyComponentNamesAreUnique()
{
  EntityManager mng(0);
  QString comp_name("");
  auto first_component_id   =mng.createNewComponent(comp_name);
  auto second_component_id  =mng.createNewComponent(comp_name);
  QCOMPARE(first_component_id, second_component_id);
}

void tst_EntityManager::entityManagerStartsWithNoEventManagerConnected()
{
  EntityManager mng;
  auto is_connected = mng.isConnectedToEventManager();
  QVERIFY(!is_connected);
}

void tst_EntityManager::entityManagerIsConnectedAfterEventManagerConnected()
{
  EntityManager mng;
  EventManager evt_mng;
  mng.connectToEventManager(&evt_mng);
  QVERIFY(mng.isConnectedToEventManager());
}

void tst_EntityManager::entityManagerCanOnlyBeConnectedOnce()
{
  EntityManager mng;
  EventManager* evt_mng1 = new EventManager();
  EventManager* evt_mng2 = new EventManager();
  mng.connectToEventManager(evt_mng1);
  mng.connectToEventManager(evt_mng2);
  QVERIFY(mng.eventManager() == evt_mng1);
}

void tst_EntityManager::entityManagerCanFindEntitiesByComponentsValue()
{
  EntityManager mng(0);
  auto entity_id = mng.createNewEntity();
  auto component_id   = mng.createNewComponent("unique_id");
  mng.createComponentAndAddTo(component_id, entity_id);
  QUuid data("{340c5f10-6d53-4c15-8fc4-4c559580cfbc}");
  QUuid data2("{340c5f10-6d53-4c15-8fc4-4c559580cfbc}");
  mng.setComponentDataForEntity(component_id, entity_id, QVariant::fromValue(data));
  auto found_ids = mng.entitiesByComponentsValue(component_id, QVariant::fromValue(data2));
  QVERIFY(found_ids.contains(entity_id));
}

void tst_EntityManager::entityManagerCanFindEntitiesByComponentsValueOnly()
{
  EntityManager mng(0);
  auto entity_id = mng.createNewEntity();
  auto component_id   = mng.createNewComponent("unique_id");
  mng.createComponentAndAddTo(component_id, entity_id);
  QUuid data("{340c5f10-6d53-4c15-8fc4-4c559580cfbc}");
  mng.setComponentDataForEntity(component_id, entity_id, data);

  auto entity_id2 = mng.createNewEntity();
  mng.createComponentAndAddTo(component_id, entity_id2);
  QUuid data2("{99060fb8-676f-47d8-b9f1-c9c492721009}");
  mng.setComponentDataForEntity(component_id, entity_id2, data2);

  auto found_ids = mng.entitiesByComponentsValue(component_id, data);
  QVERIFY(found_ids.contains(entity_id) && !found_ids.contains(entity_id2));
}

void tst_EntityManager::entityManagerCanHandleQvectordouble()
{
  EntityManager mng(0);
  auto new_entity  = mng.createNewEntity();

  mng.createComponentAndAddTo(SPECTRUM_COMPONENT, new_entity);

  auto spec_raw = QVector<double>(5, double(0.0));
  mng.setComponentDataForEntity(SPECTRUM_COMPONENT, new_entity, spec_raw);
  QVector<double>& var = mng.doubleVectorDataForEntity(SPECTRUM_COMPONENT, new_entity);

  QCOMPARE(spec_raw.data(), var.data());
}

void tst_EntityManager::entityManagerCanHandleQlistofentities()
{
  EntityManager mng(0);
  auto new_entity  = mng.createNewEntity();

  mng.createComponentAndAddTo(BOX_COMPONENT, new_entity);

  EntityManager::EntityList spec_raw;
  spec_raw<<mng.createNewEntity();
  spec_raw<<mng.createNewEntity();
  spec_raw<<mng.createNewEntity();
  spec_raw<<mng.createNewEntity();
  mng.setComponentDataForEntity(BOX_COMPONENT, new_entity, spec_raw);
  EntityManager::EntityList& var = mng.entityListDataForEntity(BOX_COMPONENT, new_entity);

  QCOMPARE(spec_raw, var);
}

void tst_EntityManager::entityManagerCanHandleQint64()
{
  EntityManager mng(0);
  auto new_entity  = mng.createNewEntity();

  qint64 set_val = std::numeric_limits<qint64>::max();
  mng.createComponentAndAddTo(CYCLE_COMPONENT, new_entity);
  mng.setComponentDataForEntity(CYCLE_COMPONENT, new_entity, set_val);
  auto ret_var=mng.componentDataForEntity(CYCLE_COMPONENT, new_entity);
  qint64 store_val=-1;
  if(ret_var.canConvert<qint64>())
    store_val = ret_var.value<qint64>();
  QCOMPARE(store_val, set_val);
}

void tst_EntityManager::entitiesCanHaveChildren()
{
  EntityManager& ent_mng      = ManagerLocator::entityManager();
  auto ent = ent_mng.createNewEntity();
  auto children = ent_mng.getChildren(ent);

  QCOMPARE(children, EntityManager::invalid_entityList_data);
}

void tst_EntityManager::entitiesHaveNoChildrenAfterCreation()
{
  EntityManager& ent_mng      = ManagerLocator::entityManager();
  auto ent = ent_mng.createNewEntity();

  QVERIFY(ent_mng.entiyHasChildren(ent) == false);
}

void tst_EntityManager::entitiesCanHaveParent()
{
  EntityManager& ent_mng      = ManagerLocator::entityManager();
  auto ent = ent_mng.createNewEntity();
  auto parent = ent_mng.getParent(ent);
  auto inv=EntityManager::invalid_id;

  QCOMPARE(parent, inv);
}

void tst_EntityManager::entitiesHaveNoParentAfterCreation()
{
  EntityManager& ent_mng      = ManagerLocator::entityManager();
  auto ent = ent_mng.createNewEntity();

  QVERIFY(ent_mng.entiyHasParent(ent) == false);
}

void tst_EntityManager::entitiesChildCanBeSetByEntityManager()
{
  EntityManager& ent_mng      = ManagerLocator::entityManager();
  auto parent = ent_mng.createNewEntity();
  auto child = ent_mng.createNewEntity();

  ent_mng.addChild(parent, child);
  EntityManager::EntityList& children = ent_mng.getChildren(parent);

  QVERIFY(children.contains(child));
}

void tst_EntityManager::entitiesChildrenCanBeSetByEntityManager()
{
  EntityManager& ent_mng      = ManagerLocator::entityManager();
  auto parent = ent_mng.createNewEntity();

  int number_of_children=5;
  EntityManager::EntityList new_children;
  for(int i=0;i<number_of_children;i++){
      new_children << ent_mng.createNewEntity();
    }

  ent_mng.addChildren(parent, new_children);
  EntityManager::EntityList& chil = ent_mng.getChildren(parent);

  QCOMPARE(chil.size(), number_of_children);
}

void tst_EntityManager::whenEntityIsAddedAsChildParentIsSet()
{
  EntityManager& ent_mng      = ManagerLocator::entityManager();
  auto parent = ent_mng.createNewEntity();
  auto child = ent_mng.createNewEntity();

  ent_mng.addChild(parent, child);

  QCOMPARE(ent_mng.getParent(child), parent);
}

void tst_EntityManager::whenChildEntityIsDeletedItIsRemovedFromParentsChilds()
{
  EntityManager& ent_mng      = ManagerLocator::entityManager();
  auto parent = ent_mng.createNewEntity();
  auto child  = ent_mng.createNewEntity();
  ent_mng.addChild(parent, child);

  ent_mng.removeEntity(child);
  EntityManager::EntityList& children = ent_mng.getChildren(parent);

  QVERIFY(children.contains(child) == false);
}

void tst_EntityManager::whenEntityIsDeletedItDeletesAllChildren()
{
  EntityManager& ent_mng      = ManagerLocator::entityManager();
  auto parent = ent_mng.createNewEntity();
  auto child  = ent_mng.createNewEntity();
  ent_mng.addChild(parent, child);

  ent_mng.removeEntity(parent);

  QVERIFY(ent_mng.entityExists(child)== false);
}

void tst_EntityManager::ParentComponentCanNotBeAddedFromOutsideOfEntityManager()
{
  EntityManager& ent_mng      = ManagerLocator::entityManager();
  auto parent_entity_id = ent_mng.createNewEntity();
  auto child_entity_id  = ent_mng.createNewEntity();

  ent_mng.createComponentAndAddTo(PARENT_COMPONENT, child_entity_id);
  ent_mng.setComponentDataForEntity(PARENT_COMPONENT, child_entity_id, QVariant::fromValue(parent_entity_id));

  QVERIFY(ent_mng.entiyHasParent(child_entity_id) == false);
}

void tst_EntityManager::ChildrenComponentCanNotBeAddedFromOutsideOfEntityManager()
{
  EntityManager& ent_mng      = ManagerLocator::entityManager();
  auto parent_entity_id = ent_mng.createNewEntity();
  int number_of_children=5;
  EntityManager::EntityList new_children;
  for(int i=0;i<number_of_children;i++){
      new_children << ent_mng.createNewEntity();
    }

  ent_mng.createComponentAndAddTo(CHILDREN_COMPONENT, parent_entity_id);
  ent_mng.setComponentDataForEntity(CHILDREN_COMPONENT, parent_entity_id, new_children);

  QVERIFY(ent_mng.entiyHasChildren(parent_entity_id) == false);
}




QTEST_APPLESS_MAIN(tst_EntityManager)

#include "tst_EntityManager.moc"

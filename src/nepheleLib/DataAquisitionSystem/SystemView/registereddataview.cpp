#include "registereddataview.hpp"

#include <DataAquisitionSystem/RegisteredDataEntity/registereddataentity.hpp>
#include <DataAquisitionSystem/RegisteredDataEntity/registereddataentityfactory.hpp>
#include <DataAquisitionSystem/RegisteredDataEntity/registeredsourceentity.hpp>

#include <gui_system/GuiConstantProvider.hpp>


namespace talorion {

  RegisteredDataView::RegisteredDataView(EntityManager::EntityID entity, QWidget *par) :
    AbstractSystemView(par),
    m_entity(entity),
    m_layout(new QGridLayout()),
    ana_views(),
    dig_views()
  {
    if(isValid() == false)
      return;

    m_layout->setMargin(GuiConstantProvider::margin());
    m_layout->setHorizontalSpacing(GuiConstantProvider::horizontal_spaceing());
    m_layout->setVerticalSpacing(0);
    setLayout(m_layout);

    connect(EventManager::get_instance(),SIGNAL(newAnalogInputValue(int)),this, SLOT(addAIV(int)));
    connect(EventManager::get_instance(),SIGNAL(newAnalogOutputValue(int)),this, SLOT(addAOV(int)));
    connect(EventManager::get_instance(),SIGNAL(newAnalogValue(int)),this, SLOT(addAV(int)));

    connect(EventManager::get_instance(),SIGNAL(newDigitalInputValue(int)),this, SLOT(addDIV(int)));
    connect(EventManager::get_instance(),SIGNAL(newDigitalOutputValue(int)),this, SLOT(addDOV(int)));
    connect(EventManager::get_instance(),SIGNAL(newDigitalValue(int)),this, SLOT(addDIOV(int)));


    connect(EventManager::get_instance(),SIGNAL(removedAnalogInputValue(int)), this, SLOT(removeAIV(int)));
    connect(EventManager::get_instance(),SIGNAL(removedAnalogOutputValue(int)), this, SLOT(removeAOV(int)));
    connect(EventManager::get_instance(),SIGNAL(removedAnalogValue(int)), this, SLOT(removeAV(int)));

    connect(EventManager::get_instance(),SIGNAL(removedDigitalInputValue(int)), this, SLOT(removeDIV(int)));
    connect(EventManager::get_instance(),SIGNAL(removedDigitalOutputValue(int)), this, SLOT(removeDOV(int)));
    connect(EventManager::get_instance(),SIGNAL(removedDigitalValue(int)), this, SLOT(removeDIOV(int)));

    connect(EventManager::get_instance(),SIGNAL(newRegisteredDataEntity(EntityManager::EntityID)),this, SLOT(addAIV(int)));

    //    EntityManager& entMng = ManagerLocator::entityManager();

    //    setWindowTitle("Registered User Data");
    //    if(isValid()){
    //        QString ret;
    //        auto var = entMng.componentDataForEntity(NAME_COMPONENT, entity);
    //        if(var.canConvert<QString>()){
    //            ret = var.value<QString>();
    //            setWindowTitle(ret);
    //          }
    //      }
    RegisteredSourceEntity helper(m_entity);
    setWindowTitle(helper.name());
    foreach (auto ch, helper.children()) {
        //RegisteredDataEntity chHelper(ch);
        addAIV(ch);
      }

  }

  bool RegisteredDataView::isValid() const
  {
    if(EntityManager::isValid(entity()) == false )
      return false;

    RegisteredSourceEntity helper(entity());
    if(helper.serialVersionUid() == RegisteredDataEntityFactory::registeredSourceEntityUid())
      return true;

    return false;
  }

  EntityManager::EntityID RegisteredDataView::entity() const
  {
    return m_entity;
  }

  bool RegisteredDataView::belongsToMe(EntityManager::EntityID en) const
  {
    if(EntityManager::isValid(en) == false )
      return false;

    if(isValid() == false)
      return false;

    int parent = ManagerLocator::entityManager().getParent(en);

    if(entity() != parent)
      return false;

    return true;
  }

  void RegisteredDataView::addAnalogView(EntityManager::EntityID entity, analogView::mode md)
  {
    if(belongsToMe(entity) == false)
      return;

    auto aivIt = ana_views.constFind(entity);
    if(aivIt != ana_views.constEnd())
      return;

    auto aiv =  new flowControllerView(entity, md, this);
    ana_views.insert(entity,aiv);

//    auto row = m_layout->rowCount();
//    int col = m_layout->columnCount()-1;

    int numOfViews = m_layout->count();
    int MaxNumRows = 20;

    auto row = numOfViews%MaxNumRows;
    auto  col = numOfViews / MaxNumRows;

    m_layout->addWidget(aiv, row, col);
    m_layout->setRowStretch(row, 0);
    m_layout->setRowStretch(++row, 1);
  }

  void RegisteredDataView::addDigitalView(EntityManager::EntityID entity, digital_view::mode md)
  {
    if(belongsToMe(entity) == false)
      return;

    auto digIt = dig_views.constFind(entity);
    if(digIt != dig_views.constEnd())
      return;

    auto tmp = new digital_view(entity, md, this);
    dig_views.insert(entity, tmp);
    m_layout->addWidget(tmp);
  }

  void RegisteredDataView::removeDIV(EntityManager::EntityID entity)
  {
    if(belongsToMe(entity) == false)
      return;

    auto it = dig_views.constFind(entity);
    if (it == dig_views.constEnd())
      return;

    auto tmp = it.value();
    if( tmp == Q_NULLPTR)
      return;

    m_layout->removeWidget(tmp);
    delete tmp;
    dig_views.remove(entity);

  }

  void RegisteredDataView::removeDOV(EntityManager::EntityID entity)
  {
    removeDIV(entity);
  }

  void RegisteredDataView::removeDIOV(EntityManager::EntityID entity)
  {
    removeDIV(entity);
  }

  void RegisteredDataView::addAIV(int entity)
  {
    addAnalogView(entity, flowControllerView::Input);
  }

  void RegisteredDataView::addAOV(int entity)
  {
    addAnalogView(entity, flowControllerView::Output);
  }

  void RegisteredDataView::addAV(int entity)
  {
    addAnalogView(entity, flowControllerView::InputOutput);
  }

  void RegisteredDataView::addDIV(int entity)
  {
    addDigitalView(entity, digital_view::Input);
  }

  void RegisteredDataView::addDOV(int entity)
  {
    addDigitalView(entity, digital_view::Output);
  }

  void RegisteredDataView::addDIOV(int entity)
  {
    addDigitalView(entity, digital_view::InputOutput);
  }

  void RegisteredDataView::removeAIV(int entity)
  {
    if(belongsToMe(entity) == false)
      return;

    auto it = ana_views.constFind(entity);
    if (it == ana_views.constEnd())
      return;

    auto tmp = it.value();
    if( tmp == Q_NULLPTR)
      return;

    m_layout->removeWidget(tmp);
    delete tmp;
    ana_views.remove(entity);
  }

  void RegisteredDataView::removeAOV(int entity)
  {
    removeAIV(entity);
  }

  void RegisteredDataView::removeAV(int entity)
  {
    removeAIV(entity);
  }

} // namespace talorion

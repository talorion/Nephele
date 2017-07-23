#include "tcpboxview.hpp"

#include <gui_system/GuiConstantProvider.hpp>

#include <tcp_box_system/TcpBoxEntity/TcpBoxEntity.hpp>
#include <tcp_box_system/TcpBoxEntity/TcpBoxEntityFacorty.hpp>
#include <tcp_box_system/TcpBoxEntity/analoginputoutputvalueentity.hpp>
#include <tcp_box_system/TcpBoxEntity/analoginputvalueentity.hpp>
#include <tcp_box_system/TcpBoxEntity/analogoutputvalueentity.hpp>
#include <tcp_box_system/TcpBoxEntity/digitalinputvalueentity.hpp>
#include <tcp_box_system/TcpBoxEntity/digitaloutputvalueentity.hpp>
#include <tcp_box_system/TcpBoxEntity/valueentity.hpp>

#include <QLabel>

#include <gui_system/CustomWidgets/AnalogControl.hpp>
#include <gui_system/CustomWidgets/AnalogIndicator.hpp>
#include <gui_system/CustomWidgets/DigitalControl.hpp>
#include <gui_system/CustomWidgets/DigitalIndicator.hpp>

namespace talorion {

  TcpBoxView::TcpBoxView(EntityManager::EntityID en, QWidget *par):
    AbstractSystemView(par),
    m_entity(en),
    m_layout(new QGridLayout()),
    m_setValueSignalMapper(new QSignalMapper(this)),
    m_enteredSignalMapper(new QSignalMapper(this)),
    m_leftSignalMapper(new QSignalMapper(this)),
    m_analogControls(),
    m_digitalControls(),
    m_analogIndicators(),
    m_digitalIndicators()
  {
    TcpBoxEntity box(entity());
    QString boxname =  box.name();
    setWindowTitle(boxname);
    setObjectName(boxname);

    //auto row = m_layout->rowCount();
    //m_layout->setRowStretch(++row, 1);
    m_layout->setColumnMinimumWidth(1, GuiConstantProvider::min_col_with());
    m_layout->setColumnMinimumWidth(2, GuiConstantProvider::min_col_with());

    m_layout->setMargin(GuiConstantProvider::margin());
    m_layout->setHorizontalSpacing(GuiConstantProvider::horizontal_spaceing());

    setLayout(m_layout);

    connect(EventManager::get_instance(),SIGNAL(newAnalogInputValue(int)),this, SLOT(addAIV(int)));
    connect(EventManager::get_instance(),SIGNAL(newAnalogOutputValue(int)),this, SLOT(addAOV(int)));
    connect(EventManager::get_instance(),SIGNAL(newAnalogValue(int)),this, SLOT(addAV(int)));

    connect(EventManager::get_instance(),SIGNAL(newDigitalInputValue(int)),this, SLOT(addDIV(int)));
    connect(EventManager::get_instance(),SIGNAL(newDigitalOutputValue(int)),this, SLOT(addDOV(int)));
    connect(EventManager::get_instance(),SIGNAL(newDigitalValue(int)),this, SLOT(addDIOV(int)));

    connect(EventManager::get_instance(),SIGNAL(connection_state_component_changed(int)),this, SLOT(connectionStateChanged(int)));

    //    connect(EventManager::get_instance(),SIGNAL(removedAnalogInputValue(int)), this, SLOT(removeAIV(int)));
    //    connect(EventManager::get_instance(),SIGNAL(removedAnalogOutputValue(int)), this, SLOT(removeAOV(int)));
    //    connect(EventManager::get_instance(),SIGNAL(removedAnalogValue(int)), this, SLOT(removeAV(int)));

    //    connect(EventManager::get_instance(),SIGNAL(removedDigitalInputValue(int)), this, SLOT(removeDIV(int)));
    //    connect(EventManager::get_instance(),SIGNAL(removedDigitalOutputValue(int)), this, SLOT(removeDOV(int)));
    //    connect(EventManager::get_instance(),SIGNAL(removedDigitalValue(int)), this, SLOT(removeDIOV(int)));

    connect(EventManager::get_instance(),SIGNAL(analogSet_component_changed(int)),this,SLOT(changeSetValue(int)));
    connect(EventManager::get_instance(),SIGNAL(analogAct_component_changed(int)),this,SLOT(changeActValue(int)));

    connect(m_setValueSignalMapper, SIGNAL(mapped(int)), this, SLOT(setValueChangedByGui(EntityManager::EntityID)));
    connect(m_enteredSignalMapper, SIGNAL(mapped(int)), this, SLOT(elementEntered(EntityManager::EntityID)));
    connect(m_leftSignalMapper, SIGNAL(mapped(int)), this, SLOT(elementLeft(EntityManager::EntityID)));

    connect(EventManager::get_instance(), SIGNAL(name_component_changed(int)),this, SLOT(boxNameChanged(EntityManager::EntityID)));
    connect(EventManager::get_instance(), SIGNAL(metadataChanged(EntityManager::EntityID)),this, SLOT(metadataChanged(EntityManager::EntityID)));

  }


  bool TcpBoxView::isValid() const
  {
    TcpBoxEntity box(entity());
    return box.isValid();
  }

  bool TcpBoxView::belongsToMe(EntityManager::EntityID en) const
  {
    if(EntityManager::isValid(en) == false )
      return false;

    if(isValid() == false)
      return false;

    auto parent = ManagerLocator::entityManager().getParent(en);
    if(entity() != parent)
      return false;

    return true;
  }

  void TcpBoxView::addView(EntityManager::EntityID en)
  {
    if(belongsToMe(en) == false)
      return;

    auto nameLabel = addNameLabel(en);

    auto vmode = TcpBoxEntityFacorty::getValueMode(en);
    auto vtype = TcpBoxEntityFacorty::getValueType(en);

    QWidget* controlW = Q_NULLPTR;
    if(vtype == ValueType::VT_ANALOG && (vmode & ValueMode::VM_OUTPUT)){
        controlW = addAnalogControl(en);
      }
    if(vtype == ValueType::VT_DIGITAL && (vmode & ValueMode::VM_OUTPUT)){
        controlW = addDigitalControl(en);
      }

    QWidget* indicatorW = Q_NULLPTR;
    if(vtype == ValueType::VT_ANALOG && (vmode & ValueMode::VM_INPUT)){
        indicatorW = addAnalogIndicator(en);
      }
    if(vtype == ValueType::VT_DIGITAL && (vmode & ValueMode::VM_INPUT)){
        indicatorW = addDigitalIndicator(en);
      }

    //auto row = m_layout->rowCount();
    ValueEntity ve(en);
    auto row =  ve.id();
    int col = 0;

    m_layout->addWidget(nameLabel, row, col);
    if(controlW != Q_NULLPTR)
      m_layout->addWidget(controlW,    row,  ++col);
    else
      ++col;

    if(indicatorW != Q_NULLPTR)
      m_layout->addWidget(indicatorW,    row,  ++col);

    row = m_layout->rowCount();
    m_layout->setRowStretch(row, 0);
    m_layout->setRowStretch(++row, 1);

  }

  void TcpBoxView::setValue(EntityManager::EntityID en)
  {
    if(belongsToMe(en) == false)
      return;

    auto vmode = TcpBoxEntityFacorty::getValueMode(en);
    auto vtype = TcpBoxEntityFacorty::getValueType(en);

    if(vtype == ValueType::VT_ANALOG && (vmode & ValueMode::VM_OUTPUT)){
        setAnalogControl(en);
      }

    if(vtype == ValueType::VT_DIGITAL && (vmode & ValueMode::VM_OUTPUT)){
        setDigitalControl(en);
      }

    if(vtype == ValueType::VT_ANALOG && (vmode & ValueMode::VM_INPUT)){
        setAnalogIndicator(en);
      }

    if(vtype == ValueType::VT_DIGITAL && (vmode & ValueMode::VM_INPUT)){
        setDigitalIndicator(en);
      }

  }

  void TcpBoxView::setAnalogControl(EntityManager::EntityID en)
  {
    auto it = m_analogControls.find(en);
    if(it == m_analogControls.end())
      return;

    AnalogOutputValueEntity aov(en);
    auto val =aov.setVal();

    auto ctrl= it.value();
    ctrl->setValue(val);
  }

  void TcpBoxView::setDigitalControl(EntityManager::EntityID en)
  {
    auto it = m_digitalControls.find(en);
    if(it == m_digitalControls.end())
      return;

    DigitalOutputValueEntity dov(en);
    auto val =dov.setVal();

    auto ctrl= it.value();
    ctrl->setValue(val);
  }

  void TcpBoxView::setAnalogIndicator(EntityManager::EntityID en)
  {
    auto it = m_analogIndicators.find(en);
    if(it == m_analogIndicators.end())
      return;

    AnalogInputValueEntity aiv(en);
    auto val =aiv.actVal();

    auto ind= it.value();
    ind->setValue(val);

    compareAnalogActWithSet(en);
  }

  void TcpBoxView::setDigitalIndicator(EntityManager::EntityID en)
  {
    auto it = m_digitalIndicators.find(en);
    if(it == m_digitalIndicators.end())
      return;

    DigitalInputValueEntity div(en);
    auto val =div.actVal();

    auto ind= it.value();
    ind->setValue(val);
  }

  void TcpBoxView::compareAnalogActWithSet(EntityManager::EntityID en)
  {
    //AnalogInputOutputValueEntity aioe(en);
    //if(aioe.isValid() == false )
    //  return;

    auto it = m_analogIndicators.find(en);
    if(it == m_analogIndicators.end())
      return;

    auto itt = m_analogControls.find(en);
    if(itt == m_analogControls.end())
      return;

    auto editAct= it.value();
    auto editSet= itt.value();

    auto actValue = editAct->value();
    auto setValue = editSet->value();

    if (qFabs(actValue - setValue) > qFabs(setValue/5.0 )+qFabs(editSet->maximum()/100.0)){
        editAct->setStyleSheet(GuiConstantProvider::analogIndicatorError());
        return;
      }
    if (qFabs(actValue - setValue) > qFabs(setValue/10.0)+qFabs(editSet->maximum()/100.0)){
        editAct->setStyleSheet(GuiConstantProvider::analogIndicatorWarning());
        return;
      }

    editAct->setStyleSheet(GuiConstantProvider::analogIndicatorOk());
  }

  void TcpBoxView::valueChanged(EntityManager::EntityID en)
  {
    if(belongsToMe(en) == false)
      return;

    auto vmode = TcpBoxEntityFacorty::getValueMode(en);
    auto vtype = TcpBoxEntityFacorty::getValueType(en);

    if(vtype == ValueType::VT_ANALOG && (vmode & ValueMode::VM_OUTPUT)){
        valueChangedAnalog(en);
      }

    if(vtype == ValueType::VT_DIGITAL && (vmode & ValueMode::VM_OUTPUT)){
        valueChangedDigital(en);
      }

    //    if(vtype == ValueType::VT_ANALOG && (vmode & ValueMode::VM_INPUT)){
    //        setAnalogIndicator(en);
    //      }

    //    if(vtype == ValueType::VT_DIGITAL && (vmode & ValueMode::VM_INPUT)){
    //        setDigitalIndicator(en);
    //      }
  }

  void TcpBoxView::valueChangedAnalog(EntityManager::EntityID en)
  {
    auto it = m_analogControls.find(en);
    if(it == m_analogControls.end())
      return;

    AnalogOutputValueEntity aov(en);
    auto val =aov.setVal();

    auto ctrl= it.value();
    auto newVal = ctrl->value();

    if(newVal != val)
      emit ManagerLocator::eventManager().change_analogSet_component(en, newVal);

  }

  void TcpBoxView::valueChangedDigital(EntityManager::EntityID en)
  {
    auto it = m_digitalControls.find(en);
    if(it == m_digitalControls.end())
      return;

    DigitalOutputValueEntity dov(en);
    auto val =dov.setVal();

    auto ctrl= it.value();
    auto newVal = ctrl->value();

    if(newVal != val)
      emit ManagerLocator::eventManager().change_digitalSet_component(en, newVal);
  }

  QString TcpBoxView::buildToolTipFromMetatata(EntityManager::EntityID en)
  {
    auto metadata = ManagerLocator::entityManager().get_metadata_component(en);

    if(metadata.canConvert<QVariantMap>() ){
        QString ret;

        auto varMap= metadata.value<QVariantMap>();
        for(auto it = varMap.cbegin(); it != varMap.cend(); ++it){
              ret += it.key();
              ret += "\t\t";
              ret += it.value().toString();
              ret += "\n";
          }
        return ret;
      }

    return QString();
  }

  QLabel *TcpBoxView::addNameLabel(EntityManager::EntityID en)
  {
    ValueEntity ve(en);
    auto name = ve.name();
    auto nameLabel = new QLabel(name,this);
    return nameLabel;
  }

  AnalogControl *TcpBoxView::addAnalogControl(EntityManager::EntityID en)
  {
    AnalogOutputValueEntity aov(en);
    auto units = aov.units();
    auto it = m_analogControls.find(en);
    if(it == m_analogControls.end()){
        auto tmp = new AnalogControl(aov.setMin(), aov.setMax(), units);
        it = m_analogControls.insert(en, tmp);
      }
    auto ctrl= it.value();
    ctrl->setValue(aov.setVal());

    connect(ctrl, SIGNAL(valueChanged(double)), m_setValueSignalMapper, SLOT(map()));
    m_setValueSignalMapper->setMapping(ctrl, en);

    connect(ctrl, SIGNAL(entered()), m_enteredSignalMapper, SLOT(map()));
    m_enteredSignalMapper->setMapping(ctrl, en);

    connect(ctrl, SIGNAL(left()), m_leftSignalMapper, SLOT(map()));
    m_leftSignalMapper->setMapping(ctrl, en);


    return ctrl;
  }

  DigitalControl *TcpBoxView::addDigitalControl(EntityManager::EntityID en)
  {
    DigitalOutputValueEntity dov(en);
    auto it = m_digitalControls.find(en);
    if(it == m_digitalControls.end()){
        auto tmp = new DigitalControl("HI", "LO");
        it = m_digitalControls.insert(en, tmp);
      }
    auto ctrl= it.value();
    ctrl->setValue(dov.setVal());

    connect(ctrl, SIGNAL(valueChanged(double)), m_setValueSignalMapper, SLOT(map()));
    m_setValueSignalMapper->setMapping(ctrl, en);

    connect(ctrl, SIGNAL(entered()), m_enteredSignalMapper, SLOT(map()));
    m_enteredSignalMapper->setMapping(ctrl, en);

    connect(ctrl, SIGNAL(left()), m_leftSignalMapper, SLOT(map()));
    m_leftSignalMapper->setMapping(ctrl, en);

    return ctrl;
  }

  AnalogIndicator *TcpBoxView::addAnalogIndicator(EntityManager::EntityID en)
  {
    AnalogInputValueEntity aiv(en);
    auto units = aiv.units();
    auto it = m_analogIndicators.find(en);
    if(it == m_analogIndicators.end()){
        auto tmp = new AnalogIndicator(aiv.actMin(), aiv.actMax(), units);
        it = m_analogIndicators.insert(en, tmp);
      }
    auto ind= it.value();
    ind->setValue(aiv.actVal());

    connect(ind, SIGNAL(entered()), m_enteredSignalMapper, SLOT(map()));
    m_enteredSignalMapper->setMapping(ind, en);

    connect(ind, SIGNAL(left()), m_leftSignalMapper, SLOT(map()));
    m_leftSignalMapper->setMapping(ind, en);

    return ind;
  }

  DigitalIndicator *TcpBoxView::addDigitalIndicator(EntityManager::EntityID en)
  {
    DigitalInputValueEntity div(en);
    auto it = m_digitalIndicators.find(en);
    if(it == m_digitalIndicators.end()){
        auto tmp = new DigitalIndicator("HI", "LO");
        it = m_digitalIndicators.insert(en, tmp);
      }
    auto ind= it.value();
    ind->setValue(div.actVal());

    connect(ind, SIGNAL(entered()), m_enteredSignalMapper, SLOT(map()));
    m_enteredSignalMapper->setMapping(ind, en);

    connect(ind, SIGNAL(left()), m_leftSignalMapper, SLOT(map()));
    m_leftSignalMapper->setMapping(ind, en);

    return ind;
  }

  void TcpBoxView::addAIV(EntityManager::EntityID en)
  {
    if(belongsToMe(en))
      addView(en);
  }

  void TcpBoxView::addAOV(EntityManager::EntityID en)
  {
    if(belongsToMe(en))
      addView(en);
  }

  void TcpBoxView::addAV(EntityManager::EntityID en)
  {
    if(belongsToMe(en))
      addView(en);
  }

  void TcpBoxView::addDIV(EntityManager::EntityID en)
  {
    if(belongsToMe(en))
      addView(en);
  }

  void TcpBoxView::addDOV(EntityManager::EntityID en)
  {
    if(belongsToMe(en))
      addView(en);
  }

  void TcpBoxView::addDIOV(EntityManager::EntityID en)
  {
    if(belongsToMe(en))
      addView(en);
  }

  void TcpBoxView::connectionStateChanged(int en)
  {
    if(EntityManager::isValid(en) == false )
      return;

    if(en != entity())
      return;

    //TcpBoxEntity box(entity());
    //auto state = box.connectionState();
  }

  void TcpBoxView::changeSetValue(EntityManager::EntityID en)
  {
    setValue(en);
  }

  void TcpBoxView::changeActValue(EntityManager::EntityID en)
  {
    setValue(en);
  }

  void TcpBoxView::setValueChangedByGui(EntityManager::EntityID en)
  {
    valueChanged(en);
  }

  void TcpBoxView::elementEntered(EntityManager::EntityID en)
  {
    //EventManager& evtMng = ManagerLocator::eventManager();
    emit ManagerLocator::eventManager().plotEntityValue(en);
  }

  void TcpBoxView::elementLeft(EntityManager::EntityID en)
  {
    Q_UNUSED(en);
    emit ManagerLocator::eventManager().plotEntityValue(EntityManager::invalid_id);
  }

  void TcpBoxView::boxNameChanged(EntityManager::EntityID en)
  {
    if(EntityManager::isValid(en) == false )
      return;

    if(isValid() == false)
      return;

    if(entity() != en)
      return;

    TcpBoxEntity box(entity());
    QString boxname =  box.name();

    auto wtitl = windowTitle();
    if(wtitl != boxname)
      setWindowTitle(boxname);

    auto oName = objectName();
    if(oName != boxname)
      setObjectName(boxname);
  }

  void TcpBoxView::metadataChanged(EntityManager::EntityID en)
  {
    if(belongsToMe(en) == false)
      return;

    auto ttip = buildToolTipFromMetatata(en);

    auto it1 = m_analogControls.find(en);
    if(it1 != m_analogControls.end())
      it1.value()->setToolTip(ttip);

    auto it2 = m_digitalControls.find(en);
    if(it2 != m_digitalControls.end())
      it2.value()->setToolTip(ttip);

    auto it3 = m_analogIndicators.find(en);
    if(it3 != m_analogIndicators.end())
      it3.value()->setToolTip(ttip);

    auto it4 = m_digitalIndicators.find(en);
    if(it4 != m_digitalIndicators.end())
      it4.value()->setToolTip(ttip);

  }

  EntityManager::EntityID TcpBoxView::entity() const
  {
    //if(isValid())
    //  return m_entity;
    //return EntityManager::invalid_id;
    return m_entity;
  }



} // namespace talorion

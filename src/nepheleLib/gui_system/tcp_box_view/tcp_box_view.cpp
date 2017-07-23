#include "tcp_box_view.hpp"

#include "core/EntityManager.hpp"
#include "core/EventManager.hpp"

#include "digital_view/digital_view.hpp"
#include "analog_view/flowcontrollerview.hpp"
#include "analog_view/analogview.hpp"

#include <gui_system/GuiConstantProvider.hpp>

#include <QDebug>

namespace talorion {

  tcp_box_view::tcp_box_view(int entity,QWidget *par) :
    AbstractSystemView(par),
    m_layout(NULL),
    //m_group_box_layout(NULL),
    //m_group_box(NULL),
    m_entity(entity),
    ana_views(),
    dig_views(),
    m_labelMinimumWidth(0)
  {
    if(m_entity<0)
      return;

    QString boxname =  EntityManager::get_instance()->get_name_component(entity);
    setWindowTitle(boxname);

    setObjectName(boxname);

    m_layout = new QGridLayout();
    m_layout->setMargin(GuiConstantProvider::margin());
    m_layout->setHorizontalSpacing(GuiConstantProvider::horizontal_spaceing());
    m_layout->setVerticalSpacing(0);
    setContentsMargins(0,0,0,0);

    //m_group_box = new QGroupBox();
    //QString boxname =  entity_manager::get_instance()->get_name_component(entity);
    //m_group_box->setTitle(boxname);
    //m_layout->addWidget(m_group_box,0,0);

    //m_group_box_layout = new QGridLayout();
    //m_group_box_layout->setMargin(0);
    //m_group_box->setLayout(m_group_box_layout);

    //m_layout->setMargin(0);
    //m_layout->setColumnMinimumWidth(1, GuiConstantProvider::min_col_with());
    //m_layout->setColumnMinimumWidth(2, GuiConstantProvider::min_col_with());
    setLayout(m_layout);

    connect(EventManager::get_instance(),SIGNAL(newAnalogInputValue(int)),this, SLOT(addAIV(int)));
    connect(EventManager::get_instance(),SIGNAL(newAnalogOutputValue(int)),this, SLOT(addAOV(int)));
    connect(EventManager::get_instance(),SIGNAL(newAnalogValue(int)),this, SLOT(addAV(int)));

    connect(EventManager::get_instance(),SIGNAL(newDigitalInputValue(int)),this, SLOT(addDIV(int)));
    connect(EventManager::get_instance(),SIGNAL(newDigitalOutputValue(int)),this, SLOT(addDOV(int)));
    connect(EventManager::get_instance(),SIGNAL(newDigitalValue(int)),this, SLOT(addDIOV(int)));


    //connect(EventManager::get_instance(),SIGNAL(disconnect_tcp_box(int)),this, SLOT(disconnect_tcp_box(int)));
    connect(EventManager::get_instance(),SIGNAL(connection_state_component_changed(int)),this, SLOT(disconnect_tcp_box(int)));

    //connect(EventManager::get_instance(),SIGNAL(tcpBoxRemoved(int)),
    connect(EventManager::get_instance(),SIGNAL(removedAnalogInputValue(int)), this, SLOT(removeAIV(int)));
    connect(EventManager::get_instance(),SIGNAL(removedAnalogOutputValue(int)), this, SLOT(removeAOV(int)));
    connect(EventManager::get_instance(),SIGNAL(removedAnalogValue(int)), this, SLOT(removeAV(int)));

    connect(EventManager::get_instance(),SIGNAL(removedDigitalInputValue(int)), this, SLOT(removeDIV(int)));
    connect(EventManager::get_instance(),SIGNAL(removedDigitalOutputValue(int)), this, SLOT(removeDOV(int)));
    connect(EventManager::get_instance(),SIGNAL(removedDigitalValue(int)), this, SLOT(removeDIOV(int)));
  }

  tcp_box_view::~tcp_box_view()
  {

  }

  void tcp_box_view::zero_all()
  {
    QMap<int, analogView* >::iterator ait;
    for(ait = ana_views.begin(); ait != ana_views.end();ait++){
        //
      }

    QMap<int, digital_view* >::iterator dit;
    for(dit = dig_views.begin(); dit != dig_views.end();dit++){
        //
      }
  }

  bool tcp_box_view::belogs_to_box(int entity)
  {
    if(m_entity<0)
      return false;

    int box_entity = EntityManager::get_instance()->get_box_id_component(entity);

    if(m_entity != box_entity)
      return false;

    return true;
  }

  void tcp_box_view::appendWidget(QWidget *wdg)
  {
    if(wdg == Q_NULLPTR)
      return;

    //      auto row = m_layout->rowCount();
    //      int col = m_layout->columnCount()-1;

    int numOfViews = m_layout->count();
    int MaxNumRows = 20;

    auto row = numOfViews%MaxNumRows;
    auto  col = numOfViews / MaxNumRows;

    m_layout->addWidget(wdg, row, col);
    m_layout->setRowStretch(row, 0);
    m_layout->setRowStretch(++row, 1);

  }

  void tcp_box_view::disconnect_tcp_box(int entity)
  {
    Q_UNUSED(entity);
    //    qDebug() << Q_FUNC_INFO << "disconnect_tcp_box()!";
    //    qDebug() << Q_FUNC_INFO << "removing all values!";

    //    if(belogs_to_box(entity) == false)
    //      return;

    //    auto is_connected = EntityManager::get_instance()->componentDataForEntity(CONNECTION_STATE_COMPONENT, entity).toBool();
    //    if(is_connected == true)
    //      return;

    //    QMap<int, analogView* >::iterator ait;
    //    for(ait = ana_views.begin(); ait != ana_views.end();ait++){
    //        analogView* tmp = ait.value();
    //        m_layout->removeWidget(tmp);
    //        delete tmp;
    //        tmp =NULL;
    //      }
    //    ana_views.clear();

    //    QMap<int, digital_view* >::iterator dit;
    //    for(dit = dig_views.begin(); dit != dig_views.end();dit++){
    //        digital_view* tmp = dit.value();
    //        m_layout->removeWidget(tmp);
    //        delete tmp;
    //        tmp =NULL;
    //      }
    //    dig_views.clear();

  }

  void tcp_box_view::addAIV(int entity)
  {
    if(belogs_to_box(entity)){
        QMap<int, analogView*>::ConstIterator fcv = ana_views.constFind(entity);
        if (fcv == ana_views.constEnd()){
            flowControllerView* tmp = new flowControllerView(entity,flowControllerView::Input, this);
            ana_views.insert(entity,tmp);
            //m_group_box_layout->addWidget(tmp);
            //m_layout->addWidget(tmp);
            appendWidget(tmp);
            setLabelMinimumWidth( tmp->updateLabelMinimumWidth(m_labelMinimumWidth));
            connect(this, SIGNAL(labelMinimumWidthChanged(int)), tmp, SLOT(updateLabelMinimumWidth(int)));
          }
      }
  }

  void tcp_box_view::addAOV(int entity)
  {
    if(belogs_to_box(entity)){
        QMap<int, analogView*>::ConstIterator fcv = ana_views.constFind(entity);
        if (fcv == ana_views.constEnd()){
            flowControllerView* tmp = new flowControllerView(entity,flowControllerView::Output, this);
            ana_views.insert(entity,tmp);
            //m_group_box_layout->addWidget(tmp);
            //m_layout->addWidget(tmp);
            appendWidget(tmp);
            setLabelMinimumWidth( tmp->updateLabelMinimumWidth(m_labelMinimumWidth));
            connect(this, SIGNAL(labelMinimumWidthChanged(int)), tmp, SLOT(updateLabelMinimumWidth(int)));
          }
      }
  }

  void tcp_box_view::addAV(int entity)
  {
    if(belogs_to_box(entity)){
        QMap<int, analogView*>::ConstIterator fcv = ana_views.constFind(entity);
        if (fcv == ana_views.constEnd()){
            flowControllerView* tmp = new flowControllerView(entity,flowControllerView::InputOutput, this);
            ana_views.insert(entity,tmp);
            //m_group_box_layout->addWidget(tmp);
            //m_layout->addWidget(tmp);
            appendWidget(tmp);
            setLabelMinimumWidth( tmp->updateLabelMinimumWidth(m_labelMinimumWidth));
            connect(this, SIGNAL(labelMinimumWidthChanged(int)), tmp, SLOT(updateLabelMinimumWidth(int)));
          }
      }

  }

  void tcp_box_view::addDIV(int entity)
  {
    if(belogs_to_box(entity)){
        QMap<int, digital_view*>::ConstIterator fcv = dig_views.constFind(entity);
        if (fcv == dig_views.constEnd()){
            digital_view* tmp = new digital_view(entity, digital_view::Input,this);
            dig_views.insert(entity,tmp);
            //m_group_box_layout->addWidget(tmp);
            //m_layout->addWidget(tmp);
            appendWidget(tmp);
            setLabelMinimumWidth( tmp->updateLabelMinimumWidth(m_labelMinimumWidth));
            connect(this, SIGNAL(labelMinimumWidthChanged(int)), tmp, SLOT(updateLabelMinimumWidth(int)));
          }
      }
  }

  void tcp_box_view::addDOV(int entity)
  {
    if(belogs_to_box(entity)){
        QMap<int, digital_view*>::ConstIterator fcv = dig_views.constFind(entity);
        if (fcv == dig_views.constEnd()){
            digital_view* tmp = new digital_view(entity, digital_view::Output,this);
            dig_views.insert(entity,tmp);
            //m_group_box_layout->addWidget(tmp);
            //m_layout->addWidget(tmp);
            appendWidget(tmp);
            setLabelMinimumWidth( tmp->updateLabelMinimumWidth(m_labelMinimumWidth));
            connect(this, SIGNAL(labelMinimumWidthChanged(int)), tmp, SLOT(updateLabelMinimumWidth(int)));
          }
      }
  }

  void tcp_box_view::addDIOV(int entity)
  {
    if(belogs_to_box(entity)){
        QMap<int, digital_view*>::ConstIterator fcv = dig_views.constFind(entity);
        if (fcv == dig_views.constEnd()){
            digital_view* tmp = new digital_view(entity, digital_view::InputOutput,this);
            dig_views.insert(entity,tmp);
            //m_group_box_layout->addWidget(tmp);
            //m_layout->addWidget(tmp);
            appendWidget(tmp);
            setLabelMinimumWidth( tmp->updateLabelMinimumWidth(m_labelMinimumWidth));
            connect(this, SIGNAL(labelMinimumWidthChanged(int)), tmp, SLOT(updateLabelMinimumWidth(int)));
          }
      }
  }

  void tcp_box_view::removeAIV(int entity)
  {
    qDebug()<< Q_FUNC_INFO;

    if(belogs_to_box(entity) == false)
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

  void tcp_box_view::removeAOV(int entity)
  {
    qDebug()<< Q_FUNC_INFO;

    if(belogs_to_box(entity) == false)
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

  void tcp_box_view::removeAV(int entity)
  {
    qDebug()<< Q_FUNC_INFO;

    if(belogs_to_box(entity) == false)
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

  void tcp_box_view::removeDIV(int entity)
  {
    qDebug()<< Q_FUNC_INFO;

    if(belogs_to_box(entity) == false)
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

  void tcp_box_view::removeDOV(int entity)
  {
    qDebug()<< Q_FUNC_INFO;

    if(belogs_to_box(entity) == false)
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

  void tcp_box_view::removeDIOV(int entity)
  {
    qDebug()<< Q_FUNC_INFO;

    if(belogs_to_box(entity) == false)
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

  void tcp_box_view::setLabelMinimumWidth(int labelMinimumWidth)
  {
    if(m_labelMinimumWidth == labelMinimumWidth)
      return;

    m_labelMinimumWidth = labelMinimumWidth;
    emit labelMinimumWidthChanged(m_labelMinimumWidth);
  }

  int tcp_box_view::entity() const
  {
    return m_entity;
  }

  int tcp_box_view::numberOfValueViews() const
  {
    return ana_views.size() + dig_views.size();
  }



} // namespace talorion


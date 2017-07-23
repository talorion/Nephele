#include "actvaluegraph.hpp"

#include <core/ManagerLocator.hpp>

#include <gui_system/GuiConstantProvider.hpp>

namespace talorion {

ActValueGraph::ActValueGraph(QWidget *par):
    AbstractSystemView(par),
    m_layout(new QGridLayout()),
    m_entity(EntityManager::invalid_id),
    m_analogPlot(new AnalogPlot()),
    m_histories()
{
    setWindowTitle("Act Data");
    setObjectName("Act Data");

    //m_layout->setColumnMinimumWidth(1, GuiConstantProvider::min_col_with());
    //m_layout->setColumnMinimumWidth(2, GuiConstantProvider::min_col_with());

    m_layout->setMargin(GuiConstantProvider::margin());
    m_layout->setHorizontalSpacing(GuiConstantProvider::horizontal_spaceing());
    setLayout(m_layout);

    m_layout->addWidget(m_analogPlot);

    EventManager& evtMng = ManagerLocator::eventManager();
    connect(&evtMng, SIGNAL(analogSet_component_changed(int)),this,SLOT(changeSetValue(int)));
    connect(&evtMng, SIGNAL(analogAct_component_changed(int)),this,SLOT(changeActValue(int)));

    connect(&evtMng, SIGNAL(plotEntityValue(EntityManager::EntityID)), this, SLOT(setEntity(EntityManager::EntityID)));

    //    m_graphTimer->setInterval(100);
    //    connect(m_graphTimer, SIGNAL(timeout()), this, SLOT(graphTimerTimeout()));
    //    m_graphTimer->start();
}

EntityManager::EntityID ActValueGraph::entity() const
{
    return m_entity;
}

void ActValueGraph::setEntity(const EntityManager::EntityID &en)
{
    if(m_entity == en)
        return;

    if(isPlotableEntity(en) == false)
        return;

    m_entity = en;
    resetPlot(m_entity);
    //updatePlot(m_entity);
}

//  void ActValueGraph::graphTimerTimeout()
//  {
//    for(auto it = m_histories.constBegin(); it != m_histories.constEnd(); ++it){
//        auto en = it.key();
//        updateHistory(en);
//        updatePlot(en);
//      }
//  }

bool ActValueGraph::belongsToMe(EntityManager::EntityID en) const
{
    if(EntityManager::isValid(en) == false )
        return false;

    if(EntityManager::isValid(entity()) == false)
        return false;

    if(en != entity())
        return false;

    return true;
}

bool ActValueGraph::isPlotableEntity(EntityManager::EntityID en) const
{
    if(EntityManager::isValid(en) == false )
        return false;

    EntityManager &eMng = ManagerLocator::entityManager();
    auto hasName      = eMng.entityHasComponent(en, NAME_COMPONENT);
    if(hasName == false)
        return false;

    auto hasAnalogAct = eMng.entityHasComponent(en, ANALOG_ACT_VALUE_COMPONENT);
    if(hasAnalogAct)
        return true;

    auto hasDigitalAct = eMng.entityHasComponent(en, DIGITAL_ACT_VALUE_COMPONENT);
    if(hasDigitalAct)
        return true;

    auto hasAnalogSet = eMng.entityHasComponent(en, ANALOG_SET_VALUE_COMPONENT);
    if(hasAnalogSet)
        return true;

    auto hasDigitalSet = eMng.entityHasComponent(en, DIGITAL_SET_VALUE_COMPONENT);
    if(hasDigitalSet)
        return true;

    return false;
}

void ActValueGraph::resetPlot(EntityManager::EntityID en)
{
    if(isPlotableEntity(en) == false)
        return;

    updateHistory(en);

    EntityManager &eMng = ManagerLocator::entityManager();
    auto name = eMng.get_name_component(en);
    m_analogPlot->setTitle(name);

    //    auto it = m_histories.find(en);
    //    if(it == m_histories.end())
    //      return;
    //    auto history = it.value();
    auto history = getHistory(en);

    m_analogPlot->resetData(history);

    auto lower = getMinValue(en);
    auto upper = getMaxValue(en);

    m_analogPlot->setRanges(lower, upper);

    updatePlot(en);
}

void ActValueGraph::updatePlot(EntityManager::EntityID en)
{
    if(belongsToMe(en) == false)
        return;

    if(isPlotableEntity(en) == false)
        return;

    auto value = getValueToPlot(en);

    m_analogPlot->updatePlot(value);
}

void ActValueGraph::updateHistory(EntityManager::EntityID en)
{
    if(isPlotableEntity(en) == false)
        return;

    auto history = getHistory(en);

    auto dta = getValueToPlot(en);
    if(history->isEmpty()){
        double value = dta.value;
        double time  = dta.key - m_analogPlot->graphPersistanceTime();
        QCPGraphData(time, value);
    }
    history->add(dta);

    //history->sort();
    auto lower = (dta.key - m_analogPlot->graphPersistanceTime());
    history->removeBefore(lower);

}

double ActValueGraph::getMinValue(EntityManager::EntityID en)
{
    static_component_id component = ACT_MIN_COMPONENT;
    EntityManager &eMng = ManagerLocator::entityManager();
    auto hasAnalogAct = eMng.entityHasComponent(en, ANALOG_ACT_VALUE_COMPONENT);
    auto hasDigitalAct = eMng.entityHasComponent(en, DIGITAL_ACT_VALUE_COMPONENT);
    auto hasAnalogSet = eMng.entityHasComponent(en, ANALOG_SET_VALUE_COMPONENT);
    auto hasDigitalSet = eMng.entityHasComponent(en, DIGITAL_SET_VALUE_COMPONENT);

    if(hasDigitalSet)
        return 0;

    if(hasAnalogSet)
        component = SET_MIN_COMPONENT;

    if(hasDigitalAct)
        return 0;

    if(hasAnalogAct)
        component = ACT_MIN_COMPONENT;

    double value = 0;
    auto var = eMng.componentDataForEntity(component, en);
    if(var.canConvert<double>()){
        value = var.value<double>();
    }

    return value;
}

double ActValueGraph::getMaxValue(EntityManager::EntityID en)
{
    static_component_id component = ACT_MAX_COMPONENT;
    EntityManager &eMng = ManagerLocator::entityManager();
    auto hasAnalogAct = eMng.entityHasComponent(en, ANALOG_ACT_VALUE_COMPONENT);
    auto hasDigitalAct = eMng.entityHasComponent(en, DIGITAL_ACT_VALUE_COMPONENT);
    auto hasAnalogSet = eMng.entityHasComponent(en, ANALOG_SET_VALUE_COMPONENT);
    auto hasDigitalSet = eMng.entityHasComponent(en, DIGITAL_SET_VALUE_COMPONENT);

    if(hasDigitalSet)
        return 1;

    if(hasAnalogSet)
        component = SET_MAX_COMPONENT;

    if(hasDigitalAct)
        return 1;

    if(hasAnalogAct)
        component = ACT_MAX_COMPONENT;

    double value = 1;
    auto var = eMng.componentDataForEntity(component, en);
    if(var.canConvert<double>()){
        value = var.value<double>();
    }

    return value;
}

QCPGraphData ActValueGraph::getValueToPlot(EntityManager::EntityID en)
{
    static_component_id component = ANALOG_ACT_VALUE_COMPONENT;
    EntityManager &eMng = ManagerLocator::entityManager();
    auto hasAnalogAct = eMng.entityHasComponent(en, ANALOG_ACT_VALUE_COMPONENT);
    auto hasDigitalAct = eMng.entityHasComponent(en, DIGITAL_ACT_VALUE_COMPONENT);
    auto hasAnalogSet = eMng.entityHasComponent(en, ANALOG_SET_VALUE_COMPONENT);
    auto hasDigitalSet = eMng.entityHasComponent(en, DIGITAL_SET_VALUE_COMPONENT);

    if(hasDigitalSet)
        component = DIGITAL_SET_VALUE_COMPONENT;

    if(hasAnalogSet)
        component = ANALOG_SET_VALUE_COMPONENT;

    if(hasDigitalAct)
        component = DIGITAL_ACT_VALUE_COMPONENT;

    if(hasAnalogAct)
        component = ANALOG_ACT_VALUE_COMPONENT;


    double value = qQNaN();
    auto var = eMng.componentDataForEntity(component, en);
    if(var.canConvert<double>()){
        value = var.value<double>();
    }

    QDateTime t;
    double time  = t.currentMSecsSinceEpoch()/1000.0;
//    QTime t;
//    double time  = t.msecsSinceStartOfDay()/1000.0;

    return QCPGraphData(time, value);
}

graphHistory_t ActValueGraph::getHistory(EntityManager::EntityID en)
{
    auto it = m_histories.find(en);
    if(it == m_histories.end()){
        graphHistory_t history = QSharedPointer< QCPGraphDataContainer >(new QCPGraphDataContainer());
        it = m_histories.insert(en, history);
    }
    return it.value();
}

void ActValueGraph::changeSetValue(EntityManager::EntityID en)
{
    updateHistory(en);
    updatePlot(en);
}

void ActValueGraph::changeActValue(EntityManager::EntityID en)
{
    updateHistory(en);
    updatePlot(en);
}

} // namespace taloroin

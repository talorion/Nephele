#include "flowcontrollerview.hpp"

#include <QWidget>
#include <QGridLayout>
#include <QLabel>
#include <QDoubleSpinBox>
#include <QDateTime>
#include <QTimer>
#include <QDebug>
#include <QtMath>


#include "math.h"
#include "core/EntityManager.hpp"
#include "core/EventManager.hpp"

#include <gui_system/CustomWidgets/AnalogPlot.hpp>
#include <gui_system/CustomWidgets/AnalogControl.hpp>
#include <gui_system/CustomWidgets/AnalogIndicator.hpp>
#include <gui_system/GuiConstantProvider.hpp>

#define bgstyle_ok "talorion--AnalogIndicator { background-color :    "+GuiConstantProvider::bg_color()+";}"
#define bgstyle_warn "talorion--AnalogIndicator { background-color :  "+GuiConstantProvider::bg_color_warn()+";}"
#define bgstyle_error "talorion--AnalogIndicator { background-color : "+GuiConstantProvider::bg_color_Error()+";}"

namespace talorion {

flowControllerView::flowControllerView(int entity, mode md, QWidget* par) :
    analogView(entity, md,  par),
    editSet(NULL),
    editAct(NULL),
    plot(NULL),
    graphTimer(NULL),
    lblName(NULL)
  //graph_persistance_time(20)
  //grap(NULL)
{

    //connect(this, SIGNAL(change_set_value(int,double)),event_manager::get_instance(),SIGNAL(change_analogSet_component(int,double)),Qt::UniqueConnection);
    this->setMouseTracking(true);

    QGridLayout* m_layout = new QGridLayout;
    m_layout->setMargin(GuiConstantProvider::margin());
    m_layout->setHorizontalSpacing(GuiConstantProvider::horizontal_spaceing());
    m_layout->setVerticalSpacing(0);

    auto units = EntityManager::get_instance()->get_units_component(entity);

    int in = 0;
    int out = 0;

    if(md & Output){
        out++;
        editSet = new AnalogControl(units, 2, this);
        auto setMin = EntityManager::get_instance()->get_setMin_component(entity);
        auto setMax = EntityManager::get_instance()->get_setMax_component(entity);
        if((setMax > 0) && (setMin < setMax)){
            editSet->setRange(setMin,setMax);
            editSet->setSingleStep(setMax/50.0);
        }

        if (setMax > 1 && log10(setMax)<=6)
            editSet->setDecimals(6-log10(setMax));

        editSet->setValue(EntityManager::get_instance()->get_analogSetValue_component(entity));


        m_layout->addWidget(editSet,0,1,1,1);

        connect(editSet, SIGNAL(valueChanged(double)), this, SLOT(slot_set_value_changed(double)));
        connect(EventManager::get_instance(),SIGNAL(analogSet_component_changed(int)),this,SLOT(changeSetValue(int)));
    }


    if(md & Input){
        in++;
        editAct = new AnalogIndicator(units, 2, this);
        editAct->setReadOnly(true);
        auto actMax = EntityManager::get_instance()->get_actMax_component(entity);
        if(actMax > 0)
            editAct->setDecimals(6-log10(actMax));

        if(units.isEmpty() == false)
            editAct->setSuffix(" " + units);
        auto actMin = EntityManager::get_instance()->get_actMin_component(entity);
        if((actMax > 0) && (actMin < actMax))
            editAct->setRange(actMin, actMax);
        editAct->setButtonSymbols(QDoubleSpinBox::NoButtons);
        editAct->setMinimumWidth(100);

        if(out >0 )
            m_layout->addWidget(editAct,0,2,1,1);
        else
            m_layout->addWidget(editAct,0,1,1,2);
        connect(EventManager::get_instance(),SIGNAL(analogAct_component_changed(int)),this,SLOT(changeActValue(int)));
    }

    auto lblNameStr = EntityManager::get_instance()->get_name_component(entity).trimmed();
    lblName = new QLabel(lblNameStr,this);
    QRect r = lblName->fontMetrics().boundingRect(lblNameStr);
    //QRect r = label->fontMetrics().boundingRect( lblNameStr ) );
    //qDebug()<<Q_FUNC_INFO<< r.width();
    lblName->setMinimumWidth( r.width() + 15);

    m_layout->addWidget(lblName,0,0,1,1);

    m_layout->setColumnMinimumWidth(1, GuiConstantProvider::min_col_with());
    m_layout->setColumnMinimumWidth(2, GuiConstantProvider::min_col_with());

    this->setLayout(m_layout);
    plot = new AnalogPlot();

    auto actMax = EntityManager::get_instance()->get_actMax_component(entity);
    auto actMin = EntityManager::get_instance()->get_actMin_component(entity);
    plot->setRanges(actMin, actMax);

    graphTimer = new QTimer();
    graphTimer->setInterval(100);
    connect(graphTimer, SIGNAL(timeout()), this, SLOT(newValueTimeout()));
    graphTimer->start();

}

flowControllerView::~flowControllerView()
{
    if(editSet)
        delete editSet;
    if(editAct)
        delete editAct;

    delete lblName;
    delete plot;
    delete graphTimer;

}

int flowControllerView::getLableWidth() const
{
  return lblName->minimumWidth();
}

void flowControllerView::setLabelMinimumWidth(int width)
{
  lblName->setMinimumWidth(width);
}

int flowControllerView::updateLabelMinimumWidth(int width)
{
  auto x = qMax(getLableWidth(), width);
  setLabelMinimumWidth(x);
  //qDebug()<<Q_FUNC_INFO<< getLableWidth();
  return getLableWidth();
}


void flowControllerView::changeActValue(double actValue)
{
    if(!editAct)
        return;

    editAct->setValue(actValue);
    updatePlot(actValue);
    graphTimer->start(); // avoid timeout since we got a new signal
    if (editSet && editAct && qFabs(actValue - editSet->value()) > qFabs(editSet->value()/5.0)+qFabs(editSet->maximum()/100.0))
    {
        editAct->setStyleSheet(bgstyle_error);
        return;
    }
    if (editSet && editAct && qFabs(actValue - editSet->value()) > qFabs(editSet->value()/10.0)+qFabs(editSet->maximum()/100.0))
    {
        editAct->setStyleSheet(bgstyle_warn);
        return;
    }
    editAct->setStyleSheet(bgstyle_ok);

}

void flowControllerView::changeSetValue(int entity)
{
    if(m_entity<0)
        return;

    if(m_entity != entity)
        return;

    double setValue = EntityManager::get_instance()->get_analogSetValue_component(entity);
    changeSetValue(setValue);
}

void flowControllerView::changeActValue(int entity)
{
    if(m_entity<0)
        return;

    if(m_entity != entity)
        return;

    double actValue = EntityManager::get_instance()->get_analogActValue_component(entity);
    changeActValue(actValue);
}

void flowControllerView::changeSetValue(double setValue)
{
    if(!editSet)
        return;

    if (!editSet->hasFocus())
    {
        bool oldState = editSet->blockSignals(true);
        editSet->setValue(setValue);
        editSet->blockSignals(oldState);
    }
}

void flowControllerView::newValueTimeout()
{
    if(!editAct)
        return;

    updatePlot(editAct->value());
}

void flowControllerView::slot_set_value_changed(double val)
{
    emit change_set_value(m_entity, val);
}

void flowControllerView::updatePlot(double value)
{
    plot->updatePlot(value);
}

void flowControllerView::enterEvent(QEvent * e)
{
    Q_UNUSED(e)
    auto lblNameStr = EntityManager::get_instance()->get_name_component(entity());
    plot->move(QCursor::pos() + QPoint(10,10));
    plot->setTitle(lblNameStr);
    plot->show();
}

void flowControllerView::leaveEvent(QEvent * e)
{
    Q_UNUSED(e)
    if (! plot->underMouse()){
        plot->hide();
    }
}

void flowControllerView::mouseMoveEvent(QMouseEvent* e)
{
    Q_UNUSED(e)
    plot->move(QCursor::pos() + QPoint(10,10));
}
}

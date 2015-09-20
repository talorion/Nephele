#include "flowcontrollerview.hpp"

#include <QWidget>
#include <QGridLayout>
#include <QLabel>
#include <QDoubleSpinBox>
#include <QDateTime>
#include <QTimer>
#include <QDebug>



#include "math.h"
#include "core/entity_manager.hpp"
#include "core/event_manager.hpp"


namespace talorion {

    flowControllerView::flowControllerView(int entity, mode md, QWidget* par) :
        analogView(entity, md,  par),
        editSet(NULL),
        editAct(NULL),
        plot(NULL),
        graphTimer(NULL),
        lblName(NULL),
        graph_persistance_time(60),
        grap(NULL)
    {

        //connect(this, SIGNAL(change_set_value(int,double)),event_manager::get_instance(),SIGNAL(change_analogSet_component(int,double)),Qt::UniqueConnection);
        this->setMouseTracking(true);

        QGridLayout* m_layout = new QGridLayout;

        if(md & Output){
            editSet = new QDoubleSpinBox(this);
            editSet->setRange(
                        entity_manager::get_instance()->get_setMin_component(entity),
                        entity_manager::get_instance()->get_setMax_component(entity));
            editSet->setSingleStep(entity_manager::get_instance()->get_setMax_component(entity)/50.0);
            editSet->setSuffix(" " + entity_manager::get_instance()->get_units_component(entity));
            if (entity_manager::get_instance()->get_setMax_component(entity) > 1 && log10(entity_manager::get_instance()->get_setMax_component(entity))<=6)
                editSet->setDecimals(6-log10(entity_manager::get_instance()->get_setMax_component(entity)));

            editSet->setValue(entity_manager::get_instance()->get_analogSetValue_component(entity));

            //connect(editSet,SIGNAL(valueChanged(double)),this,SLOT(slot_set_value_changed(double)));

            m_layout->addWidget(editSet,0,1,1,1);

            connect(editSet, SIGNAL(valueChanged(double)), this, SLOT(slot_set_value_changed(double)));
            connect(event_manager::get_instance(),SIGNAL(analogSet_component_changed(int)),this,SLOT(changeSetValue(int)));
        }

        if(md & Input){
            editAct = new QDoubleSpinBox(this);
            editAct->setReadOnly(true);
            editAct->setDecimals(6-log10(entity_manager::get_instance()->get_actMax_component(entity)));
            editAct->setSuffix(" " + entity_manager::get_instance()->get_units_component(entity));
            editAct->setRange(entity_manager::get_instance()->get_actMin_component(entity), entity_manager::get_instance()->get_actMax_component(entity));
            editAct->setButtonSymbols(QDoubleSpinBox::NoButtons);
            editAct->setMinimumWidth(100);
            editAct->setStyleSheet("QDoubleSpinBox { background-color :  lightGray;}");

            m_layout->addWidget(editAct,0,2,1,1);

            //connect(editSet,SIGNAL(valueChanged(double)),this,SLOT(slot_set_value_changed(double)));
            connect(event_manager::get_instance(),SIGNAL(analogAct_component_changed(int)),this,SLOT(changeActValue(int)));

        }

        lblName = new QLabel(entity_manager::get_instance()->get_name_component(entity),this);

        m_layout->addWidget(lblName,0,0,1,1);


        m_layout->setMargin(0);
        this->setLayout(m_layout);

        plot = new QCustomPlot();
        grap= plot->addGraph();
        plot->setWindowFlags(Qt::ToolTip);

        plot->setFixedHeight(100);
        plot->setFixedWidth(400);
        double lowerRange = entity_manager::get_instance()->get_actMin_component(entity) - (entity_manager::get_instance()->get_actMax_component(entity)-entity_manager::get_instance()->get_actMin_component(entity))/10;
        double upperRange = entity_manager::get_instance()->get_actMax_component(entity)
                + (entity_manager::get_instance()->get_actMax_component(entity)
                   - entity_manager::get_instance()->get_actMin_component(entity))/10;
        plot->yAxis->setRangeLower(lowerRange);
        plot->yAxis->setRangeUpper(upperRange);
        plot->yAxis->setAutoTickCount(2);
        plot->xAxis->setTickLabelType(QCPAxis::ltDateTime);
        plot->xAxis->setAutoTickCount(2);
        plot->xAxis->setDateTimeFormat("hh:mm:ss");
        plot->xAxis->setRangeLower(QDateTime::currentMSecsSinceEpoch()/1000.0);
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


    void flowControllerView::changeActValue(double actValue)
    {
        if(!editAct)
            return;

        editAct->setValue(actValue);
        updatePlot(actValue);
        graphTimer->start(); // avoid timeout since we got a new signal
    }

    void flowControllerView::changeSetValue(int entity)
    {
        if(m_entity<0)
            return;

        if(m_entity != entity)
            return;

        double setValue = entity_manager::get_instance()->get_analogSetValue_component(entity);
        changeSetValue(setValue);
    }

    void flowControllerView::changeActValue(int entity)
    {
        if(m_entity<0)
            return;

        if(m_entity != entity)
            return;

        double actValue = entity_manager::get_instance()->get_analogActValue_component(entity);
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



    //#define graph_persistance_time 60
    void flowControllerView::updatePlot(double value)
    {
        //QDateTime* t = new QDateTime();
        QDateTime t;
        double time = t.currentMSecsSinceEpoch()/1000.0;
        grap->addData(time, value);
        plot->xAxis->setRangeUpper(time);
        plot->xAxis->setRangeLower(time - getGraph_persistance_time());
        grap->removeDataBefore(time - getGraph_persistance_time());
        plot->replot();
    }

    int flowControllerView::getGraph_persistance_time() const
    {
        return graph_persistance_time;
    }


    void flowControllerView::enterEvent(QEvent * e)
    {
        Q_UNUSED(e)
        plot->move(QCursor::pos() + QPoint(10,10));
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

#include "flowcontrollerview.hpp"

#include <QWidget>
#include <QGridLayout>
#include <QLabel>
#include <QDoubleSpinBox>
#include <QDateTime>
#include <QTimer>
#include <QDebug>

#include <qcustomplot.hpp>

#include "math.h"
#include "core/entity_manager.hpp"
#include "core/event_manager.hpp"


namespace talorion {

    flowControllerView::flowControllerView(int entity,QWidget* par) :
        analogView(entity, par),
        editSet(NULL),
        editAct(NULL),
        plot(NULL),
        graphTimer(NULL),
        lblName(NULL)
    {

        connect(this, SIGNAL(change_set_value(int,double)),event_manager::get_instance(),SIGNAL(change_analogSet_component(int,double)),Qt::UniqueConnection);
        this->setMouseTracking(true);
        editSet = new QDoubleSpinBox(this);
        editSet->setRange(
                    entity_manager::get_instance()->get_setMin_component(entity),
                    entity_manager::get_instance()->get_setMax_component(entity));
        editSet->setSingleStep(entity_manager::get_instance()->get_setMax_component(entity)/50.0);
        editSet->setSuffix(" " + entity_manager::get_instance()->get_units_component(entity));
        if (entity_manager::get_instance()->get_setMax_component(entity) > 1 && log10(entity_manager::get_instance()->get_setMax_component(entity))<=6)
            editSet->setDecimals(6-log10(entity_manager::get_instance()->get_setMax_component(entity)));

        editSet->setValue(entity_manager::get_instance()->get_analogSetValue_component(entity));

        editAct = new QDoubleSpinBox(this);
        editAct->setReadOnly(true);
        editAct->setDecimals(editSet->decimals());
        editAct->setSuffix(editSet->suffix());
        editAct->setRange(entity_manager::get_instance()->get_actMin_component(entity), entity_manager::get_instance()->get_actMax_component(entity));
        editAct->setButtonSymbols(QDoubleSpinBox::NoButtons);
        editAct->setMinimumWidth(100);
        editAct->setStyleSheet("QDoubleSpinBox { background-color :  lightGray;}");

        lblName = new QLabel(entity_manager::get_instance()->get_name_component(entity),this);
        QGridLayout* m_layout = new QGridLayout;
        m_layout->addWidget(lblName,0,0,1,1);
        m_layout->addWidget(editSet,0,1,1,1);
        m_layout->addWidget(editAct,0,2,1,1);
        m_layout->setMargin(0);
        this->setLayout(m_layout);

        plot = new QCustomPlot();
        plot->addGraph();
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

        connect(editSet, SIGNAL(valueChanged(double)), this, SLOT(slot_set_value_changed(double)));

    }

    flowControllerView::~flowControllerView()
    {
        delete editSet;
        delete editAct;
        delete lblName;
        delete plot;
        delete graphTimer;

    }


    void flowControllerView::changeActValue(double actValue)
    {
        editAct->setValue(actValue);
        updatePlot(actValue);
        graphTimer->start(); // avoid timeout since we got a new signal
    }

    void flowControllerView::changeSetValue(double setValue)
    {
        if (!editSet->hasFocus())
        {
            bool oldState = editSet->blockSignals(true);
            editSet->setValue(setValue);
            editSet->blockSignals(oldState);
        }
    }

    void flowControllerView::newValueTimeout()
    {
        updatePlot(editAct->value());
    }

    void flowControllerView::slot_set_value_changed(double val)
    {
        //entity_manager::get_instance()->set_setValue_component(entity, val);
        qDebug()<<"flowControllerView::slot_set_value_changed"<<val;
        //bool oldState = editSet->blockSignals(true);
        emit change_set_value(m_entity, val);
        //editSet->blockSignals(oldState);
    }

    void flowControllerView::updatePlot(double value)
    {
        //QDateTime* t = new QDateTime();
        QDateTime t;
        double time = t.currentMSecsSinceEpoch()/1000.0;
        plot->graph(0)->addData(time, value);
        plot->xAxis->setRangeUpper(time);
        plot->xAxis->setRangeLower(time - 20.0);
        plot->replot();
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
        if (! plot->underMouse())
            plot->hide();
    }

    void flowControllerView::mouseMoveEvent(QMouseEvent* e)
    {
        Q_UNUSED(e)
        plot->move(QCursor::pos() + QPoint(10,10));
    }
}

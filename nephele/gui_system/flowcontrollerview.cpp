#include "flowcontrollerview.hpp"

#include "math.h"
#include "core/entity_manager.hpp"

namespace talorion {

    flowControllerView::flowControllerView(analogValue* ref_analogValue, int hash,QWidget* par) :
        analogView(ref_analogValue,hash, par),
        editSet(NULL),
        editAct(NULL),
        plot(NULL),
        graphTimer(NULL)
    {
        this->setMouseTracking(true);
        editSet = new QDoubleSpinBox(this);
        //        editSet->setRange(data->getSmin(), data->getSmax());
        //        editSet->setSingleStep(data->getSmax()/50.0);
        //        editSet->setSuffix(" " + data->getUnits());
        //        if (data->getSmax() > 1 && log10(data->getSmax())<=6)
        //            editSet->setDecimals(6-log10(data->getSmax()));
        editSet->setRange(
                    entity_manager::get_instance()->get_setMin_component(hashValue),
                    entity_manager::get_instance()->get_setMax_component(hashValue));
        editSet->setSingleStep(entity_manager::get_instance()->get_setMax_component(hashValue)/50.0);
        //editSet->setSuffix(" " + data->getUnits());
        editSet->setSuffix(" " + entity_manager::get_instance()->get_units_component(hashValue));
        if (entity_manager::get_instance()->get_setMax_component(hashValue) > 1 && log10(entity_manager::get_instance()->get_setMax_component(hashValue))<=6)
            editSet->setDecimals(6-log10(entity_manager::get_instance()->get_setMax_component(hashValue)));

        editSet->setValue(entity_manager::get_instance()->get_setValue_component(hashValue));
        //    connect(editSet, SIGNAL(valueChanged(double)), data, SLOT(changeSetByGui(double)));

        editAct = new QDoubleSpinBox(this);
        editAct->setReadOnly(true);
        editAct->setDecimals(editSet->decimals());
        editAct->setSuffix(editSet->suffix());
        //editAct->setRange(data->getAmin(), data->getAmax());
        editAct->setRange(entity_manager::get_instance()->get_actMin_component(hashValue), entity_manager::get_instance()->get_actMax_component(hashValue));

        //QLabel* lblName = new QLabel(data->getName(),this);
        QLabel* lblName = new QLabel(entity_manager::get_instance()->get_name_component(hashValue),this);
        QGridLayout* m_layout = new QGridLayout;
        m_layout->addWidget(lblName,0,0,1,1);
        m_layout->addWidget(editSet,0,1,1,1);
        m_layout->addWidget(editAct,0,2,1,1);
        this->setLayout(m_layout);

        plot = new QCustomPlot();
        plot->addGraph();
        plot->setWindowFlags(Qt::ToolTip);

        plot->setFixedHeight(100);
        plot->setFixedWidth(400);
        //double lowerRange = data->getAmin() - (data->getAmax()-data->getAmin())/10;
        //double upperRange = data->getAmax() + (data->getAmax()-data->getAmin())/10;
        double lowerRange = entity_manager::get_instance()->get_actMin_component(hashValue) - (entity_manager::get_instance()->get_actMax_component(hashValue)-entity_manager::get_instance()->get_actMin_component(hashValue))/10;
        double upperRange = entity_manager::get_instance()->get_actMax_component(hashValue)
                            + (entity_manager::get_instance()->get_actMax_component(hashValue)
                            - entity_manager::get_instance()->get_actMin_component(hashValue))/10;
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

        //connect(data,SIGNAL(actChanged(double)),this,SLOT(changeActValue(double)));
        //connect(data,SIGNAL(setChangedByConnection(double)),this, SLOT(changeSetValue(double)));
        ///connect(editSet, SIGNAL(valueChanged(double)), data, SLOT(changeSetByGui(double)));
        //connect(data,SIGNAL(destroyed()), this, SLOT(deleteLater()));
        connect(editSet, SIGNAL(valueChanged(double)), this, SLOT(slot_set_value_changed(double)));
    }

    flowControllerView::~flowControllerView()
    {

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
            editSet->blockSignals(true);
            editSet->setValue(setValue);
            editSet->blockSignals(false);
        }
    }

    void flowControllerView::newValueTimeout()
    {
        updatePlot(editAct->value());
    }

    void flowControllerView::slot_set_value_changed(double val)
    {
        entity_manager::get_instance()->set_setValue_component(hashValue, val);
    }

    void flowControllerView::updatePlot(double value)
    {
        QDateTime* t = new QDateTime();
        double time = t->currentMSecsSinceEpoch()/1000.0;
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

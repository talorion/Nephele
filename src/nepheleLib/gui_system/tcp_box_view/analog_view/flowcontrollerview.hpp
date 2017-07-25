#ifndef FLOWCONTROLLERVIEW_H
#define FLOWCONTROLLERVIEW_H

#include "analogview.hpp"

QT_BEGIN_NAMESPACE
class QWidget;
class QGridLayout;
class QLabel;
class QDoubleSpinBox;
class QDateTime;
class QTimer;
class QTimer;
QT_END_NAMESPACE

//#include <qcustomplot.h>


namespace talorion {

    class AnalogPlot;
    class AnalogControl;
    class AnalogIndicator;

    class flowControllerView : public analogView
    {
        Q_OBJECT
    public:
        flowControllerView(int m_entity,mode md=InputOutput, QWidget* par = 0);
        ~flowControllerView(void);
        Q_DISABLE_COPY(flowControllerView)

        //int getGraph_persistance_time() const;
        virtual int getLableWidth()const Q_DECL_OVERRIDE;
        virtual void setLabelMinimumWidth(int width) Q_DECL_OVERRIDE;

    signals:
        void setValueChangedByGui(double setValue);
        //void change_set_value(int m_entity, double value);

    public slots:
        virtual void changeActValue(double actValue) Q_DECL_OVERRIDE;
        virtual void changeSetValue(double setValue) Q_DECL_OVERRIDE;
        virtual int updateLabelMinimumWidth(int width) Q_DECL_OVERRIDE;

    private slots:
        void newValueTimeout();
        void slot_set_value_changed(double val);

        void changeSetValue(int entity);
        void changeActValue(int entity);

    private:
        void updatePlot(double value);

    private:
        //QDoubleSpinBox* editSet;
        //QDoubleSpinBox* editAct;
        AnalogControl* editSet;
        AnalogIndicator* editAct;
        //QCustomPlot* plot;
        AnalogPlot* plot;
        QTimer* graphTimer;
        QLabel* lblName;
        //int graph_persistance_time;
        //QCPGraph* grap;

    protected:
        virtual void enterEvent(QEvent* e) Q_DECL_OVERRIDE;
        virtual void leaveEvent(QEvent* e) Q_DECL_OVERRIDE;
        virtual void mouseMoveEvent(QMouseEvent *e) Q_DECL_OVERRIDE;
    };
}

#endif // FLOWCONTROLLERVIEW_H

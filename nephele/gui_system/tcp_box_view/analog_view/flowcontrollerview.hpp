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



#include <qcustomplot.hpp>

namespace talorion {

    class flowControllerView : public analogView
    {
        Q_OBJECT
    public:
        flowControllerView(int m_entity,mode md=InputOutput, QWidget* par = 0);
        ~flowControllerView(void);
        Q_DISABLE_COPY(flowControllerView)

        int getGraph_persistance_time() const;

    signals:
        void setValueChangedByGui(double setValue);
        //void change_set_value(int m_entity, double value);

    public slots:
        virtual void changeActValue(double actValue) Q_DECL_OVERRIDE;
        virtual void changeSetValue(double setValue) Q_DECL_OVERRIDE;

    private slots:
        void newValueTimeout();
        void slot_set_value_changed(double val);

        void changeSetValue(int entity);
        void changeActValue(int entity);

    private:
        void updatePlot(double value);

    private:
        QDoubleSpinBox* editSet;
        QDoubleSpinBox* editAct;
        QCustomPlot* plot;
        QTimer* graphTimer;
        QLabel* lblName;
        int graph_persistance_time;
        QCPGraph* grap;
        QPalette pal;

    protected:
        void enterEvent(QEvent* e);
        void leaveEvent(QEvent* e);
        void mouseMoveEvent(QMouseEvent *e);
    };
}

#endif // FLOWCONTROLLERVIEW_H

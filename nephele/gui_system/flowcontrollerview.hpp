#ifndef FLOWCONTROLLERVIEW_H
#define FLOWCONTROLLERVIEW_H

#include <QWidget>


#include <QGridLayout>
#include <QLabel>
#include <QDoubleSpinBox>

#include <QDateTime>
#include <QTimer>

#include <qcustomplot.hpp>

#include "analogview.hpp"


namespace talorion {

    class flowControllerView : public analogView
    {
        Q_OBJECT
    public:
        flowControllerView(analogValue* ref_analogValue, QWidget* par = 0);
        ~flowControllerView(void);
        Q_DISABLE_COPY(flowControllerView)

    signals:
        void setValueChangedByGui(double setValue);

    public slots:
        void changeActValue(double actValue);
        void changeSetValue(double setValue);
    private slots:
        void newValueTimeout();

    private:
        void updatePlot(double value);

    private:
        QDoubleSpinBox* editSet;
        QDoubleSpinBox* editAct;
        QCustomPlot* plot;
        QTimer* graphTimer;

    protected:
        void enterEvent(QEvent* e);
        void leaveEvent(QEvent* e);
        void mouseMoveEvent(QMouseEvent *e);
    };
}

#endif // FLOWCONTROLLERVIEW_H

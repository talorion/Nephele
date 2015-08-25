#ifndef ANALOGVIEW_H
#define ANALOGVIEW_H

#include <QWidget>

#include "core/analogvalue.hpp"

namespace talorion {

    class analogView : public QWidget
    {
        Q_OBJECT
    public:
        analogView(analogValue* ref_analogValue, QWidget *par = 0);
        ~analogView(void);
        Q_DISABLE_COPY(analogView)

    signals:
        void setValueChangedByGui(double setValue);

    public slots:
        virtual void changeActValue(double actValue) = 0;
        virtual void changeSetValue(double setValue) = 0;
    protected:
        analogValue* data;

    };
}

#endif // ANALOGVIEW_H

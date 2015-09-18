#ifndef DIGITALVIEW_H
#define DIGITALVIEW_H

#include <QGridLayout>
#include <QWidget>

#include "switch_button.hpp"
#include "digital_indicator.hpp"

namespace talorion{
    class digital_view : public QWidget
    {
        Q_OBJECT
    public:
        typedef enum mode_{
            Input = 0x0001,
            Output = 0x0002,
            InputOutput = Input | Output
        }mode;

    public:
        explicit digital_view(int entity, mode md, QWidget *par = 0);
        ~digital_view();
        Q_DISABLE_COPY(digital_view)

    signals:
        void setValueChangedByGui(bool setValue);
        void change_set_value(int m_entity, bool value);

    public slots:
        void changeActValue(bool actValue) ;
        void changeSetValue(bool setValue) ;

    private slots:
        void changeSetValue(int entity) ;
        void slot_set_value_changed(bool val);

    private:
        QGridLayout* m_layout;

        QLabel* lblName;
        switch_button* editSet;
        digital_indicator* editAct;

        const int m_entity;
        mode m_mode;
    };
}

#endif // DIGITALVIEW_H

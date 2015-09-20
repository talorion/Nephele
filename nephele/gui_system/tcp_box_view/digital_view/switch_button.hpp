#ifndef SWITCH_BUTTON_H
#define SWITCH_BUTTON_H

#include <QProgressBar>
#include <QScrollBar>
#include <QWidget>
#include <QSlider>
#include <QPushButton>
#include <QLabel>
#include <QGridLayout>

namespace talorion{
    class switch_button : public QWidget
    {
        Q_OBJECT
    public:
        explicit switch_button(QWidget *par = 0);
        ~switch_button(void);
        Q_DISABLE_COPY(switch_button)

    signals:
        void valueChanged(bool newval);

    public slots:
        void setValue(bool newval);

    private slots:
        void button_pressed(bool checked);

    private:
        QGridLayout* m_layout;
        QPushButton* m_butt;
        QLabel* m_lbl;
    };
}

#endif // SWITCH_BUTTON_H

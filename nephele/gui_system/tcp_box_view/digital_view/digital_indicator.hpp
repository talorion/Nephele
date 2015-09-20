#ifndef DIGITAL_INDICATOR_H
#define DIGITAL_INDICATOR_H

#include <QGridLayout>
#include <QLabel>
#include <QWidget>

namespace talorion{
    class digital_indicator : public QWidget
    {
        Q_OBJECT
    public:
        explicit digital_indicator(QWidget *par = 0);
        ~digital_indicator(void);
        Q_DISABLE_COPY(digital_indicator)

    signals:

    public slots:
        void setValue(bool newval);

    private:
        QGridLayout* m_layout;
        QLabel* m_lbl;
    };
}

#endif // DIGITAL_INDICATOR_H

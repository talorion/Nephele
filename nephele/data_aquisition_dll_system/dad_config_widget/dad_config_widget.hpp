#ifndef TALORION_DAD_CONFIG_WIDGET_HPP
#define TALORION_DAD_CONFIG_WIDGET_HPP

#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QWidget>

#include "core/abstract_configuration_widget.hpp"

namespace talorion {

    class dad_config_widget : public abstract_configuration_widget
    {
        Q_OBJECT
    public:
        explicit dad_config_widget(QWidget *par = 0);
        ~dad_config_widget();
        Q_DISABLE_COPY(dad_config_widget)

    signals:

    private slots:
        void browse_button_pressed();

    private:
        QGridLayout* mainLayout;
        QLabel* name_label;
        QLineEdit* dll_name_field;
        QPushButton* browse_button;
    };

} // namespace talorion

#endif // TALORION_DAD_CONFIG_WIDGET_HPP

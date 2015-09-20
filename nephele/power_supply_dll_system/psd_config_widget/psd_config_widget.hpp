#ifndef TALORION_PSD_CONFIG_WIDGET_HPP
#define TALORION_PSD_CONFIG_WIDGET_HPP

#include "core/abstract_configuration_widget.hpp"

#include <QDoubleSpinBox>
#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>

namespace talorion {

    class psd_config_widget : public abstract_configuration_widget
    {
        Q_OBJECT
    public:
        explicit psd_config_widget(QWidget *par = 0);
        ~psd_config_widget();
        Q_DISABLE_COPY(psd_config_widget)

    signals:
        void change_data_aquistition_dll_component(int , QString );
        void change_timeout_component(int , int );

    private:
        virtual void do_refresh_data() Q_DECL_OVERRIDE;

    private slots:
        void browse_button_pressed();
        void timout_value_changed(int val);

    private:
        QGridLayout* mainLayout;
        QLabel* name_label;
        QLabel* timeout_label;
        QLineEdit* dll_name_field;
        QPushButton* browse_button;
        int current_entity;
        QSpinBox* timeout_field;
    };

} // namespace talorion

#endif // TALORION_PSD_CONFIG_WIDGET_HPP

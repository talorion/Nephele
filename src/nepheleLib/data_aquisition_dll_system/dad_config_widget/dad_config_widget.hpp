#ifndef TALORION_DAD_CONFIG_WIDGET_HPP
#define TALORION_DAD_CONFIG_WIDGET_HPP

#include <QDoubleSpinBox>
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
        void change_data_aquistition_dll_component(int , QString );
        void change_timeout_component(int , int );
        void change_user_data_updaterate_component(int , int );
        void change_user_data_path_component(int , QString );

    private:
        virtual void do_refresh_data() Q_DECL_OVERRIDE;

    private slots:
        void browse_button_pressed();
        void timout_value_changed(int val);
        void user_data_updaterate_changed(int val);
        void user_data_path_changed(QString val);

    private:
        QGridLayout* mainLayout;
        QLabel* name_label;
        QLabel* timeout_label;
        QLineEdit* dll_name_field;
        QLabel* user_data_updaterate_label;
        QSpinBox* user_data_updaterate_field;
        QSpinBox* timeout_field;
        QPushButton* browse_button;
        QLabel* userDataPath_label;
        QLineEdit* userDataPath_field;
        int current_entity;
        QSpacerItem* horizontal_spacer;
    };

} // namespace talorion

#endif // TALORION_DAD_CONFIG_WIDGET_HPP

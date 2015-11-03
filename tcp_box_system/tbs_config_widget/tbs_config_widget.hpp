#ifndef TALORION_TBS_CONFIG_WIDGET_HPP
#define TALORION_TBS_CONFIG_WIDGET_HPP

#include <QDoubleSpinBox>
#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>
#include <QListWidget>
#include <QPushButton>

#include "abstract_configuration_widget.hpp"
#include "entity_manager/entity_manager_locator.hpp"

namespace talorion {

    class tbs_config_widget : public abstract_configuration_widget
    {
        Q_OBJECT
    public:
        explicit tbs_config_widget(QWidget *par = 0);
        ~tbs_config_widget();
        Q_DISABLE_COPY(tbs_config_widget)

    signals:
        void connect_tcp_box(int entity);
        void disconnect_tcp_box(int entity);
        void change_name_component(int, QString);
//        void change_ip_address_component(int entity, QString value);
//        void change_port_component(int entity, quint16 value);
//        void change_connection_state_component(int entity, bool value);
//        void change_tcp_box_backend_component(int entity, int value);

    private:
        virtual void do_refresh_data() Q_DECL_OVERRIDE;

    private slots:
        void slot_newTcpBox(int entity);
        void slot_add_button_clicked(bool);
        void slot_remove_button_clicked(bool);
        void slot_itemClicked(QListWidgetItem * item);
        void slot_connection_state_changed(int entity);
        void slot_name_component_changed(int entity);

        void update_visibility(bool connected);

        void slot_connect_button_clicked(bool);
        void slot_disconnect_button_clicked(bool);

        void box_name_field_textChanged(const QString name);

    private:
        QListWidget* boxes_list;
        QLabel* box_name_label;
        QLabel* ip_address_label;
        QLabel* port_label;
        QLabel* backend_label;
        QGridLayout* mainLayout;
        QGridLayout* editLayout;
        QLineEdit* box_name_field;
        QLineEdit* ip_address_field;
        //QLineEdit* backend_field;
        QSpinBox* backend_field;
        QSpinBox* portField;

        QPushButton* add_button;
        QPushButton* remove_button;
        QPushButton* connect_button;
        QPushButton* disconnect_button;

        QMap<QListWidgetItem*,int> list2entity;
        int current_entity;
    };

} // namespace talorion

#endif // TALORION_TBS_CONFIG_WIDGET_HPP

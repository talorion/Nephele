#ifndef TALORION_GUI_SYSTEM_CONFIG_WIDGET_HPP
#define TALORION_GUI_SYSTEM_CONFIG_WIDGET_HPP

#include <QDoubleSpinBox>
#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>
#include <QListWidget>
#include <QPushButton>

#include "core/abstract_configuration_widget.hpp"
#include "core/EntityManager.hpp"

namespace talorion {

    class gui_system_config_widget : public abstract_configuration_widget
    {
        Q_OBJECT
    public:
        explicit gui_system_config_widget(QWidget *par = 0);
        ~gui_system_config_widget();
        Q_DISABLE_COPY(gui_system_config_widget)

    signals:
        //void connect_tcp_box(int entity);
        //void disconnect_tcp_box(int entity);
        void change_name_component(int, QString);

    private:
        virtual void do_refresh_data() Q_DECL_OVERRIDE;

    private slots:
        void slot_newQuickScript(int entity);
        void slot_add_button_clicked(bool);
        void slot_remove_button_clicked(bool);
        void slot_itemClicked(QListWidgetItem * item);
        void slot_connection_state_changed(int entity);
        void slot_name_component_changed(int entity);

        //void update_visibility(bool connected);

//        void slot_connect_button_clicked(bool);
//        void slot_disconnect_button_clicked(bool);

        void quickscript_name_field_textChanged(const QString name);
        void quickscript_path_field_textChanged(const QString name);

    private:
        QListWidget* quickscripts_list;
        QLabel* quickscript_name_label;
        QLabel* path_label;
        //QLabel* port_label;
        //QLabel* backend_label;
        QGridLayout* mainLayout;
        QGridLayout* editLayout;
        QLineEdit* quickscript_name_field;
        QLineEdit* path_field;
        //QLineEdit* backend_field;
        //QSpinBox* backend_field;
        //QSpinBox* portField;

        QPushButton* add_button;
        QPushButton* remove_button;
//        QPushButton* connect_button;
        //QPushButton* disconnect_button;

        QMap<QListWidgetItem*,int> list2entity;
        int current_entity;
    };

} // namespace talorion

#endif // TALORION_GUI_SYSTEM_CONFIG_WIDGET_HPP

#ifndef TALORION_sbs_config_widget_HPP
#define TALORION_sbs_config_widget_HPP

#include <QDoubleSpinBox>
#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>
#include <QListWidget>
#include <QPushButton>

#include <tcp_box_system/tbs_config_widget/boxconfiguredialog.hpp>

#include "core/abstract_configuration_widget.hpp"
#include "core/EntityManager.hpp"

namespace talorion {

    class sbs_config_widget : public abstract_configuration_widget
    {
        Q_OBJECT
    public:
        explicit sbs_config_widget(QWidget *par = 0);
        ~sbs_config_widget();
        Q_DISABLE_COPY(sbs_config_widget)

        QListWidget *getBoxes_list() const;

        QPushButton *getAdd_button() const;

        QPushButton *getRemove_button() const;

        int getCurrent_entity() const;

        QLineEdit *getBox_name_field() const;

        QLineEdit *getIp_address_field() const;

        QSpinBox *getPortField() const;

        QSpinBox *getBackend_field() const;

        QPushButton *getConnect_button() const;

        QPushButton *getDisconnect_button() const;

        EntityManager::EntityID getSelectedBox() const;

        QPushButton *getConfigure_button() const;

    signals:
        void connect_tcp_box(int entity);
        void disconnect_tcp_box(int entity);
        void change_name_component(int, QString);

    private:
        virtual void do_refresh_data() Q_DECL_OVERRIDE;

    private slots:
        void slot_newTcpBox(int entity);
        void slot_add_button_clicked(bool);
        void slot_remove_button_clicked(bool);
        void slot_scan_button_clicked(bool);
        void slot_itemClicked(QListWidgetItem * item);
        void slot_connection_state_changed(int entity);
        void slot_name_component_changed(int entity);

        void update_visibility(bool connected);

        void slot_connect_button_clicked(bool);
        void slot_disconnect_button_clicked(bool);
        void slot_configure_button_clicked(bool);

        void box_name_field_textChanged(const QString name);

        void slot_removeTcpBox(int entity);

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
        QPushButton* scan_button;
        QPushButton* connect_button;
        QPushButton* disconnect_button;
        QPushButton* configure_button;

        QMap<QListWidgetItem*,int> list2entity;
        int current_entity;

        BoxConfigureDialog* m_boxCfgDlg;
    };

} // namespace talorion

#endif // TALORION_sbs_config_widget_HPP

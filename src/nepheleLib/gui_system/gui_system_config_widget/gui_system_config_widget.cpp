#include "gui_system_config_widget.hpp"

#include "core/EventManager.hpp"

#include <gui_system/GuiConstantProvider.hpp>

namespace talorion {

    gui_system_config_widget::gui_system_config_widget(QWidget *par) :
        abstract_configuration_widget(par),
        quickscripts_list(NULL),
        quickscript_name_label(NULL),
        path_label(NULL),
        //port_label(NULL),
        //backend_label(NULL),
        mainLayout(NULL),
        editLayout(NULL),
        quickscript_name_field(NULL),
        path_field(NULL),
        //backend_field(NULL),
        //portField(NULL),
        add_button(NULL),
        remove_button(NULL),
        //connect_button(NULL),
        //disconnect_button(NULL),
        list2entity(),
        current_entity(-1)
    {
        connect(EventManager::get_instance(),SIGNAL(newQuickScript(int)),this,SLOT(slot_newQuickScript(int)));
        //connect(event_manager::get_instance(),SIGNAL(connection_state_component_changed(int)),this,SLOT(slot_connection_state_changed(int)));
        //connect(this,SIGNAL(connect_tcp_box(int)),event_manager::get_instance(),SIGNAL(connect_tcp_box(int)));
        //connect(this,SIGNAL(disconnect_tcp_box(int)),event_manager::get_instance(),SIGNAL(disconnect_tcp_box(int)));
        connect(EventManager::get_instance(),SIGNAL(name_component_changed(int)),this,SLOT(slot_name_component_changed(int)));
        //connect(this,SIGNAL(change_name_component(int,QString)),event_manager::get_instance(),SIGNAL(change_

        quickscripts_list = new QListWidget();

        quickscript_name_label=new QLabel(tr("Name:"));
        path_label=new QLabel(tr("Path:"));
//        backend_label=new QLabel(tr("backend:"));
//        port_label=new QLabel(tr("Port:"));

        quickscript_name_field =new QLineEdit();
        path_field =new QLineEdit();
//        backend_field =new QSpinBox();
//        backend_field->setRange(0,65535);
//        backend_field->setSingleStep(1);
//        backend_field->setButtonSymbols( QSpinBox::NoButtons);
//        backend_field->setMinimumWidth(100);

//        portField = new  QSpinBox();
//        portField->setRange(0,65535);
//        portField->setSingleStep(1);
//        portField->setButtonSymbols( QSpinBox::NoButtons);
//        portField->setMinimumWidth(100);

        add_button = new QPushButton(tr("Add"));
        remove_button = new QPushButton(tr("Remove"));
        //connect_button= new QPushButton(tr("Connect"));
        //connect_button->setDisabled(true);
//        disconnect_button= new QPushButton(tr("Disconnect"));
//        disconnect_button->setDisabled(true);

        mainLayout = new QGridLayout();
        mainLayout->addWidget(quickscripts_list,0,0,3,2);
        mainLayout->addWidget(add_button,0,2,1,1);
        mainLayout->addWidget(remove_button,1,2,1,1);
        mainLayout->setMargin(GuiConstantProvider::margin());
        mainLayout->setHorizontalSpacing(GuiConstantProvider::horizontal_spaceing());
        mainLayout->setVerticalSpacing(0);


        editLayout = new QGridLayout();
        editLayout->addWidget(quickscript_name_label,0,0,1,1);
        editLayout->addWidget(quickscript_name_field,0,1,1,7);
        editLayout->addWidget(path_label,1,0,1,1);
        editLayout->addWidget(path_field,1,1,1,4);
        mainLayout->setMargin(GuiConstantProvider::margin());
        mainLayout->setHorizontalSpacing(GuiConstantProvider::horizontal_spaceing());
        mainLayout->setVerticalSpacing(0);

//        editLayout->addWidget(port_label,1, 6,1,1);
//        editLayout->addWidget(portField,1, 7,1,1);

//        editLayout->addWidget(backend_label,2, 0,1,1);
//        editLayout->addWidget(backend_field,2, 1,1,7);

        mainLayout->addLayout(editLayout,4,0,1,2);
        //mainLayout->addWidget(connect_button,4,2,1,1,Qt::AlignTop);
        //mainLayout->addWidget(disconnect_button,4,2,1,1,Qt::AlignVCenter);

        setLayout(mainLayout);

        connect(add_button,SIGNAL(clicked(bool)),this,SLOT(slot_add_button_clicked(bool)));
        connect(remove_button,SIGNAL(clicked(bool)),this,SLOT(slot_remove_button_clicked(bool)));
        connect(quickscripts_list,SIGNAL(itemClicked(QListWidgetItem*)),this,SLOT(slot_itemClicked(QListWidgetItem*)));

        //connect(connect_button,SIGNAL(clicked(bool)),this,SLOT(slot_connect_button_clicked(bool)));
        //connect(disconnect_button,SIGNAL(clicked(bool)),this,SLOT(slot_disconnect_button_clicked(bool)));

        connect(quickscript_name_field, SIGNAL(textChanged(QString)),this,SLOT(quickscript_name_field_textChanged(QString)));
        connect(path_field, SIGNAL(textChanged(QString)),this,SLOT(quickscript_path_field_textChanged(QString)));

        foreach (int id, EntityManager::get_instance()->get_all_quickScripts()) {
            slot_newQuickScript(id);
          }


    }

    gui_system_config_widget::~gui_system_config_widget()
    {
        delete quickscripts_list;
        delete quickscript_name_label;
        delete path_label;
//        delete backend_label;
//        delete port_label;
        delete quickscript_name_field;
        delete path_field;
//        delete backend_field;
//        delete portField;
        delete add_button;
        delete remove_button;
        //delete connect_button;
//        delete disconnect_button;
        delete mainLayout;
        delete editLayout;

        QMap<QListWidgetItem*,int> ::iterator it;
        for(it=list2entity.begin(); it!=list2entity.end(); it++)
        {
            QListWidgetItem* tmp = it.key();
            if(tmp)
                delete tmp;
        }
        list2entity.clear();

    }

    void gui_system_config_widget::do_refresh_data()
    {

    }

    void gui_system_config_widget::slot_newQuickScript(int entity)
    {
        QString itemText =EntityManager::get_instance()->get_name_component(entity);
        QListWidgetItem *newItem = new QListWidgetItem;
        newItem->setText(itemText);
        quickscripts_list->addItem(newItem);
        list2entity.insert(newItem, entity);
    }

    void gui_system_config_widget::slot_add_button_clicked(bool)
    {
        QString name = "Quick Script "+QString::number((list2entity.size()+1));

        EntityManager::get_instance()->createQuickScript(name,"");
    }

    void gui_system_config_widget::slot_remove_button_clicked(bool)
    {

    }

    void gui_system_config_widget::slot_itemClicked(QListWidgetItem *item)
    {
        current_entity = -1;
        QMap<QListWidgetItem*,int>::iterator it=list2entity.find(item);
        if(it == list2entity.end())
            return;

        current_entity = it.value();

        quickscript_name_field->setText(EntityManager::get_instance()->get_name_component(current_entity));
        path_field->setText(EntityManager::get_instance()->get_user_data_path_component(current_entity));
        //portField->setValue(entity_manager::get_instance()->get_port_component(current_entity));
        //backend_field->setValue(entity_manager::get_instance()->get_tcp_box_backend_component(current_entity));

        //bool tmp = entity_manager::get_instance()->get_connection_state_component(current_entity);
        //update_visibility(tmp);

    }

    void gui_system_config_widget::slot_connection_state_changed(int entity)
    {
        if(current_entity != entity)
            return;
//        bool tmp = entity_manager::get_instance()->get_connection_state_component(entity);
//        update_visibility(tmp);
    }

    void gui_system_config_widget::slot_name_component_changed(int entity)
    {
        if(current_entity != entity)
            return;
        QString tmp = EntityManager::get_instance()->get_name_component(entity);
        quickscripts_list->currentItem()->setText(tmp);
    }

//    void gui_system_config_widget::update_visibility(bool connected)
//    {
//        quickscript_name_field->setDisabled(connected);
//        path_field->setDisabled(connected);
//        //backend_field->setDisabled(connected);
//        //portField->setDisabled(connected);

//        //connect_button->setDisabled(connected);
//        //disconnect_button->setDisabled(!connected);
//    }

//    void gui_system_config_widget::slot_connect_button_clicked(bool)
//    {
//        //int mode = 0;
//        //if(!backend_field->text().isEmpty())
//        //mode = backend_field->value();

//        if(current_entity>=0){
//            entity_manager::get_instance()->slot_change_name_component(current_entity, quickscript_name_field->text());
//            //entity_manager::get_instance()->slot_change_ip_address_component(current_entity, path_field->text());
//            //entity_manager::get_instance()->slot_change_port_component(current_entity, portField->value());
//            entity_manager::get_instance()->slot_change_tcp_box_backend_component(current_entity, backend_field->value());

//            emit connect_tcp_box(current_entity);
//        }
//    }

//    void gui_system_config_widget::slot_disconnect_button_clicked(bool)
//    {
//        if(current_entity>=0)
//            emit disconnect_tcp_box(current_entity);
//    }

    void gui_system_config_widget::quickscript_name_field_textChanged(const QString name)
    {
      Q_UNUSED(name);
      EntityManager::get_instance()->slot_change_name_component(current_entity, quickscript_name_field->text());
    }

    void gui_system_config_widget::quickscript_path_field_textChanged(const QString name)
    {
      Q_UNUSED(name);
      EntityManager::get_instance()->slot_change_userDataPath_component(current_entity, path_field->text());
    }

} // namespace talorion


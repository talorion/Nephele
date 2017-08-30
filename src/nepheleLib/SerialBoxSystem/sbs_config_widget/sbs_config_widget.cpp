#include "sbs_config_widget.hpp"


#include "core/EventManager.hpp"
#include <QApplication>
#include <QtConcurrent>
#include <core/ManagerLocator.hpp>
#include <gui_system/GuiConstantProvider.hpp>
#include <tcp_box_system/JsonRpcDriver/jsonrpcdriverutils.hpp>

namespace talorion {

sbs_config_widget::sbs_config_widget(QWidget *par) :
    abstract_configuration_widget(par),
    boxes_list(NULL),
    box_name_label(NULL),
    ip_address_label(NULL),
    port_label(NULL),
    backend_label(NULL),
    mainLayout(NULL),
    editLayout(NULL),
    box_name_field(NULL),
    ip_address_field(NULL),
    backend_field(NULL),
    portField(NULL),
    add_button(NULL),
    remove_button(NULL),
    scan_button(Q_NULLPTR),
    connect_button(NULL),
    disconnect_button(NULL),
    configure_button(Q_NULLPTR),
    list2entity(),
    current_entity(EntityManager::invalid_id),
    m_boxCfgDlg(Q_NULLPTR)
{
    connect(EventManager::get_instance(),SIGNAL(newTcpBox(int)),this,SLOT(slot_newTcpBox(int)));
    connect(EventManager::get_instance(),SIGNAL(tcpBoxRemoved(int)),this,SLOT(slot_removeTcpBox(int)));

    connect(EventManager::get_instance(),SIGNAL(connection_state_component_changed(int)),this,SLOT(slot_connection_state_changed(int)));
    connect(this,SIGNAL(connect_tcp_box(int)),EventManager::get_instance(),SIGNAL(connect_tcp_box(int)));
    connect(this,SIGNAL(disconnect_tcp_box(int)),EventManager::get_instance(),SIGNAL(disconnect_tcp_box(int)));
    connect(EventManager::get_instance(),SIGNAL(name_component_changed(int)),this,SLOT(slot_name_component_changed(int)));
    //connect(this,SIGNAL(change_name_component(int,QString)),event_manager::get_instance(),SIGNAL(change_

    boxes_list = new QListWidget();

    box_name_label=new QLabel(tr("Name:"));
    ip_address_label=new QLabel(tr("Host:"));
    backend_label=new QLabel(tr("backend:"));
    port_label=new QLabel(tr("Port:"));

    box_name_field =new QLineEdit();
    ip_address_field =new QLineEdit();
    backend_field =new QSpinBox();
    backend_field->setRange(0,65535);
    backend_field->setSingleStep(1);
    backend_field->setButtonSymbols( QSpinBox::NoButtons);
    backend_field->setMinimumWidth(100);

    portField = new  QSpinBox();
    portField->setRange(0,65535);
    portField->setSingleStep(1);
    portField->setButtonSymbols( QSpinBox::NoButtons);
    portField->setMinimumWidth(100);

    add_button = new QPushButton(tr("Add"));
    remove_button = new QPushButton(tr("Remove"));
    scan_button = new QPushButton(tr("Scan"));
    connect_button= new QPushButton(tr("Connect"));
    connect_button->setDisabled(true);
    disconnect_button= new QPushButton(tr("Disconnect"));
    disconnect_button->setDisabled(true);
    configure_button= new QPushButton(tr("Configure"));
    configure_button->setDisabled(true);

    mainLayout = new QGridLayout();
    mainLayout->setMargin(GuiConstantProvider::margin());
    mainLayout->setHorizontalSpacing(GuiConstantProvider::horizontal_spaceing());
    mainLayout->setVerticalSpacing(0);
    mainLayout->addWidget(boxes_list,0,0,4,2);
    mainLayout->addWidget(add_button,0,2,1,1);
    mainLayout->addWidget(remove_button,1,2,1,1);
    mainLayout->addWidget(scan_button,2,2,1,1);

    editLayout = new QGridLayout();
    editLayout->setMargin(GuiConstantProvider::margin());
    editLayout->setHorizontalSpacing(GuiConstantProvider::horizontal_spaceing());
    editLayout->setVerticalSpacing(0);
    editLayout->addWidget(box_name_label,0,0,1,1);
    editLayout->addWidget(box_name_field,0,1,1,7);
    editLayout->addWidget(ip_address_label,1,0,1,1);
    editLayout->addWidget(ip_address_field,1,1,1,4);
    editLayout->addWidget(port_label,1, 6,1,1);
    editLayout->addWidget(portField,1, 7,1,1);

    editLayout->addWidget(backend_label,2, 0,1,1);
    editLayout->addWidget(backend_field,2, 1,1,7);

    mainLayout->addLayout(editLayout,4,0,1,2);
    mainLayout->addWidget(connect_button,4,2,1,1,Qt::AlignTop);
    mainLayout->addWidget(disconnect_button,4,2,1,1,Qt::AlignVCenter);
    mainLayout->addWidget(configure_button,4,2,1,1,Qt::AlignBottom);

    setLayout(mainLayout);

    connect(add_button,SIGNAL(clicked(bool)),this,SLOT(slot_add_button_clicked(bool)));
    connect(remove_button,SIGNAL(clicked(bool)),this,SLOT(slot_remove_button_clicked(bool)));
    connect(scan_button,SIGNAL(clicked(bool)),this,SLOT(slot_scan_button_clicked(bool)));

    connect(boxes_list,SIGNAL(itemClicked(QListWidgetItem*)),this,SLOT(slot_itemClicked(QListWidgetItem*)));


    connect(connect_button,SIGNAL(clicked(bool)),this,SLOT(slot_connect_button_clicked(bool)));
    connect(disconnect_button,SIGNAL(clicked(bool)),this,SLOT(slot_disconnect_button_clicked(bool)));
    connect(configure_button,SIGNAL(clicked(bool)),this,SLOT(slot_configure_button_clicked(bool)));

    connect(box_name_field, SIGNAL(textChanged(QString)),this,SLOT(box_name_field_textChanged(QString)));

    foreach (int box, EntityManager::get_instance()->get_all_tcpBoxes()) {
        slot_newTcpBox(box);
    }

    //updateVisibility();
    update_visibility(false);

}

sbs_config_widget::~sbs_config_widget()
{
    //QMap<QListWidgetItem*,int>::iterator it;
    auto it =list2entity.begin();
    for(it=list2entity.begin(); it!=list2entity.end(); it++)
    {
        QListWidgetItem* tmp = it.key();
        if(tmp)
            delete tmp;
    }
    list2entity.clear();


    delete boxes_list;
    delete box_name_label;
    delete ip_address_label;
    delete backend_label;
    delete port_label;
    delete box_name_field;
    delete ip_address_field;
    delete backend_field;
    delete portField;
    delete add_button;
    delete remove_button;
    delete connect_button;
    delete disconnect_button;
    delete editLayout;
    delete mainLayout;
}

void sbs_config_widget::do_refresh_data()
{

}

void sbs_config_widget::slot_newTcpBox(int entity)
{
    int trans = EntityManager::get_instance()->getTransportComponent(entity);
    if(trans != 1 )
        return;

    QString itemText =EntityManager::get_instance()->get_name_component(entity);
    QListWidgetItem *newItem = new QListWidgetItem;
    newItem->setText(itemText);
    boxes_list->addItem(newItem);
    list2entity.insert(newItem, entity);
}

void sbs_config_widget::slot_add_button_clicked(bool)
{
    EntityManager::get_instance()->createNewSerialBox();
}

void sbs_config_widget::slot_remove_button_clicked(bool)
{
    auto box = current_entity;
    if(EntityManager::isValid(box)== false)
        return;
    emit disconnect_tcp_box(current_entity);
    EntityManager::get_instance()->removeEntity(box);
    update_visibility(false);
}

void sbs_config_widget::slot_scan_button_clicked(bool)
{
}

void sbs_config_widget::slot_itemClicked(QListWidgetItem *item)
{
    current_entity = EntityManager::invalid_id;
    QMap<QListWidgetItem*,int>::iterator it=list2entity.find(item);
    if(it == list2entity.end())
        return;

    current_entity = it.value();

    box_name_field->setText(EntityManager::get_instance()->get_name_component(current_entity));
    ip_address_field->setText(EntityManager::get_instance()->get_ip_address_component(current_entity));
    portField->setValue(EntityManager::get_instance()->get_port_component(current_entity));
    backend_field->setValue(EntityManager::get_instance()->get_tcp_box_backend_component(current_entity));

    bool tmp = EntityManager::get_instance()->get_connection_state_component(current_entity);
    update_visibility(tmp);

}

void sbs_config_widget::slot_connection_state_changed(int entity)
{
    int trans = EntityManager::get_instance()->getTransportComponent(entity);
    if(trans != 1 )
        return;

    if(current_entity != entity)
        return;
    bool tmp = EntityManager::get_instance()->get_connection_state_component(entity);
    update_visibility(tmp);
}

void sbs_config_widget::slot_name_component_changed(int entity)
{
    int trans = EntityManager::get_instance()->getTransportComponent(entity);
    if(trans != 1 )
        return;

    if(box_name_field->hasFocus())
        return;

    if(current_entity != entity)
        return;
    QString tmp = EntityManager::get_instance()->get_name_component(entity);
    boxes_list->currentItem()->setText(tmp);
}

void sbs_config_widget::update_visibility(bool connected)
{
    auto sel_row = boxes_list->currentRow();
    if(sel_row >= 0){
        remove_button->setDisabled(false);
        box_name_field->setDisabled(connected == false);
        //box_name_field->setDisabled(connected);
        ip_address_field->setDisabled(connected);
        backend_field->setDisabled(connected);
        portField->setDisabled(connected);

        connect_button->setDisabled(connected);
        disconnect_button->setDisabled(!connected);
        configure_button->setDisabled(!connected);
    }else{
        remove_button->setDisabled(true);
        box_name_field->setDisabled(true);
        ip_address_field->setDisabled(true);
        backend_field->setDisabled(true);
        portField->setDisabled(true);

        connect_button->setDisabled(true);
        disconnect_button->setDisabled(true);
        configure_button->setDisabled(true);

        box_name_field->clear();
        ip_address_field->clear();
        backend_field->clear();
        portField->clear();
    }
}

void sbs_config_widget::slot_connect_button_clicked(bool)
{
    //int mode = 0;
    //if(!backend_field->text().isEmpty())
    //mode = backend_field->value();

    if(current_entity>=0){
        EntityManager::get_instance()->slot_change_name_component(current_entity, box_name_field->text());
        EntityManager::get_instance()->slot_change_ip_address_component(current_entity, ip_address_field->text());
        EntityManager::get_instance()->slot_change_port_component(current_entity, portField->value());
        EntityManager::get_instance()->slot_change_tcp_box_backend_component(current_entity, backend_field->value());
        EntityManager::get_instance()->setComponentDataForEntity(AUTO_RECONNECT_COMPONENT, current_entity, QVariant::fromValue(true));

        emit connect_tcp_box(current_entity);
    }
}

void sbs_config_widget::slot_disconnect_button_clicked(bool)
{
    if(current_entity>=0){
        EntityManager::get_instance()->setComponentDataForEntity(AUTO_RECONNECT_COMPONENT, current_entity, QVariant::fromValue(false));
        emit disconnect_tcp_box(current_entity);
    }
}

void sbs_config_widget::slot_configure_button_clicked(bool)
{
    if(EntityManager::isValid(current_entity) == false)
        return;

    if(m_boxCfgDlg == Q_NULLPTR){
        m_boxCfgDlg= new BoxConfigureDialog(current_entity);
    }

    m_boxCfgDlg->setBox(current_entity);
    m_boxCfgDlg->open();

}

void sbs_config_widget::box_name_field_textChanged(const QString name)
{
    //Q_UNUSED(name)
    auto en = current_entity;

    ManagerLocator::entityManager().slot_change_name_component(en, name);
}

void sbs_config_widget::slot_removeTcpBox(int entity)
{
    int trans = EntityManager::get_instance()->getTransportComponent(entity);
    if(trans != 1 )
        return;

    auto it=list2entity.begin();
    for(it = list2entity.begin(); it != list2entity.end(); ++it){
        if(it.value() == entity)
            break;
    }

    if(it == list2entity.end())
        return;

    auto item = it.key();

    list2entity.remove(item);
    boxes_list->removeItemWidget(item);
    delete item;

    //QString itemText =EntityManager::get_instance()->get_name_component(entity);
    //QListWidgetItem *newItem = new QListWidgetItem;
    //newItem->setText(itemText);
    //boxes_list->addItem(newItem);
    //list2entity.insert(newItem, entity);
}

QPushButton *sbs_config_widget::getConfigure_button() const
{
    return configure_button;
}

QPushButton *sbs_config_widget::getDisconnect_button() const
{
    return disconnect_button;
}

EntityManager::EntityID sbs_config_widget::getSelectedBox() const
{
    auto sel_row = boxes_list->currentRow();
    if(sel_row >= 0){
        return current_entity;
    }
    return EntityManager::invalid_id;

}

QPushButton *sbs_config_widget::getConnect_button() const
{
    return connect_button;
}

QSpinBox *sbs_config_widget::getBackend_field() const
{
    return backend_field;
}

QSpinBox *sbs_config_widget::getPortField() const
{
    return portField;
}

QLineEdit *sbs_config_widget::getIp_address_field() const
{
    return ip_address_field;
}

QLineEdit *sbs_config_widget::getBox_name_field() const
{
    return box_name_field;
}

int sbs_config_widget::getCurrent_entity() const
{
    return current_entity;
}

QPushButton *sbs_config_widget::getRemove_button() const
{
    return remove_button;
}

QPushButton *sbs_config_widget::getAdd_button() const
{
    return add_button;
}

QListWidget *sbs_config_widget::getBoxes_list() const
{
    return boxes_list;
}

} // namespace talorion


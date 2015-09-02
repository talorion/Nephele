#include "settings_dialog.hpp"

#include "core/event_manager.hpp"
#include "core/entity_manager.hpp"
#include "core/system_manager.hpp"

#include "core/abstract_configuration_widget.hpp"

namespace talorion {

    settings_dialog::settings_dialog(QWidget *par, Qt::WindowFlags f) :
        QDialog(par,f),
        serach_field(NULL),
        settings_group_list(NULL),
        settings_widget_label(NULL),
        settings_widget(NULL),
        lst_mdl(NULL),
        mainLayout(NULL),
        list2entity()
    {
        connect(event_manager::get_instance(),SIGNAL(newSystem(int)),this,SLOT(newSystem(int)));

        int list_width = 175;

        setMinimumSize(800,600);

        serach_field =  new QLineEdit();
        serach_field->setEnabled(false);
        serach_field->setFixedWidth(list_width);

        settings_group_list =  new QListWidget();
        settings_group_list->setFixedWidth(list_width);
        //settings_group_list->setViewMode(QListView::ListMode);

        settings_widget_label =  new QLabel("my fancy system");
        //settings_widget =  new QWidget();

        connect(settings_group_list,SIGNAL(itemClicked(QListWidgetItem*)),this,SLOT(slot_itemClicked(QListWidgetItem*)));

        //lst_mdl = new QStringListModel();
        //settings_group_list->setModel(lst_mdl);

        mainLayout = new QGridLayout();
        mainLayout->addWidget(serach_field,0,0,1,1);
        mainLayout->addWidget(settings_group_list,1,0,1,1);
        mainLayout->addWidget(settings_widget_label,0,1,1,1);
        //mainLayout->addWidget(settings_widget,1,1,1,1);

        setLayout(mainLayout);
        setWindowTitle(tr("Settings"));

        QList<int> systems =system_manager::get_instance()->get_all_systems();
        foreach (int entity, systems) {
            newSystem(entity);
        }
    }

    settings_dialog::~settings_dialog()
    {

    }

    void settings_dialog::newSystem(int entity)
    {
        QString itemText =entity_manager::get_instance()->get_name_component(entity);

        //QStringList t = lst_mdl->stringList();
        //t.append(nme);
        //lst_mdl->setStringList(t);
        QListWidgetItem *newItem = new QListWidgetItem;
        newItem->setText(itemText);
        settings_group_list->addItem(newItem);
        list2entity.insert(newItem, entity);
    }

    void settings_dialog::slot_itemClicked(QListWidgetItem *item)
    {
        if(settings_widget != NULL){
            mainLayout->removeWidget(settings_widget);
            settings_widget->hide();
        }


        settings_widget_label->setText(item->text());
        QMap<QListWidgetItem*,int>::iterator it=list2entity.find(item);
        if(it == list2entity.end())
            return;

        int entity = it.value();
        settings_widget = entity_manager::get_instance()->get_systemConfigurationWidget_component(entity);

        if(!settings_widget)
            settings_widget = new QWidget();

        settings_widget->show();
        mainLayout->addWidget(settings_widget,1,1,1,1);
    }

} // namespace talorion


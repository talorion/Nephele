#include "settings_dialog.hpp"

#include "core/event_manager.hpp"
#include "core/entity_manager.hpp"

namespace talorion {

    settings_dialog::settings_dialog(QWidget *par, Qt::WindowFlags f) :
        QDialog(par,f),
        serach_field(NULL),
        settings_group_list(NULL),
        settings_widget_label(NULL),
        settings_widget(NULL),
        lst_mdl(NULL),
        mainLayout(NULL)
    {
        connect(event_manager::get_instance(),SIGNAL(newSystem(int)),this,SLOT(newSystem(int)));

        serach_field =  new QLineEdit();
        settings_group_list =  new QListView();
        settings_group_list->setViewMode(QListView::ListMode);
        settings_widget_label =  new QLabel("my fancy system");
        settings_widget =  new QWidget();

        lst_mdl = new QStringListModel();
        settings_group_list->setModel(lst_mdl);

         mainLayout = new QGridLayout();
         mainLayout->addWidget(serach_field,0,0,1,1);
         mainLayout->addWidget(settings_group_list,1,0,1,1);
         mainLayout->addWidget(settings_widget_label,0,1,1,1);
         mainLayout->addWidget(settings_widget,1,1,1,1);

         setLayout(mainLayout);
    }

    settings_dialog::~settings_dialog()
    {

    }

    void settings_dialog::newSystem(int entity)
    {
        QString nme =entity_manager::get_instance()->get_name_component(entity);

        QStringList t = lst_mdl->stringList();
        t.append(nme);
        lst_mdl->setStringList(t);
    }

} // namespace talorion


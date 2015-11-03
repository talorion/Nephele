#include "psd_config_widget.hpp"

#include "event_manager/event_manager_locator.hpp"

#include "entity_manager/entity_manager_locator.hpp"

#include <QFileDialog>

namespace talorion {

    psd_config_widget::psd_config_widget(QWidget *par) :
        abstract_configuration_widget(par),
        mainLayout(NULL),
        name_label(NULL),
        timeout_label(NULL),
        dll_name_field(NULL),
        browse_button(NULL),
        current_entity(-1),
        timeout_field(NULL)
    {
        name_label = new QLabel(tr("TofDaqDll Path"));
        dll_name_field =  new QLineEdit();
        browse_button = new QPushButton(tr("..."));
        connect(browse_button,SIGNAL(clicked(bool)),this,SLOT(browse_button_pressed()));

        timeout_label = new QLabel(tr("Timeout"));
        timeout_field = new QSpinBox();
        timeout_field->setMinimum(0);
        timeout_field->setMaximum(0xFFFF);
        timeout_field->setSuffix(tr(" ms"));
        connect(timeout_field,SIGNAL(valueChanged(int)),this,SLOT(timout_value_changed(int)));


        mainLayout = new QGridLayout();
        mainLayout->addWidget(name_label,0,0,1,1,Qt::AlignTop);
        mainLayout->addWidget(dll_name_field,0,1,7,1,Qt::AlignTop);
        mainLayout->addWidget(browse_button,0,8,1,1,Qt::AlignTop);
        mainLayout->addWidget(timeout_label,1,0,3,1,Qt::AlignTop);
        mainLayout->addWidget(timeout_field,1,1,1,1,Qt::AlignTop);
        //        mainLayout->addWidget(user_data_updaterate_label,2,0,3,1,Qt::AlignTop);
        //        mainLayout->addWidget(user_data_updaterate_field,2,1,1,1,Qt::AlignTop);
        //        mainLayout->addWidget(userDataPath_label,3,0,3,1,Qt::AlignTop);
        //        mainLayout->addWidget(userDataPath_field,3,1,1,1,Qt::AlignTop);
        //        mainLayout->setRowStretch(4,5);

        mainLayout->setMargin(0);
        setLayout(mainLayout);

        connect(this,SIGNAL(change_data_aquistition_dll_component(int,QString)),event_manager_locator::get_instance(),SIGNAL(change_data_aquistition_dll_component(int,QString)));
        connect(this,SIGNAL(change_timeout_component(int,int)),event_manager_locator::get_instance(),SIGNAL(change_timeout_component(int,int)));


        refresh_data();

    }

    psd_config_widget::~psd_config_widget()
    {

    }

    void psd_config_widget::do_refresh_data()
    {
        QList<int> dlls = entity_manager_locator::get_instance()->get_all_TofDaqDlls();
        if(!dlls.isEmpty())
            current_entity =  dlls[0];
        else
            current_entity = entity_manager_locator::get_instance()->createTofDaqDll();

        QString tmp = entity_manager_locator::get_instance()->get_data_aquistion_dll_component(current_entity);
        dll_name_field->setText(tmp);

        int  to =  entity_manager_locator::get_instance()->get_timeout_component(current_entity);
        timeout_field->setValue(to);
    }

    void psd_config_widget::browse_button_pressed()
    {
        QString d = QFileDialog::getOpenFileName(this, tr("Open File"), "", "TofDaqDll (TofDaqDll.dll)");
        if(!d.isNull()){
            dll_name_field->setText(d);
            emit change_data_aquistition_dll_component(current_entity,d);
        }
    }

    void psd_config_widget::timout_value_changed(int val)
    {
        if(current_entity <0)
            return;
        emit change_timeout_component(current_entity,val);
    }

} // namespace talorion


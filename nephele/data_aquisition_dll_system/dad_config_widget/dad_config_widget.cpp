#include "dad_config_widget.hpp"
#include <QFileDialog>

namespace talorion {

    dad_config_widget::dad_config_widget(QWidget *par) :
        abstract_configuration_widget(par),
        mainLayout(NULL),
        name_label(NULL),
        dll_name_field(NULL),
        browse_button(NULL)
    {
        name_label = new QLabel(tr("TofDaqDll Path"));
        dll_name_field =  new QLineEdit();
        browse_button = new QPushButton(tr("..."));
        connect(browse_button,SIGNAL(clicked(bool)),this,SLOT(browse_button_pressed()));

        mainLayout = new QGridLayout();
        mainLayout->addWidget(name_label,0,0,1,1,Qt::AlignTop);
        mainLayout->addWidget(dll_name_field,0,1,1,7,Qt::AlignTop);
        mainLayout->addWidget(browse_button,0,8,1,1,Qt::AlignTop);

        setLayout(mainLayout);
    }

    dad_config_widget::~dad_config_widget()
    {

    }

    void dad_config_widget::browse_button_pressed()
    {
        QString d = QFileDialog::getOpenFileName(NULL, tr("Open File"), "", "TofDaqDll (TofDaqDll.dll)");
        dll_name_field->setText(d);
    }

} // namespace talorion


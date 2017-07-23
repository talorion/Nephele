#include "DataAquisitionConfigurationWidget.hpp"

#include <QFileDialog>

#include "../DaqDllEntity/DaqDllEntityFactory.hpp"

#include <gui_system/GuiConstantProvider.hpp>

namespace talorion {

DataAquisitionConfigurationWidget::DataAquisitionConfigurationWidget(QWidget *par) :
    abstract_configuration_widget(par),
    m_mainLayout(Q_NULLPTR),
    m_name_label(Q_NULLPTR),
    m_timeout_label(Q_NULLPTR),
    m_dll_name_field(Q_NULLPTR),
    m_user_data_updaterate_label(Q_NULLPTR),
    m_user_data_updaterate_field(Q_NULLPTR),
    m_timeout_field(Q_NULLPTR),
    m_browse_button(Q_NULLPTR),
    m_user_data_path_label(Q_NULLPTR),
    m_user_data_path_field(Q_NULLPTR),
    m_horizontal_spacer(Q_NULLPTR),
    m_current_entity(EntityManager::invalid_id),
    m_daqDllEnitity(Q_NULLPTR)
{
    m_name_label = new QLabel(tr("TofDaqDll Path"));
    m_dll_name_field =  new QLineEdit();
    //dll_name_field->setValidator(win_file_validator);
    connect(m_dll_name_field,SIGNAL(editingFinished()), this,SLOT(userDataPathEditFinished()));
    m_browse_button = new QPushButton(tr("..."));
    connect(m_browse_button,SIGNAL(clicked(bool)),this,SLOT(browseButtonPressed()));

    m_timeout_label = new QLabel(tr("Timeout"));
    m_timeout_field = new QSpinBox();
    m_timeout_field->setMinimum(0);
    m_timeout_field->setMaximum(0xFFFF);
    m_timeout_field->setSuffix(tr(" ms"));
    connect(m_timeout_field,SIGNAL(valueChanged(int)),this,SLOT(timoutValueChanged(int)));

    m_user_data_updaterate_label = new QLabel(tr("user data update rate"));
    m_user_data_updaterate_field = new QSpinBox();
    connect(m_user_data_updaterate_field,SIGNAL(valueChanged(int)),this,SLOT(userDataUpdaterateChanged(int)));
    m_user_data_updaterate_field->setMinimum(0);
    m_user_data_updaterate_field->setMaximum(0xFFFF);
    m_user_data_updaterate_field->setSuffix(tr(" ms"));

    m_user_data_path_label = new QLabel(tr("user data path"));
    m_user_data_path_field =  new QLineEdit();
    connect(m_user_data_path_field,SIGNAL(textChanged(QString)),this,SLOT(userDataPathChanged(QString)));

    m_mainLayout = new QGridLayout();
    m_mainLayout->addWidget(m_name_label,0,0,1,1,Qt::AlignTop);
    m_mainLayout->addWidget(m_dll_name_field,0,1,7,1,Qt::AlignTop);
    m_mainLayout->addWidget(m_browse_button,0,8,1,1,Qt::AlignTop);
    m_mainLayout->addWidget(m_timeout_label,1,0,3,1,Qt::AlignTop);
    m_mainLayout->addWidget(m_timeout_field,1,1,1,1,Qt::AlignTop);
    m_mainLayout->addWidget(m_user_data_updaterate_label,2,0,3,1,Qt::AlignTop);
    m_mainLayout->addWidget(m_user_data_updaterate_field,2,1,1,1,Qt::AlignTop);
    m_mainLayout->addWidget(m_user_data_path_label,3,0,3,1,Qt::AlignTop);
    m_mainLayout->addWidget(m_user_data_path_field,3,1,1,1,Qt::AlignTop);
    m_mainLayout->setRowStretch(4,5);

    m_mainLayout->setMargin(GuiConstantProvider::margin());
    m_mainLayout->setHorizontalSpacing(GuiConstantProvider::horizontal_spaceing());
    m_mainLayout->setVerticalSpacing(0);
    setLayout(m_mainLayout);

    refresh_data();
}

void DataAquisitionConfigurationWidget::browseButtonPressed()
{
    QString d = QFileDialog::getOpenFileName(this, tr("Open File"), QString(), "TofDaqDll (TofDaqDll.dll *TofDaqDll.so*)", Q_NULLPTR, QFileDialog::DontUseNativeDialog);
    if(!d.isNull()){
        m_dll_name_field->selectAll();
        m_dll_name_field->insert(d);//Deletes any selected text, inserts newText, and validates the result. If it is valid, it sets it as the new contents of the line edit.
        userDataPathEditFinished();
    }
}

void DataAquisitionConfigurationWidget::timoutValueChanged(int val)
{
    if(m_daqDllEnitity != Q_NULLPTR)
        m_daqDllEnitity->setTimout(val);
}

void DataAquisitionConfigurationWidget::userDataUpdaterateChanged(int val)
{
    if(m_daqDllEnitity != Q_NULLPTR)
        m_daqDllEnitity->setUpdaterate(val);
}

void DataAquisitionConfigurationWidget::userDataPathChanged(QString val)
{
    if(m_daqDllEnitity != Q_NULLPTR )
        m_daqDllEnitity->setUserDataPath(val);
}

void DataAquisitionConfigurationWidget::userDataPathEditFinished()
{
    QString val = m_dll_name_field->text();
    if(m_daqDllEnitity != Q_NULLPTR )
        m_daqDllEnitity->setDllPath(val);
}

void DataAquisitionConfigurationWidget::do_refresh_data()
{
    auto new_id = DaqDllEntityFactory::createNewDaqDllEntity();

    if(m_current_entity != new_id){
        delete m_daqDllEnitity;
        m_current_entity = new_id;
        m_daqDllEnitity = new DaqDllEntity(m_current_entity);
    }

    QString tmp = m_daqDllEnitity->dllPath();
    m_dll_name_field->setText(tmp);

    int  to = m_daqDllEnitity->timeout();
    m_timeout_field->setValue(to);

    int ur = m_daqDllEnitity->updaterate();
    m_user_data_updaterate_field->setValue(ur);

    QString udp = m_daqDllEnitity->userDataPath();
    m_user_data_path_field->setText(udp);
}

QSpinBox* DataAquisitionConfigurationWidget::userDataUpdaterateField() const
{
    return m_user_data_updaterate_field;
}

QLineEdit* DataAquisitionConfigurationWidget::userDataPathField() const
{
    return m_user_data_path_field;
}

QSpinBox* DataAquisitionConfigurationWidget::timeoutField() const
{
    return m_timeout_field;
}

QLineEdit* DataAquisitionConfigurationWidget::dllNameField() const
{
    return m_dll_name_field;
}

} // namespace talorion

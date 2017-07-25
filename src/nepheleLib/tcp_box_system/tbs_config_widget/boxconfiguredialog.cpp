#include "boxconfiguredialog.hpp"

#include "genericmethodwidget.hpp"

#include <tcp_box_system/JsonRpcDriver/jsonrpcdriverutils.hpp>

#include <tcp_box_system/TcpBoxEntity/TcpBoxEntityFacorty.hpp>

#include <gui_system/GuiConstantProvider.hpp>

namespace talorion {

  BoxConfigureDialog::BoxConfigureDialog(EntityManager::EntityID box, QWidget *par, Qt::WindowFlags f) :
    QDialog(par,f),
    serach_field(Q_NULLPTR),
    settings_group_list(Q_NULLPTR),
    settings_widget_label(Q_NULLPTR),
    lst_mdl(Q_NULLPTR),
    mainLayout(Q_NULLPTR),
    list2entity(),
    m_box(EntityManager::invalid_id),
    m_MethodWidget(Q_NULLPTR)
  {
    int list_width = 175;

    setMinimumSize(800,600);

    serach_field =  new QLineEdit();
    serach_field->setEnabled(false);
    serach_field->setFixedWidth(list_width);

    settings_group_list =  new QListWidget();
    settings_group_list->setFixedWidth(list_width);

    settings_widget_label =  new QLabel("please select a Method");

    connect(settings_group_list,SIGNAL(itemClicked(QListWidgetItem*)),this,SLOT(slot_itemClicked(QListWidgetItem*)));

    mainLayout = new QGridLayout();

    mainLayout->setMargin(GuiConstantProvider::margin());
    mainLayout->setHorizontalSpacing(GuiConstantProvider::horizontal_spaceing());
    mainLayout->setVerticalSpacing(0);

    mainLayout->addWidget(serach_field,0,0,1,1);
    mainLayout->addWidget(settings_group_list,1,0,1,1);
    mainLayout->addWidget(settings_widget_label,0,1,1,1);

    setLayout(mainLayout);
    setWindowTitle(tr("Methods"));

    setBox(box);

  }

  void BoxConfigureDialog::setBox(EntityManager::EntityID box)
  {
    if(m_box == box)
      return;

    m_box = box;
    removeAllMethods();
    auto meths = TcpBoxEntityFacorty::getMethodsOfBox(m_box);
    foreach (auto meth, meths) {
        newMethod(m_box, meth);
      }
  }

  void BoxConfigureDialog::removeAllMethods()
  {
    settings_group_list->clear();
    list2entity.clear();
  }

  void BoxConfigureDialog::slot_itemClicked(QListWidgetItem *item)
  {
    if(m_MethodWidget != Q_NULLPTR){
        mainLayout->removeWidget(m_MethodWidget);
        m_MethodWidget->hide();
    }

    auto it = list2entity.find(item);
    if(it == list2entity.end())
        return;

    int method = it.value();

    auto txt = item->text();
    settings_widget_label->setText(txt);

    //m_MethodWidget = EntityManager::get_instance()->get_systemConfigurationWidget_component(entity);

    m_MethodWidget = new GenericMethodWidget(m_box, method);
    //if(m_MethodWidget == Q_NULLPTR)
    //    m_MethodWidget = new empty_config_widget();

    //m_MethodWidget->show();

    mainLayout->addWidget(m_MethodWidget,1,1,1,1);
    m_MethodWidget->refreshData();
  }

  void BoxConfigureDialog::newMethod(EntityManager::EntityID box, EntityManager::EntityID method)
  {
    if(box != m_box)
      return;

    if(EntityManager::isValid(method) == false)
      return;

    QString itemText =EntityManager::get_instance()->get_name_component(method);

    QListWidgetItem *newItem = new QListWidgetItem;
    newItem->setText(itemText);
    settings_group_list->addItem(newItem);
    list2entity.insert(newItem, method);

  }

} // namespace talorion

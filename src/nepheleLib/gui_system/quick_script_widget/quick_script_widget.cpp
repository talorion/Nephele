#include "quick_script_widget.hpp"


#include <QPushButton>
#include <core/EntityManager.hpp>
#include <core/EventManager.hpp>
#include <gui_system/GuiConstantProvider.hpp>

#include <QDebug>


namespace talorion {

  quick_script_widget::quick_script_widget(QWidget *par) :
    QWidget(par),
    signalMapper(NULL),
    gridLayout(NULL),
    buttons()
  {

    this->setObjectName("quick_script_widget");

    signalMapper = new QSignalMapper(this);

    gridLayout = new QGridLayout;
    gridLayout->setMargin(GuiConstantProvider::margin());
    gridLayout->setHorizontalSpacing(GuiConstantProvider::horizontal_spaceing());
    gridLayout->setVerticalSpacing(0);

    connect(signalMapper, SIGNAL(mapped(int)), this, SLOT(clicked(int)));

    connect(this,SIGNAL(start_script_file(QString)),EventManager::get_instance(),SIGNAL(start_script_file(QString)));
    connect(EventManager::get_instance(),SIGNAL(newQuickScript(int)),this,SLOT(add_button(int)));

    setLayout(gridLayout);

    foreach (int id, EntityManager::get_instance()->get_all_quickScripts()) {
        add_button(id);
      }

  }

  void quick_script_widget::clicked(const int &id)
  {
    QString name= EntityManager::get_instance()->get_name_component(id);
    QString path= EntityManager::get_instance()->get_user_data_path_component(id);

    qDebug()<<"starting Quick Script "+name;

    emit start_script_file(path);

  }

  void quick_script_widget::add_button(int id)
  {

    QString text= EntityManager::get_instance()->get_name_component(id);

    QPushButton *button = new QPushButton(text);
    connect(button, SIGNAL(clicked()), signalMapper, SLOT(map()));
    signalMapper->setMapping(button, id);
    int i=buttons.size();
    gridLayout->addWidget(button, i / 3, i % 3);

    buttons.insert(id,button);
  }

  void quick_script_widget::remove_button(int id)
  {
    auto it= buttons.find(id);
    if(it != buttons.end()){
        QPushButton *button = it.value();

        disconnect(button, SIGNAL(clicked()), signalMapper, SLOT(map()));
        gridLayout->removeWidget(button);

        buttons.remove(id);

      }

  }

} // namespace talorion

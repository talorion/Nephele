#include "GuiConstantProvider.hpp"

#include "gui_system.hpp"


#include <QDebug>
#include <QDesktopWidget>
#include <QInputDialog>
#include <QSplashScreen>
#include <QTime>
#include <QApplication>
#include <QDir>
#include <QMessageBox>
#include <QFileDialog>

#include "nephele_main_window.hpp"
#include "core/EventManager.hpp"
#include "core/EntityManager.hpp"

#include "config_file/config_file.hpp"

#include "gui_system_config_widget/gui_system_config_widget.hpp"

#include <core/ManagerLocator.hpp>

#include <gui_system/SystemView/actvaluegraph.hpp>

namespace talorion{
  gui_system::gui_system(QObject *par) :
    QObject(par),
    abstract_system("{93647299-a839-4cb6-ada7-7dabf01f297d}"),
    window(NULL),
    gui_system_cfg_wdgt(NULL),
    m_actions(),
    m_toolbars(),
    m_systemViews()
  {

    QObject::connect(EventManager::get_instance(),SIGNAL(fatal(QString)),this,SLOT(slot_open_info_dialog(QString)));

    gui_system_cfg_wdgt = new gui_system_config_widget();
  }

  gui_system::~gui_system()
  {
    if(window)
      delete window;
  }

  void gui_system::slot_open_numeric_dialog()
  {

    bool ok;
    double d = QInputDialog::getDouble(NULL, tr("QInputDialog::getDouble()"), tr("Amount:"), 37.56, -10000, 10000, 2, &ok);

    if (!ok)
      d=-1;

    emit dialog_finished(d);
  }

  void gui_system::slot_open_string_dialog()
  {
    bool ok;
    QString d = QInputDialog::getText(NULL, tr("QInputDialog::getText()"),tr("User name:"), QLineEdit::Normal,QDir::home().dirName(), &ok);

    if (!ok)
      d="";

    emit dialog_finished(d);
  }

  void gui_system::slot_open_file_dialog()
  {

    QString d = QFileDialog::getOpenFileName(NULL, tr("Open File"), "", "any file (*.*)");

    emit dialog_finished(d);
  }

  void gui_system::slot_open_info_dialog(const QString& msg)
  {
    QMessageBox::information(NULL, tr("Info"),msg);
    emit dialog_finished();
  }

  void gui_system::slot_open_plot_dialog()
  {
    QMessageBox::critical(NULL, tr("Plot Dilaog"),tr("Plot Dialog not implemented"));
  }

  void gui_system::slot_box_disconnected(int boxId)
  {
    QString boxname =  EntityManager::get_instance()->get_name_component(boxId);
    QMessageBox::warning(NULL, "Plot Dilaog", "WARNING!!!! BOX "+boxname+" DISCONNECTED!!!! ADD DATA MAYBE CHANGED!!!!");
  }

  int gui_system::doInitialize()
  {
    qApp->setStyleSheet(GuiConstantProvider::stylesheet());
    ActValueGraph* graph = new ActValueGraph();
    addSystemView(graph);
    return 0;
  }

  int gui_system::doDispose()
  {
    return 0;
  }

  int gui_system::do_start_system()
  {
    config_file *cfg_hdl = new config_file();
    EntityManager::get_instance()->createScriptableObject(cfg_hdl->script_name(), cfg_hdl);
    int W = 250;
    int H = 250;

    int screenWidth;
    int screenHeight;

    int x, y;

    QPixmap pixmap(":/images/images/splash.png");
    QPixmap pixmapForSplash = pixmap.scaledToHeight(600);

    QSplashScreen splash(pixmapForSplash);
    //splash.show();

    //QTime dieTime= QTime::currentTime().addSecs(2); //wait for 2 s
    //while( QTime::currentTime() < dieTime )
    //    QApplication::instance()->processEvents();

    window= new nephele_main_window();
    window->setCfg_hdl(cfg_hdl);
    QDesktopWidget *desktop = QApplication::desktop();

    screenWidth = desktop->width();
    screenHeight = desktop->height();

    x = (screenWidth - W) / 2;
    y = (screenHeight - H) / 2;

    window->resize(W, H);
    window->move( x, y );
    window->readSettings();
    window->show();

    splash.finish(window);

    // connect dialoges
    connect(EventManager::get_instance(),SIGNAL(open_numeric_dialog()),this,SLOT(slot_open_numeric_dialog()));
    connect(EventManager::get_instance(),SIGNAL(open_string_dialog()),this,SLOT(slot_open_string_dialog()));
    connect(EventManager::get_instance(),SIGNAL(open_file_dialog()),this,SLOT(slot_open_file_dialog()));
    connect(EventManager::get_instance(),SIGNAL(open_info_dialog(QString)),this,SLOT(slot_open_info_dialog(QString)));
    connect(EventManager::get_instance(),SIGNAL(open_plot_dialog()),this,SLOT(slot_open_plot_dialog()));

    connect(this, SIGNAL(dialog_finished(double)),EventManager::get_instance(),SIGNAL(dialog_finished(double)));
    connect(this, SIGNAL(dialog_finished(QString)),EventManager::get_instance(),SIGNAL(dialog_finished(QString)));
    connect(this, SIGNAL(dialog_finished()),EventManager::get_instance(),SIGNAL(dialog_finished()));

    connect(EventManager::get_instance(),SIGNAL(disconnect_tcp_box(int)),this,SLOT(slot_box_disconnected(int)));

    // connect dialoges

    return 0;
  }

  abstract_configuration_widget *gui_system::do_get_configuration_widget()
  {
    return gui_system_cfg_wdgt;
  }

  abstract_system::ActionsContainer gui_system::doActions() const
  {
    return m_actions;
  }

  abstract_system::ToolbarsContainer gui_system::doToolbars() const
  {
    return m_toolbars;
  }

  SystemViewsContainer gui_system::doSystemViews() const
  {
    return m_systemViews;
  }

  void gui_system::doProvideEntityManager(EntityManager *service)
  {
    ManagerLocator::provideEntityManager(service);
  }

  void gui_system::doProvideEventManager(EventManager *service)
  {
    ManagerLocator::provideEventManager(service);
  }

  void gui_system::doProvideSystemManager(SystemManager *service)
  {
    ManagerLocator::provideSystemManager(service);
  }

  QCoreApplication *gui_system::doMyQApp() const
  {
    return qApp;
  }


}

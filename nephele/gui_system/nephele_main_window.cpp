#include "nephele_main_window.hpp"

#include <QMessageBox>
#include <QMenuBar>
#include <QStatusBar>
#include <QFileDialog>
#include <QTextStream>
#include <QApplication>
#include <QScriptEngineDebugger>

#include "core/event_manager.hpp"
#include "core/entity_manager.hpp"

#include "flowcontrollerview.hpp"
#include "script_editor/script_editor_window.hpp"
#include "settings_dialog/settings_dialog.hpp"

#include "version.hpp"

namespace talorion {

    nephele_main_window::nephele_main_window(QWidget *par) :
        QMainWindow(par),
        //cmd(NULL),
        //response(NULL),
        mainLayout(NULL),
        //scriptButton(NULL),
        //settingsButton(NULL),
        script_wnd(NULL),
        sett_dlg(NULL),
        fc_views(),
        scriptMenu(NULL),
        toolsMenu(NULL),
        helpMenu(NULL),
        scriptToolBar(NULL),
        scriptEditAct(NULL),
        optionsEditAct(NULL),
        aboutAct(NULL),
        central_wdgt(NULL)
    {



        connect(event_manager::get_instance(),SIGNAL(newAnalogValue(int)),this, SLOT(addAV(int)));
        connect(this, SIGNAL(send_custom_command(QString)),event_manager::get_instance(),SIGNAL(send_custom_command(QString)));
        connect(event_manager::get_instance(),SIGNAL(receivedCustomData(QString)),this,SLOT(displayCustomResponse(QString)));
        connect(event_manager::get_instance(),SIGNAL(analogAct_component_changed(int)),this,SLOT(slot_act_value_changed(int)));
        connect(event_manager::get_instance(),SIGNAL(analogSet_component_changed(int)),this,SLOT(slot_set_value_changed(int)));
        connect(event_manager::get_instance(),SIGNAL(error(QString)),statusBar(),SLOT(showMessage(QString)));

        //script_wnd = new script_editor_window();

        //QLabel* lbl = new QLabel("&Custom Command");
        //cmd = new QLineEdit();
        //lbl->setBuddy(cmd);

        //response = new QLabel();


        //scriptButton = new QPushButton("script");
        //connect(scriptButton,SIGNAL(clicked(bool)),this,SLOT(open_script_window()));

        sett_dlg = new settings_dialog(this);

        //settingsButton = new QPushButton("settings");
        //connect(settingsButton,SIGNAL(clicked(bool)),sett_dlg,SLOT(open()));

        mainLayout = new QGridLayout();
        //mainLayout->addWidget(scriptButton,0,0,1,1);
        //mainLayout->addWidget(settingsButton,0,1,1,1);
        //mainLayout->addWidget(lbl,1,0,1,1);
        //mainLayout->addWidget(cmd,1,1,1,1);
        //mainLayout->addWidget(response,2,0,1,2);

        central_wdgt = new QWidget();
        central_wdgt->setLayout(mainLayout);

        setWindowTitle("Nephele");
        setCentralWidget(central_wdgt);

        createActions();
        createMenus();
        createToolBars();
        createStatusBar();

    }

    nephele_main_window::~nephele_main_window()
    {
        delete sett_dlg;
        delete mainLayout;
        delete centralWidget();
        QMap<int, flowControllerView*>::iterator it;
        for (it=fc_views.begin(); it != fc_views.end(); it++){
             flowControllerView* tmp = it.value();
             if(tmp)
                 delete tmp;
              it.value() = NULL;
        }
        fc_views.clear();

        if(script_wnd)
            delete script_wnd;

        delete scriptEditAct;
        delete optionsEditAct;
        delete aboutAct;
    }

    void nephele_main_window::displayCustomResponse(const QString &cm)
    {
        //response->setText(cm);
        statusBar()->showMessage(cm);
    }

    QString lastcmd = "";
    void nephele_main_window::dispatchCommand()
    {
        //        if (lastcmd != cmd->text())// workaround for Qt bug
        //        {
        //            lastcmd = cmd->text();// workaround for Qt bug
        //            cmd->blockSignals(true); // workaround for Qt bug
        //            emit send_custom_command(cmd->text());
        //            cmd->blockSignals(false); // workaround for Qt bug
        //        }
    }

    void nephele_main_window::addAV(int entity)
    {
        QMap<int, flowControllerView*>::ConstIterator fcv = fc_views.constFind(entity);
        if (fcv == fc_views.constEnd()){
            flowControllerView* tmp = new flowControllerView(entity, this);
            connect(tmp, SIGNAL(change_set_value(int,double)),event_manager::get_instance(),SIGNAL(change_analogSet_component(int,double)));
            fc_views.insert(entity,tmp);
            mainLayout->addWidget(tmp);
        }
    }

    void nephele_main_window::slot_act_value_changed(int entity)
    {
        QMap<int, flowControllerView*>::ConstIterator fcv = fc_views.constFind(entity);
        if (fcv != fc_views.constEnd()){
            double tmp = entity_manager::get_instance()->get_analogActValue_component(entity);
            fcv.value()->changeActValue(tmp);
        }
    }

    void nephele_main_window::slot_set_value_changed(int entity)
    {
        QMap<int, flowControllerView*>::ConstIterator fcv = fc_views.constFind(entity);
        if (fcv != fc_views.constEnd()){
            double tmp = entity_manager::get_instance()->get_analogSetValue_component(entity);
            fcv.value()->changeSetValue(tmp);
        }
    }

    void nephele_main_window::open_script_window()
    {
        if(!script_wnd)
            script_wnd = new script_editor_window();
        script_wnd->show();
        script_wnd->updateModel();
    }

    void nephele_main_window::about()
    {
        QMessageBox::about(this, tr("About Nephele"),
                           tr("<b>Nephele</b> provides a script functionality "
                              "\nBuild: "
                              )+QString::number(BUILD));
    }


    void nephele_main_window::createStatusBar()
    {
        statusBar()->showMessage(tr("Ready"));
    }

    void nephele_main_window::createActions()
    {
        scriptEditAct = new QAction(QIcon(":/images/images/application-javascript.png"), tr("&Script Editor..."), this);
        //scriptEditAct->setShortcuts(QKeySequence::New);
        scriptEditAct->setStatusTip(tr("start script editor"));
        connect(scriptEditAct, SIGNAL(triggered()), this, SLOT(open_script_window()));

        optionsEditAct= new QAction(tr("&Options..."), this);
        optionsEditAct->setStatusTip(tr("Show the application's Options"));
        connect(optionsEditAct, SIGNAL(triggered()), sett_dlg, SLOT(open()));

        aboutAct = new QAction(tr("&About"), this);
        aboutAct->setStatusTip(tr("Show the application's About box"));
        connect(aboutAct, SIGNAL(triggered()), this, SLOT(about()));
    }

    void nephele_main_window::createMenus()
    {
        scriptMenu = menuBar()->addMenu(tr("&Script"));
        scriptMenu->addAction(scriptEditAct);

        toolsMenu = menuBar()->addMenu(tr("&Tools"));
        toolsMenu->addAction(optionsEditAct);

        helpMenu = menuBar()->addMenu(tr("&Help"));
        helpMenu->addAction(aboutAct);

    }

    void nephele_main_window::createToolBars()
    {
        scriptToolBar = addToolBar(tr("Script"));
        scriptToolBar->addAction(scriptEditAct);
    }

}

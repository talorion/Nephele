#include "nephele_main_window.hpp"

#include "core/event_manager.hpp"
#include "core/entity_manager.hpp"

#include "flowcontrollerview.hpp"
#include "script_editor/script_editor_window.hpp"

namespace talorion {

    nephele_main_window::nephele_main_window(QWidget *par) :
        QMainWindow(par),
        cmd(NULL),
        response(NULL),
        mainLayout(NULL),
        scriptButton(NULL),
        script_wnd(NULL),
        fc_views()
    {

        connect(event_manager::get_instance(),SIGNAL(newAnalogValue(int)),this, SLOT(addAV(int)));
        connect(this, SIGNAL(send_custom_command(QString)),event_manager::get_instance(),SIGNAL(send_custom_command(QString)));
        connect(event_manager::get_instance(),SIGNAL(receivedCustomData(QString)),this,SLOT(displayCustomResponse(QString)));
        connect(event_manager::get_instance(),SIGNAL(act_value_changed(int)),this,SLOT(slot_act_value_changed(int)));
        connect(event_manager::get_instance(),SIGNAL(set_value_changed(int)),this,SLOT(slot_set_value_changed(int)));

        script_wnd = new script_editor_window();

        QLabel* lbl = new QLabel("&Custom Command");
        cmd = new QLineEdit();
        lbl->setBuddy(cmd);

        response = new QLabel();

        scriptButton = new QPushButton("script");
        connect(scriptButton,SIGNAL(clicked(bool)),this,SLOT(open_script_window()));

        mainLayout = new QGridLayout();
        mainLayout->addWidget(scriptButton,0,0,1,1);
        mainLayout->addWidget(lbl,1,0,1,1);
        mainLayout->addWidget(cmd,1,1,1,1);
        mainLayout->addWidget(response,2,0,1,2);

        QWidget *central_wdgt = new QWidget();
        central_wdgt->setLayout(mainLayout);

        setWindowTitle("Nephele");
        setCentralWidget(central_wdgt);
    }

    nephele_main_window::~nephele_main_window()
    {

    }

    void nephele_main_window::displayCustomResponse(const QString &cm)
    {
        response->setText(cm);
    }

    QString lastcmd = "";
    void nephele_main_window::dispatchCommand()
    {
        if (lastcmd != cmd->text())// workaround for Qt bug
        {
            lastcmd = cmd->text();// workaround for Qt bug
            cmd->blockSignals(true); // workaround for Qt bug
            emit send_custom_command(cmd->text());
            cmd->blockSignals(false); // workaround for Qt bug
        }
    }

    void nephele_main_window::addAV(int entity)
    {
        QMap<int, flowControllerView*>::ConstIterator fcv = fc_views.constFind(entity);
        if (fcv == fc_views.constEnd()){
            flowControllerView* tmp = new flowControllerView(entity, this);
            connect(tmp, SIGNAL(change_set_value(int,double)),event_manager::get_instance(),SIGNAL(change_set_value(int,double)));
            fc_views.insert(entity,tmp);
            mainLayout->addWidget(tmp);
        }
    }

    void nephele_main_window::slot_act_value_changed(int entity)
    {
        QMap<int, flowControllerView*>::ConstIterator fcv = fc_views.constFind(entity);
        if (fcv != fc_views.constEnd()){
            double tmp = entity_manager::get_instance()->get_actValue_component(entity);
            fcv.value()->changeActValue(tmp);
        }
    }

    void nephele_main_window::slot_set_value_changed(int entity)
    {
        QMap<int, flowControllerView*>::ConstIterator fcv = fc_views.constFind(entity);
        if (fcv != fc_views.constEnd()){
            double tmp = entity_manager::get_instance()->get_setValue_component(entity);
            fcv.value()->changeSetValue(tmp);
        }
    }

    void nephele_main_window::open_script_window()
    {
        if(!script_wnd)
            return;
        script_wnd->show();
    }


}

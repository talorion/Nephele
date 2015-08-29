#include "nephele_main_window.hpp"

#include "core/event_manager.hpp"
#include "core/entity_manager.hpp"



namespace talorion {

    nephele_main_window::nephele_main_window(QWidget *par) :
        QMainWindow(par),
        //dev1(NULL),
        //dcs(NULL),
        cmd(NULL),
        response(NULL),
        mainLayout(NULL),
        scriptButton(NULL),
        script_wnd(NULL),
        fc_views()
    {

        //        tcpDriver* dev1;
        //        qvmbackend* dcs;

        //        dcs = new qvmbackend(this);
        //        dev1 = new tcpDriver("uibkav getAll","uibkav getAll"); // for DC Board
        //        //dev1 = new tcpDriver("uibkafc getAll","uibkafc getActSet"); // for AFC Board

        //        //connect(dcs,SIGNAL(avSetChangeCommand(QByteArray)), dev1, SLOT(setDataCommand(QByteArray)));
        //        connect(dcs,SIGNAL(avSetChangeCommand(QByteArray)),event_manager::get_instance(),SIGNAL(avSetChangeCommand(QByteArray)));
        //        connect(event_manager::get_instance(),SIGNAL(avSetChangeCommand(QByteArray)),dev1,SLOT(setDataCommand(QByteArray)));

        //        //connect(dcs, SIGNAL(newAnalogValue(analogValue*)), this, SLOT(addAV(analogValue*)));
        //        connect(dcs,SIGNAL(newAnalogValue(analogValue*)),event_manager::get_instance(),SIGNAL(newAnalogValue(analogValue*)));
        connect(event_manager::get_instance(),SIGNAL(newAnalogValue(analogValue*)),this, SLOT(addAV(analogValue*)));

        //        //connect(dev1, SIGNAL(receivedData(QVariantMap,tcpDriverDataTypes::dataType)), dcs, SLOT(processData(QVariantMap,tcpDriverDataTypes::dataType)));
        //        connect(dev1, SIGNAL(receivedData(QVariantMap,tcpDriverDataTypes::dataType)),event_manager::get_instance(),SIGNAL(receivedData(QVariantMap,tcpDriverDataTypes::dataType)));
        //        connect(event_manager::get_instance(),SIGNAL(receivedData(QVariantMap,tcpDriverDataTypes::dataType)), dcs, SLOT(processData(QVariantMap,tcpDriverDataTypes::dataType)));

        //        //connect(dev1,SIGNAL(error(QString)), dcs, SLOT(logError(QString)));
        //        connect(dev1,SIGNAL(error(QString)),event_manager::get_instance(),SIGNAL(error(QString)));
        //        connect(event_manager::get_instance(),SIGNAL(error(QString)), dcs, SLOT(logError(QString)));

        //        dev1->connectDevice("192.168.0.90");
        //        //connect(dev1, SIGNAL(receivedCustomData(QByteArray)),this,SLOT(displayCustomResponse(QByteArray)));


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

        //connect(cmd,SIGNAL(editingFinished()),this,SLOT(dispatchCommand()));

        mainLayout = new QGridLayout();
        mainLayout->addWidget(scriptButton,0,0,1,1);
        mainLayout->addWidget(lbl,1,0,1,1);
        mainLayout->addWidget(cmd,1,1,1,1);
        mainLayout->addWidget(response,2,0,1,2);
        //setLayout(mainLayout);

        // Set layout in QWidget
        QWidget *central_wdgt = new QWidget();
        central_wdgt->setLayout(mainLayout);
        //setLayout(mainLayout);

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
            //dev1->customCommand(cmd->text().toLocal8Bit());
            emit send_custom_command(cmd->text());
            cmd->blockSignals(false); // workaround for Qt bug
        }
    }

    void nephele_main_window::addAV(analogValue *av)
    {
        int hash = av->getHashVal();
        QMap<int, flowControllerView*>::ConstIterator fcv = fc_views.constFind(hash);
        if (fcv == fc_views.constEnd()){
            flowControllerView* tmp = new flowControllerView(av, av->getHashVal(), this);
            fc_views.insert(hash,tmp);
            mainLayout->addWidget(tmp);
        }
    }

    void nephele_main_window::slot_act_value_changed(int hash)
    {
        QMap<int, flowControllerView*>::ConstIterator fcv = fc_views.constFind(hash);
        if (fcv != fc_views.constEnd()){
            double tmp = entity_manager::get_instance()->get_actValue_component(hash);
            fcv.value()->changeActValue(tmp);
        }
    }

    void nephele_main_window::slot_set_value_changed(int hash)
    {
        QMap<int, flowControllerView*>::ConstIterator fcv = fc_views.constFind(hash);
        if (fcv != fc_views.constEnd()){
            //double tmp = entity_manager::get_instance()->get_actValue_component(hash);
            double tmp = entity_manager::get_instance()->get_setValue_component(hash);
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

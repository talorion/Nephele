#include "GuiConstantProvider.hpp"
#include "nephele_main_window.hpp"


#include <QMessageBox>
#include <QMenuBar>
#include <QStatusBar>
#include <QFileDialog>
#include <QTextStream>
#include <QApplication>
#include <QScriptEngineDebugger>

#include "core/EventManager.hpp"
#include "core/EntityManager.hpp"
#include "core/ManagerLocator.hpp"

#include "tcp_box_view/tcp_box_view.hpp"
#include "tcp_box_view/analog_view/flowcontrollerview.hpp"
#include "tcp_box_view/digital_view/digital_view.hpp"
#include "script_editor/script_editor_window.hpp"
#include "settings_dialog/settings_dialog.hpp"
#include "quick_script_widget/quick_script_widget.hpp"

//#include "version.hpp"

#include "config_file/config_file.hpp"

#include <gui_system/CustomWidgets/NepheleDockWidget.hpp>
#include <gui_system/CustomWidgets/TitleLabel.hpp>


namespace talorion {

nephele_main_window::nephele_main_window(QWidget *par) :
    QMainWindow(par),
    curFile(""),
    modified(false),
    script_wnd(NULL),
    sett_dlg(NULL),
    m_systemViews(),
    m_systemViewsDocks(),
    fileMenu(NULL),
    scriptMenu(NULL),
    toolsMenu(NULL),
    helpMenu (NULL),
    viewMenu(NULL),
    fileToolBar(NULL),
    scriptToolBar (NULL),
    scriptEditAct (NULL),
    optionsEditAct(NULL),
    newAct(NULL),
    openAct(NULL),
    saveAct(NULL),
    saveAsAct(NULL),
    exitAct (NULL),
    aboutAct(NULL),
    central_wdgt(NULL),
    cfg_hdl(NULL),
    qs_widget(NULL)
{

    //        connect(event_manager::get_instance(),SIGNAL(newAnalogInputValue(int)),this, SLOT(addAIV(int)));
    //        connect(event_manager::get_instance(),SIGNAL(newAnalogOutputValue(int)),this, SLOT(addAOV(int)));
    //        connect(event_manager::get_instance(),SIGNAL(newAnalogValue(int)),this, SLOT(addAV(int)));

    //        connect(event_manager::get_instance(),SIGNAL(newDigitalInputValue(int)),this, SLOT(addDIV(int)));
    //        connect(event_manager::get_instance(),SIGNAL(newDigitalOutputValue(int)),this, SLOT(addDOV(int)));
    //        connect(event_manager::get_instance(),SIGNAL(newDigitalValue(int)),this, SLOT(addDIOV(int)));



    connect(this, SIGNAL(send_custom_command(QString)),EventManager::get_instance(),SIGNAL(send_custom_command(QString)));
    connect(EventManager::get_instance(),SIGNAL(receivedCustomData(QString)),this,SLOT(displayCustomResponse(QString)));
    //connect(event_manager::get_instance(),SIGNAL(analogAct_component_changed(int)),this,SLOT(slot_act_value_changed(int)));
    //connect(event_manager::get_instance(),SIGNAL(analogSet_component_changed(int)),this,SLOT(slot_set_value_changed(int)));
    connect(EventManager::get_instance(),SIGNAL(error(QString)),statusBar(),SLOT(showMessage(QString)));

    connect(this,SIGNAL(change_set_value(int,double)),EventManager::get_instance(),SIGNAL(change_analogSet_component(int,double)));
    //connect(EventManager::get_instance(),SIGNAL(tcpBoxRemoved(int)), this, SLOT(remove_box(int)));

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

    //mainLayout = new QGridLayout();

    //mainLayout->addWidget(scriptButton,0,0,1,1);
    //mainLayout->addWidget(settingsButton,0,1,1,1);
    //mainLayout->addWidget(lbl,1,0,1,1);
    //mainLayout->addWidget(cmd,1,1,1,1);
    //mainLayout->addWidget(response,2,0,1,2);

    //central_wdgt = new TitleLabel("NEPHELE");
    central_wdgt = new QWidget();
    setContentsMargins(0,0,0,0);

    auto m_layout = new QGridLayout();
    m_layout->setMargin(GuiConstantProvider::margin());
    m_layout->setHorizontalSpacing(GuiConstantProvider::horizontal_spaceing());
    m_layout->setVerticalSpacing(0);
    central_wdgt->setLayout(m_layout);

    setWindowTitle("Nephele");

    //setCentralWidget(central_wdgt);
    setCentralWidget(central_wdgt);

    qs_widget = new quick_script_widget();
    qs_widget->setWindowTitle("Quick Scripts");
    NepheleDockWidget *dock = new NepheleDockWidget(qs_widget->windowTitle(),this);
    dock->setWidget(qs_widget);
    dock->setObjectName("QuickScriptsdockWidget");
    //dock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    //if(boxes.isEmpty())
    addDockWidget(Qt::TopDockWidgetArea, dock, Qt::Vertical);

    createActions();
    createMenus();
    createToolBars();
    createStatusBar();

    foreach (int entity, EntityManager::get_instance()->get_all_Systems()) {
        newSystem(entity);
    }
    connect(EventManager::get_instance(),SIGNAL(newSystemView(int)), this, SLOT(newSystem(int)));

//    foreach (int entity, EntityManager::get_instance()->get_all_tcpBoxes()) {
//        add_box(entity);
//    }
//    connect(EventManager::get_instance(),SIGNAL(newTcpBox(int)), this,SLOT(add_box(int)));

}

nephele_main_window::~nephele_main_window()
{
    //delete sett_dlg;
    sett_dlg->deleteLater();
    //delete mainLayout;
    delete centralWidget();
//    QMap<int, flowControllerView*>::iterator it;
//    for (it=fc_views.begin(); it != fc_views.end(); it++){
//        flowControllerView* tmp = it.value();
//        if(tmp)
//            delete tmp;
//        it.value() = NULL;
//    }
//    fc_views.clear();

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

//void nephele_main_window::add_box(int entity)
//{
//    QMap<int, tcp_box_view* >::ConstIterator it = boxes.constFind(entity);
//    if(it == boxes.constEnd()){
//        tcp_box_view* tmp = new tcp_box_view(entity);

//        //mainLayout->addWidget(tmp);
//        //addDockWidget(Qt::RightDockWidgetArea, tmp);
//        //viewMenu->addAction(tmp->toggleViewAction());
//        QString boxname =  EntityManager::get_instance()->get_name_component(entity);

//        NepheleDockWidget *dock = new NepheleDockWidget(tmp->windowTitle(),this);
//        dock->setContentsMargins(0,0,0,0);
//        dock->setWidget(tmp);
//        dock->setObjectName(boxname+"dockWidget");
//        //dock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
//        //if(boxes.isEmpty())
//        addDockWidget(Qt::TopDockWidgetArea, dock, Qt::Vertical);
//        //else
//        //addDockWidget(Qt::BottomDockWidgetArea, dock);
//        viewMenu->addAction(dock->toggleViewAction());

//        boxes.insert(entity, tmp);
//        boxesdocks.insert(entity, dock);
//    }
//}

//void nephele_main_window::remove_box(int entity)
//{
//    auto it = boxesdocks.constFind(entity);
//    if(it == boxesdocks.constEnd())
//        return;

//    auto dock = it.value();
//    viewMenu->removeAction(dock->toggleViewAction());
//    removeDockWidget(dock);

//    delete dock;

//}

//    void nephele_main_window::addAIV(int entity)
//    {
//        QMap<int, flowControllerView*>::ConstIterator fcv = fc_views.constFind(entity);
//        if (fcv == fc_views.constEnd()){
//            flowControllerView* tmp = new flowControllerView(entity,flowControllerView::Input, this);
//            fc_views.insert(entity,tmp);
//            mainLayout->addWidget(tmp);
//        }
//    }

//    void nephele_main_window::addAOV(int entity)
//    {
//        QMap<int, flowControllerView*>::ConstIterator fcv = fc_views.constFind(entity);
//        if (fcv == fc_views.constEnd()){
//            flowControllerView* tmp = new flowControllerView(entity,flowControllerView::Output, this);
//            fc_views.insert(entity,tmp);
//            mainLayout->addWidget(tmp);
//        }
//    }

//    void nephele_main_window::addAV(int entity)
//    {
//        QMap<int, flowControllerView*>::ConstIterator fcv = fc_views.constFind(entity);
//        if (fcv == fc_views.constEnd()){
//            flowControllerView* tmp = new flowControllerView(entity,flowControllerView::InputOutput, this);
//            fc_views.insert(entity,tmp);
//            mainLayout->addWidget(tmp);
//        }
//    }

//    void nephele_main_window::addDIV(int entity)
//    {
//        QMap<int, digital_view*>::ConstIterator fcv = dig_views.constFind(entity);
//        if (fcv == dig_views.constEnd()){
//            digital_view* tmp = new digital_view(entity, digital_view::Input,this);
//            dig_views.insert(entity,tmp);
//            mainLayout->addWidget(tmp);
//        }
//    }

//    void nephele_main_window::addDOV(int entity)
//    {
//        QMap<int, digital_view*>::ConstIterator fcv = dig_views.constFind(entity);
//        if (fcv == dig_views.constEnd()){
//            digital_view* tmp = new digital_view(entity, digital_view::Output,this);
//            dig_views.insert(entity,tmp);
//            mainLayout->addWidget(tmp);
//        }
//    }

//    void nephele_main_window::addDIOV(int entity)
//    {
//        QMap<int, digital_view*>::ConstIterator fcv = dig_views.constFind(entity);
//        if (fcv == dig_views.constEnd()){
//            digital_view* tmp = new digital_view(entity, digital_view::InputOutput,this);
//            dig_views.insert(entity,tmp);
//            mainLayout->addWidget(tmp);
//        }
//    }

//    void nephele_main_window::slot_act_value_changed(int entity)
//    {
//        QMap<int, flowControllerView*>::ConstIterator fcv = fc_views.constFind(entity);
//        if (fcv != fc_views.constEnd()){
//            double tmp = entity_manager::get_instance()->get_analogActValue_component(entity);
//            fcv.value()->changeActValue(tmp);
//        }
//    }

//    void nephele_main_window::slot_set_value_changed(int entity)
//    {
//        QMap<int, flowControllerView*>::ConstIterator fcv = fc_views.constFind(entity);
//        if (fcv != fc_views.constEnd()){
//            double tmp = entity_manager::get_instance()->get_analogSetValue_component(entity);
//            fcv.value()->changeSetValue(tmp);
//        }
//    }

void nephele_main_window::open_script_window()
{
    if(!script_wnd)
        script_wnd = new script_editor_window();
    script_wnd->show();
    script_wnd->updateModel();
}

void nephele_main_window::newFile()
{
    if (maybeSave()) {
        //editor->clear();
        //QMap<int, flowControllerView* > fc_views;
        zero_all();
        setCurrentFile("");
    }
}

void nephele_main_window::open()
{
    if (maybeSave()) {
        QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"), "", "nephele values file (*.json)");
        if (!fileName.isEmpty())
            loadFile(fileName);
    }
}

bool nephele_main_window::save()
{
    if (curFile.isEmpty()) {
        return saveAs();
    } else {
        return saveFile(curFile);
    }
}

bool nephele_main_window::saveAs()
{
    QFileDialog dialog(this, tr("Open File"), "", "nephele values file (*.json)");
    dialog.setWindowModality(Qt::WindowModal);
    dialog.setAcceptMode(QFileDialog::AcceptSave);
    QStringList files;
    if (dialog.exec())
        files = dialog.selectedFiles();
    else
        return false;

    return saveFile(files.at(0));
}

void nephele_main_window::about()
{
    QMessageBox::about(this, tr("About Nephele"),
                       tr("<b>Nephele</b> provides a script functionality "
                          "\nBuild: "
                          //)+QString::number(BUILD));
    )+QString::number(1111));
}


void nephele_main_window::createStatusBar()
{
    statusBar()->showMessage(tr("Ready"));
}

bool nephele_main_window::maybeSave()
{
    //if (editor->document()->isModified()) {
    if (modified) {
        QMessageBox::StandardButton ret;
        ret = QMessageBox::warning(this, tr("Application"),
                                   tr("The document has been modified.\n"
                                      "Do you want to save your changes?"),
                                   QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
        if (ret == QMessageBox::Save)
            return save();
        else if (ret == QMessageBox::Cancel)
            return false;
    }
    return true;
}

void nephele_main_window::loadFile(const QString &fileName)
{

    QFile file(fileName);
    if (!file.open(QFile::ReadOnly)) {
        QMessageBox::warning(this, tr("Application"),
                             tr("Cannot read file %1:\n%2.")
                             .arg(fileName)
                             .arg(file.errorString()));
        return;
    }

    //QTextStream in(&file);
#ifndef QT_NO_CURSOR
    QApplication::setOverrideCursor(Qt::WaitCursor);
#endif
    //editor->setPlainText(in.readAll());
    //========
    //if (!file.open(QIODevice::ReadOnly)) {
    //    QMessageBox::warning(this, tr("Application"),tr("Couldn't open save file."));
    //    return ;
    //}
    //        QByteArray saveData = file.readAll();
    //        QJsonDocument loadDoc(QJsonDocument::fromJson(saveData));

    //        read(loadDoc.object());
    if(! cfg_hdl)
        return;
    cfg_hdl->loadFile(fileName);
    //========
#ifndef QT_NO_CURSOR
    QApplication::restoreOverrideCursor();
#endif

    setCurrentFile(fileName);
    statusBar()->showMessage(tr("File loaded"), 2000);
}

bool nephele_main_window::saveFile(const QString &fileName)
{
    QFile file(fileName);
    if (!file.open(QFile::WriteOnly)) {
        QMessageBox::warning(this, tr("Application"),
                             tr("Cannot write file %1:\n%2.")
                             .arg(fileName)
                             .arg(file.errorString()));
        return false;
    }

    //QTextStream out(&file);
#ifndef QT_NO_CURSOR
    QApplication::setOverrideCursor(Qt::WaitCursor);
#endif
    //out << editor->toPlainText();
    //=========
    //if (!file.open(QIODevice::WriteOnly)) {
    //    QMessageBox::warning(this, tr("Application"), tr("Couldn't open save file."));
    //    return false;
    //}

    //        QJsonObject gameObject;
    //        write(gameObject);
    //        QJsonDocument saveDoc(gameObject);
    //        file.write(saveDoc.toJson());
    if(! cfg_hdl)
        return false;
    cfg_hdl->saveFile(fileName);
    //=========
#ifndef QT_NO_CURSOR
    QApplication::restoreOverrideCursor();
#endif

    setCurrentFile(fileName);
    statusBar()->showMessage(tr("File saved"), 2000);
    return true;
}

void nephele_main_window::setCurrentFile(const QString &fileName)
{
    curFile = fileName;
    //editor->document()->setModified(false);
    modified = false;
    setWindowModified(false);

    QString shownName = curFile;
    if (curFile.isEmpty())
        shownName = "untitled.js";
    //else if(my_engine>=0)
    //    emit change_script_file_component(my_engine, fileName);
    setWindowFilePath(shownName);
}

//    void nephele_main_window::read(const QJsonObject &json)
//    {
//        zero_all();
//        bool ok;
//        //int entity;
//        int box_id;
//        int bid;
//        double val;
//        QJsonObject obj;

//        foreach(QString box_nme, json.keys()){

//            if(json[box_nme].isObject()){
//                obj = json[box_nme].toObject();
//                foreach (QString nme, obj.keys()) {

//                    QList<int> entities =  entity_manager::get_instance()->get_entity_by_name(nme);

//                    foreach (int entity, entities) {
//                        box_id = entity_manager::get_instance()->get_box_id_component(entity);
//                        bid=  box_nme.toInt(&ok);

//                        if(ok && bid == box_id){
//                            val =  obj.value(nme).toDouble(0);
//                            emit change_set_value(entity,val);
//                        }
//                    }
//                }
//            }
//        }

//    }

//    void nephele_main_window::write(QJsonObject &json) const
//    {
//        int box_id;
//        QString box_nme;
//        QJsonObject obj;
//        QString nme;
//        double val;

//        QJsonObject::iterator it;
//        foreach (int entity , entity_manager::get_instance()->get_all_AnalogValues()) {

//            box_id = entity_manager::get_instance()->get_box_id_component(entity);
//            box_nme= QString::number(box_id);

//            it = json.find(box_nme);
//            if(it == json.end())
//                it= json.insert(box_nme, obj);

//            if((*it).isObject())
//                obj = (*it).toObject();

//            nme= entity_manager::get_instance()->get_name_component(entity);
//            val = entity_manager::get_instance()->get_analogSetValue_component(entity);

//            if(!nme.isEmpty()){
//                obj.insert(nme, val);
//            }

//            json.insert(box_nme, obj);
//        }
//    }

void nephele_main_window::zero_all()
{
    //setFocus();
    //        QMap<int, flowControllerView* >::iterator it;
    //        for(it= fc_views.begin(); it != fc_views.end(); it++){
    //        //    int entity =  fcv.key();
    //            if(it.value())
    //                it.value()->clearFocus();
    //        }
    //foreach (int entity , entity_manager::get_instance()->get_all_AnalogValues()) {
    //foreach (int entity , entity_manager::get_instance()->get_all_Values()) {
    //    emit change_set_value(entity,0);
    //}

}

void nephele_main_window::newSystem(int entity)
{

    if(EntityManager::isValid(entity) == false)
        return;

    EntityManager& entMng = ManagerLocator::entityManager();
    QUuid ret;
    auto var = entMng.componentDataForEntity(SERIAL_VERSION_UID_COMPONENT, entity);
    if(var.canConvert<QUuid>()){
        ret = var.value<QUuid>();
    }
    if(ret != entMng.get_System_uid())
        return;

    SystemViewsContainer sysViews;
    var = entMng.componentDataForEntity(SYSTEM_VIEW_WIDGET_COMPONENT, entity);
    if(var.canConvert<SystemViewsContainer>()){
        sysViews = var.value<SystemViewsContainer>();
    }
    if(sysViews.isEmpty())
        return;

    foreach (auto view, sysViews) {
        if(m_systemViews.contains(view))
            continue;

        if(view->dockWidgetAreaHint() == Qt::NoDockWidgetArea)
            setCentralWidget(view);
        else{
            NepheleDockWidget *dock = new NepheleDockWidget(view->windowTitle(),this);
            dock->setWidget(view);
            dock->setObjectName(windowTitle()+"dockWidget");
            addDockWidget(view->dockWidgetAreaHint(), dock, Qt::Vertical);
            viewMenu->addAction(dock->toggleViewAction());

            connect(view, SIGNAL(windowTitleChanged(QString)), dock, SLOT(slotSetWindowTitle(QString)));

            m_systemViews.append(view);
            m_systemViewsDocks.append(dock);
        }

    }

}

void nephele_main_window::setCfg_hdl(config_file *value)
{
    cfg_hdl = value;
}

void nephele_main_window::readSettings()
{
    QSettings settings;
    restoreGeometry(settings.value("geometry").toByteArray());
    restoreState(settings.value("windowState").toByteArray());
}

void nephele_main_window::closeEvent(QCloseEvent *event)
{
    QSettings settings;
    settings.setValue("geometry", saveGeometry());
    settings.setValue("windowState", saveState());
    QMainWindow::closeEvent(event);
}


void nephele_main_window::createActions()
{
    newAct = new QAction(QIcon(":/images/images/new.png"), tr("&New"), this);
    newAct->setShortcuts(QKeySequence::New);
    newAct->setStatusTip(tr("Create a new file"));
    connect(newAct, SIGNAL(triggered()), this, SLOT(newFile()));

    openAct = new QAction(QIcon(":/images/images/open.png"), tr("&Open..."), this);
    openAct->setShortcuts(QKeySequence::Open);
    openAct->setStatusTip(tr("Open an existing file"));
    connect(openAct, SIGNAL(triggered()), this, SLOT(open()));

    saveAct = new QAction(QIcon(":/images/images/save.png"), tr("&Save"), this);
    saveAct->setShortcuts(QKeySequence::Save);
    saveAct->setStatusTip(tr("Save the document to disk"));
    connect(saveAct, SIGNAL(triggered()), this, SLOT(save()));

    saveAsAct = new QAction(tr("Save &As..."), this);
    saveAsAct->setShortcuts(QKeySequence::SaveAs);
    saveAsAct->setStatusTip(tr("Save the document under a new name"));
    connect(saveAsAct, SIGNAL(triggered()), this, SLOT(saveAs()));

    exitAct = new QAction(tr("E&xit"), this);
    exitAct->setShortcuts(QKeySequence::Quit);
    exitAct->setStatusTip(tr("Exit the application"));
    connect(exitAct, SIGNAL(triggered()), this, SLOT(close()));

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
    fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(newAct);
    fileMenu->addAction(openAct);
    fileMenu->addAction(saveAct);
    fileMenu->addAction(saveAsAct);
    fileMenu->addSeparator();
    fileMenu->addAction(exitAct);

    scriptMenu = menuBar()->addMenu(tr("&Script"));
    scriptMenu->addAction(scriptEditAct);

    toolsMenu = menuBar()->addMenu(tr("&Tools"));
    toolsMenu->addAction(optionsEditAct);

    viewMenu = menuBar()->addMenu(tr("&View"));

    menuBar()->addSeparator();

    helpMenu = menuBar()->addMenu(tr("&Help"));
    helpMenu->addAction(aboutAct);

}

void nephele_main_window::createToolBars()
{
    fileToolBar = addToolBar(tr("File"));
    fileToolBar->setObjectName("fileToolBar");
    fileToolBar->addAction(newAct);
    fileToolBar->addAction(openAct);
    fileToolBar->addAction(saveAct);

    scriptToolBar = addToolBar(tr("Script"));
    scriptToolBar->setObjectName("scriptToolBar");
    scriptToolBar->addAction(scriptEditAct);
}

}

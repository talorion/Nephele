#include "script_editor_window.hpp"

#include <QMessageBox>
#include <QMenuBar>
#include <QStatusBar>
#include <QFileDialog>
#include <QTextStream>
#include <QApplication>
#include <QScriptEngineDebugger>
#include <QTreeWidget>
#include <QScriptValue>
#include <QStandardItemModel>
#include <QScriptEngine>
#include <QScriptValueIterator>

#include "core/event_manager.hpp"
#include "core/entity_manager.hpp"

namespace talorion
{
    script_editor_window::script_editor_window(QWidget *par) :
        QMainWindow(par),
        editor(NULL),
        console(NULL),
        layout(NULL),
        highlighter(NULL),
        curFile(),
        fileMenu(NULL),
        runMenu(NULL),
        helpMenu(NULL),
        fileToolBar(NULL),
        runToolBar(NULL),
        newAct(NULL),
        openAct(NULL),
        saveAct(NULL),
        saveAsAct(NULL),
        exitAct(NULL),
        runAct(NULL),
        debugAct(NULL),
        stopAct(NULL),
        skipSleepAct(NULL),
        aboutAct(NULL),
        clsAct(NULL),
        my_engine(-1),
        m_debugger(NULL),
        completer(NULL),
        itemModel(NULL),
        wnd(NULL),
        model(NULL),
        std_items()
    {
        //m_debugger = new QScriptEngineDebugger(this);

        setMinimumSize(800,600);

        setupEditor();
        setupConsole();

        createActions();
        createMenus();
        createToolBars();
        createStatusBar();


        layout = new QVBoxLayout();
        wnd = new QWidget();
        layout->addWidget(editor,3);
        layout->addWidget(console,1);
        wnd->setLayout(layout);
        setCentralWidget(wnd);


        //connect(this,SIGNAL(start_script(QString)),event_manager::get_instance(),SIGNAL(start_script(QString)));
        connect(this,SIGNAL(start_script_file(QString)),event_manager::get_instance(),SIGNAL(start_script_file(QString)));
        connect(this,SIGNAL(debug_script_file(QString)),event_manager::get_instance(),SIGNAL(debug_script_file(QString)));

        connect(this,SIGNAL(change_script_file_component(int,QString)),event_manager::get_instance(),SIGNAL(change_script_file_component(int,QString)));

        connect(event_manager::get_instance(),SIGNAL(newQtScriptEngine(int)),this,SLOT(slot_newQtScriptEngine(int)));

        //QScriptEngineDebugger *m_debugger = new QScriptEngineDebugger(this);

        m_debugger = new QScriptEngineDebugger(this);

        QList<int> script_engines = entity_manager::get_instance()->get_all_Qt_Script_Engines();
        if(!script_engines.isEmpty())
            init_engine(script_engines[0]);

    }

    script_editor_window::~script_editor_window()
    {
        delete m_debugger;
        delete layout;
        delete wnd;

        if(editor)
            delete editor;

        if(highlighter)
            delete highlighter;

        if(completer)
            delete completer;

        delete model;

        foreach (QStandardItem* var, std_items) {
            if(var)
                delete var;
            var=NULL;
        }
        std_items.clear();

        if(console)
            delete console;

        if(openAct)
            delete openAct;

        if(saveAct)
            delete saveAct;

        if(saveAsAct)
            delete saveAsAct;

        if(exitAct)
            delete exitAct;

        if(runAct)
            delete runAct;

        if(debugAct)
            delete debugAct;

        if(stopAct)
            delete stopAct;

        if(skipSleepAct)
            delete skipSleepAct;

        if(aboutAct)
            delete aboutAct;

        if(clsAct)
            delete clsAct;
    }



    void script_editor_window::newFile()
    {
        if (maybeSave()) {
            editor->clear();
            setCurrentFile("");
        }

    }

    void script_editor_window::open()
    {
        if (maybeSave()) {
            QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"), "", "qt script file (*.js)");
            if (!fileName.isEmpty())
                loadFile(fileName);
        }
    }

    bool script_editor_window::save()
    {
        if (curFile.isEmpty()) {
            return saveAs();
        } else {
            return saveFile(curFile);
        }
    }

    bool script_editor_window::saveAs()
    {
        QFileDialog dialog(this, tr("Open File"), "", "qt script file (*.js)");
        dialog.setWindowModality(Qt::WindowModal);
        dialog.setAcceptMode(QFileDialog::AcceptSave);
        QStringList files;
        if (dialog.exec())
            files = dialog.selectedFiles();
        else
            return false;

        return saveFile(files.at(0));
    }

    void script_editor_window::about()
    {
        QMessageBox::about(this, tr("About Nephele"),
                           tr("<b>Nephele</b> provides a script functionality "
                              "bla bla "
                              "bla bla"));
    }

    void script_editor_window::start_script()
    {
        //console->write("Starting "+curFile+"...", "blue");

        if (maybeSave()) {
            emit  start_script_file(curFile);
        }
    }

    void script_editor_window::debug_script()
    {
        if (maybeSave()) {
            if(my_engine >= 0){
                QScriptEngine* eng =  entity_manager::get_instance()->get_qt_script_engine_component(my_engine);
                m_debugger->attachTo(eng);
                m_debugger->action(QScriptEngineDebugger::InterruptAction)->trigger();
            }
            emit  debug_script_file(curFile);
        }
    }

    void script_editor_window::slot_newQtScriptEngine(int entity)
    {
        init_engine(entity);
    }

    //    void script_editor_window::stop_script()
    //    {

    //    }

    void script_editor_window::setupEditor()
    {
        editor = new TextEdit(this);
        setStyleSheet("QTextEdit { font-family: Courier; font-size: 12pt; background-color: 'white'; }");

        highlighter = new Highlighter(editor->document());

        //editor =  m_debugger->widget(QScriptEngineDebugger::CodeWidget);

        completer = new TreeModelCompleter(this);
        updateModel();
        //completer->setMaxVisibleItems(5);
        //completer->setModelSorting(QCompleter::CaseInsensitivelySortedModel);
        completer->setSeparator(".");
        //completer->setWrapAround(true);
        //completer->setCompletionMode(QCompleter::InlineCompletion);
        //completer->setCompletionMode(QCompleter::PopupCompletion);
        editor->setCompleter(completer);

        newFile();

    }

    void script_editor_window::updateModel()
    {
        //completer->setModel(NULL);

        if(itemModel)
            delete itemModel;

        itemModel =  modelFromEngine();
        completer->setModel(itemModel);
    }

    QAbstractItemModel *script_editor_window::modelFromEngine()
    {

        model = new QStandardItemModel(completer);

        if(my_engine >= 0){
            QScriptEngine* eng =  entity_manager::get_instance()->get_qt_script_engine_component(my_engine);

            QStandardItem * rt = model->invisibleRootItem();
            QScriptValue object =eng->globalObject();
            rt->appendColumn(rec_get_values(object));
        }

        return model;
    }

    QList<QStandardItem *> script_editor_window::rec_get_values(QScriptValue object)
    {
        QList<QStandardItem *> items;

        QScriptValueIterator it(object);
        while (it.hasNext()) {
            it.next();
            //qDebug() << it.name() << ": " << it.value().toString();
            QScriptValue v = it.value();
            QStandardItem* qtwi = new QStandardItem();
            std_items.append(qtwi);

            qtwi->setIcon(QIcon(":/images/images/new.png"));
            qtwi->setText(it.name());

            if(v.isFunction())
                qtwi->setIcon(QIcon(":/images/images/func.png"));

            if(v.isQObject()){
                qtwi->setIcon(QIcon(":/images/images/class.png"));
                qtwi->appendColumn(rec_get_values(v));
                //qtwi->addChildren(rec_get_values(v));
            }

            //if(v.isObject()){
            //    qtwi->setIcon(0,QIcon(":/images/images/class.png"));
            //qtwi->addChildren(rec_get_values(v));
            //}

            if(v.isArray() || v.isBool() || v.isBoolean() || v.isDate() || v.isNumber() || v.isString() || v.isVariant()){
                qtwi->setIcon(QIcon(":/images/images/var.png"));
                //qtwi->setText(v.toString());
            }


            items.append(qtwi);
        }
        return items;
    }

    void script_editor_window::setupConsole()
    {
        console = new Console(this);
        console->cls();
    }

    void script_editor_window::createActions()
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

        runAct= new QAction(QIcon(":/images/images/start_script.png"), tr("Start script"), this);
        runAct->setShortcut(Qt::CTRL + Qt::Key_R);
        runAct->setStatusTip(tr("Run"));
        connect(runAct, SIGNAL(triggered()), this, SLOT(start_script()));

        debugAct= new QAction(QIcon(":/images/images/debug_script.png"), tr("Debug script"), this);
        debugAct->setShortcut(Qt::Key_F5);
        debugAct->setStatusTip(tr("Start Debugging"));
        connect(debugAct, SIGNAL(triggered()), this, SLOT(debug_script()));

        stopAct= new QAction(QIcon(":/images/images/stop_script.png"), tr("Stop script"), this);
        stopAct->setStatusTip(tr("Stop"));
        //connect(stopAct, SIGNAL(triggered()), this, SLOT(stop_script()));
        connect(stopAct, SIGNAL(triggered()), event_manager::get_instance(),SIGNAL(abort_script()));

        skipSleepAct = new QAction(QIcon(":/images/images/skip.png"), tr("Skip Sleep"), this);
        skipSleepAct->setStatusTip(tr("Skip Sleep"));
        connect(skipSleepAct, SIGNAL(triggered()), event_manager::get_instance(),SIGNAL(script_skip_sleep()));

        aboutAct = new QAction(tr("&About"), this);
        aboutAct->setStatusTip(tr("Show the application's About box"));
        connect(aboutAct, SIGNAL(triggered()), this, SLOT(about()));

        clsAct = new QAction(QIcon(":/images/images/clear.png"),tr("Clear"), this);
        clsAct->setShortcut(Qt::CTRL + Qt::Key_L);
        clsAct->setStatusTip(tr("clear the console"));
        connect(clsAct, SIGNAL(triggered()), console, SLOT(cls()));

    }

    void script_editor_window::createMenus()
    {
        fileMenu = menuBar()->addMenu(tr("&File"));
        fileMenu->addAction(newAct);
        fileMenu->addAction(openAct);
        fileMenu->addAction(saveAct);
        fileMenu->addAction(saveAsAct);
        fileMenu->addSeparator();
        fileMenu->addAction(exitAct);

        runMenu = menuBar()->addMenu(tr("&Run"));
        runMenu->addAction(runAct);
        runMenu->addAction(debugAct);
        runMenu->addAction(stopAct);
        runMenu->addSeparator();
        runMenu->addAction(skipSleepAct);
        runMenu->addSeparator();
        runMenu->addAction(clsAct);

        menuBar()->addSeparator();

        helpMenu = menuBar()->addMenu(tr("&Help"));
        helpMenu->addAction(aboutAct);
    }

    void script_editor_window::createToolBars()
    {
        fileToolBar = addToolBar(tr("File"));
        fileToolBar->addAction(newAct);
        fileToolBar->addAction(openAct);
        fileToolBar->addAction(saveAct);

        runToolBar = addToolBar(tr("Run"));
        runToolBar->addAction(runAct);
        runToolBar->addAction(debugAct);
        runToolBar->addAction(stopAct);
        runToolBar->addSeparator();
        runToolBar->addAction(skipSleepAct);
        runToolBar->addSeparator();
        runToolBar->addAction(clsAct);
    }

    void script_editor_window::createStatusBar()
    {
        statusBar()->showMessage(tr("Ready"));
    }

    bool script_editor_window::maybeSave()
    {
        if (editor->document()->isModified()) {
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

    void script_editor_window::loadFile(const QString &fileName)
    {
        QFile file(fileName);
        if (!file.open(QFile::ReadOnly | QFile::Text)) {
            QMessageBox::warning(this, tr("Application"),
                                 tr("Cannot read file %1:\n%2.")
                                 .arg(fileName)
                                 .arg(file.errorString()));
            return;
        }

        QTextStream in(&file);
#ifndef QT_NO_CURSOR
        QApplication::setOverrideCursor(Qt::WaitCursor);
#endif
        editor->setPlainText(in.readAll());
#ifndef QT_NO_CURSOR
        QApplication::restoreOverrideCursor();
#endif

        setCurrentFile(fileName);
        statusBar()->showMessage(tr("File loaded"), 2000);
    }

    bool script_editor_window::saveFile(const QString &fileName)
    {
        QFile file(fileName);
        if (!file.open(QFile::WriteOnly | QFile::Text)) {
            QMessageBox::warning(this, tr("Application"),
                                 tr("Cannot write file %1:\n%2.")
                                 .arg(fileName)
                                 .arg(file.errorString()));
            return false;
        }

        QTextStream out(&file);
#ifndef QT_NO_CURSOR
        QApplication::setOverrideCursor(Qt::WaitCursor);
#endif
        out << editor->toPlainText();
#ifndef QT_NO_CURSOR
        QApplication::restoreOverrideCursor();
#endif

        setCurrentFile(fileName);
        statusBar()->showMessage(tr("File saved"), 2000);
        return true;
    }

    void script_editor_window::setCurrentFile(const QString &fileName)
    {
        curFile = fileName;
        editor->document()->setModified(false);
        setWindowModified(false);

        QString shownName = curFile;
        if (curFile.isEmpty())
            shownName = "untitled.js";
        else if(my_engine>=0)
            emit change_script_file_component(my_engine, fileName);
        setWindowFilePath(shownName);
    }

    QString script_editor_window::strippedName(const QString &fullFileName)
    {
        return QFileInfo(fullFileName).fileName();
    }

    void script_editor_window::init_engine(int entity)
    {
        if(entity<0)
            return;

        QString tmp= entity_manager::get_instance()->get_script_file_component(entity);
        if(!tmp.isEmpty())
            loadFile(tmp);

        my_engine = entity;
    }


}

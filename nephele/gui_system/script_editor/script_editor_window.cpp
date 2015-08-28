#include "script_editor_window.hpp"

#include <QMessageBox>
#include <QMenuBar>
#include <QStatusBar>
#include <QFileDialog>
#include <QTextStream>
#include <QApplication>

#include "core/event_manager.hpp"

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
        aboutAct(NULL),
        clsAct(NULL)
    {


        createActions();
        createMenus();
        createToolBars();
        createStatusBar();
        setupEditor();
        setupConsole();

        layout = new QVBoxLayout();
        QWidget *wnd = new QWidget();
        layout->addWidget(editor);
        layout->addWidget(console);
        wnd->setLayout(layout);
        setCentralWidget(wnd);


        //connect(this,SIGNAL(start_script(QString)),event_manager::get_instance(),SIGNAL(start_script(QString)));
        connect(this,SIGNAL(start_script_file(QString)),event_manager::get_instance(),SIGNAL(start_script_file(QString)));

    }

    script_editor_window::~script_editor_window()
    {

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

    }

    void script_editor_window::stop_script()
    {

    }

    void script_editor_window::setupEditor()
    {
        //        QFont fnt;
        //        fnt.setFamily("Courier");
        //        fnt.setFixedPitch(true);
        //        fnt.setPointSize(10);

        editor = new QTextEdit(this);
        //editor->setFont(fnt);
        setStyleSheet("QTextEdit { font-family: Courier; font-size: 12pt; background-color: 'white'; }");

        highlighter = new Highlighter(editor->document());

        newFile();

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
        connect(stopAct, SIGNAL(triggered()), this, SLOT(stop_script()));

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
        setWindowFilePath(shownName);
    }

    QString script_editor_window::strippedName(const QString &fullFileName)
    {
        return QFileInfo(fullFileName).fileName();
    }

}

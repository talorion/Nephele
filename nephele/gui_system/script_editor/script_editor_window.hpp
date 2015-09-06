#ifndef SCRIPT_WINDOW_HPP
#define SCRIPT_WINDOW_HPP

#include "console.h"
#include "highlighter.h"

#include <QMenu>
#include <QPlainTextEdit>
#include <QToolBar>
#include <QMainWindow>
#include <QVBoxLayout>
#include <QScriptEngineDebugger>

namespace talorion{
    class script_editor_window : public QMainWindow
    {
        Q_OBJECT
    public:
        explicit script_editor_window(QWidget *parent = 0);
        ~script_editor_window();
        Q_DISABLE_COPY(script_editor_window)

    signals:
        void start_script(const QString& script);
        void start_script_file(const QString& script);

        void debug_script_file(const QString& script);

        void change_script_file_component(int , QString );

    private slots:
        void newFile();
        void open();
        bool save();

        bool saveAs();
        void about();

        void start_script();
        void debug_script();
        //void stop_script();

        void slot_newQtScriptEngine(int entity);

    private:
        void setupEditor();
        void setupConsole();
        void createActions();
        void createMenus();
        void createToolBars();
        void createStatusBar();

        bool maybeSave();

        void loadFile(const QString &fileName);
        bool saveFile(const QString &fileName);
        void setCurrentFile(const QString &fileName);
        QString strippedName(const QString &fullFileName);

        void init_engine(int entity);

    private:
        QTextEdit *editor;
        Console *console;
        QVBoxLayout * layout;
        Highlighter *highlighter;
        QString curFile;

        QMenu *fileMenu;
        QMenu *runMenu;
        QMenu *helpMenu;

        QToolBar *fileToolBar;
        QToolBar *runToolBar;
        QAction *newAct;
        QAction *openAct;
        QAction *saveAct;
        QAction *saveAsAct;
        QAction *exitAct;

        QAction *runAct;
        QAction *debugAct;
        QAction *stopAct;

        QAction *aboutAct;

        QAction *clsAct;

        int my_engine;
        QScriptEngineDebugger* m_debugger;


    };
}

#endif // SCRIPT_WINDOW_HPP

#ifndef NEPHELE_MAIN_WINDOW_HPP
#define NEPHELE_MAIN_WINDOW_HPP

#include <QMainWindow>
#include <QJsonDocument>
#include <QPushButton>
#include <QGridLayout>
#include <QLineEdit>
#include <QLabel>
#include <QMap>


namespace talorion {

    class flowControllerView;
    class script_editor_window;
    class settings_dialog;

    class nephele_main_window : public QMainWindow
    {
        Q_OBJECT

    public:
        explicit nephele_main_window(QWidget *par = 0);
        ~nephele_main_window();
        Q_DISABLE_COPY(nephele_main_window)

    signals:
        void send_custom_command(const QString& cm);

        void change_set_value(int m_entity, double value);

    private:
        void createActions();
        void createMenus();
        void createToolBars();
        void createStatusBar();

        bool maybeSave();

        void loadFile(const QString &fileName);
        bool saveFile(const QString &fileName);
        void setCurrentFile(const QString &fileName);

        void read(const QJsonObject &json);
        void write(QJsonObject &json) const;

        void zero_all();

    private:
        QString curFile;
        bool modified;
        QGridLayout* mainLayout;
        script_editor_window* script_wnd;
        settings_dialog* sett_dlg;
        QMap<int, flowControllerView* > fc_views;

        QMenu *fileMenu;
        QMenu *scriptMenu;
        QMenu *toolsMenu;
        QMenu *helpMenu;

        QToolBar *fileToolBar;
        QToolBar *scriptToolBar;

        QAction *scriptEditAct;

        QAction *optionsEditAct;
        QAction *newAct;
        QAction *openAct;
        QAction *saveAct;
        QAction *saveAsAct;
        QAction *exitAct;

        QAction *aboutAct;
        QWidget *central_wdgt;

    private slots:
        void displayCustomResponse(const QString& cm);
        void dispatchCommand();
        void addAV(int entity);
        void slot_act_value_changed(int entity);
        void slot_set_value_changed(int entity);

        void open_script_window();

        void newFile();
        void open();
        bool save();

        bool saveAs();
        void about();
    };
}

#endif // NEPHELE_MAIN_WINDOW_HPP

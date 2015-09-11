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

    private:
        void createActions();
        void createMenus();
        void createToolBars();
        void createStatusBar();

    private:
       // QLineEdit* cmd;
        //QLabel* response;
        QGridLayout* mainLayout;
        //QPushButton* scriptButton;
        //QPushButton* settingsButton;
        script_editor_window* script_wnd;
        settings_dialog* sett_dlg;
        QMap<int, flowControllerView* > fc_views;

        QMenu *scriptMenu;
        QMenu *toolsMenu;
        QMenu *helpMenu;

        QToolBar *scriptToolBar;

        QAction *scriptEditAct;

        QAction *optionsEditAct;

        QAction *aboutAct;

    private slots:
        void displayCustomResponse(const QString& cm);
        void dispatchCommand();
        void addAV(int entity);
        void slot_act_value_changed(int entity);
        void slot_set_value_changed(int entity);

        void open_script_window();
        void about();
    };
}

#endif // NEPHELE_MAIN_WINDOW_HPP

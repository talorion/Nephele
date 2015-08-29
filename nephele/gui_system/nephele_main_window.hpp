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
        QLineEdit* cmd;
        QLabel* response;
        QGridLayout* mainLayout;
        QPushButton* scriptButton;
        script_editor_window* script_wnd;
        QMap<int, flowControllerView* > fc_views;

    private slots:
        void displayCustomResponse(const QString& cm);
        void dispatchCommand();
        void addAV(int entity);
        void slot_act_value_changed(int entity);
        void slot_set_value_changed(int entity);

        void open_script_window();
    };
}

#endif // NEPHELE_MAIN_WINDOW_HPP

#ifndef GUI_SYSTEM_HPP
#define GUI_SYSTEM_HPP

#include <QObject>
#include <core/abstract_system.hpp>

namespace talorion{

    class nephele_main_window;

    class gui_system : public QObject, public abstract_system
    {
        Q_OBJECT
    public:
        explicit gui_system(QObject *par = 0);
        ~gui_system();
        Q_DISABLE_COPY(gui_system)

    signals:
        void dialog_finished();
        void dialog_finished(double val);
        void dialog_finished(QString val);

    private slots:
        void slot_open_numeric_dialog();
        void slot_open_string_dialog();
        void slot_open_file_dialog();
        void slot_open_info_dialog(const QString &msg);
        void slot_open_plot_dialog();

        void slot_box_disconnected(int boxId);

    private:
        virtual void do_start_system() Q_DECL_OVERRIDE;
        virtual QString do_get_system_name() Q_DECL_OVERRIDE {return "gui_system";}
        virtual abstract_configuration_widget* do_get_configuration_widget() Q_DECL_OVERRIDE{return NULL;}

    private:
        nephele_main_window* window;
    };
}

#endif // GUI_SYSTEM_HPP

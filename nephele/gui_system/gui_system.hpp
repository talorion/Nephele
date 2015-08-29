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
        void dialog_finished(double val);

    private slots:
        void slot_open_dialog();

    private:
        virtual void do_start_system() Q_DECL_OVERRIDE;

    private:
        nephele_main_window* window;
    };
}

#endif // GUI_SYSTEM_HPP

#ifndef GUI_SYSTEM_HPP
#define GUI_SYSTEM_HPP

#include <QObject>
#include "abstract_system.hpp"

namespace talorion{

    class nephele_main_window;

    class gui_system : public QObject, public abstract_system
    {
        Q_OBJECT
    public:
        explicit gui_system(QObject *par = 0);
        ~gui_system();
        Q_DISABLE_COPY(gui_system)

        virtual void init_system() Q_DECL_OVERRIDE;
        virtual void dispose_system() Q_DECL_OVERRIDE;

        virtual void register_entity_manager(abstract_entity_manager* entity_manager=NULL) Q_DECL_OVERRIDE {entity_manager_locator::provide(entity_manager);}
        virtual void register_event_manager(abstract_event_manager* event_manager=NULL) Q_DECL_OVERRIDE {event_manager_locator::provide(event_manager);}

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

    private:
        virtual void do_start_system() Q_DECL_OVERRIDE;
        virtual QString do_get_system_name() Q_DECL_OVERRIDE {return "gui_system";}
        virtual abstract_configuration_widget* do_get_configuration_widget() Q_DECL_OVERRIDE{return NULL;}

    private:
        nephele_main_window* window;
    };
}

#endif // GUI_SYSTEM_HPP

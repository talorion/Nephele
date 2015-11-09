#ifndef SCRIPTING_THREAD_H
#define SCRIPTING_THREAD_H

#include <QThread>
#include <QScriptEngine>

#include "abstract_system.hpp"


namespace talorion {

    class script_config_widget;

    class script_system : public QThread, public abstract_system
    {
        Q_OBJECT
    public:
        explicit script_system(QObject *par = 0);
        ~script_system();
        Q_DISABLE_COPY(script_system)



        virtual void register_entity_manager(abstract_entity_manager* entity_manager=NULL) Q_DECL_OVERRIDE {entity_manager_locator::provide(entity_manager);}
        virtual void register_event_manager(abstract_event_manager* event_manager=NULL) Q_DECL_OVERRIDE {event_manager_locator::provide(event_manager);}

    signals:

    protected:
        virtual void run() Q_DECL_OVERRIDE;


        // abstract_system interface
    private:
        virtual void do_init_system() Q_DECL_OVERRIDE;
        virtual void do_dispose_system() Q_DECL_OVERRIDE;

        virtual void do_start_system() Q_DECL_OVERRIDE;
        virtual QString do_get_system_name() Q_DECL_OVERRIDE {return "script_system";}
        virtual abstract_configuration_widget* do_get_configuration_widget() Q_DECL_OVERRIDE;

    private:
        script_config_widget* config_wdg;

    };
}

#endif // SCRIPTING_THREAD_H

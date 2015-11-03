#ifndef POWER_SUPPLY_DLL_SYSTEM_HPP
#define POWER_SUPPLY_DLL_SYSTEM_HPP

#include<QObject>
#include<QThread>

#include "abstract_system.hpp"

namespace talorion{

    class psd_config_widget;

    class power_supply_dll_system: public QThread, public abstract_system
    {
        Q_OBJECT
        Q_PLUGIN_METADATA(IID "com.talorion.core.abstract_system" FILE "power_supply_dll_system.json")
        Q_INTERFACES(talorion::abstract_system)
    public:
        power_supply_dll_system(QObject *par = 0);
        ~power_supply_dll_system();
        Q_DISABLE_COPY(power_supply_dll_system)

        virtual void init_system() Q_DECL_OVERRIDE;
        virtual void dispose_system() Q_DECL_OVERRIDE;

        virtual void register_entity_manager(abstract_entity_manager* entity_manager=NULL) Q_DECL_OVERRIDE {entity_manager_locator::provide(entity_manager);}
        virtual void register_event_manager(abstract_event_manager* event_manager=NULL) Q_DECL_OVERRIDE {event_manager_locator::provide(event_manager);}

        // QThread interface
    protected:
        virtual void run() Q_DECL_OVERRIDE;

        // abstract_system interface
    private:
        virtual void do_start_system() Q_DECL_OVERRIDE;
        virtual QString do_get_system_name() Q_DECL_OVERRIDE {return "power_supply_dll_system";}
        virtual abstract_configuration_widget* do_get_configuration_widget() Q_DECL_OVERRIDE;

    private:
        psd_config_widget* config_wdg;
    };
}

#endif // POWER_SUPPLY_DLL_SYSTEM_HPP

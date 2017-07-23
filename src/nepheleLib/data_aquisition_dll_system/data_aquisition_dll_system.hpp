#ifndef TALORION_DATA_AQUISITION_SYSTEM_HPP
#define TALORION_DATA_AQUISITION_SYSTEM_HPP

#include<QObject>
#include<QThread>

#include "core/abstract_system.hpp"

namespace talorion {

    class dad_config_widget;
    class data_aquisition_dll_wrapper;

    class data_aquisition_dll_system : public QThread, public abstract_system
    {
        Q_OBJECT
    public:
        explicit data_aquisition_dll_system(QObject *par = 0);
        ~data_aquisition_dll_system();
        Q_DISABLE_COPY(data_aquisition_dll_system)

        // QThread interface
    protected:
        virtual void run() Q_DECL_OVERRIDE;

        // abstract_system interface
    private:
        virtual int doInitialize() Q_DECL_OVERRIDE;
        virtual int doDispose() Q_DECL_OVERRIDE;
        virtual int do_start_system() Q_DECL_OVERRIDE;
        virtual QString do_get_system_name() Q_DECL_OVERRIDE {return "data_aquisition_dll_system";}
        virtual abstract_configuration_widget* do_get_configuration_widget() Q_DECL_OVERRIDE;

    protected:
        virtual void doProvideEntityManager(EntityManager* service) Q_DECL_OVERRIDE;
        virtual void doProvideEventManager(EventManager* service) Q_DECL_OVERRIDE;
        virtual void doProvideSystemManager(SystemManager* service) Q_DECL_OVERRIDE;

        QCoreApplication *doMyQApp() const Q_DECL_OVERRIDE;

    private:
        dad_config_widget* config_wdg;
    };

} // namespace talorion

#endif // TALORION_DATA_AQUISITION_SYSTEM_HPP

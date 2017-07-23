#ifndef SCRIPTING_THREAD_H
#define SCRIPTING_THREAD_H

#include <QThread>
#include <QScriptEngine>

#include "core/abstract_system.hpp"

#include <QCoreApplication>

namespace talorion {

    class script_config_widget;
    class TofDaqScriptable;

    class script_system : public QThread, public abstract_system
    {
        Q_OBJECT
    public:
        explicit script_system(QObject *par = 0);
        ~script_system();
        Q_DISABLE_COPY(script_system)

    signals:

    protected:
        virtual void run() Q_DECL_OVERRIDE;


        // abstract_system interface
    private:
        virtual int doInitialize() Q_DECL_OVERRIDE;
        virtual int doDispose() Q_DECL_OVERRIDE;
        virtual int do_start_system() Q_DECL_OVERRIDE;
        virtual QString do_get_system_name() Q_DECL_OVERRIDE {return "script_system";}
        virtual abstract_configuration_widget* do_get_configuration_widget() Q_DECL_OVERRIDE;

    protected:
        virtual void doProvideEntityManager(EntityManager* service) Q_DECL_OVERRIDE;
        virtual void doProvideEventManager(EventManager* service) Q_DECL_OVERRIDE;
        virtual void doProvideSystemManager(SystemManager* service) Q_DECL_OVERRIDE;

        QCoreApplication *doMyQApp() const Q_DECL_OVERRIDE;

    private:
        script_config_widget* config_wdg;

        TofDaqScriptable* m_direct_daq;

    };
}

#endif // SCRIPTING_THREAD_H

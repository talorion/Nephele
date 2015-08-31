#ifndef POWER_SUPPLY_DLL_SYSTEM_HPP
#define POWER_SUPPLY_DLL_SYSTEM_HPP

#include<QObject>
#include<QThread>

#include "core/abstract_system.hpp"

namespace talorion{
    class power_supply_dll_system: public QThread, public abstract_system
    {
        Q_OBJECT
    public:
        power_supply_dll_system(QObject *par = 0);
        ~power_supply_dll_system();
        Q_DISABLE_COPY(power_supply_dll_system)

        // QThread interface
    protected:
        virtual void run() Q_DECL_OVERRIDE;

        // abstract_system interface
    private:
        virtual void do_start_system() Q_DECL_OVERRIDE;
    };
}

#endif // POWER_SUPPLY_DLL_SYSTEM_HPP

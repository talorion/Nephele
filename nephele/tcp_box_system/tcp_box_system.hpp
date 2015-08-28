#ifndef TALORION_TCP_BOX_COM_THREAD_HPP
#define TALORION_TCP_BOX_COM_THREAD_HPP

#include <QObject>
#include <QThread>

#include <core/abstract_system.hpp>

namespace talorion {

    class tcp_box_system : public QThread, public abstract_system
    {
    public:
        tcp_box_system(QObject* par=0);

        // QThread interface
    protected:
        virtual void run() Q_DECL_OVERRIDE;

        // abstract_system interface
    private:
        virtual void do_start_system() Q_DECL_OVERRIDE;
    };

} // namespace talorion

#endif // TALORION_TCP_BOX_COM_THREAD_HPP

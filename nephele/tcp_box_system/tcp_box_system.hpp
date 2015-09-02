#ifndef TALORION_TCP_BOX_COM_THREAD_HPP
#define TALORION_TCP_BOX_COM_THREAD_HPP

#include <QObject>
#include <QThread>
#include <QList>

#include <core/abstract_system.hpp>

namespace talorion {

    class tcpDriver;
    class tbs_config_widget;

    class tcp_box_system : public QThread, public abstract_system
    {
    public:
        tcp_box_system(QObject* par=0);
        ~tcp_box_system();
        Q_DISABLE_COPY(tcp_box_system)

        // QThread interface
    protected:
        virtual void run() Q_DECL_OVERRIDE;

        // abstract_system interface
    private:
        virtual void do_start_system() Q_DECL_OVERRIDE;
        virtual QString do_get_system_name() Q_DECL_OVERRIDE {return "tcp_box_system";}
        virtual abstract_configuration_widget* do_get_configuration_widget() Q_DECL_OVERRIDE;

        int new_box_id();
        void connect_to_fc_box(QString ip, quint16 port=2701);
        void connect_to_av_box(QString ip, quint16 port=2701);

    private:
        int curr_box_id;
        QList<tcpDriver*> drivers;

        tbs_config_widget* config_wdg;
    };

} // namespace talorion

#endif // TALORION_TCP_BOX_COM_THREAD_HPP

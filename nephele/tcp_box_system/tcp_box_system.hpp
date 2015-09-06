#ifndef TALORION_TCP_BOX_COM_THREAD_HPP
#define TALORION_TCP_BOX_COM_THREAD_HPP

#include <QObject>
#include <QThread>
#include <QList>
//#include <QMap>

#include <core/abstract_system.hpp>

namespace talorion {

    //class tcpDriver;
    class tbs_config_widget;

    class tcp_box_system : public QThread, public abstract_system
    {
        Q_OBJECT
    public:
        tcp_box_system(QObject* par=0);
        ~tcp_box_system();
        Q_DISABLE_COPY(tcp_box_system)

        // QThread interface
    protected:
        virtual void run() Q_DECL_OVERRIDE;

//    private slots:
//        void slot_connect_tcp_box(int entity);
//        void slot_disconnect_tcp_box(int entity);

        // abstract_system interface
    private:
        virtual void do_start_system() Q_DECL_OVERRIDE;
        virtual QString do_get_system_name() Q_DECL_OVERRIDE {return "tcp_box_system";}
        virtual abstract_configuration_widget* do_get_configuration_widget() Q_DECL_OVERRIDE;

//        int new_box_id();
//        void connect_to_fc_box(int box_id);
//        void connect_to_av_box(int box_id);

    private:
//        int curr_box_id;
//        QMap<int, tcpDriver*> boxes;
//        //QList<tcpDriver*> drivers;
        tbs_config_widget* config_wdg;
    };

} // namespace talorion

#endif // TALORION_TCP_BOX_COM_THREAD_HPP

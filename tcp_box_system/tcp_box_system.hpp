#ifndef TALORION_TCP_BOX_COM_THREAD_HPP
#define TALORION_TCP_BOX_COM_THREAD_HPP

#include <QObject>
#include <QThread>
#include <QList>
//#include <QMap>

#include "abstract_system.hpp"

namespace talorion {

    //class tcpDriver;
    class tbs_config_widget;

    class tcp_box_system : public QThread, public abstract_system
    {
        Q_OBJECT
        Q_PLUGIN_METADATA(IID "com.talorion.core.abstract_system" FILE "tcp_box_system.json")
        Q_INTERFACES(talorion::abstract_system)
    public:
        tcp_box_system(QObject* par=0);
        ~tcp_box_system();
        Q_DISABLE_COPY(tcp_box_system)

        virtual void init_system() Q_DECL_OVERRIDE;
        virtual void dispose_system() Q_DECL_OVERRIDE;

        virtual void register_entity_manager(abstract_entity_manager* entity_manager=NULL) Q_DECL_OVERRIDE {entity_manager_locator::provide(entity_manager);}
        virtual void register_event_manager(abstract_event_manager* event_manager=NULL) Q_DECL_OVERRIDE {event_manager_locator::provide(event_manager);}

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

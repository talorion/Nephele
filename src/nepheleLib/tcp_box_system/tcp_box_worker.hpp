#ifndef TALORION_TCP_BOX_WORKER_HPP
#define TALORION_TCP_BOX_WORKER_HPP

#include <QObject>
#include <QMap>
#include <QTimer>

#include "abstract_backend.hpp"
#include "abstracttcpdriver.hpp"

namespace talorion {

    class tcpDriver;

    class tcp_box_worker : public QObject
    {
        Q_OBJECT
    public:
        explicit tcp_box_worker(QObject *par = 0);
        ~tcp_box_worker();
        Q_DISABLE_COPY(tcp_box_worker)

        int count()const;
        void connectToTcpBox(int entity)const;

    signals:
      void tcp_box_disconnected(int entity);
      void tcp_box_connected(int entity);

    private slots:
        void slot_connect_tcp_box(int entity);
        void slot_disconnect_tcp_box(int entity);

        void slot_tcp_box_disconnected(int entity);
        void reconnect_all_boxes();

        void tcpBoxRemoved(int boxId);

    private:
        int new_box_id();
        void connect_to_fc_box(int box_id);
        void connect_to_av_box(int box_id);
        void connect_to_rf_box(int box_id);
        void connect_to_jrpc_box(int box_id);

        void reconnect_tcp_box(int box_id);

    private:
        int curr_box_id;
        QMap<int, AbstractTcpDriver*> boxes;
        QList<abstract_backend*> bkends;
        //QList<tcpDriver*> drivers;
        QTimer* reconnectTimer;
    };

} // namespace talorion

#endif // TALORION_TCP_BOX_WORKER_HPP

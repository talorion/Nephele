#ifndef TALORION_TCP_BOX_WORKER_HPP
#define TALORION_TCP_BOX_WORKER_HPP

#include <QtSerialPort/QSerialPort>

#include <QObject>
#include <QMap>
#include <QTimer>

#include "abstract_backend.hpp"

namespace talorion {

    class tcpDriver;
    class tcp_box_worker : public QObject
    {
        Q_OBJECT
    public:
        explicit tcp_box_worker(QObject *par = 0);
        ~tcp_box_worker();
        Q_DISABLE_COPY(tcp_box_worker)

    signals:
      void tcp_box_disconnected(int entity);

    public slots:

    private slots:
        void slot_connect_tcp_box(int entity);
        void slot_disconnect_tcp_box(int entity);

        void slot_tcp_box_disconnected(int entity);
        void reconnect_all_boxes();

    private:
        int new_box_id();
        void connect_to_fc_box(int box_id);
        void connect_to_av_box(int box_id);
        void connect_to_rf_box(int box_id);
        void reconnect_tcp_box(int box_id);

    private:
        int curr_box_id;
        QMap<int, tcpDriver*> boxes;
        QList<abstract_backend*> bkends;
        //QList<tcpDriver*> drivers;
        QTimer* reconnectTimer;
    };

} // namespace talorion

#endif // TALORION_TCP_BOX_WORKER_HPP

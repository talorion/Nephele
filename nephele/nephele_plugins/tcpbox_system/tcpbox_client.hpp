#ifndef TCPBOX_CLIENT_HPP
#define TCPBOX_CLIENT_HPP

//find a way to forward declare entity_manager::entity_id_t
#include"entity_manager/entity_manager.hpp"
#include "tcpbox_factory.hpp"
#include "ecmd_connection/ecmd_connection.hpp"

#include <QObject>

namespace talorion {


  class tcpbox_system_thread;
  class tcpbox_system;

  class tcpbox_client : public QObject
  {

    Q_OBJECT
  private:
    friend class tcpbox_system_thread;
    friend class tcpbox_factory;

  public:
    explicit tcpbox_client(tcpbox_factory::tcpbox_t tcpbox_id, const tcpbox_system& sys, QObject *par = Q_NULLPTR);

  signals:
    void connect_box(entity_manager::entity_id_t tcpbox_id);
    void disconnect_box(entity_manager::entity_id_t tcpbox_id);

    void send_command_to_box(entity_manager::entity_id_t tcpbox_id, const QString& cmd);

    void box_connected();
    void box_disconnected();

    void box_command_started();
    void box_command_finished();
    void box_command_error();

  public slots:
    void set_box_name(const QString& bx_name);
    void set_host_name(const QString& hst_name);
    void set_port(const quint16 prt);
    void set_box_id(const quint32 bx_id);
    void set_timeout(const int to);
    void set_state(QAbstractSocket::SocketState st);
    void set_command_state(ecmd_command_state_t st);

  private slots:
    void slot_box_connected(entity_manager::entity_id_t box);
    void slot_box_disconnected(entity_manager::entity_id_t box);

    void slot_box_command_started(entity_manager::entity_id_t box);
    void slot_box_command_finished(entity_manager::entity_id_t box);
    void slot_box_command_error(entity_manager::entity_id_t box);

  public:
    tcpbox_factory::tcpbox_t tcpbox() const;
    QString box_name()const;
    QString host_name()const;
    quint16 port()const;
    qint32 box_id()const;
    int timeout()const;
    QUuid serial_version_uid()const;
    ecmd_command_state_t command_state()const;

    bool is_deleted()const;
    bool is_configured()const;

    void open_connection();
    void close_connection();

    void send_command(const QString& cmd);
    bool is_command_supported(const QString& cmd) const;

    bool wait_for_connect();

    bool wait_for_command_started();

    bool wait_for_command_finished();

    QAbstractSocket::SocketState state()const;

  private:
    entity_manager& entity_mng()const;


  private:
    const tcpbox_factory::tcpbox_t m_tcpbox;
    //QScopedPointer<ecmd_connection> m_connection;
    const tcpbox_system& m_sys;
  };
}

#endif // TCPBOX_CLIENT_HPP

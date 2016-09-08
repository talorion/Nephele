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

  public slots:
    void set_box_name(const QString& bx_name);
    void set_host_name(const QString& hst_name);
    void set_port(const quint16 prt);
    void set_box_id(const quint32 bx_id);
    void set_timeout(const int to);
    void set_state(QAbstractSocket::SocketState st);

  public:
    tcpbox_factory::tcpbox_t tcpbox() const;
    QString box_name()const;
    QString host_name()const;
    quint16 port()const;
    qint32 box_id()const;
    int timeout()const;
    QUuid serial_version_uid()const;

    bool is_deleted()const;
    bool is_configured()const;

    void open_connection();
    void close_connection();

    bool send_command(const QString& cmd);
    bool is_command_supported(const QString& cmd) const;

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

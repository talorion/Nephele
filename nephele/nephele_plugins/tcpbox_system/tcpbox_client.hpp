#ifndef TCPBOX_CLIENT_HPP
#define TCPBOX_CLIENT_HPP

//find a way to forward declare entity_manager::entity_id_t
#include"entity_manager/entity_manager.hpp"
#include "tcpbox_factory.hpp"

#include <QTcpSocket>
//QT_BEGIN_NAMESPACE
//class QTcpSocket;
//QT_END_NAMESPACE

#include <QObject>

namespace talorion {

  class tcpbox_factory;
  class tcpbox_system;
  //class entity_manager;

  class tcpbox_client : public QObject
  {
    friend class tcpbox_factory;
    Q_OBJECT
  private:
    explicit tcpbox_client(tcpbox_factory::tcpbox_t tcpbox_id, const tcpbox_system& sys, QObject *par = Q_NULLPTR);

  signals:

  public slots:
    void set_box_name(const QString& bx_name);
    void set_host_name(const QString& hst_name);
    void set_port(const quint16 prt);
    void set_box_id(const quint32 bx_id);

  public:
    tcpbox_factory::tcpbox_t tcpbox() const;
    QString box_name()const;
    QString host_name()const;
    quint16 port()const;
    qint32 box_id()const;

    bool is_configured();

    void open_connection();

    QTcpSocket::SocketState state()const;

  private:
    entity_manager& entity_mng()const;

  private:
    const tcpbox_factory::tcpbox_t m_tcpbox;
    QScopedPointer<QTcpSocket> m_tcpsocket;
    const tcpbox_system& m_sys;
  };
}

#endif // TCPBOX_CLIENT_HPP

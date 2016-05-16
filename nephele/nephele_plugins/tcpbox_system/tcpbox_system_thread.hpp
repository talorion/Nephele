#ifndef TALORION_TCPBOX_SYSTEM_THREAD_HPP
#define TALORION_TCPBOX_SYSTEM_THREAD_HPP

#include <QMutex>
#include <QThread>

#include "tcpbox_factory.hpp"
#include "tcpbox_client.hpp"
#include "ecmd_connection.hpp"

namespace talorion {

  class tcpbox_system;

  class tcpbox_system_thread : public QThread
  {
    Q_OBJECT
  public:
    friend class tcpbox_factory;

  public:
    using tcpbox_container = QList<tcpbox_factory::tcpbox_t>;
    using ecmd_connection_container = QMap<tcpbox_factory::tcpbox_t, QPointer<ecmd_connection> >;

  private:
    Q_DISABLE_COPY(tcpbox_system_thread)

  public:
    explicit tcpbox_system_thread(const tcpbox_system &sys, QObject *par = Q_NULLPTR);

    Qt::HANDLE thread_id();

    tcpbox_container get_configured_boxes()const;

  private slots:
    void slot_connect_box(entity_manager::entity_id_t tcpbox_id);


  private:
    const tcpbox_system& m_sys;
    Qt::HANDLE m_thread_id;
    QMutex m_mutex;
    ecmd_connection_container m_connections;

    // QThread interface
  protected:
    virtual void run() Q_DECL_OVERRIDE;
  };

} // namespace talorion

#endif // TALORION_TCPBOX_SYSTEM_THREAD_HPP

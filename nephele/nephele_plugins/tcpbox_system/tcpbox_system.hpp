#ifndef TALORION_TCPBOX_SYSTEM_HPP
#define TALORION_TCPBOX_SYSTEM_HPP

#include <QMap>
#include <QThread>

#include "system_manager/abstract_system.hpp"
#include "tcpbox_factory.hpp"
#include "tcpbox_client.hpp"
#include "tcpbox_system_thread.hpp"

namespace talorion {

  //class tcpbox_client;

  class tcpbox_system : public abstract_system
  {
    Q_OBJECT
  public:
    friend class tcpbox_factory;

  public:
    using tcpbox_container = tcpbox_system_thread::tcpbox_container;

  private:
      Q_DISABLE_COPY(tcpbox_system)

  public:
    explicit tcpbox_system(QObject *par = Q_NULLPTR);
    virtual ~tcpbox_system();

  //signals:
  //  void connect_tcp_box(const tcpbox_container::value_type & tcpbox);

  public slots:

  public:
    bool contains_tcpbox(const tcpbox_container::value_type & tcpbox)const;
    void delete_box(const tcpbox_container::value_type & tcpbox);
    void delete_all_boxes();

    Qt::HANDLE thread_id();

  public:
    tcpbox_container get_configured_boxes()const;

    // abstract_system interface
  private:
    virtual state_trans_ret_t do_initialize() Q_DECL_OVERRIDE;
    virtual state_trans_ret_t do_dispose() Q_DECL_OVERRIDE;
    virtual state_trans_ret_t do_start() Q_DECL_OVERRIDE;
    virtual QStringList do_get_default_components() const Q_DECL_OVERRIDE;

    void add_box(const tcpbox_container::value_type & tcpbox);

  private:
    tcpbox_container m_tcpbox_clients;
    QPointer<tcpbox_system_thread> m_thread;

  };

} // namespace talorion

#endif // TALORION_TCPBOX_SYSTEM_HPP

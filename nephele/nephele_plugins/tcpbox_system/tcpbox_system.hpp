#ifndef TALORION_TCPBOX_SYSTEM_HPP
#define TALORION_TCPBOX_SYSTEM_HPP

#include <QMap>

#include "system_manager/abstract_system.hpp"
#include "tcpbox_factory.hpp"
#include "tcpbox_client.hpp"

namespace talorion {

  //class tcpbox_client;

  class tcpbox_system : public abstract_system
  {
    Q_OBJECT
  public:
    friend class tcpbox_factory;

  public:
    //using tcpbox_t = int;
    using tcpbox_container = QList<tcpbox_factory::tcpbox_t>;
    using tcpbox_client_container = QMap<tcpbox_factory::tcpbox_t, QPointer<tcpbox_client> >;

  public:
    explicit tcpbox_system(QObject *par = Q_NULLPTR);
    virtual ~tcpbox_system();

  signals:

  public slots:

  public:
    tcpbox_container get_configured_boxes()const;
    tcpbox_client_container get_tcpbox_clients()const;

    // abstract_system interface
  private:
    virtual state_trans_ret_t do_initialize() Q_DECL_OVERRIDE;
    virtual state_trans_ret_t do_dispose() Q_DECL_OVERRIDE;
    virtual state_trans_ret_t do_start() Q_DECL_OVERRIDE;

    void add_box(const tcpbox_container::value_type & box);
    bool add_client(const tcpbox_client_container::mapped_type &client);

  private:
    tcpbox_container m_configured_boxes;
    tcpbox_client_container m_tcpbox_clients;

  };

} // namespace talorion

#endif // TALORION_TCPBOX_SYSTEM_HPP

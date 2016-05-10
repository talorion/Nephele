#ifndef TALORION_TCPBOX_FACTORY_HPP
#define TALORION_TCPBOX_FACTORY_HPP

#include <QObject>

//find a way to forward declare entity_manager::entity_id_t
#include"entity_manager/entity_manager.hpp"
//

namespace talorion {

  class tcpbox_system;
  class tcpbox_client;

  class tcpbox_factory : public QObject
  {
    Q_OBJECT
  public:
    using tcpbox_t = entity_manager::entity_id_t;

  private:
    explicit tcpbox_factory(QObject *par = Q_NULLPTR);

  public:
    // delete copy and move constructors and assign operators
    tcpbox_factory(tcpbox_factory const&) = delete;             // Copy construct
    tcpbox_factory(tcpbox_factory&&) = delete;                  // Move construct
    tcpbox_factory& operator=(tcpbox_factory const&) = delete;  // Copy assign
    tcpbox_factory& operator=(tcpbox_factory &&) = delete;      // Move assign

  public:
    static tcpbox_factory& get_instance();

    tcpbox_t create_new_tcpbox(tcpbox_system &sys, const QString &box_name=QString(), const QString &host_name=QString(), quint16 port=0, qint32 box_id=0)const;

    tcpbox_client *create_new_tcpbox_client(tcpbox_system& sys, tcpbox_t tcpbox)const;

  signals:

  public slots:

  private:

  };


} // namespace talorion

#endif // TALORION_TCPBOX_FACTORY_HPP

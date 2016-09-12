#ifndef TALORION_TCPBOX_FACTORY_HPP
#define TALORION_TCPBOX_FACTORY_HPP

#include <QObject>
#include <QUuid>

//find a way to forward declare entity_manager::entity_id_t
#include "entity_manager/entity_manager.hpp"


namespace talorion {

  class tcpbox_system;
  class tcpbox_client;

  class tcpbox_factory : public QObject
  {
    Q_OBJECT
  public:
    using tcpbox_t = entity_manager::entity_id_t;
    using tcpbox_container_t = QList<tcpbox_t>;

    using tcpbox_command_t = entity_manager::entity_id_t;
    using tcpbox_command_container_t = QList<tcpbox_command_t>;

  private:
    explicit tcpbox_factory(QObject *par = Q_NULLPTR);

  public:
    // delete copy and move constructors and assign operators
    tcpbox_factory(tcpbox_factory const&) = delete;             // Copy construct
    tcpbox_factory(tcpbox_factory&&) = delete;                  // Move construct
    tcpbox_factory& operator=(tcpbox_factory const&) = delete;  // Copy assign
    tcpbox_factory& operator=(tcpbox_factory &&) = delete;      // Move assign

  public:
    static entity_manager::component_id_t name_component_id();
    static entity_manager::component_id_t host_name_component_id();
    static entity_manager::component_id_t port_component_id();
    static entity_manager::component_id_t box_id_component_id();
    static entity_manager::component_id_t timeout_component_id();
    static entity_manager::component_id_t connection_state_component_id();
    static entity_manager::component_id_t serial_version_uid_component_id();
    static entity_manager::component_id_t command_state_component_id();

    static tcpbox_factory& get_instance();

public:
    QUuid get_TcpBox_uid()const ;
    tcpbox_container_t get_all_tcpboxes(const tcpbox_system &sys)const;
    tcpbox_t create_new_tcpbox(tcpbox_system &sys, const QString &box_name=QString(), const QString &host_name=QString(), quint16 port=0, qint32 box_id=0)const;

    tcpbox_command_container_t get_all_tcpbox_commands(const tcpbox_system &sys, tcpbox_t tcpbox =entity_manager::invalid_id)const;
    tcpbox_command_t create_new_tcpbox_command(tcpbox_system &sys, tcpbox_t tcpbox =entity_manager::invalid_id) const;

  signals:

  public slots:

  private:

  };


} // namespace talorion

#endif // TALORION_TCPBOX_FACTORY_HPP

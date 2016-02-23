#ifndef TALORION_TCPBOX_SYSTEM_HPP
#define TALORION_TCPBOX_SYSTEM_HPP

#include "system_manager/abstract_system.hpp"

#include "tcpbox_factory.hpp"

namespace talorion {

  class tcpbox_system : public abstract_system
  {
    Q_OBJECT
  public:
    friend class tcpbox_factory;

  public:
    using tcpbox = int;

  public:
    explicit tcpbox_system(QObject *par = nullptr);
    virtual ~tcpbox_system();

  signals:

  public slots:

  public:
    QList<tcpbox> get_configured_boxes()const;

    // abstract_system interface
  private:
    virtual state_trans_ret_t do_initialize() Q_DECL_OVERRIDE;
    virtual state_trans_ret_t do_dispose() Q_DECL_OVERRIDE;
    virtual state_trans_ret_t do_start() Q_DECL_OVERRIDE;
    virtual abstract_configuration_widget *do_get_configuration_widget() const Q_DECL_OVERRIDE;

    void add_box(tcpbox box);

  private:
    QList<tcpbox> m_configured_boxes;

  };

} // namespace talorion

#endif // TALORION_TCPBOX_SYSTEM_HPP

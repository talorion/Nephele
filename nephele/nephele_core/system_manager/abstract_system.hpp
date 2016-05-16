#ifndef ABSTRACT_SYSTEM_HPP
#define ABSTRACT_SYSTEM_HPP

#include <QObject>
#include <QPointer>


namespace talorion {

  class abstract_system_state;
  class abstract_configuration_widget;
  class entity_manager;
  class event_manager;

  class abstract_system : public QObject
  {
    Q_OBJECT
  public:     //typedefs
    using state_trans_ret_t=int;

  public:
    explicit abstract_system(QObject *par = Q_NULLPTR);

  public:
    void initialize();
    void dispose();
    void start();

    abstract_configuration_widget* get_configuration_widget()const{return do_get_configuration_widget();}

    entity_manager& get_entity_manager()const;

    event_manager& get_event_manager()const;

  signals:

  public slots:

    QStringList get_default_components()const;

  private:
    virtual state_trans_ret_t do_initialize()=0;
    virtual state_trans_ret_t do_dispose()=0;
    virtual state_trans_ret_t do_start()=0;
    virtual QStringList do_get_default_components()const{return QStringList();}

    virtual abstract_configuration_widget* do_get_configuration_widget()const{return Q_NULLPTR;}

  private:
    static entity_manager s_ent_mng;
    static event_manager  s_evt_mng;

  };
}

#endif // ABSTRACT_SYSTEM_HPP

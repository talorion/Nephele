#ifndef ABSTRACT_SYSTEM_HPP
#define ABSTRACT_SYSTEM_HPP

#include <QObject>
#include <QPointer>

namespace talorion {

  class abstract_system_state;
  class abstract_configuration_widget;

  class abstract_system : public QObject
  {
    Q_OBJECT
  public:     //typedefs
    using state_trans_ret_t=int;

  public:
    explicit abstract_system(QObject *par = nullptr);

  public:
    void initialize();
    void dispose();
    void start();

    abstract_configuration_widget* get_configuration_widget()const{return do_get_configuration_widget();}

  signals:

  public slots:

  private:
    virtual state_trans_ret_t do_initialize()=0;
    virtual state_trans_ret_t do_dispose()=0;
    virtual state_trans_ret_t do_start()=0;

    virtual abstract_configuration_widget* do_get_configuration_widget()const=0;

  private:

  };
}

#endif // ABSTRACT_SYSTEM_HPP

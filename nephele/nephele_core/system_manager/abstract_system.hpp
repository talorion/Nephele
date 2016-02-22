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
    //enum class state_t;
    //using state_t=_state_t;

    //enum class state_input_t;
    //using state_input_t=_state_input_t;

    enum class state_t{
           SYSTEM_STATE_UNDEFINED,
           SYSTEM_STATE_DISPOSED,
           SYSTEM_STATE_INITIALIZED,
           SYSTEM_STATE_STARTED,
           SYSTEM_STATE_ERROR
       };

    enum class state_input_t{
        SYSTEM_INPUT_INITIALIZE,
        SYSTEM_INPUT_START,
        SYSTEM_INPUT_STOP,
        SYSTEM_INPUT_DISPOSE
    };

    using state_trans_ret_t=int;

  public:
    explicit abstract_system(QObject *par = nullptr);

  //private:
    //Q_DISABLE_COPY(abstract_system)

  public:

    state_t state()const;

    void initialize();
    void start();

    abstract_configuration_widget* get_configuration_widget()const{return do_get_configuration_widget();}

  signals:

  public slots:

  private:
    //state_t change_state( state_t new_state);
    void state_transition(state_input_t input);

    virtual state_trans_ret_t do_initialize()=0;
    virtual state_trans_ret_t do_start()=0;

    virtual abstract_configuration_widget* do_get_configuration_widget()const;

  private:
    QPointer<abstract_system_state> m_state;
  };
}

#endif // ABSTRACT_SYSTEM_HPP

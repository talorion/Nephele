#ifndef ABSTRACT_SYSTEM_HPP
#define ABSTRACT_SYSTEM_HPP

#include <QObject>

namespace talorion {

  class abstract_system_state;
class abstract_system : public QObject
{
    Q_OBJECT
  //friend class abstract_system_state;
  //friend class initialized_state;
  //friend class undefined_state;
public:     //typedefs
    typedef enum _state_t{
        SYSTEM_STATE_UNDEFINED,
        SYSTEM_STATE_DISPOSED,
        SYSTEM_STATE_INITIALIZED,
        SYSTEM_STATE_STARTED,
        SYSTEM_STATE_ERROR
    }state_t;

  typedef enum _state_input_t{
      SYSTEM_INPUT_INITIALIZE,
      SYSTEM_INPUT_START,
      SYSTEM_INPUT_STOP,
      SYSTEM_INPUT_DISPOSE
  }state_input_t;

  typedef int state_trans_ret_t;

public:
    explicit abstract_system(QObject *par = 0);
    virtual ~abstract_system();
    Q_DISABLE_COPY(abstract_system)

    state_t state()const;

    void initialize();
    void start();

signals:

public slots:

private:
    //state_t change_state( state_t new_state);
    void state_transition(state_input_t input);

    virtual state_trans_ret_t do_initialize()=0;
    virtual state_trans_ret_t do_start()=0;

private:
    abstract_system_state* m_state;
};
}

#endif // ABSTRACT_SYSTEM_HPP

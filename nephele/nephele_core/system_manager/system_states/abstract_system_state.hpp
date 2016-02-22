#ifndef ABSTRACT_SYSTEM_STATE_H
#define ABSTRACT_SYSTEM_STATE_H

#include <QObject>
#include "../abstract_system.hpp"

namespace talorion{

  //class abstract_system;

  class abstract_system_state : public QObject
  {
    Q_OBJECT
  public:
    explicit abstract_system_state(QObject *par = nullptr): QObject(par){}
    virtual ~abstract_system_state()=default;
  private:
    Q_DISABLE_COPY(abstract_system_state)

  public:
    virtual  abstract_system_state* state_transition(abstract_system* sys, abstract_system::state_input_t input)=0;
    //virtual abstract_system_state* update(abstract_system& sys)=0;

    virtual void enter(abstract_system* sys)=0;
    virtual void exit(abstract_system* sys)=0;

    virtual abstract_system::state_t type()const=0;

  };
}
#endif // ABSTRACT_SYSTEM_STATE_H

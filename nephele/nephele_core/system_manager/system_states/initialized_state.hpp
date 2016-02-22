#ifndef TALORION_INITIALIZED_STATE_HPP
#define TALORION_INITIALIZED_STATE_HPP

#include "abstract_system_state.hpp"

namespace talorion {

  class initialized_state : public abstract_system_state
  {
    Q_OBJECT
  public:
    explicit initialized_state(QObject *par = nullptr);
    virtual ~initialized_state()=default;
  private:
    Q_DISABLE_COPY(initialized_state)

    // abstract_system_state interface
  public:
    virtual abstract_system_state *state_transition(abstract_system *sys, abstract_system::state_input_t input)Q_DECL_OVERRIDE;
    virtual abstract_system::state_t type() const Q_DECL_OVERRIDE{return abstract_system::state_t::SYSTEM_STATE_INITIALIZED;}

    virtual void enter(abstract_system *sys) Q_DECL_OVERRIDE{Q_UNUSED(sys);}
    virtual void exit(abstract_system *sys) Q_DECL_OVERRIDE{Q_UNUSED(sys);}
  };

} // namespace talorion

#endif // TALORION_INITIALIZED_STATE_HPP

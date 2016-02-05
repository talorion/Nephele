#ifndef TALORION_DISPOSED_STATE_HPP
#define TALORION_DISPOSED_STATE_HPP

#include "abstract_system_state.hpp"

namespace talorion {

  class disposed_state : public abstract_system_state
  {
    Q_OBJECT
  public:
    explicit disposed_state(QObject *par = 0);
    ~disposed_state();
    Q_DISABLE_COPY(disposed_state)

    // abstract_system_state interface
  public:
    virtual abstract_system_state *state_transition(abstract_system *sys, abstract_system::state_input_t input)Q_DECL_OVERRIDE;
    virtual void enter(abstract_system *sys) Q_DECL_OVERRIDE{Q_UNUSED(sys);}
    virtual void exit(abstract_system *sys) Q_DECL_OVERRIDE{Q_UNUSED(sys);}
    virtual abstract_system::state_t type() const Q_DECL_OVERRIDE{return abstract_system::SYSTEM_STATE_DISPOSED;}
  };

} // namespace talorion

#endif // TALORION_DISPOSED_STATE_HPP

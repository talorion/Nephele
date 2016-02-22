#include "started_state.hpp"

#include "disposed_state.hpp"

namespace talorion {

  started_state::started_state(QObject *par)
    : abstract_system_state(par)
  {

  }

  abstract_system_state *started_state::state_transition(abstract_system *sys, abstract_system::state_input_t input)
  {
    Q_UNUSED(sys);
    if(input == abstract_system::state_input_t::SYSTEM_INPUT_STOP){
        //sys->do_initialize();
        return new disposed_state();
      }

    // Stay in this state.
    return NULL;
  }

} // namespace talorion


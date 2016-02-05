#include "initialized_state.hpp"

#include "started_state.hpp"
#include "disposed_state.hpp"

namespace talorion {

  initialized_state::initialized_state(QObject *par)
    : abstract_system_state(par)
  {

  }

  initialized_state::~initialized_state()
  {

  }

  abstract_system_state *initialized_state::state_transition(abstract_system *sys, abstract_system::state_input_t input)
  {
    Q_UNUSED(sys);
    if(input == abstract_system::SYSTEM_INPUT_START){
        //sys->do_initialize();
        return new started_state();
      }

    if(input == abstract_system::SYSTEM_INPUT_DISPOSE){
        //sys->do_initialize();
        return new disposed_state();
      }

    // Stay in this state.
    return NULL;
  }

} // namespace talorion


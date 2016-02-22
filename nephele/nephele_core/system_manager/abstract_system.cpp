#include "abstract_system.hpp"

#include "system_states/abstract_system_state.hpp"
#include "system_states/undefined_state.hpp"
#include "configuration_widget/abstract_configuration_widget.hpp"


namespace talorion {



  abstract_system::abstract_system(QObject *par) :
    QObject(par),
    m_state(new undefined_state())
  {

  }

  abstract_system::state_t abstract_system::state() const{
    return m_state->type();
  }

  void abstract_system::initialize(){
    state_transition(state_input_t::SYSTEM_INPUT_INITIALIZE);
  }

  void abstract_system::start()
  {

  }

  void abstract_system::state_transition(state_input_t input)
  {
    abstract_system_state * st =m_state->state_transition(this, input);
    if (st != nullptr){
        m_state->exit(this);
        delete m_state;
        m_state = st;

        m_state->enter(this);
      }

  }

  abstract_configuration_widget *abstract_system::do_get_configuration_widget() const
  {
    return nullptr;
  }



}

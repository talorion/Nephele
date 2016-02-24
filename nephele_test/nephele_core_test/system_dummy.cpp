#include "system_dummy.hpp"

namespace talorion{

  system_dummy::system_dummy(QObject *par) : abstract_system(par)
  {

  }

  system_dummy::state_trans_ret_t system_dummy::do_initialize()
  {
    return 0;
  }

  abstract_system::state_trans_ret_t system_dummy::do_start()
  {
    return 0;
  }

  abstract_system::state_trans_ret_t system_dummy::do_dispose()
  {
    return 0;
  }


}

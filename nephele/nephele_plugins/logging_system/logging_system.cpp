#include "logging_system.hpp"



namespace talorion {

  logging_system::logging_system(QObject *par) :
    abstract_system(par),
    m_entries()
  {

  }

  logging_system::~logging_system()
  {

  }

  abstract_system::state_trans_ret_t logging_system::do_initialize()
  {
    //To restore the message handler, call qInstallMessageHandler(0).

    return 0;
  }

  abstract_system::state_trans_ret_t logging_system::do_start()
  {
return 0;
  }

} // namespace talorion

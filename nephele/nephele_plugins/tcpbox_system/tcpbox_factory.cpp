#include "tcpbox_factory.hpp"

#include "tcpbox_system.hpp"

namespace talorion {

  tcpbox_factory::tcpbox_factory(QObject *par) : QObject(par)
  {

  }

  tcpbox_factory &tcpbox_factory::get_instance()
  {
    // Since it's a static variable, if the class has already been created,
    // It won't be created again.
    // And it **is** thread-safe in C++11.

    static tcpbox_factory myInstance;

    // Return a reference to our instance.
    return myInstance;
  }

  void tcpbox_factory::create_new_tcpbox(tcpbox_system& sys)
  {
    tcpbox_system::tcpbox box=42;

    sys.add_box(box);
  }

} // namespace talorion

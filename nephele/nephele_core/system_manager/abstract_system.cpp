#include "abstract_system.hpp"

#include "configuration_widget/abstract_configuration_widget.hpp"


namespace talorion {



  abstract_system::abstract_system(QObject *par) :
    QObject(par)
  {

  }


  void abstract_system::initialize(){
    do_initialize();
  }

  void abstract_system::dispose()
  {
    do_dispose();
  }

  void abstract_system::start()
  {

  }

}

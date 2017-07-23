#include "abstracttcpdriver.hpp"

namespace talorion {

  AbstractTcpDriver::AbstractTcpDriver(int box, QObject *par):
    QObject(par),
    m_boxId(box)
  {

  }

} // namespace talorion

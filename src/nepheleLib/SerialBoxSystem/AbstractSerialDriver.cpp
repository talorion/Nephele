#include "AbstractSerialDriver.hpp"

namespace talorion {

AbstractSerialDriver::AbstractSerialDriver(EntityManager::EntityID box, QObject *par) :
    QObject(par),
    m_boxId(box)
{

}

} // namespace talorion

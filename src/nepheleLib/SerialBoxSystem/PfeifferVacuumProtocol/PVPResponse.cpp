#include "PVPResponse.hpp"

namespace talorion {

PVPResponse::PVPResponse():
    PVPTelegramm(),
    m_isError(false)
{

}

bool PVPResponse::isError() const
{
    return m_isError;
}

} // namespace talorion

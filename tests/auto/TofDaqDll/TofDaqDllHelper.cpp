#include "TofDaqDllHelper.hpp"

namespace talorion {

TofDaqDllHelper::TofDaqDllHelper()
{

}

TofDaqDllHelper& TofDaqDllHelper::get_instance()
{
    static TofDaqDllHelper instance;
    return instance;
}

void TofDaqDllHelper::destroy()
{

}

bool TofDaqDllHelper::DaqActive() const
{
    return m_daqActive;
}

int TofDaqDllHelper::getTimeout() const
{
    return m_timeout;
}

void TofDaqDllHelper::setTimeout(int timeout)
{
    m_timeout = timeout;
}

TSharedMemoryDesc *TofDaqDllHelper::getPBufDesc() const
{
    return pBufDesc;
}

bool TofDaqDllHelper::getTofDaqRunning() const
{
    return m_tofDaqRunning;
}

void TofDaqDllHelper::setTofDaqRunning(bool tofDaqRunning)
{
    m_tofDaqRunning = tofDaqRunning;
}

std::list<std::string, std::string> TofDaqDllHelper::getSources() const
{
    return m_sources;
}

void TofDaqDllHelper::setSources(const std::list<std::string, std::string> &sources)
{
    m_sources = sources;
}

} // namespace talorion

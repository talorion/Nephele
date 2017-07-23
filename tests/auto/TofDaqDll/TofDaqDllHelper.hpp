#ifndef TOFDAQDLLHELPER_HPP
#define TOFDAQDLLHELPER_HPP

#include "TofIpcStrucs.h"

#include <map>
#include <string>
//#include <list>

namespace talorion {

//using twVariant       = std::variant<std::string, int, bool, float, long long, double>;
//using twVariantMap    = std::map<std::string, twVariant>;


class TofDaqDllHelper
{
private:
    TofDaqDllHelper();
    ~TofDaqDllHelper() = default;

public:
    static TofDaqDllHelper& get_instance();
    static void destroy();

public:
    bool DaqActive() const;

    int getTimeout() const;
    void setTimeout(int timeout);

    TSharedMemoryDesc *getPBufDesc() const;

    bool getTofDaqRunning() const;
    void setTofDaqRunning(bool tofDaqRunning);

    //std::list<std::string, std::string> getSources() const;
    //void setSources(const std::list<std::string, std::string> &sources);

private:
    bool m_daqActive;
    bool m_tofDaqRunning;
    int m_timeout;

    TSharedMemoryDesc *pBufDesc;

    //std::map<std::string, std::v
    //std::list<std::string, std::string>  m_sources;

};

} // namespace talorion

#endif // TOFDAQDLLHELPER_HPP

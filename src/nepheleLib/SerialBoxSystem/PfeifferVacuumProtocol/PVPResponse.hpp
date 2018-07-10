#ifndef PVPRESPONSE_HPP
#define PVPRESPONSE_HPP


#include "PVPFactory.hpp"
#include "PVPTelegramm.hpp"

namespace talorion {

class PVPResponse: public PVPTelegramm
{
    Q_OBJECT
    friend class PVPFactory;
private:
    PVPResponse();

public:
    bool isError()const;
    QByteArray rawData()const{return toTelegramm();}
    bool isValid()const{return isError()==false;}

private:
    bool m_isError;
};

} // namespace talorion

#endif // PVPRESPONSE_HPP

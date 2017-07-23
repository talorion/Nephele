#ifndef TCPDRIVERDATATYPES
#define TCPDRIVERDATATYPES

namespace tcpDriverDataTypes {
    enum dataType {
        UIBK_ECMD_ALLDATA,
        UIBK_ECMD_ACTSETDATA,
        UIBK_ECMD_SETDATA,
        UIBK_ECMD_CUSTOMCOMMAND,
        UIBK_ECMD_IDLE,
        UIBK_ECMD_NONE
    };
}

Q_DECLARE_METATYPE(tcpDriverDataTypes::dataType)



#endif // TCPDRIVERDATATYPES


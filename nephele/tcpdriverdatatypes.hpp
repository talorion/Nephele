#ifndef TCPDRIVERDATATYPES
#define TCPDRIVERDATATYPES

namespace tcpDriverDataTypes {
    enum dataType {ALLDATA, ACTSETDATA, SETDATA, CUSTOMCOMMAND, IDLE, NONE};
}

Q_DECLARE_METATYPE(tcpDriverDataTypes::dataType);



#endif // TCPDRIVERDATATYPES


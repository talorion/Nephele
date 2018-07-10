#ifndef BACKEND_TYPE_HPP
#define BACKEND_TYPE_HPP

namespace talorion{
enum class backend_type : int{
    UIBK_ECMD_FC = 0,
    UIBK_ECMD_QVM = 1,
    UIBK_ECMD_RF = 2,
    JSONRPC = 3,
    PFEIFFER_VACUUM_PROTOCOL = 4,
    COUNT
};

inline const char* ToString(backend_type v)
{
    switch (v)
    {
        case backend_type::UIBK_ECMD_FC:  return "ECMD FC";
        case backend_type::UIBK_ECMD_QVM: return "ECMD QVM";
        case backend_type::UIBK_ECMD_RF:  return "ECMD RF";
        case backend_type::JSONRPC:       return "JSON RPC";
        case backend_type::PFEIFFER_VACUUM_PROTOCOL:       return "PFEIFFER VACUUM PROTOCOL";
        default:      return "[Unknown]";
    }
}

}

Q_DECLARE_METATYPE(talorion::backend_type)


#endif // BACKEND_TYPE_HPP

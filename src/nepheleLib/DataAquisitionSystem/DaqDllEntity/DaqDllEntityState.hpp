#ifndef DAQDLLENTITYSTATE_HPP
#define DAQDLLENTITYSTATE_HPP

namespace talorion {

enum class DaqDllEntityState : int {
    DAQ_DLL_STATE_UNKNOWN,
    DAQ_DLL_STATE_MEASUREMENT_RUNNING,
    DAQ_DLL_STATE_IDLE,
    DAQ_DLL_STATE_RECORDER_NOT_RUNNING,
    DAQ_DLL_STATE_NOT_INITIALIZED
};
}
//Q_DECLARE_METATYPE(talorion::DaqDllEntityState)

#endif // DAQDLLENTITYSTATE_HPP

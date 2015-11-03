#ifndef TALORION_TOF_DAQ_DLL_TOOLS_HPP
#define TALORION_TOF_DAQ_DLL_TOOLS_HPP

#include <QString>

namespace talorion {

    QString get_tw_error_description(int error);
    int twErrChk(int error);

} // namespace talorion

#endif // TALORION_TOF_DAQ_DLL_TOOLS_HPP

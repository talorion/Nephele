#include "tof_daq_dll_tools.hpp"

#include <QDebug>

namespace talorion {

    QString get_tw_error_description(int error)
    {
        QString description = "";
        switch(error)
        {
        case 0:{description = "TwDaqRecNotRunning";break;}
        case 1:{description = "TwAcquisitionActive";break;}
        case 2:{description = "TwNoActiveAcquisition";break;}
        case 3:{description = "TwFileNotFound";break;}
        case 4:{description = "TwSuccess";break;}
        case 5:{description = "TwError";break;}
        case 7:{description = "TwNoData";break;}
        case 8:{description = "TwTimeout";break;}
        case 9:{description = "TwValueAdjusted";break;}
        case 10:{description = "TwInvalidParameter";break;}
        case 11:{description = "TwInvalidValue";break;}
        case 12:{description = "TwAborted";break;}
        default:{description = "UNKNOWN ERROR ["+QString::number(error)+"]";break;}
        }

        return description;
    }

    int twErrChk(int error)
    {
        if (error != 4)
        {
            qDebug()<<"TW ERROR:"<< get_tw_error_description(error);
        }
        return error;
    }

} // namespace talorion


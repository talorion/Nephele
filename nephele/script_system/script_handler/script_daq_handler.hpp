#ifndef SCRIPT_DAQ_HANDLER_HPP
#define SCRIPT_DAQ_HANDLER_HPP

#include "core/abstract_scriptable_object.hpp"

namespace talorion{
    class script_daq_handler : public abstract_scriptable_object
    {
        Q_OBJECT
    public:
        explicit script_daq_handler(QObject *par = 0);
        ~script_daq_handler();
        Q_DISABLE_COPY(script_daq_handler)

    signals:
        void start_aq();
        void stop_aq();

    public slots:
    };
}

#endif // SCRIPT_DAQ_HANDLER_HPP

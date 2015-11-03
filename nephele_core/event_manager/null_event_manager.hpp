#ifndef TALORION_NULL_EVENT_MANAGER_HPP
#define TALORION_NULL_EVENT_MANAGER_HPP

#include "abstract_event_manager.hpp"

namespace talorion {

    class null_event_manager : public abstract_event_manager
    {
        Q_OBJECT
    public:
        explicit null_event_manager(QObject *par = 0);
        ~null_event_manager();
        Q_DISABLE_COPY(null_event_manager)
    signals:

    public slots:
    };

} // namespace talorion

#endif // TALORION_NULL_EVENT_MANAGER_HPP

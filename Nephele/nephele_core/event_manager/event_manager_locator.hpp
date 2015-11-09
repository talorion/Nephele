#ifndef EVENT_MANAGER_LOCATOR_HPP
#define EVENT_MANAGER_LOCATOR_HPP

#include <QObject>
#include "abstract_event_manager.hpp"
#include "null_event_manager.hpp"

namespace talorion {

    class event_manager_locator : public QObject
    {
        Q_OBJECT
    private:
        explicit event_manager_locator(QObject *par = 0);
        ~event_manager_locator();
        Q_DISABLE_COPY(event_manager_locator)
    public:

        static abstract_event_manager& manager(){return *manager_;}
        static abstract_event_manager* get_instance(){return manager_;}

        static void provide(abstract_event_manager* evt_manager);

    private:
        static abstract_event_manager* manager_;
        static null_event_manager null_manager_;

    };

}
#endif // EVENT_MANAGER_LOCATOR_HPP

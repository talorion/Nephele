#ifndef SCRIPT_UTIL_HANDLER_H
#define SCRIPT_UTIL_HANDLER_H

#include "core/abstract_scriptable_object.hpp"

namespace talorion {

    class script_util_handler : public abstract_scriptable_object
    {
        Q_OBJECT
    public:
        explicit script_util_handler(QObject *par = 0);
        ~script_util_handler();
        Q_DISABLE_COPY(script_util_handler)

        Q_INVOKABLE void sleep(unsigned long millisecs);
        Q_INVOKABLE int get_entity_by_name(QString name);

    signals:
        void skip_sleep();
        //void dialog_finished();

    public slots:
    };
}

#endif // SCRIPT_UTIL_HANDLER_H

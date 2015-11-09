#ifndef SCRIPT_ACT_HANDLER_H
#define SCRIPT_ACT_HANDLER_H

#include "abstract_scriptable_object.hpp"

namespace talorion {

    class script_act_handler : public abstract_scriptable_object
    {
        Q_OBJECT
    public:
        explicit script_act_handler(QObject *par = 0);
        ~script_act_handler();
        Q_DISABLE_COPY(script_act_handler)

        //Q_INVOKABLE void sleep(unsigned long millisecs);
    private slots:
        void slot_act_value_changed(int entity);
        void slot_dig_act_value_changed(int entity);
        void slot_newAnalogValue(int entity);
    };

}

#endif // SCRIPT_ACT_HANDLER_H

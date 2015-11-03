#ifndef SCRIPT_HANDLER_H
#define SCRIPT_HANDLER_H

#include "abstract_scriptable_object.hpp"

namespace talorion {

    class script_set_handler : public abstract_scriptable_object
    {
        Q_OBJECT
    public:
        explicit script_set_handler(QObject *par = 0);
        ~script_set_handler();
        Q_DISABLE_COPY(script_set_handler)

        Q_INVOKABLE void val(QString name, double value);
        Q_INVOKABLE void val(QString name, bool value);

    signals:
        void set_value_changed(int entity, double set_val);
        void set_dig_value_changed(int entity, bool set_val);

    private slots:
        void slot_set_value_changed(int entity);
        void slot_digital_set_value_changed(int entity);
        void slot_newAnalogValue(int entity);
        void slot_newDigitalValue(int entity);
    };
}

#endif // SCRIPT_HANDLER_H

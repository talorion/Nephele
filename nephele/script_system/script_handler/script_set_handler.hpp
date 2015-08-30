#ifndef SCRIPT_HANDLER_H
#define SCRIPT_HANDLER_H

#include <QObject>

namespace talorion {

    class script_set_handler : public QObject
    {
        Q_OBJECT
    public:
        explicit script_set_handler(QObject *par = 0);
        ~script_set_handler();
        Q_DISABLE_COPY(script_set_handler)

        Q_INVOKABLE void val(QString name, double value);

    signals:
        void set_value_changed(int entity, double set_val);

    };
}

#endif // SCRIPT_HANDLER_H

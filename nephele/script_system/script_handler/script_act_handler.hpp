#ifndef SCRIPT_ACT_HANDLER_H
#define SCRIPT_ACT_HANDLER_H

#include <QObject>
namespace talorion {

    class script_act_handler : public QObject
    {
        Q_OBJECT
    public:
        explicit script_act_handler(QObject *par = 0);
        ~script_act_handler();
        Q_DISABLE_COPY(script_act_handler)

        //Q_INVOKABLE void sleep(unsigned long millisecs);
    };

}

#endif // SCRIPT_ACT_HANDLER_H

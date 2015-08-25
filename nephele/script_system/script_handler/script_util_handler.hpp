#ifndef SCRIPT_UTIL_HANDLER_H
#define SCRIPT_UTIL_HANDLER_H

#include <QObject>

namespace talorion {

    class script_util_handler : public QObject
    {
        Q_OBJECT
    public:
        explicit script_util_handler(QObject *par = 0);
        ~script_util_handler();
        Q_DISABLE_COPY(script_util_handler)

        Q_INVOKABLE void sleep(unsigned long millisecs);

    signals:

    public slots:
    };
}

#endif // SCRIPT_UTIL_HANDLER_H

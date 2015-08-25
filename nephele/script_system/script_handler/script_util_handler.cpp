#include "script_util_handler.hpp"

#include <QEventLoop>
#include <QTimer>

namespace talorion {

    script_util_handler::script_util_handler(QObject *par) : QObject(par)
    {

    }

    script_util_handler::~script_util_handler()
    {

    }

    void script_util_handler::sleep(unsigned long millisecs)
    {
        QEventLoop loop;
        QTimer::singleShot(millisecs,&loop,SLOT(quit()));
        loop.exec();
    }
}

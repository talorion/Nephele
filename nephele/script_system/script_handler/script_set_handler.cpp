#include "script_set_handler.h"

#include <QCoreApplication>
#include <QThread>
#include <QTime>
#include <QTimer>

namespace talorion {

    script_set_handler::script_set_handler(QObject *par) :
        QObject(par)
    {
    }

    script_set_handler::~script_set_handler()
    {

    }
}


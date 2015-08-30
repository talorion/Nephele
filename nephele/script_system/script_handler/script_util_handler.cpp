#include "script_util_handler.hpp"
#include "core/entity_manager.hpp"

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

    int script_util_handler::get_entity_by_name(QString name)
    {
        return entity_manager::get_instance()->get_entity_by_name(name);
    }
}

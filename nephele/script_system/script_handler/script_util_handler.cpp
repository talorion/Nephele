#include "script_util_handler.hpp"
#include "core/entity_manager.hpp"

#include <QEventLoop>
#include <QTimer>

#include "core/event_manager.hpp"
#include "core/entity_manager.hpp"

namespace talorion {

    script_util_handler::script_util_handler(QObject *par) :
        abstract_scriptable_object("util", par)
    {
        connect(event_manager::get_instance(), SIGNAL(script_skip_sleep()),this,SIGNAL(skip_sleep()));
    }

    script_util_handler::~script_util_handler()
    {

    }

    void script_util_handler::sleep_ms(unsigned long millisecs)
    {
        QEventLoop loop;
        //connect(this,SIGNAL(dialog_finished()),&loop,SLOT(quit()));
        connect(this,SIGNAL(script_finished()),&loop,SLOT(quit()));
        connect(this,SIGNAL(skip_sleep()),&loop,SLOT(quit()));
        QTimer::singleShot(millisecs,&loop,SLOT(quit()));
        loop.exec();
        disconnect(this,SIGNAL(script_finished()),&loop,SLOT(quit()));
    }

    int script_util_handler::get_entity_by_name(QString name)
    {
        return entity_manager::get_instance()->get_entity_by_name(name);
    }
}

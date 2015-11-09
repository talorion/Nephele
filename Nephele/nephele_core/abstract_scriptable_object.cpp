#include "abstract_scriptable_object.hpp"

#include <QScriptValue>

#include "event_manager/event_manager_locator.hpp"

namespace talorion {

    abstract_scriptable_object::abstract_scriptable_object(QString scrpt_name, QObject *par) :
        QObject(par),
        m_script_name(scrpt_name),
        scrip_value(NULL)
    {
        connect(event_manager_locator::get_instance(),SIGNAL(script_finished()),this,SIGNAL(script_finished()));
    }


} // namespace talorion


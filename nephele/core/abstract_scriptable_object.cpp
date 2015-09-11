#include "abstract_scriptable_object.hpp"

#include <QScriptValue>

#include "event_manager.hpp"

namespace talorion {

    abstract_scriptable_object::abstract_scriptable_object(QString scrpt_name, QObject *par) :
        QObject(par),
        m_script_name(scrpt_name),
        scrip_value(NULL)
    {
        connect(event_manager::get_instance(),SIGNAL(script_finished()),this,SIGNAL(script_finished()));
    }

    abstract_scriptable_object::~abstract_scriptable_object()
    {

    }

    QString abstract_scriptable_object::script_name() const
    {
        return m_script_name;
    }
    QScriptValue *abstract_scriptable_object::getScrip_value() const
    {
        return scrip_value;
    }

    void abstract_scriptable_object::setScrip_value(QScriptValue *value)
    {
        scrip_value = value;
    }



} // namespace talorion


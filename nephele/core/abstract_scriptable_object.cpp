#include "abstract_scriptable_object.hpp"

namespace talorion {

    abstract_scriptable_object::abstract_scriptable_object(QString scrpt_name, QObject *par) :
        QObject(par),
        m_script_name(scrpt_name)
    {

    }

    abstract_scriptable_object::~abstract_scriptable_object()
    {

    }

    QString abstract_scriptable_object::script_name() const
    {
        return m_script_name;
    }
       
    
} // namespace talorion


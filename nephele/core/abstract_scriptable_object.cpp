#include "abstract_scriptable_object.hpp"

namespace talorion {

    abstract_scriptable_object::abstract_scriptable_object(QObject *par) :
        QObject(par),
        m_script_name()
    {

    }

    abstract_scriptable_object::~abstract_scriptable_object()
    {

    }



} // namespace talorion


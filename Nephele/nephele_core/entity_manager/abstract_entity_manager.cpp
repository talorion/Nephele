#include "abstract_entity_manager.hpp"

namespace talorion {

    abstract_entity_manager::abstract_entity_manager(QObject *par) :
        QObject(par)
    {

    }

    abstract_entity_manager::~abstract_entity_manager()
    {

    }

    QList<int> abstract_entity_manager::get_all_Values() const
    {
        QList<int> tmp= get_all_AnalogInputValues();
        tmp += get_all_AnalogOutputValues();
        tmp += get_all_AnalogValues();

        return tmp;
    }

    QList<int> abstract_entity_manager::get_all_DValues() const
    {
        QList<int> tmp= get_all_DigitalInputValues();
        tmp += get_all_DigitalOutputValues();
        tmp += get_all_DigitalValues();

        return tmp;
    }

} // namespace talorion


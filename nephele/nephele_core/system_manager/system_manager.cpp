#include "system_manager.hpp"

namespace talorion {

system_manager::system_manager(QObject *par) :
    QObject(par),
    m_all_systems()
{}

system_manager::~system_manager(){}

system_manager::system_container_t system_manager::get_all_systems() const
{
    return m_all_systems;
}

system_manager::system_container_t::size_type system_manager::size() const
{
    return m_all_systems.size();
}



} // namespace talorion

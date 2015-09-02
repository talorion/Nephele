#include "system_manager.hpp"

#include "entity_manager.hpp"

namespace talorion {

    system_manager* system_manager::_instance;
    QMutex system_manager::_mutex;

    system_manager::system_manager(QObject *par) :
        QObject(par),
        m_systems()
    {

    }

    system_manager::~system_manager()
    {

    }

    system_manager* system_manager::get_instance()
    {
        if ( !_instance ){
            _mutex.lock();
            if (!_instance)
                _instance = new system_manager();
            _mutex.unlock();
        }
        return _instance;
    }//end getInstance

    void system_manager::destroy()
    {
        _mutex.lock();
        if ( _instance )
            delete _instance;
        _instance = 0;
        _mutex.unlock();
    }

    QList<int> system_manager::get_all_systems() const
    {
        QList<int> tmp;
        //QSet<abstract_system*> m_systems;
        foreach (abstract_system* sys, m_systems) {
            if(sys)
                tmp.append(sys->getEntity_id());
        }
        return tmp;
    }

    void system_manager::load_system(abstract_system *sys)
    {
        if(!sys)
            return;
        abstract_configuration_widget* tmp = sys->get_configuration_widget();
        int entity = entity_manager::get_instance()->createNewSystem(sys->get_system_name(), tmp);
        sys->setEntity_id(entity);

        sys->start_system();
    }


} // namespace talorion


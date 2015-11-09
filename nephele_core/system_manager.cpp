#include "system_manager.hpp"

#include "entity_manager/entity_manager_locator.hpp"

#ifndef NEPHELE_PLUGINS_DIR
#define NEPHELE_PLUGINS_DIR "plugins"
#endif

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
//        foreach (abstract_system* sys, m_systems) {
//            if(sys != NULL)
//                //sys->dispose_system();
//        }
//        m_systems.clear();
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

    void system_manager::load_plugin_systems()
    {
        QDir pluginsDir = get_plugins_dir();
        qDebug()<<"loading systems from "<<pluginsDir.absolutePath();
        foreach (QString fileName, pluginsDir.entryList(QDir::Files)) {
            QPluginLoader loader(pluginsDir.absoluteFilePath(fileName));

            QObject *plugin = loader.instance();
            if(plugin){
                abstract_system* sys = qobject_cast<abstract_system *>(plugin);
                load_system(sys);
            }

        }
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

    QDir system_manager::get_plugins_dir()
    {
        QDir pluginsDir = QDir::current();

        //        #if defined(Q_OS_WIN)
        //            if (pluginsDir.dirName().toLower() == "debug" || pluginsDir.dirName().toLower() == "release")
        //                pluginsDir.cdUp();
        //        #elif defined(Q_OS_MAC)
        //            if (pluginsDir.dirName() == "MacOS") {
        //                pluginsDir.cdUp();
        //                pluginsDir.cdUp();
        //                pluginsDir.cdUp();
        //            }
        //        #endif

        pluginsDir.cd(NEPHELE_PLUGINS_DIR);

        return  pluginsDir;
    }

    void system_manager::load_system(abstract_system *sys)
    {
        qDebug()<<"loading system: "<<sys->get_system_name();
        if(!sys)
            return;
        abstract_entity_manager* ent = entity_manager_locator::get_instance();
        sys->register_entity_manager(ent);
        sys->register_event_manager(event_manager_locator::get_instance());
        sys->init_system();

        abstract_configuration_widget* tmp = sys->get_configuration_widget();
        int entity = entity_manager_locator::get_instance()->createNewSystem(sys->getSerialVersionUID(),sys->get_system_name(), tmp);
        sys->setEntity_id(entity);

        sys->start_system();

        m_systems.append(sys);
    }


} // namespace talorion


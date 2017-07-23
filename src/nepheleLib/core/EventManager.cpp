#include "EventManager.hpp"

#include <QAtomicPointer>
#include <QMutex>
#include <QFile>

#include "abstract_system.hpp"
#include "ManagerLocator.hpp"

namespace talorion {

    //QAtomicPointer<event_manager> event_manager::_instance;
    EventManager* EventManager::_instance;
    QMutex EventManager::_mutex;

    EventManager::EventManager(QObject *par):
        QObject(par)
    {
        qRegisterMetaType<EntityManager::EntityID>("EntityManager::EntityID");
        qRegisterMetaType<EntityManager::ComponentID>("EntityManager::ComponentID");
        qRegisterMetaType<EntityManager::ComponentData>("EntityManager::ComponentData");
        //qRegisterMetaType<abstract_system*>("abstract_system*");
        //qRegisterMetaType<QSharedPointer<abstract_system>>("QSharedPointer<abstract_system>");
        //qRegisterMetaType<QSharedPointer<talorion::abstract_system>>("QSharedPointer<talorion::abstract_system>");

    }

    EventManager::~EventManager(){}

    EventManager* EventManager::get_instance()
    {
        if ( !_instance ){
            _mutex.lock();
            if (!_instance){
                //_instance = new EventManager();
                _instance = &(ManagerLocator::eventManager());
            }
            _mutex.unlock();
        }
        return _instance;
    }//end getInstance

    void EventManager::destroy()
    {
        _mutex.lock();
        if ( _instance )
            delete _instance;
        _instance = 0;
        _mutex.unlock();
    }

} // namespace talorion


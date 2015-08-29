#include "event_manager.hpp"

#include <QAtomicPointer>
#include <QMutex>
#include <QFile>


namespace talorion {

    QAtomicPointer<event_manager> event_manager::_instance;
    QMutex event_manager::_mutex;

    event_manager::event_manager(QObject *par):
        QObject(par)
    {
        qRegisterMetaType<tcpDriverDataTypes::dataType>("tcpDriverDataTypes::dataType");
    }

    event_manager::~event_manager(){}

    event_manager* event_manager::get_instance()
    {
        if ( !_instance ){
            _mutex.lock();
            if (!_instance)
                _instance = new event_manager();
            _mutex.unlock();
        }
        return _instance;
    }//end getInstance

    void event_manager::destroy()
    {
        _mutex.lock();
        if ( _instance )
            delete _instance;
        _instance = 0;
        _mutex.unlock();
    }

} // namespace talorion


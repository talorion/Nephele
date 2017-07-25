#ifndef TALORION_SYSTEM_MANAGER_HPP
#define TALORION_SYSTEM_MANAGER_HPP

#include <QMutex>
#include <QObject>
#include <QtCore>
#include <QList>

#include "abstract_system.hpp"

namespace talorion {

  class AbstractSystemView;

  class SystemManager : public QObject
  {
    Q_OBJECT

    friend class ManagerLocator;

  public:
    //using AbstractSystemPtr=QSharedPointer<abstract_system> ;
    using AbstractSystemPtr= abstract_system*;
    using SystemContainer=QList<AbstractSystemPtr> ;

  public:
    explicit SystemManager(QObject *par = 0);
    ~SystemManager();
    Q_DISABLE_COPY(SystemManager)


  public:
    static SystemManager *get_instance();
    static void destroy();

    template<typename T>
    AbstractSystemPtr register_new_system();

    QList<int> allSystems()const;
    SystemContainer::size_type size()const;

    void startAllSystems();
    void disposeAllSystems();
    void deleteAllSystems();

  private:
    void load_system(abstract_system* sys);

  private:
    QList<abstract_system*> m_systems;

  private:
    static SystemManager* _instance;
    static QMutex _mutex;
  };

  template<typename T>
  SystemManager::AbstractSystemPtr SystemManager::register_new_system()
  {
    static_assert(std::is_base_of<abstract_system,T>::value,"T must be derived from abstract_system!");
    //std::is_base_of<base, derived>::value;
    T* tmp = new T();
    load_system(tmp);
    //m_systems.append(tmp);

    //return AbstractSystemPtr(tmp);
    return tmp;
  }

  //    template <typename T>
  //    void system_manager::register_new_system<T*>() {
  //        static_assert(false, "T must not be a pointer type");
  //    }

} // namespace talorion

#endif // TALORION_SYSTEM_MANAGER_HPP

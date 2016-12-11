#ifndef TALORION_SYSTEM_MANAGER_HPP
#define TALORION_SYSTEM_MANAGER_HPP

#include <QDir>
#include <QObject>
#include <QSharedPointer>


namespace talorion {

class AbstractSystem;

class SystemManager : public QObject
{
    Q_OBJECT
public:
    //using AbstractSystemPtr=QPointer<AbstractSystem> ;
    using AbstractSystemPtr=QSharedPointer<AbstractSystem> ;
    using SystemContainer=QList<AbstractSystemPtr> ;

public:
    explicit SystemManager(QObject *par = Q_NULLPTR);
    virtual ~SystemManager() = default;

public:
    SystemContainer allSystems()const;
    SystemContainer::size_type size()const;

    void startAllSystems();
    void disposeAllSystems();
    void deleteAllSystems();

    template<typename T>
    AbstractSystemPtr registerNewSystem();

public slots:
    void loadPluginSystems();

private:
    QDir pluginsDir();

    void loadSystem(AbstractSystemPtr sys);
    void createSystemEntity(AbstractSystemPtr sys);

private:
    SystemContainer m_all_systems;

};

template<typename T>
SystemManager::AbstractSystemPtr SystemManager::registerNewSystem()
{
    static_assert(std::is_base_of<AbstractSystem,T>::value,"T must be derived from abstract_system!");
    AbstractSystemPtr tmp (new T());
    //abstract_system_t tmp = new T();
    //m_all_systems.append(tmp);
    loadSystem(tmp);

    return tmp;
}

} // namespace talorion

#endif // TALORION_SYSTEM_MANAGER_HPP

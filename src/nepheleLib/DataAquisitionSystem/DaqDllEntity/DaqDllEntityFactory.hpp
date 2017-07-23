#ifndef DAQDLLENTITYFACTORY_HPP
#define DAQDLLENTITYFACTORY_HPP

#include <QObject>
#include <QUuid>
#include "core/EntityManager.hpp"
#include "userdatasource.hpp"


namespace talorion {

class DaqDllEntityFactory : public QObject
{
    Q_OBJECT
private:
    explicit DaqDllEntityFactory(QObject *par = 0);
    virtual ~DaqDllEntityFactory() = default;
    Q_DISABLE_COPY(DaqDllEntityFactory)

signals:
    void dataAquisitionDllChanged(EntityManager::EntityID, const QString &);
    void timoutChanged(EntityManager::EntityID, int);
    void updaterateChanged(EntityManager::EntityID, int);
    void userDataPathChanged(EntityManager::EntityID, const QString &);

public:
    static DaqDllEntityFactory& instance();// Move assign

    static QUuid daqDllEntityUid();

    static EntityManager::ComponentID serialVersionUidComponentId();
    static EntityManager::ComponentID nameComponentId();
    static EntityManager::ComponentID dataAquisitionDllComponent();
    static EntityManager::ComponentID userDataUpdaterateComponent();
    static EntityManager::ComponentID timeoutComponent();
    static EntityManager::ComponentID userDataPathComponent();
    static EntityManager::ComponentID daqDataIdComponent();
    static EntityManager::ComponentID stateComponent();

    static EntityManager::EntityID createNewDaqDllEntity(const QString &dll_name=QString(), const QString &dll_path=QString("C:\\Tofwerk\\TofDaq\\TofDaqDll.dll"), int timeout=500, const QString &usr_dta_path="/NEPHELE", int updaterate=200);
    static EntityManager::EntityList allDaqDllEntities();

    static UserDataSourceList getUserDataComponents(EntityManager::EntityID daqDll);

    static EntityManager::EntityList allEntitiesWithUserData();

};

} // namespace talorion

#endif // DAQDLLENTITYFACTORY_HPP

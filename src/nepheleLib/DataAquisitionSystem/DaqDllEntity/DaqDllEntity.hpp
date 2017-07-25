#ifndef DAQDLLENTITY_HPP
#define DAQDLLENTITY_HPP

#include <QObject>
#include "core/EntityManager.hpp"
#include "DaqDllEntityState.hpp"

namespace talorion {

class DaqDllEntity : public QObject
{
    Q_OBJECT
public:
    explicit DaqDllEntity(EntityManager::EntityID daqdllId, QObject *par = 0);

public slots:
    void setDllPath(const QString& new_val);
    void setTimout(int new_val);
    void setUpdaterate(int new_val);
    void setUserDataPath(const QString& new_val);
    void setDaqDataId(EntityManager::ID new_val);
    //void setState(DaqDllEntityState new_val);

public:
    QString dllPath() const;
    int timeout() const;
    int updaterate() const;
    QString userDataPath() const;
    EntityManager::EntityID daqDataId()const;
    //DaqDllEntityState state()const;

    EntityManager::EntityID daqDllId() const;

    bool isValid()const;

private:
    EntityManager& entityManager() const;
    EventManager& eventManager()const;

private:
    const EntityManager::EntityID m_daqDll;
};

} // namespace talorion

#endif // DAQDLLENTITY_HPP

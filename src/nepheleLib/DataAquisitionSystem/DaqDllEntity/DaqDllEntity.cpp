#include "DaqDllEntity.hpp"
#include "DaqDllEntityFactory.hpp"

#include "core/ManagerLocator.hpp"
#include <QDebug>

namespace talorion {

DaqDllEntity::DaqDllEntity(EntityManager::EntityID daqdllId, QObject *par) :
    QObject(par),
    m_daqDll(daqdllId)
{

}

void DaqDllEntity::setDllPath(const QString &new_val)
{
    auto old_val = dllPath();
    if(old_val != new_val){
        entityManager().setComponentDataForEntity(DaqDllEntityFactory::dataAquisitionDllComponent(), daqDllId(), new_val);
        //emit eventManager().dataAquisitionDllChanged(daqDll(), new_val);
        emit DaqDllEntityFactory::instance().dataAquisitionDllChanged(daqDllId(), new_val);
    }
}

void DaqDllEntity::setTimout(int new_val)
{
    auto old_val = timeout();
    if(old_val != new_val){
        entityManager().setComponentDataForEntity(DaqDllEntityFactory::timeoutComponent(), daqDllId(), new_val);
        //emit eventManager().timoutChanged(daqDll(), new_val);
        emit DaqDllEntityFactory::instance().timoutChanged(daqDllId(), new_val);
    }
}

void DaqDllEntity::setUpdaterate(int new_val)
{
    auto old_val = updaterate();
    if(old_val != new_val){
        entityManager().setComponentDataForEntity(DaqDllEntityFactory::userDataUpdaterateComponent(), daqDllId(), new_val);
        //emit eventManager().updaterateChanged(daqDll(), new_val);
        emit DaqDllEntityFactory::instance().updaterateChanged(daqDllId(), new_val);
    }
}

void DaqDllEntity::setUserDataPath(const QString &new_val)
{
    auto old_val = userDataPath();
    if(old_val != new_val){
        entityManager().setComponentDataForEntity(DaqDllEntityFactory::userDataPathComponent(), daqDllId(), new_val);
        //emit eventManager().userDataPathChanged(daqDll(), new_val);
        emit DaqDllEntityFactory::instance().userDataPathChanged(daqDllId(), new_val);
    }
}

void DaqDllEntity::setDaqDataId(EntityManager::ID new_val)
{
    auto old_val = daqDataId();
    if(old_val != new_val){
        entityManager().setComponentDataForEntity(DaqDllEntityFactory::daqDataIdComponent(), daqDllId(), new_val);
    }
}

//void DaqDllEntity::setState(DaqDllEntityState new_val)
//{
//    auto old_val = state();
//    if(old_val != new_val){
//        QVariant tmp = QVariant::fromValue(new_val);
//        entityManager().setComponentDataForEntity(DaqDllEntityFactory::stateComponent(), daqDllId(), tmp);
//    }
//}

QString DaqDllEntity::dllPath() const
{
    return entityManager().componentDataForEntity(DaqDllEntityFactory::dataAquisitionDllComponent(), daqDllId()).toString();
}

int DaqDllEntity::timeout() const
{
    bool ok = false;
    auto ret= entityManager().componentDataForEntity(DaqDllEntityFactory::timeoutComponent(), daqDllId()).toInt(&ok);
    if(!ok)
      ret= -1;
    return ret;
}

int DaqDllEntity::updaterate() const
{
    bool ok = false;
    auto ret= entityManager().componentDataForEntity(DaqDllEntityFactory::userDataUpdaterateComponent(), daqDllId()).toInt(&ok);
    if(!ok)
      ret= -1;
    return ret;
}

QString DaqDllEntity::userDataPath() const
{
    return entityManager().componentDataForEntity(DaqDllEntityFactory::userDataPathComponent(), daqDllId()).toString();
}

EntityManager::EntityID DaqDllEntity::daqDataId() const
{
    bool ok = false;
    EntityManager::EntityID ret= entityManager().componentDataForEntity(DaqDllEntityFactory::daqDataIdComponent(), daqDllId()).toInt(&ok);
    if(!ok)
      ret= -1;
    return ret;
}

//DaqDllEntityState DaqDllEntity::state() const
//{
//    auto tmp = entityManager().componentDataForEntity(DaqDllEntityFactory::stateComponent(), daqDllId());
//    int ret_int=(int)(DaqDllEntityState::DAQ_DLL_STATE_UNKNOWN);
//    if (tmp.canConvert<int>()) {
//        ret_int= tmp.value<int>();
//      }
//    talorion::DaqDllEntityState ret = (talorion::DaqDllEntityState)(ret_int);
//    return ret;
//}

EntityManager &DaqDllEntity::entityManager() const
{
    return ManagerLocator::entityManager();
}

EventManager &DaqDllEntity::eventManager() const
{
    return ManagerLocator::eventManager();
}

EntityManager::EntityID DaqDllEntity::daqDllId() const
{
    return m_daqDll;
}

bool DaqDllEntity::isValid() const
{
    auto path = dllPath();
    auto to = timeout();
    return path.isNull() == false && path.isEmpty() == false && to >0;
}

} // namespace talorion

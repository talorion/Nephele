#include "abstract_system.hpp"


#include "ManagerLocator.hpp"

#include "EventManager.hpp"
#include "EntityManager.hpp"
#include "SystemManager.hpp"

namespace talorion{

abstract_system::abstract_system(QUuid id):
    m_state(SystemState::UNKNOWN),
    serialVersionUID(id),
    entity_id(-1)
  //entity_id(EntityManager::invalid_id)
{
    setState(SystemState::NOT_INITIALIZED);
}

void abstract_system::initialize(){

    foreach(QString comp_nme, defaultComponents()){
        ManagerLocator::entityManager().createNewComponent(comp_nme);
    }

    auto ret =doInitialize();
    if(ret==0)
        setState(SystemState::INITIALIZED);
    else
        setState(SystemState::NOT_INITIALIZED);
}

void abstract_system::dispose()
{
    auto ret =doDispose();
    if(ret==0)
        setState(SystemState::DISPOSED);
    else
        setState(SystemState::NOT_DISPOSED);
}

void abstract_system::start_system(){
    auto ret = do_start_system();
    if(ret==0)
        setState(SystemState::STARTED);
    else
        setState(SystemState::NOT_STARTED);
}

EntityManager &abstract_system::entityManager() const
{
    return ManagerLocator::entityManager();
}

QStringList abstract_system::defaultComponents() const{
    QStringList tmp{""};
    tmp.append(doGetDefaultComponents());
    return tmp;
}

void abstract_system::addSystemView(AbstractSystemView *view)
{
    auto sysviews = systemViews();
    if(sysviews.contains(view))
        return;
    sysviews.append(view);
    setSystemViews(sysviews);

}

void abstract_system::setSystemViews(SystemViewsContainer views)
{
    doSetSystemViews(views);

    EntityManager& entMng = ManagerLocator::entityManager();

    if(EntityManager::isValid(getEntity_id()) == false)
        return;

    SystemViewsContainer sysViews;
    auto var = entMng.componentDataForEntity(SYSTEM_VIEW_WIDGET_COMPONENT, getEntity_id());
    if(var.canConvert<SystemViewsContainer>()){
        sysViews = var.value<SystemViewsContainer>();
    }

    if(systemViews() != sysViews){
        auto tmp = QVariant::fromValue(systemViews());
        entMng.setComponentDataForEntity(SYSTEM_VIEW_WIDGET_COMPONENT, getEntity_id(), tmp);
    }
}

}

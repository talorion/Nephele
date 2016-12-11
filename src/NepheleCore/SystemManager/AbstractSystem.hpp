#ifndef ABSTRACT_SYSTEM_HPP
#define ABSTRACT_SYSTEM_HPP

#include <QObject>
#include <QPointer>

#include <QtWidgets/QAction>
#include <QtWidgets/QToolBar>

#include "EntityManager/EntityManager.hpp"
#include "EventManager/EventManager.hpp"
#include "SystemManager.hpp"
#include "SystemState.hpp"

namespace talorion {

//class abstract_system_state;
class AbstractConfigurationWidget;
class AbstractSystemView;
class EntityManager;
class EventManager;


class AbstractSystem : public QObject
{
    Q_OBJECT
    friend class SystemManager;

public:     //typedefs
    using StateTransRet=int;

    using ActionsContainer = QList<QAction*>;
    using ToolbarsContainer = QList<QToolBar*>;

public:
    explicit AbstractSystem(QObject *par = Q_NULLPTR);

public:
    void initialize();
    void dispose();
    void start();

    QString systemName()const{return doSystemName();}

    AbstractConfigurationWidget* configurationWidget()const{return doConfigurationWidget();}
    AbstractSystemView* systemView()const{return doSystemView();}
    ActionsContainer actions()const{return doActions();}
    ToolbarsContainer toolbars()const{return doToolbars();}

    EntityManager &entityManager()const;

    EntityManager::EntityID entity()const{return doEntity();}

public:
    SystemState state() const;

signals:

public slots:
    QStringList defaultComponents()const;

    void provideEntityManager(EntityManager* service){doProvideEntityManager(service);}
    void provideEventManager(EventManager* service){doProvideEventManager(service);}
    void provideSystemManager(SystemManager* service){doProvideSystemManager(service);}

private:
    virtual StateTransRet doInitialize()=0;
    virtual StateTransRet doDispose()=0;
    virtual StateTransRet doStart()=0;

    virtual QString doSystemName()const=0;

    virtual QStringList doGetDefaultComponents()const{return QStringList();}

    virtual AbstractConfigurationWidget* doConfigurationWidget()const;
    virtual AbstractSystemView* doSystemView()const;
    virtual ActionsContainer doActions()const{return ActionsContainer();}
    virtual ToolbarsContainer doToolbars()const{return ToolbarsContainer();}

    virtual EntityManager::EntityID doEntity()const{return m_entity_id;}

    void setState(SystemState st);

    void setEntityId(EntityManager::EntityID new_id){m_entity_id = new_id;}

protected:
    virtual void doProvideEntityManager(EntityManager* service)=0;
    virtual void doProvideEventManager(EventManager* service)=0;
    virtual void doProvideSystemManager(SystemManager* service)=0;

private:
    SystemState m_state;
    EntityManager::EntityID m_entity_id;

};
}

#define Sytem_iid "com.nephele.core.AbstractSystem"
Q_DECLARE_INTERFACE(talorion::AbstractSystem, Sytem_iid)

#endif // ABSTRACT_SYSTEM_HPP

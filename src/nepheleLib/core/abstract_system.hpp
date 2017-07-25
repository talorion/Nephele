#ifndef ABSTRACT_SYSTEM
#define ABSTRACT_SYSTEM

#include <QString>
#include <QUuid>

#include "AbstractSystemView.hpp"
#include "AbstractAction.hpp"

#include <QCoreApplication>

#include <QtWidgets/QToolBar>

#include "SystemState.hpp"

#include "systemviewscontainer.h"

namespace talorion{

class abstract_configuration_widget;
class EntityManager;
class EventManager;
class SystemManager;

class abstract_system
{
public:     //typedefs
    //using SystemViewsContainer = QList<AbstractSystemView *>;
    using ActionsContainer = QList<AbstractAction*>;
    using ToolbarsContainer = QList<QToolBar*>;

public:
    abstract_system(QUuid id=QUuid::createUuid());
    virtual ~abstract_system(){}
    Q_DISABLE_COPY(abstract_system)

     void initialize();
     void dispose();

    void start_system();
    QString get_system_name(){return do_get_system_name();}
    abstract_configuration_widget* get_configuration_widget(){return do_get_configuration_widget();}

    SystemViewsContainer systemViews()const{return doSystemViews();}
    ActionsContainer actions()const{return doActions();}
    ToolbarsContainer toolbars()const{return doToolbars();}

    int getEntity_id() const{return entity_id;}
    void setEntity_id(int value){entity_id = value;}

    QCoreApplication* myQApp()const{return doMyQApp();}

    QUuid getSerialVersionUID()const{return serialVersionUID;}
    SystemState state() const{return m_state;}

    EntityManager &entityManager()const;

public slots:
    QStringList defaultComponents()const;

    void provideEntityManager(EntityManager* service){doProvideEntityManager(service);}
    void provideEventManager(EventManager* service){doProvideEventManager(service);}
    void provideSystemManager(SystemManager* service){doProvideSystemManager(service);}

protected:
  void addSystemView(AbstractSystemView* view);
  void setSystemViews(SystemViewsContainer views);
  virtual void doSetSystemViews(SystemViewsContainer){}

private:
    void setState(SystemState st)  {m_state =st;}

private:
    virtual int doInitialize()=0;
    virtual int doDispose()=0;
    virtual int do_start_system()=0;

    virtual QString do_get_system_name()=0;

    virtual QCoreApplication* doMyQApp()const=0;

    virtual abstract_configuration_widget* do_get_configuration_widget()=0;

    virtual SystemViewsContainer doSystemViews()const{return SystemViewsContainer();}
    virtual ActionsContainer doActions()const{return ActionsContainer();}
    virtual ToolbarsContainer doToolbars()const{return ToolbarsContainer();}

    virtual QStringList doGetDefaultComponents()const{return QStringList();}

protected:
    virtual void doProvideEntityManager(EntityManager* service)=0;
    virtual void doProvideEventManager(EventManager* service)=0;
    virtual void doProvideSystemManager(SystemManager* service)=0;

private:
    SystemState m_state;
    const QUuid serialVersionUID;
    int entity_id;
};
}
//Q_DECLARE_METATYPE(talorion::abstract_system*);
Q_DECLARE_METATYPE(QSharedPointer<talorion::abstract_system>)
Q_DECLARE_METATYPE(talorion::abstract_system*)

#endif // ABSTRACT_SYSTEM


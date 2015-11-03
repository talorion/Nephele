#ifndef ABSTRACT_SYSTEM
#define ABSTRACT_SYSTEM

#include <QString>
#include <QUuid>
#include "entity_manager/abstract_entity_manager.hpp"
#include "entity_manager/entity_manager_locator.hpp"
#include "event_manager/abstract_event_manager.hpp"
#include "event_manager/event_manager_locator.hpp"

namespace talorion{

    class abstract_configuration_widget;

    class abstract_system
    {
    public:
        abstract_system(QUuid id=QUuid::createUuid()):
            serialVersionUID(id),
            entity_id(-1)
        {

        }

        virtual ~abstract_system(){}
        Q_DISABLE_COPY(abstract_system)

        virtual void init_system() =0;
        virtual void dispose_system()=0;

        void start_system(){return do_start_system();}
        QString get_system_name(){return do_get_system_name();}
        abstract_configuration_widget* get_configuration_widget(){return do_get_configuration_widget();}

        int getEntity_id() const{return entity_id;}
        void setEntity_id(int value){entity_id = value;}

        QUuid getSerialVersionUID()const{return serialVersionUID;}

        virtual void register_entity_manager(abstract_entity_manager* entity_manager=NULL)=0;
        virtual void register_event_manager(abstract_event_manager* event_manager=NULL)=0;
//        virtual void register_entity_manager(abstract_entity_manager* entity_manager=NULL){entity_manager_locator::provide(entity_manager);}
//        virtual void register_event_manager(abstract_event_manager* event_manager=NULL){event_manager_locator::provide(event_manager);}

    private:
        virtual void do_start_system()=0;
        virtual QString do_get_system_name()=0;
        virtual abstract_configuration_widget* do_get_configuration_widget()=0;

    private:
        const QUuid serialVersionUID;
        int entity_id;
    };
}

//#define BrushInterface_iid "org.qt-project.Qt.Examples.PlugAndPaint.BrushInterface"
#define Sytem_iid "com.talorion.core.abstract_system"
Q_DECLARE_INTERFACE(talorion::abstract_system, Sytem_iid)

#endif // ABSTRACT_SYSTEM


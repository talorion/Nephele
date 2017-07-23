#include "analogview.hpp"


#include "core/EntityManager.hpp"
#include "core/EventManager.hpp"

namespace talorion {

    analogView::analogView(int entity, mode md, QWidget *par):
        QWidget(par),
        m_entity(entity),
        m_mode(md)
    {
        //connect(this, SIGNAL(change_set_value(int,double)),event_manager::get_instance(),SIGNAL(change_analogSet_component(int,double)),Qt::UniqueConnection);
        //connect(editSet,SIGNAL(valueChanged(double)),this,SLOT(slot_set_value_changed(double)));
        connect(this, SIGNAL(change_set_value(int,double)), EventManager::get_instance(),SIGNAL(change_analogSet_component(int,double)),Qt::UniqueConnection);
        //connect(event_manager::get_instance(),SIGNAL(analogSet_component_changed(int)),this,SLOT(changeSetValue(int)));

    }

    analogView::~analogView()
    {

    }
    
    int analogView::entity() const
    {
        return m_entity;
    }
}

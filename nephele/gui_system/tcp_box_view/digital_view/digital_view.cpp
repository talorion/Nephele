#include "digital_view.hpp"

#include "entity_manager/entity_manager_locator.hpp"
#include "event_manager/event_manager_locator.hpp"

namespace talorion{
    digital_view::digital_view(int entity,mode md=InputOutput, QWidget *par) :
        QWidget(par),
        m_layout(NULL),
        lblName(NULL),
        editSet(NULL),
        editAct(NULL),
        m_entity(entity),
        m_mode(md)
    {
        m_layout = new QGridLayout();

        lblName = new QLabel();
        m_layout->addWidget(lblName,0,0);

        if(md & Output){
            editSet = new switch_button();

            m_layout->addWidget(editSet,0,1);

            connect(editSet,SIGNAL(valueChanged(bool)),this,SLOT(slot_set_value_changed(bool)));
            connect(event_manager_locator::get_instance(),SIGNAL(digitalSet_component_changed(int)),this,SLOT(changeSetValue(int)));

        }

        if(md & Input){
            editAct =new digital_indicator();

            m_layout->addWidget(editAct,0,2);

            connect(event_manager_locator::get_instance(),SIGNAL(digitalAct_component_changed(int)),this,SLOT(changeActValue(int)));
        }

        m_layout->setMargin(0);
        setLayout(m_layout);

        //lblName->setText("Hugo");
        lblName->setText(entity_manager_locator::get_instance()->get_name_component(entity));

        //connect(editSet,SIGNAL(valueChanged(bool)), editAct,SLOT(setValue(bool)));

        connect(this, SIGNAL(change_set_value(int,bool)), event_manager_locator::get_instance(),SIGNAL(change_digitalSet_component(int,bool)),Qt::UniqueConnection);

        if(m_entity<0)
            return;
        //bool actval = entity_manager::get_instance()->get_digitalActValue_component(m_entity);

        bool setval = entity_manager_locator::get_instance()->get_digitalSetValue_component(m_entity);
        changeSetValue(setval);
    }

    digital_view::~digital_view()
    {
        if(editSet)
            delete editSet;
        if(editAct)
            delete editAct;
    }

    void digital_view::changeActValue(bool actValue)
    {
        if(!editAct)
            return;
        editAct->setValue(actValue);
    }

    void digital_view::changeSetValue(int entity){
        if(m_entity<0)
            return;

        if(m_entity != entity)
            return;

        bool setValue =  entity_manager_locator::get_instance()->get_digitalSetValue_component(entity);
        changeSetValue(setValue);
    }

    void digital_view::changeActValue(int entity)
    {
        if(m_entity<0)
            return;

        if(m_entity != entity)
            return;

        bool actValue =  entity_manager_locator::get_instance()->get_digitalActValue_component(entity);
        changeActValue(actValue);
    }

    void digital_view::changeSetValue( bool setValue)
    {
        if(!editSet)
            return;

        //if (!editSet->hasFocus())
        //{
        bool oldState = editSet->blockSignals(true);
        editSet->setValue(setValue);
        editSet->blockSignals(oldState);
        //}
    }

    void digital_view::slot_set_value_changed(bool val)
    {
        emit change_set_value(m_entity, val);
    }
}

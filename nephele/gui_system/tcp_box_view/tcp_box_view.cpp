#include "tcp_box_view.hpp"

#include "core/entity_manager.hpp"
#include "core/event_manager.hpp"

#include "digital_view/digital_view.hpp"
#include "analog_view/flowcontrollerview.hpp"
#include "analog_view/analogview.hpp"

namespace talorion {

    tcp_box_view::tcp_box_view(int entity,QWidget *par) :
        QWidget(par),
        m_layout(NULL),
        //m_group_box_layout(NULL),
        //m_group_box(NULL),
        m_entity(entity),
        ana_views(),
        dig_views()
    {
        if(m_entity<0)
            return;

        QString boxname =  entity_manager::get_instance()->get_name_component(entity);
        setWindowTitle(boxname);

        setObjectName(boxname);

        m_layout = new QGridLayout();

        //m_group_box = new QGroupBox();
        //QString boxname =  entity_manager::get_instance()->get_name_component(entity);
        //m_group_box->setTitle(boxname);
        //m_layout->addWidget(m_group_box,0,0);

        //m_group_box_layout = new QGridLayout();
        //m_group_box_layout->setMargin(0);
        //m_group_box->setLayout(m_group_box_layout);

        m_layout->setMargin(0);
        setLayout(m_layout);

        connect(event_manager::get_instance(),SIGNAL(newAnalogInputValue(int)),this, SLOT(addAIV(int)));
        connect(event_manager::get_instance(),SIGNAL(newAnalogOutputValue(int)),this, SLOT(addAOV(int)));
        connect(event_manager::get_instance(),SIGNAL(newAnalogValue(int)),this, SLOT(addAV(int)));

        connect(event_manager::get_instance(),SIGNAL(newDigitalInputValue(int)),this, SLOT(addDIV(int)));
        connect(event_manager::get_instance(),SIGNAL(newDigitalOutputValue(int)),this, SLOT(addDOV(int)));
        connect(event_manager::get_instance(),SIGNAL(newDigitalValue(int)),this, SLOT(addDIOV(int)));


        connect(event_manager::get_instance(),SIGNAL(disconnect_tcp_box(int)),this, SLOT(disconnect_tcp_box(int)));

    }

    tcp_box_view::~tcp_box_view()
    {

    }

    void tcp_box_view::zero_all()
    {
        QMap<int, analogView* >::iterator ait;
        for(ait = ana_views.begin(); ait != ana_views.end();ait++){
            //
        }

        QMap<int, digital_view* >::iterator dit;
        for(dit = dig_views.begin(); dit != dig_views.end();dit++){
            //
        }
    }

    bool tcp_box_view::belogs_to_box(int entity)
    {
        if(m_entity<0)
            return false;

        int box_entity = entity_manager::get_instance()->get_box_id_component(entity);

        if(m_entity != box_entity)
            return false;

        return true;
    }

    void tcp_box_view::disconnect_tcp_box(int entity)
    {
        qDebug() << "disconnect_tcp_box()!";
        qDebug() << "removing all values!";

      if(belogs_to_box(entity)){

          QMap<int, analogView* >::iterator ait;
          for(ait = ana_views.begin(); ait != ana_views.end();ait++){
             analogView* tmp = ait.value();
             m_layout->removeWidget(tmp);
             delete tmp;
             tmp =NULL;
          }
          ana_views.clear();

          QMap<int, digital_view* >::iterator dit;
          for(dit = dig_views.begin(); dit != dig_views.end();dit++){
              digital_view* tmp = dit.value();
              m_layout->removeWidget(tmp);
              delete tmp;
              tmp =NULL;
          }
          dig_views.clear();

        }
    }

    void tcp_box_view::addAIV(int entity)
    {
        if(belogs_to_box(entity)){
            QMap<int, analogView*>::ConstIterator fcv = ana_views.constFind(entity);
            if (fcv == ana_views.constEnd()){
                flowControllerView* tmp = new flowControllerView(entity,flowControllerView::Input, this);
                ana_views.insert(entity,tmp);
                //m_group_box_layout->addWidget(tmp);
                m_layout->addWidget(tmp);
            }
        }
    }

    void tcp_box_view::addAOV(int entity)
    {
        if(belogs_to_box(entity)){
            QMap<int, analogView*>::ConstIterator fcv = ana_views.constFind(entity);
            if (fcv == ana_views.constEnd()){
                flowControllerView* tmp = new flowControllerView(entity,flowControllerView::Output, this);
                ana_views.insert(entity,tmp);
                //m_group_box_layout->addWidget(tmp);
                m_layout->addWidget(tmp);
            }
        }
    }

    void tcp_box_view::addAV(int entity)
    {
        if(belogs_to_box(entity)){
            QMap<int, analogView*>::ConstIterator fcv = ana_views.constFind(entity);
            if (fcv == ana_views.constEnd()){
                flowControllerView* tmp = new flowControllerView(entity,flowControllerView::InputOutput, this);
                ana_views.insert(entity,tmp);
                //m_group_box_layout->addWidget(tmp);
                m_layout->addWidget(tmp);
            }
        }

    }

    void tcp_box_view::addDIV(int entity)
    {
        if(belogs_to_box(entity)){
            QMap<int, digital_view*>::ConstIterator fcv = dig_views.constFind(entity);
            if (fcv == dig_views.constEnd()){
                digital_view* tmp = new digital_view(entity, digital_view::Input,this);
                dig_views.insert(entity,tmp);
                //m_group_box_layout->addWidget(tmp);
                m_layout->addWidget(tmp);
            }
        }
    }

    void tcp_box_view::addDOV(int entity)
    {
        if(belogs_to_box(entity)){
            QMap<int, digital_view*>::ConstIterator fcv = dig_views.constFind(entity);
            if (fcv == dig_views.constEnd()){
                digital_view* tmp = new digital_view(entity, digital_view::Output,this);
                dig_views.insert(entity,tmp);
                //m_group_box_layout->addWidget(tmp);
                m_layout->addWidget(tmp);
            }
        }
    }

    void tcp_box_view::addDIOV(int entity)
    {
        if(belogs_to_box(entity)){
            QMap<int, digital_view*>::ConstIterator fcv = dig_views.constFind(entity);
            if (fcv == dig_views.constEnd()){
                digital_view* tmp = new digital_view(entity, digital_view::InputOutput,this);
                dig_views.insert(entity,tmp);
                //m_group_box_layout->addWidget(tmp);
                m_layout->addWidget(tmp);
            }
        }
    }



} // namespace talorion


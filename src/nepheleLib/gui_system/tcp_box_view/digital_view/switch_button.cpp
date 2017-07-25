#include "switch_button.hpp"

#include <QGridLayout>
namespace talorion{
    switch_button::switch_button(QWidget *par) :
        QWidget(par),
        m_layout(NULL),
        m_butt(NULL),
        m_lbl(NULL)

    {
        m_layout = new QGridLayout;

        m_butt = new QPushButton();
        m_butt->setCheckable(true);
        m_butt->setAutoExclusive(true);
        m_butt->setMaximumHeight(20);
        connect(m_butt,SIGNAL(clicked(bool)),this,SLOT(button_pressed(bool)));

        m_lbl = new QLabel();
        m_lbl->setMaximumHeight(22);
        //m_lbl->setStyleSheet("QLabel { background-color : darkgrey;}");

        m_layout->addWidget(m_lbl,0,0,1,2);
        m_layout->addWidget(m_butt,0,0,1,1);

        //m_layout->setMargin(0);
        m_layout->setContentsMargins(0,0,0,0);
        this->setLayout(m_layout);
    }

    switch_button::~switch_button()
    {

    }

    void switch_button::setValue(bool newval)
    {
        m_butt->setChecked(newval);
        m_layout->removeWidget(m_butt);
        if(newval){
            m_layout->addWidget(m_butt,0,1,1,1);
            m_butt->setText("HI");

        }
        else{
            m_layout->addWidget(m_butt,0,0,1,1);
            m_butt->setText("LO");
        }
    }

    void switch_button::button_pressed(bool checked )
    {
        setValue(checked);

        emit valueChanged(checked);
    }
}


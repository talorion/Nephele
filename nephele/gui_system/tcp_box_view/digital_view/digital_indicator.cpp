#include "digital_indicator.hpp"

namespace talorion{
    digital_indicator::digital_indicator(QWidget *par) :
        QWidget(par),
        m_layout(NULL),
        m_lbl(NULL)
    {
        m_layout = new QGridLayout;

        m_lbl = new QLabel();
        m_lbl->setMaximumHeight(22);
        m_lbl->setStyleSheet("QLabel { \
                             background-color : darkgrey;qproperty-alignment: AlignCenter; \
        border-style: outset; \
        border-width: 1px; \
        border-color: white; \
    }");

    m_layout->addWidget(m_lbl,0,0);

    m_layout->setMargin(0);
    this->setLayout(m_layout);
}

digital_indicator::~digital_indicator()
{

}

void digital_indicator::setValue(bool newval)
{
    if(newval){
        m_lbl->setStyleSheet("QLabel { background-color : #CCE4F7;border-style: outset; border-width: 1px;border-color: black;}");
        m_lbl->setText("HI");
    }
    else{
        m_lbl->setStyleSheet("QLabel { background-color : lightgrey;border-style: outset; border-width: 1px;border-color: white;}");
        m_lbl->setText("LO");
    }
}
}


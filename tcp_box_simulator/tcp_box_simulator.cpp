#include "tcp_box_simulator.hpp"

tcp_box_simulator::tcp_box_simulator(QWidget *par) :
    QMainWindow(par),
    m_layout(),
    m_label()
{
     m_layout.setDirection(QHBoxLayout::TopToBottom);

     QWidget *wnd = new QWidget();
     setCentralWidget(wnd);
     wnd->setLayout(&m_layout);

     m_label.setText("Hello, World");

     m_layout.addWidget(&m_label);

}

tcp_box_simulator::~tcp_box_simulator()
{

}

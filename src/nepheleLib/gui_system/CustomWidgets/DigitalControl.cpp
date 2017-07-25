#include "DigitalControl.hpp"

#include <gui_system/GuiConstantProvider.hpp>

namespace talorion {

  DigitalControl::DigitalControl(QString truestring, QString falsestring, QWidget *par) :
    QWidget(par),
    m_layout(new QGridLayout()),
    m_butt(new DigitalControlButton()),
    m_lbl(new DigitalControlLabel()),
    m_true_string(truestring),
    m_false_string(falsestring)
  {

    connect(m_butt,SIGNAL(clicked(bool)),this,SLOT(button_pressed(bool)));

    m_layout->addWidget(m_lbl,0,0,1,2);
    m_layout->addWidget(m_butt,0,0,1,1);

    m_layout->setMargin(GuiConstantProvider::margin());
    this->setLayout(m_layout);

  }

  bool DigitalControl::value() const
  {
    return m_butt->isChecked();
  }

  void DigitalControl::setValue(bool newval)
  {
    m_butt->setChecked(newval);
    m_layout->removeWidget(m_butt);
    if(newval){
        m_layout->addWidget(m_butt,0,1,1,1);
        m_butt->setText(m_true_string);

      }else{
        m_layout->addWidget(m_butt,0,0,1,1);
        m_butt->setText(m_false_string);
      }
  }

  void DigitalControl::button_pressed(bool checked)
  {
    setValue(checked);
    emit valueChanged(checked);
  }

  void DigitalControl::enterEvent(QEvent *event)
  {
    emit entered();
    QWidget::enterEvent(event);
  }

  void DigitalControl::leaveEvent(QEvent *event)
  {
    emit left();
    QWidget::enterEvent(event);
  }

} // namespace talorion

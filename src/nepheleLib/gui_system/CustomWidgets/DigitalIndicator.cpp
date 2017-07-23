#include "DigitalIndicator.hpp"

namespace talorion {


  void DigitalIndicator::setValue(bool newval)
  {
    if(newval){
        //m_lbl->setStyleSheet("QLabel { background-color : #CCE4F7;border-style: outset; border-width: 1px;border-color: black;}");
        setText(m_true_string);
      }
    else{
        //m_lbl->setStyleSheet("QLabel { background-color : lightgrey;border-style: outset; border-width: 1px;border-color: white;}");
        setText(m_false_string);
      }
  }

  void DigitalIndicator::enterEvent(QEvent *event)
  {
    emit entered();
    QLabel::enterEvent(event);
  }

  void DigitalIndicator::leaveEvent(QEvent *event)
  {
    emit left();
    QLabel::enterEvent(event);
  }

  DigitalIndicator::DigitalIndicator(QString truestring, QString falsestring, QWidget *parent, Qt::WindowFlags f):
    QLabel(parent, f),
    m_true_string(truestring),
    m_false_string(falsestring)
  {
    setMaximumHeight(22);
  }

  void DigitalIndicator::setPointSize(int sz)
  {
    auto actfont = font();
    actfont.setPointSize(sz);
    setFont(actfont);
  }

  void DigitalIndicator::setPixelSize(int sz)
  {
    auto actfont = font();
    actfont.setPixelSize(sz);
    setFont(actfont);
  }

} // namespace talorion

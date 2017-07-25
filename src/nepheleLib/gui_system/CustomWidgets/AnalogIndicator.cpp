#include "AnalogIndicator.hpp"

#include <math.h>       /* log10 */

namespace talorion {

  AnalogIndicator::AnalogIndicator(QString unit, int decimals, QWidget *par) :
    QDoubleSpinBox(par)
  {
    setup(0, 999,unit, decimals);
  }

  AnalogIndicator::AnalogIndicator(int min, int max, QString unit, QWidget *par):
    QDoubleSpinBox(par)
  {
    setup(min, max,unit, 0);
  }

  void AnalogIndicator::setPointSize(int sz)
  {
      auto actfont = font();
      actfont.setPointSize(sz);
      setFont(actfont);
  }

  void AnalogIndicator::setPixelSize(int sz)
  {
      auto actfont = font();
      actfont.setPixelSize(sz);
      setFont(actfont);
  }

  void AnalogIndicator::setup(int min, int max, QString unit, int dec)
  {
    setButtonSymbols(QDoubleSpinBox::NoButtons);
    setReadOnly(true);
    setFocusPolicy(Qt::NoFocus);

    setMinimum(min);
    setMaximum(max);

    //setAlignment(Qt::AlignHCenter);
    setAlignment(Qt::AlignRight);

    if ((minimum()) > 1 && log10((maximum()))<=6)
      setDecimals(6-log10((maximum())));

    if(!unit.isNull()){
        setSuffix(" " + unit);
      }

    if(decimals() < dec)
      setDecimals(dec);

  }

  void AnalogIndicator::enterEvent(QEvent *event)
  {
    emit entered();
    QDoubleSpinBox::enterEvent(event);
  }

  void AnalogIndicator::leaveEvent(QEvent *event)
  {
    emit left();
    QDoubleSpinBox::enterEvent(event);
  }

} // namespace talorion

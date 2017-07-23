#include "AnalogControl.hpp"

#include <math.h>       /* log10 */

namespace talorion {

  AnalogControl::AnalogControl(QString unit, int decimals, QWidget *par) :
    QDoubleSpinBox(par)
  {
    setup(0, 999, unit, decimals);
  }

  AnalogControl::AnalogControl(int min, int max, QString unit, QWidget *par):
    QDoubleSpinBox(par)
  {
    setup(min, max, unit, 0);
  }

  void AnalogControl::setValue(double val)
  {
    if(hasFocus())
      return;

    QDoubleSpinBox::setValue(val);
  }

  void AnalogControl::setup(int min, int max, QString unit, int dec)
  {
    setButtonSymbols(QDoubleSpinBox::NoButtons);
    setAlignment(Qt::AlignRight);

    setMinimum(min);
    setMaximum(max);

    if(!unit.isNull()){
        setSuffix(" " + unit);
      }

    if ((minimum()) > 1 && log10((maximum()))<=6)
      setDecimals(6-log10((maximum())));

    setSingleStep(max/150.0);

    if(decimals() < dec)
      setDecimals(dec);

  }

  void AnalogControl::enterEvent(QEvent *event)
  {
    emit entered();
    QDoubleSpinBox::enterEvent(event);
  }

  void AnalogControl::leaveEvent(QEvent *event)
  {
    emit left();
    QDoubleSpinBox::enterEvent(event);
  }

} // namespace talorion

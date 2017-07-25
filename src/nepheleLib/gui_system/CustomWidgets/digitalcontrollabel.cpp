#include "digitalcontrollabel.hpp"

#include <gui_system/GuiConstantProvider.hpp>

namespace talorion {


  DigitalControlLabel::DigitalControlLabel(QWidget *parent, Qt::WindowFlags f):
    QLabel(parent, f)
  {
    setup();
  }

  DigitalControlLabel::DigitalControlLabel(const QString &text, QWidget *parent, Qt::WindowFlags f):
    QLabel(text, parent, f)
  {
    setup();
  }

  void DigitalControlLabel::setup()
  {
    setMaximumHeight(22);
  }

} // namespace talorion

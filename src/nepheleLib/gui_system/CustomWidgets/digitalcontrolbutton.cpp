#include "digitalcontrolbutton.hpp"

#include <gui_system/GuiConstantProvider.hpp>

namespace talorion {


DigitalControlButton::DigitalControlButton(QWidget *parent):
    QPushButton(parent)
{
    setup();
}

DigitalControlButton::DigitalControlButton(const QString &text, QWidget *parent):
    QPushButton(text, parent)
{
    setup();
}

DigitalControlButton::DigitalControlButton(const QIcon &icon, const QString &text, QWidget *parent):
    QPushButton(icon, text, parent)
{
    setup();
}

void DigitalControlButton::setup()
{
    setCheckable(true);
    setAutoExclusive(true);
    setMaximumHeight(20);
}

} // namespace talorion

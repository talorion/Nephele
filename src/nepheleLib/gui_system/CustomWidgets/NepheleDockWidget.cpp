#include "NepheleDockWidget.hpp"

namespace talorion {

NepheleDockWidget::NepheleDockWidget(const QString &title, QWidget *parent, Qt::WindowFlags flags):
    QDockWidget(title, parent, flags)
{
    setContentsMargins(0,0,0,0);

}

NepheleDockWidget::NepheleDockWidget(QWidget *parent, Qt::WindowFlags flags):
    QDockWidget(parent, flags)
{
  setContentsMargins(0,0,0,0);
}

void NepheleDockWidget::slotSetWindowTitle(const QString &title)
{
  auto currtitle = windowTitle();
  if(currtitle != title)
    setWindowTitle(title);
}

} // namespace talorion

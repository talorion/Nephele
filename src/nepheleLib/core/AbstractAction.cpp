#include "AbstractAction.hpp"

namespace talorion {

AbstractAction::AbstractAction(QObject *parent):
    QAction(parent),
    m_preferedMenu()
{

}

AbstractAction::AbstractAction(const QString &text, QObject *parent):
    QAction(text, parent),
    m_preferedMenu()
{

}

AbstractAction::AbstractAction(const QIcon &icon, const QString &text, QObject *parent):
    QAction(icon, text, parent),
    m_preferedMenu()
{

}

void AbstractAction::setPreferedMenu(const QString &preferedMenu)
{
    m_preferedMenu = preferedMenu;
}

} // namespace talorion

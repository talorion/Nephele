#ifndef TALORION_ABSTRACTACTION_HPP
#define TALORION_ABSTRACTACTION_HPP

//#include <QAction>
#include <QtWidgets/QAction>

namespace talorion {

class AbstractAction : public QAction
{
    Q_OBJECT
public:
    explicit AbstractAction(QObject *parent = Q_NULLPTR);
    explicit AbstractAction(const QString &text, QObject *parent = Q_NULLPTR);
    explicit AbstractAction(const QIcon &icon, const QString &text, QObject *parent = Q_NULLPTR);
    virtual ~AbstractAction()=default;

    const QString& preferedMenu()const{return m_preferedMenu;}
    void setPreferedMenu(const QString &preferedMenu);

private:
    QString m_preferedMenu;

};

} // namespace talorion

#endif // TALORION_ABSTRACTACTION_HPP

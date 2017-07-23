#ifndef TALORION_ABSTRACT_SYSTEM_VIEW_HPP
#define TALORION_ABSTRACT_SYSTEM_VIEW_HPP

#include <QtWidgets/QWidget>

namespace talorion {

class AbstractSystemView : public QWidget
{
    Q_OBJECT
public:
    explicit AbstractSystemView(QWidget *par = 0);
    virtual ~AbstractSystemView()=default;

    Qt::DockWidgetArea dockWidgetAreaHint(){return doDockWidgetAreaHint();}

private:
    virtual Qt::DockWidgetArea doDockWidgetAreaHint(){return Qt::LeftDockWidgetArea;}

};

} // namespace talorion

#endif // TALORION_ABSTRACT_SYSTEM_VIEW_HPP

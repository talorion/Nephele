#ifndef ABSTRACT_CONFIGURATION_WIDGET_HPP
#define ABSTRACT_CONFIGURATION_WIDGET_HPP

#include <QtWidgets/QWidget>
#include "EntityManager/EntityManager.hpp"

namespace talorion{


class AbstractConfigurationWidget : public QWidget
{
    Q_OBJECT
public:
    explicit AbstractConfigurationWidget(QWidget *par = Q_NULLPTR );
    virtual ~AbstractConfigurationWidget()=default;

    void refreshData(){doRefreshData();}

private:
    virtual void doRefreshData()=0;

protected:
    EntityManager& entityMng()const;

};
}

#endif // ABSTRACT_CONFIGURATION_WIDGET_HPP

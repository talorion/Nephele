#include "AbstractConfigurationWidget.hpp"

#include <ManagerLocator.hpp>

namespace talorion{
  AbstractConfigurationWidget::AbstractConfigurationWidget(QWidget *par)
    : QWidget(par)
  {

  }

  EntityManager &AbstractConfigurationWidget::entityMng() const
  {
    return ManagerLocator::entityManager();
  }



}

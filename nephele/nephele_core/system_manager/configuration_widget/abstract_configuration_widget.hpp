#ifndef ABSTRACT_CONFIGURATION_WIDGET_HPP
#define ABSTRACT_CONFIGURATION_WIDGET_HPP

#include <QWidget>

namespace talorion{
  class abstract_configuration_widget : public QWidget
  {
    Q_OBJECT
  public:
    explicit abstract_configuration_widget(QWidget *par = Q_NULLPTR );
    virtual ~abstract_configuration_widget()=default;

  private:
    Q_DISABLE_COPY(abstract_configuration_widget)

  signals:

  public slots:
  };
}

#endif // ABSTRACT_CONFIGURATION_WIDGET_HPP

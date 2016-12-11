#ifndef TALORION_ABSTRACTPLOT_HPP
#define TALORION_ABSTRACTPLOT_HPP

#include <QGridLayout>
#include <QPointer>
#include <QWidget>

#include <qcustomplot.h>

#include "guisystem_global.hpp"

namespace talorion {

  class AbstractPlot : public QWidget
  {
    Q_OBJECT
  public:
    explicit AbstractPlot(QWidget *par = 0);

  private:
    QPointer<QGridLayout> m_layout;
    QPointer<QCustomPlot> m_plot;
  };

} // namespace talorion

#endif // TALORION_ABSTRACTPLOT_HPP

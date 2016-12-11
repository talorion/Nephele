#ifndef TALORION_TRACESPLOT_HPP
#define TALORION_TRACESPLOT_HPP

#include "AbstractPlot.hpp"

namespace talorion {

  class TracesPlot : public AbstractPlot
  {
    Q_OBJECT
  public:
    explicit TracesPlot(QWidget *par = 0);

  };

} // namespace talorion

#endif // TALORION_TRACESPLOT_HPP

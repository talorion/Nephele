#ifndef TALORION_SPECTRAPLOT_HPP
#define TALORION_SPECTRAPLOT_HPP

#include "AbstractPlot.hpp"

namespace talorion {

  class SpectraPlot : public AbstractPlot
  {
    Q_OBJECT
  public:
    explicit SpectraPlot(QWidget *par = 0);

  };

} // namespace talorion

#endif // TALORION_SPECTRAPLOT_HPP

#include "NepheleMainWindow.hpp"

#include <NepheleMainWindow/CustomWidgets/SpectraPlot.hpp>
#include <NepheleMainWindow/CustomWidgets/TracesPlot.hpp>

namespace talorion {

  NepheleMainWindow::NepheleMainWindow(QWidget *par) :
    QMainWindow(par),
    m_layout(new QGridLayout())
  {
    setCentralWidget(new QWidget(this));
    auto ctrl_wdg = centralWidget();

    SpectraPlot* specPlot = new SpectraPlot();
    m_layout->addWidget(specPlot,    0, 1, 3, 4);

    TracesPlot* tracePlot = new TracesPlot();
    m_layout->addWidget(tracePlot,    3, 1, 3, 4);


    m_layout->setMargin(MARGIN);
    m_layout->setHorizontalSpacing(HORIZONTAL_SPACEING);
    //m_layout->setVerticalSpacing(0);
    ctrl_wdg->setLayout(m_layout);
  }

} // namespace talorion

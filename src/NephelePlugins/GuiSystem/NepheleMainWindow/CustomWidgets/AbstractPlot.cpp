#include "AbstractPlot.hpp"

namespace talorion {

  AbstractPlot::AbstractPlot(QWidget *par) :
    QWidget(par),
    m_layout(new QGridLayout()),
    m_plot(new QCustomPlot())
  {
    QFont nephFont = font();  // start out with MainWindow's font..
    nephFont.setPointSize(FONT_SIZE_PX); // and make a bit smaller for legend
    nephFont.setFamily(IC_FONT_FAMILY);

    m_plot->setBackground(QColor(BG_COLOR));
    m_plot->axisRect()->setBackground(QColor(BG_COLOR));
    m_plot->xAxis->setTickLabelColor(QColor(IC_COLOR_GREY));
    m_plot->xAxis->setLabelFont(nephFont);
    m_plot->xAxis->setLabelColor(QColor(IC_COLOR_GREY));
    m_plot->xAxis->setBasePen(QPen(QColor(IC_COLOR_GREY)));

    m_plot->yAxis->setTickLabelColor(QColor(IC_COLOR_GREY));
    m_plot->yAxis->setLabelFont(nephFont);
    m_plot->yAxis->setLabelColor(QColor(IC_COLOR_GREY));
    m_plot->yAxis->setBasePen(QPen(QColor(IC_COLOR_GREY)));
    m_plot->yAxis->setScaleType(QCPAxis::stLogarithmic);
    QSharedPointer<QCPAxisTickerLog> logTicker(new QCPAxisTickerLog);
    m_plot->yAxis->setTicker(logTicker);
    m_plot->yAxis->setNumberPrecision(0); // makes sure "1*10^4" is displayed only as "10^4"
    m_plot->xAxis->setRange(0, 19.9);
    m_plot->yAxis->setRange(1e-2, 1e3);

    m_plot->legend->setFont(nephFont);
    m_plot->legend->setTextColor(QColor(IC_COLOR_GREY));
    m_plot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectAxes | QCP::iSelectLegend | QCP::iSelectPlottables);

//    m_current_spectrum =  m_plot->addGraph();
//    m_current_spectrum->setAdaptiveSampling(true);
//    m_current_spectrum->setPen(QPen(QColor(IC_COLOR_ORANGE)));

    m_layout->addWidget(m_plot,0,0);
    m_layout->setMargin(MARGIN);
    m_layout->setHorizontalSpacing(HORIZONTAL_SPACEING);
    m_layout->setVerticalSpacing(0);
    setLayout(m_layout);
  }

} // namespace talorion

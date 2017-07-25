#include "SpectraPlot.hpp"

#include <QDebug>

#include <gui_system/GuiConstantProvider.hpp>

namespace talorion {

  SpectraPlot::SpectraPlot(QWidget *par) :
    QWidget(par),
    m_layout(new QGridLayout()),
    m_plot(new QCustomPlot()),
    m_current_spectrum()
  {
    QFont ionicFont = font();  // start out with MainWindow's font..
    ionicFont.setPointSize(GuiConstantProvider::neph_font_size_px()); // and make a bit smaller for legend
    ionicFont.setFamily(GuiConstantProvider::neph_font_family());

    m_plot->setBackground(QColor(GuiConstantProvider::bg_color()));
    m_plot->axisRect()->setBackground(QColor(GuiConstantProvider::bg_color()));
    m_plot->xAxis->setTickLabelColor(QColor(GuiConstantProvider::fg_color()));
    m_plot->xAxis->setLabelFont(ionicFont);
    m_plot->xAxis->setLabelColor(QColor(GuiConstantProvider::fg_color()));
    m_plot->xAxis->setBasePen(QPen(QColor(GuiConstantProvider::fg_color())));

    m_plot->yAxis->setTickLabelColor(QColor(GuiConstantProvider::fg_color()));
    m_plot->yAxis->setLabelFont(ionicFont);
    m_plot->yAxis->setLabelColor(QColor(GuiConstantProvider::fg_color()));
    m_plot->yAxis->setBasePen(QPen(QColor(GuiConstantProvider::fg_color())));
    m_plot->yAxis->setScaleType(QCPAxis::stLogarithmic);
    QSharedPointer<QCPAxisTickerLog> logTicker(new QCPAxisTickerLog);
    m_plot->yAxis->setTicker(logTicker);
    m_plot->yAxis->setNumberPrecision(0); // makes sure "1*10^4" is displayed only as "10^4"
    m_plot->xAxis->setRange(0, 19.9);
    m_plot->yAxis->setRange(1e-2, 1e3);

    m_plot->legend->setFont(ionicFont);
    m_plot->legend->setTextColor(QColor(GuiConstantProvider::fg_color()));
    m_plot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectAxes | QCP::iSelectLegend | QCP::iSelectPlottables);
    m_layout->addWidget(m_plot,0,0);

    m_current_spectrum =  m_plot->addGraph();
    m_current_spectrum->setAdaptiveSampling(true);
    m_current_spectrum->setPen(QPen(QColor(GuiConstantProvider::complementaryColor())));

    m_layout->setHorizontalSpacing(GuiConstantProvider::horizontal_spaceing());
    m_layout->setMargin(GuiConstantProvider::margin());
    m_layout->setVerticalSpacing(0);
    setLayout(m_layout);

    connect(m_plot, SIGNAL(mouseWheel(QWheelEvent*)), this, SLOT(plotMouseWheel()));
    connect(m_plot->xAxis, SIGNAL(rangeChanged(QCPRange)), this, SLOT(plotRangeChanged(QCPRange)));

    connect(m_plot,SIGNAL(mouseDoubleClick(QMouseEvent*)), this, SLOT(plotMouseDoubleClick(QMouseEvent*)));
  }

  SpectraPlot::~SpectraPlot()
  {

  }

  void SpectraPlot::newSpectrum(QVector<double>& timebin, QVector<double>& dta)
  {
      m_current_spectrum->setData(timebin, dta);
      m_plot->replot();
  }

  void SpectraPlot::plotMouseDoubleClick(QMouseEvent *event)
  {
    Q_UNUSED(event);
    m_current_spectrum->rescaleAxes();
  }

  void SpectraPlot::plotMouseWheel()
  {
    // if an axis is selected, only allow the direction of that axis to be zoomed
    // if no axis is selected, both directions may be zoomed
    if (m_plot->xAxis->selectedParts().testFlag(QCPAxis::spAxis))
      m_plot->axisRect()->setRangeZoom(m_plot->xAxis->orientation());
    else if (m_plot->yAxis->selectedParts().testFlag(QCPAxis::spAxis))
      m_plot->axisRect()->setRangeZoom(m_plot->yAxis->orientation());
    else
      m_plot->axisRect()->setRangeZoom(Qt::Horizontal|Qt::Vertical);
  }

  void SpectraPlot::plotRangeChanged(QCPRange range)
  {
    Q_UNUSED(range);
  }

  void SpectraPlot::mouseDoubleClickEvent(QMouseEvent *event)
  {
    Q_UNUSED(event);
    m_plot->replot();
  }


  void SpectraPlot::resizeEvent(QResizeEvent *event)
  {
    Q_UNUSED(event);
  }


} // namespace talorion





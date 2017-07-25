#include "AnalogPlot.hpp"

#include <gui_system/GuiConstantProvider.hpp>

namespace talorion {

  AnalogPlot::AnalogPlot(QWidget *par) :
    QWidget(par),
    m_title(new TitleLabel()),
    m_actValue(Q_NULLPTR),
    m_layout(new QGridLayout()),
    m_plot(new QCustomPlot()),
    m_actGraph(Q_NULLPTR),
    m_graphPersistanceTime(20),
    m_graphTimer(new QTimer()),
    m_lastValue(qQNaN())
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

    m_plot->legend->setFont(ionicFont);
    m_plot->legend->setTextColor(QColor(GuiConstantProvider::fg_color()));

    m_plot->legend->setVisible(false);

    m_actValue = new AnalogIndicator();
    //m_actValue->setPixelSize(32);
    auto fontsize = QString("%1px").arg(GuiConstantProvider::neph_font_size_px()*2);
    m_actValue->setStyleSheet("talorion--AnalogIndicator { font-size: "+fontsize+"; }");
    //m_plot->setStyleSheet("");

    int row = 0;
    int column = 0;

    m_layout->addWidget(m_title,row++,column,1,0);
    m_layout->addWidget(m_plot,row++,column,10,0);
    row+=10;
    m_layout->addWidget(m_actValue, row++, column,1,0);

    m_actGraph = m_plot->addGraph();
    //setWindowFlags(Qt::ToolTip);
    //setFixedHeight(200);
    //setFixedWidth(400);

    m_plot->legend->setVisible(false);

    setRanges(0, 100);

    QSharedPointer<QCPAxisTicker> yticker(new QCPAxisTicker);
    yticker->setTickCount(2);
    m_plot->yAxis->setTicker(yticker);

    QSharedPointer<QCPAxisTickerTime> xticker(new QCPAxisTickerTime);
    xticker->setTimeFormat("%d \n%h:%m:%s");
    m_plot->xAxis->setTicker(xticker);
    m_plot->xAxis->setTickLabels(true);

    m_plot->xAxis->setRangeLower(QDateTime::currentMSecsSinceEpoch()/1000.0);
    //m_plot->xAxis->setRangeLower(QTime::currentTime().msecsSinceStartOfDay()/1000.0);

    m_layout->setHorizontalSpacing(GuiConstantProvider::horizontal_spaceing());
    m_layout->setMargin(GuiConstantProvider::margin());
    m_layout->setVerticalSpacing(0);

    setLayout(m_layout);

    m_graphTimer->setInterval(100);
    connect(m_graphTimer, SIGNAL(timeout()), this, SLOT(newValueTimeout()));
    m_graphTimer->start();
  }

  AnalogPlot::~AnalogPlot()
  {

  }

  void AnalogPlot::setRanges(double lowerRange, double upperRange)
  {
    auto tmpMin = qMin(lowerRange, upperRange);
    auto tmpMax = qMax(lowerRange, upperRange);
    if((tmpMax > 0) && (tmpMin < tmpMax)){
        double lRange = tmpMin - (tmpMax-tmpMin)/10;
        double uRange = tmpMax + (tmpMax-tmpMin)/10;
        m_plot->yAxis->setRangeLower(lRange);
        m_plot->yAxis->setRangeUpper(uRange);
      }

  }

  void AnalogPlot::updatePlot(double value)
  {
    if(qIsNaN(value))
      return;

    QDateTime t;
    double time  = t.currentMSecsSinceEpoch()/1000.0;
//    QTime t;
//    double time  = t.msecsSinceStartOfDay()/1000.0;

    QCPGraphData tmp(time, value);
    updatePlot(tmp);
  }

  void AnalogPlot::updatePlot(QCPGraphData &val)
  {
    double time = val.key;
    double value = val.value;

    m_actGraph->addData(time, value);
    m_plot->xAxis->setRangeUpper(time);
    m_plot->xAxis->setRangeLower(time - m_graphPersistanceTime);
    m_actGraph->data()->removeBefore(val.key - graphPersistanceTime());
    m_plot->replot();

    m_actValue->setValue(value); //indicator
    m_lastValue = value;

    m_graphTimer->start();
  }

  void AnalogPlot::setTitle(const QString &title)
  {
    m_title->setText(title);
    m_actGraph->setName(title);
  }

  void AnalogPlot::resetData(const graphHistory_t &history )
  {
    m_graphTimer->stop();
    m_actGraph->data().clear();
    const QCPGraphDataContainer* tmp = history.data();
    m_actGraph->data()->set(*tmp);
    m_plot->replot();
  }

  void AnalogPlot::newValueTimeout()
  {
    updatePlot(m_lastValue);
  }

  int AnalogPlot::graphPersistanceTime() const
  {
    return m_graphPersistanceTime;
  }

  void AnalogPlot::setGraphPersistanceTime(int graphPersistanceTime)
  {
    m_graphPersistanceTime = graphPersistanceTime;
  }

} // namespace talorion

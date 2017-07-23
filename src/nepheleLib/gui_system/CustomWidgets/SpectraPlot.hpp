#ifndef TALORION_SPECTRAPLOT_HPP
#define TALORION_SPECTRAPLOT_HPP

#include <QGridLayout>
#include <QPointer>
#include <QWidget>

//class QCustomPlot;
class QCPGraph;
//class QCPRange;
#include <qcustomplot.h>

namespace talorion {

  class SpectraPlot : public QWidget
  {
    Q_OBJECT
  public:
    explicit SpectraPlot(QWidget *par = 0);
    virtual ~SpectraPlot();

  private:
      Q_DISABLE_COPY(SpectraPlot)

  public slots:
      void newSpectrum(QVector<double> &timebin, QVector<double> &dta);

  private slots:
    void plotMouseDoubleClick (QMouseEvent *event);
    void plotMouseWheel();
    void plotRangeChanged(QCPRange range);

  private:
    QPointer<QGridLayout> m_layout;
    QPointer<QCustomPlot> m_plot;
    QPointer<QCPGraph> m_current_spectrum;

    // QWidget interface
  protected:
    virtual void mouseDoubleClickEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    virtual void resizeEvent(QResizeEvent *event) Q_DECL_OVERRIDE;
  };

} // namespace talorion

#endif // TALORION_SPECTRAPLOT_HPP
